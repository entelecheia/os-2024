# 컨디션 변수[^ypilseong]

[^ypilseong]: [양필성](https://github.com/ypilseong)

우리가 배운 _'락'_ 하나만 가지고는 제대로 병행 프로그램을 작성할 수 없습니다. 쓰레드가 계속 진행하기 전에 특정 조건이 만족되었는지 검사가 필요한 경우가 있습니다. 예를 들면 자식 쓰레드가 작업을 끝냈는지 여부를 알 필요가 있습니다. 이런 걸 어떻게 구현할 수 있을까요?

```c
volatile int done = 0;

void *child(void *arg) {
	printf("child\n");
	done = 1;
	return NULL;
}

int main(int argc, char *argv[]) {
	printf("parent: begin\n");
	pthread_t c;
	Pthread_create(&c, NULL, child, NULL);
	while (done == 0)
	; // spin
	printf("parent: end\n");
	return 0;
}
```

이렇게 공유 변수로 구현할 수 있습니다. 하지만 부모 쓰레드가 `spin` 하면서 자원을 낭비하고 있습니다. 이 방법 대신 부모 쓰레드가 특정 조건이 만족될때까지 잠자면서 기다리는 것이 더 좋습니다.

## 컨디션 변수 정의와 사용 함수들

조건이 참이 될 때까지 기다리기 위해 **컨디션 변수(Condition Variable)**를 활용할 수 있습니다. 컨디션 변수는 일종의 큐 자료 구조로서, 어떤 실행의 상태 (또는 어떤 조건)가 원하는 것과 다를 때 참이 되기를 기다리며 스레드가 대기할 수 있는 큐입니다. 다른 쓰레드가 상태를 변경시켰을 때, 대기 중이던 쓰레드를 깨우고, 계속 진행할 수 있도록 합니다.

`pthread_cond_t c;` 라고 써서 `c`가 컨디션 변수가 되도록 선언하고 초기화합니다. 컨디션 변수에는 `wait()` 과 `signal()` 이라는 두 가지 연산이 존재합니다.

- `wait()` : 쓰레드가 스스로를 잠재우기 위해 호출하는 함수
- `signal()` : 쓰레드가 무엇인가를 변경했기 때문에 조건이 참이 되기를 기다리며 잠자고 있던 쓰레드를 깨울 때 호출하는 함수

```c
pthread_cond_wait(pthread_cond_t *c, pthread_mutex_t *m);
pthread_cond_signal(pthread_cond_t *c);
```

`wait()`에서 주의할 점은 `mutex`를 매개변수로 사용한다는 것입니다. `wait()`가 호출될 때 `mutex`는 잠겨있었다고 가정합니다. `wait()`는 락을 해제하고 호출한 쓰레드를 재운 후, 어떤 다른 쓰레드가 시그널을 보내 쓰레드가 깨어나면 `wait()`에서 리턴하기 전에 락을 다시 획득해야 합니다.

즉, 조건이 만족되어 잠에서 깨어났더라도 락을 획득하지 못하면 다시 잠에 드는 것입니다. 이렇게 복잡한 이유는 쓰레드가 스스로 잠들려고 할 때 경쟁 조건(Race Condition)의 발생을 방지하기 위해서입니다.

이해를 돕기 위해 예제 코드를 살펴보겠습니다.

```c
int done = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void thr_exit() {
	Pthread_mutex_lock(&m);
	done = 1;
	Pthread_cond_signal(&c);
	Pthread_mutex_unlock(&m);
}

void *child(void *arg) {
	printf("child\n");
	thr_exit();
	return NULL;
}

void thr_join() {
	Pthread_mutex_lock(&m);
	while (done == 0)
		Pthread_cond_wait(&c, &m);
	Pthread_mutex_unlock(&m);
}

int main(int argc, char *argv[]) {
	printf("parent: begin\n");
	pthread_t p;
	Pthread_create(&p, NULL, child, NULL);
	thr_join();
	printf("parent: end\n");
	return 0;
}
```

```{note}
조건을 검사할 때 `if`문이 아니라 `while`문을 사용해야 하는 이유는 뒤에서 다시 설명하겠습니다.
```

### 첫 번째 실행 시나리오

1. 부모 쓰레드가 자식 쓰레드를 생성하고 `thr_join()`을 호출하여 자식 쓰레드가 끝나기를 기다립니다.
2. 부모 쓰레드는 락을 획득하고 자식이 끝났는지 검사(`while (done == 0)`)한 후, 자식이 아직 끝나지 않았으므로 `wait()`을 호출하여 락을 해제하고 스스로를 잠재웁니다.
3. 나중에 자식 쓰레드가 실행되어 `thr_exit()`을 호출하면 부모 쓰레드가 깨어나고, `wait()`에서 락을 다시 획득한 채로 리턴하여 부모 쓰레드가 계속 실행되고 락을 해제한 후 종료됩니다.

### 두 번째 실행 시나리오

1. 자식 쓰레드가 생성되자마자 즉시 실행되어 `done`을 1로 설정하고, 자고 있는 쓰레드를 깨우기 위해 시그널을 보냅니다. 하지만 아직 자고 있는 쓰레드가 없기 때문에 아무런 효과가 없습니다.
2. 이후 부모 쓰레드가 실행되어 `thr_join()`을 호출하지만 이미 `done`이 1이므로 `while` 루프를 건너뛰고 바로 락을 해제하고 리턴합니다.

`thr_exit()`, `thr_join()` 함수의 중요성을 이해할 수 있도록 몇 가지 잘못된 구현 방식을 살펴보겠습니다.

1. 잘못된 구현 1

```c
void thr_exit() {
	Pthread_mutex_lock(&m);
	Pthread_cond_signal(&c);
	Pthread_mutex_unlock(&m);
}

void thr_join() {
	Pthread_mutex_lock(&m);
	Pthread_cond_wait(&c, &m);
	Pthread_mutex_unlock(&m);
}
```

이런 식으로 작성하면 자식 쓰레드가 생성된 즉시 실행되어 `thr_exit()`을 호출하는 경우(두 번째 시나리오)에 제대로 작동하지 않습니다. 자식 프로세스가 시그널을 보내지만, 아직 깨울 쓰레드가 없어 아무 효과가 없습니다. 이후 부모 쓰레드는 `wait()`을 호출하고 거기서 영원히 블록됩니다.

2. 잘못된 구현 2

```c
void thr_exit() {
	done = 1;
	Pthread_cond_signal(&c);
}

void thr_join() {
	if (done == 0)
		Pthread_cond_wait(&c);
}
```

이 경우는 부모 쓰레드가 `thr_join()`을 호출하고 `done`이 `0`인 것을 확인한 직후, 바로 `wait()`을 호출하기 전에 자식 쓰레드에게 선점(preemption)당하는 경우 문제가 발생합니다. 자식 쓰레드가 실행되어 `done`을 `1`로 변경하고 시그널을 보내지만 대기 중인 쓰레드가 없습니다. 다시 부모 쓰레드가 실행되어 `wait()`을 호출하면 영원히 블록됩니다.

```{note}
시그널을 보내는 코드는 반드시 조건 변수를 보호하는 락으로 둘러싸여 있어야 합니다. 그래야 wait()하려는 쓰레드와 시그널을 보내는 쓰레드 간의 경쟁 조건을 피할 수 있습니다.
```

## 생산자/소비자 (Producer/Consumer) 문제

다음으로 살펴볼 동기화 문제는 Edsger Dijkstra가 처음 제시한 생산자/소비자(Producer/Consumer) 문제입니다. 유한 버퍼(Bounded Buffer) 문제로도 알려져 있습니다. 이 문제를 해결하기 위해 Dijkstra는 `lock` 대신 일반화된 세마포어(Semaphore)를 발명했습니다.

### 문제 설명

- 여러 개의 생산자(Producer) 쓰레드와 소비자(Consumer) 쓰레드가 있습니다.
- 생산자는 데이터를 만들어 버퍼에 집어넣습니다.
- 소비자는 버퍼에서 데이터를 꺼내 사용합니다.
- 버퍼는 유한한 크기를 가집니다.
- 버퍼가 가득 차면 생산자는 기다려야 하고, 비어있으면 소비자가 기다려야 합니다.

이러한 생산자-소비자 관계는 실제 시스템에서 자주 볼 수 있습니다.

- 멀티 쓰레드 웹 서버에서 생산자는 HTTP 요청을 작업 큐(유한 버퍼)에 넣고, 소비자 쓰레드는 이 큐에서 요청을 꺼내 처리합니다.
- `grep foo file.txt | wc -l`와 같은 쉘 파이프 명령어에서도 생산자(`grep`)가 결과를 파이프(유한 버퍼)에 쓰면 소비자(`wc`)가 파이프에서 읽어가는 방식으로 동작합니다.

여러 쓰레드가 버퍼라는 공유 자원에 동시에 접근하므로, 경쟁 조건을 피하기 위해 동기화가 필요합니다.

### 첫 번째 시도

우선 버퍼 크기를 1로 제한하고, 공유 버퍼에 값을 넣는 함수(`put()`)와 꺼내는 함수(`get()`)를 아래와 같이 정의해 봅시다.

```c
int buffer;
int count = 0;

void put(int value) {
	assert(count == 0);
	count = 1;
	buffer = value;
}

int get() {
	assert(count == 1);
	count = 0;
	return buffer;
}
```

이제 생산자와 소비자 코드를 작성해 보겠습니다. 컨디션 변수 하나와 관련 `mutex` 락을 사용합니다.

```c
cond_t cond;
mutex_t mutex;

void *producer(void *arg) {
	int i;
	for (i = 0; i < loops; i++) {
		Pthread_mutex_lock(&mutex); // p1
		if (count == 1) // p2
			Pthread_cond_wait(&cond, &mutex); // p3
		put(i); // p4
		Pthread_cond_signal(&cond); // p5
		Pthread_mutex_unlock(&mutex); // p6
	}
}

void *consumer(void *arg) {
	int i;
	for (i = 0; i < loops; i++) {
		Pthread_mutex_lock(&mutex); // c1
		if (count == 0) // c2
			Pthread_cond_wait(&cond, &mutex); // c3
		int tmp = get(); // c4
		Pthread_cond_signal(&cond); // c5
		Pthread_mutex_unlock(&mutex); // c6
		printf("%d\n", tmp);
	}
}
```

- 생산자는 버퍼가 빌 때까지(`count == 1`) 기다립니다.
- 소비자는 버퍼에 데이터가 있을 때까지(`count == 0`) 기다립니다.

생산자, 소비자가 각각 하나씩만 있다면 이 코드는 정상 동작합니다. 하지만 둘 이상의 생산자와 소비자가 있는 경우 문제가 생깁니다.

#### 첫 번째 문제점

대기 루프의 `if`문과 관련이 있습니다.

소비자 쓰레드가 2개(Tc1, Tc2) 있고 생산자 쓰레드가 1개(Tp) 있다고 가정해 보겠습니다.

1. 먼저 소비자 Tc1이 실행됩니다. 락을 획득하고(c1) 버퍼를 소비할 수 있는지 검사합니다(c2). 현재 버퍼가 비어있으므로 Tc1은 `wait()`을 호출하여(c3) 락을 해제하고 sleep합니다.
2. 이제 생산자 Tp가 실행됩니다. 락을 획득하고(p1) 버퍼가 비었음을 확인합니다(p2). 데이터를 버퍼에 넣고(p4) 소비자에게 시그널을 보냅니다(p5). 이때 대기 중이던 Tc1이 깨어나 실행 가능한 상태가 됩니다.
3. 하지만 Tc1이 바로 실행되는 것이 아니라 단지 실행 가능한 상태일 뿐입니다. 그 사이에 생산자 Tp는 계속 실행 중이며 다시 버퍼 상태를 검사합니다. 버퍼가 차 있으므로 이번엔 자신이 `wait()`을 호출하고(p3) 잠듭니다.
4. 이제 문제가 발생합니다. Tc1 대신 다른 소비자 Tc2가 먼저 실행될 수 있습니다. Tc2는 버퍼에서 데이터를 꺼내 가버립니다(c1-c6).
5. 이제 Tc1이 비로소 실행됩니다. 그러나 `get()`을 호출했을 때(c4) 버퍼는 이미 비어있습니다! Tc2가 먼저 데이터를 가져갔기 때문입니다.

여기서 핵심은 *Tc1이 시그널에 의해 깨어났을 때 조건이 여전히 만족된다는 보장이 없다*는 것입니다. 시그널은 쓰레드를 깨우기만 할 뿐, 깨어난 쓰레드가 즉시 실행된다는 보장은 없습니다. 이런 시그널 방식을 *Mesa semantics*라고 합니다. 반대 개념인 *Hoare semantics*는 구현하기는 더 어렵지만 깨어난 쓰레드가 바로 실행됨을 보장합니다.

#### 해결책: `if`에서 `while`로 변경

이 문제는 대기 루프의 `if`문을 `while`문으로 바꾸면 해결됩니다.

```c
cond_t cond;
mutex_t mutex;

void *producer(void *arg) {
	int i;
	for (i = 0; i < loops; i++) {
		Pthread_mutex_lock(&mutex); // p1
		while (count == 1) // p2
			Pthread_cond_wait(&cond, &mutex); // p3
		put(i); // p4
		Pthread_cond_signal(&cond); // p5
		Pthread_mutex_unlock(&mutex); // p6
	}
}

void *consumer(void *arg) {
	int i;
	for (i = 0; i < loops; i++) {
		Pthread_mutex_lock(&mutex); // c1
		while (count == 0) // c2
			Pthread_cond_wait(&cond, &mutex); // c3
		int tmp = get(); // c4
		Pthread_cond_signal(&cond); // c5
		Pthread_mutex_unlock(&mutex); // c6
		printf("%d\n", tmp);
	}
}
```

이제 Tc1이 시그널에 의해 깨어나면(lock을 획득한 상태로) 즉시 조건을 다시 검사합니다(`while (count == 0)`). 만약 그 사이에 Tc2가 데이터를 가져갔다면 Tc1은 다시 `wait()`을 호출하여 잠듭니다. 즉, Mesa semantics에서는 조건 검사에 `while`문을 쓰는 것이 정석입니다.

#### 두 번째 문제점

이 문제는 Tc1과 Tc2 두 소비자가 먼저 실행된 후 둘 다 `wait()` 상태(c3)에 있을 때 발생합니다.

1. 이때 생산자 Tp가 실행되어 데이터를 버퍼에 넣고(p4) 시그널을 보냅니다(p5). 시그널로 인해 소비자 중 하나(Tc1)가 깨어납니다.
2. 그러나 Tc1이 깨어나 실행되기 전에 다시 생산자 Tp에게 선점 당할 수 있습니다. 생산자는 버퍼가 차 있음을 보고 자신이 `wait()`을 호출합니다(p3).
3. 이제 소비자 Tc1이 실행됩니다. `wait()`에서 리턴 받은 후(c3) 조건을 재확인하고(c2) 버퍼에서 데이터를 꺼냅니다(c4). 그리고 시그널을 보내 대기 중인 쓰레드 중 하나를 깨웁니다(c5).
4. 그런데 만약 깨워진 쓰레드가 다른 소비자 Tc2라면 문제가 발생합니다. Tc2가 실행되어 빈 버퍼를 보고(c2) 다시 `wait()`을 호출합니다(c3).
5. 결국 데이터를 채워야 할 생산자는 대기 상태이고, 두 소비자도 모두 대기 상태에 빠집니다.

핵심은 *시그널 대상을 정확히 지정하지 않았기 때문*입니다. 데이터를 넣은 생산자가 깨어나야 하는데 다른 소비자가 깨어난 것이죠.

#### 해결책: 조건 변수를 2개 사용

각 조건에 대응하는 *별도의 컨디션 변수*를 사용하면 이 문제를 해결할 수 있습니다.

```c
cond_t empty, fill;
mutex_t mutex;

void *producer(void *arg) {
	int i;
	for (i = 0; i < loops; i++) {
		Pthread_mutex_lock(&mutex); // p1
		while (count == 1) // p2
			Pthread_cond_wait(&empty, &mutex); // p3
		put(i); // p4
		Pthread_cond_signal(&fill); // p5
		Pthread_mutex_unlock(&mutex); // p6
	}
}

void *consumer(void *arg) {
	int i;
	for (i = 0; i < loops; i++) {
		Pthread_mutex_lock(&mutex); // c1
		while (count == 0) // c2
			Pthread_cond_wait(&fill, &mutex); // c3
		int tmp = get(); // c4
		Pthread_cond_signal(&empty); // c5
		Pthread_mutex_unlock(&mutex); // c6
		printf("%d\n", tmp);
	}
}
```

이제 소비자는 `empty` 시그널을 보내 생산자를 깨우고, 생산자는 `fill` 시그널로 소비자를 깨웁니다.

### 버퍼 크기를 늘려서 병행성 향상시키기

지금까지 버퍼 크기를 1로 가정했는데, 이를 늘리면 생산자와 소비자가 서로를 기다리는 시간을 줄일 수 있습니다. 즉, 병행성이 향상됩니다.

먼저 원형 버퍼 자료구조와 `put()`, `get()` 함수를 아래와 같이 수정합니다.

```c
int buffer[MAX];
int fill = 0;
int use = 0;
int count = 0;

void put(int value) {
	buffer[fill] = value;
	fill = (fill + 1) % MAX;
	count++;
}

int get() {
	int tmp = buffer[use];
	use = (use + 1) % MAX;
	count--;
	return tmp;
}
```

그리고 생산자와 소비자의 대기 조건을 버퍼 크기에 맞게 수정합니다.

```c
cond_t empty, fill;
mutex_t mutex;

void *producer(void *arg) {
	int i;
	for (i = 0; i < loops; i++) {
		Pthread_mutex_lock(&mutex); // p1
		while (count == MAX) // p2
			Pthread_cond_wait(&empty, &mutex); // p3
		put(i); // p4
		Pthread_cond_signal(&fill); // p5
		Pthread_mutex_unlock(&mutex); // p6
	}
}

void *consumer(void *arg) {
	int i;
	for (i = 0; i < loops; i++) {
		Pthread_mutex_lock(&mutex); // c1
		while (count == 0) // c2
			Pthread_cond_wait(&fill, &mutex); // c3
		int tmp = get(); // c4
		Pthread_cond_signal(&empty); // c5
		Pthread_mutex_unlock(&mutex); // c6
		printf("%d\n", tmp);
	}
}
```

이제 생산자는 버퍼가 가득 차야(`count == MAX`) 기다리고, 소비자는 버퍼가 완전히 비어야(`count == 0`) 기다립니다.

버퍼 크기가 1일 때는 생산자가 넣으면 소비자가 곧바로 가져갔어야 했지만, 이제는 생산자가 여러 번 데이터를 넣고 난 후에야 소비자의 차례가 오게 됩니다. 즉, CPU를 양보하는 횟수가 줄어들어 성능이 향상됩니다.

## 컨디션 변수 사용 시 주의점

```{note}
팁 : 조건에 while 문을 사용하자 (if 문은 아니다)
멀티 쓰레드 프로그램에서 조건을 검사할 때에는 항상 while 문을 사용하는 것이 옳다. 시그널 전달의 의미에 따라 if 문을 사용하는 것은 맞을 수도 있을 뿐이다. 그러므로 항상 while 문을 사용하자. 그러면 작성한 코드가 의도한 대로 동작할 것이다.
조건 검사에 while 문을 사용하는 것은 거짓으로 깨운 경우 (spurious wakeup)에 대처할 수 있도록 해 준다.
어떤 쓰레드 패키지는 구현상의 문제로 하나의 시그널에 의해서 두 개의 쓰레드가 깨어나는 경우도 가능하다.
쓰레드가 조건을 재검사해야 하는 이유는 바로 이런 거짓으로 깨운 경우 때문이다.
```

아래는 멀티 쓰레드 기반 메모리 할당 라이브러리의 간단한 예제 코드입니다.

```c
// 몇 byte나 힙이 비었는가?
int bytesLeft = MAX_HEAP_SIZE;
cond_t c;
mutex_t m;

void *allocate(int size) {
	Pthread_mutex_lock(&m);
	while (bytesLeft < size)
		Pthread_cond_wait(&c, &m);
	void *ptr = . . . ; // 힙에서 메모리를 할당 받음
	bytesLeft -= size;
	Pthread_mutex_unlock(&m);
	return ptr;
}

void free(void *ptr, int size) {
	Pthread_mutex_lock(&m);
	bytesLeft += size;
	Pthread_cond_signal(&c); // 시그널 전달 대상은?..
	Pthread_mutex_unlock(&m);
}
```

메모리 할당 함수 `allocate()`를 호출하면 요청한 크기만큼 힙에 여유 공간이 생길 때까지 기다려야 할 수 있습니다.
반대로 `free()`를 호출하여 메모리를 반환하면 힙에 사용 가능한 공간이 늘어났음을 알리는 시그널을 보냅니다.

그런데 이 코드에는 문제가 있습니다. 어떤 쓰레드를 깨워야 할까요?

쓰레드 Ta는 100 바이트를 요청했고 Tb는 10 바이트를 요청해서 둘 다 대기 중인데, 다른 쓰레드가 50 바이트를 반환했다고 해봅시다.
이때는 Ta가 아니라 Tb를 깨워서 메모리를 할당해 주어야 합니다. 그런데 앞서 배운 컨디션 변수 방식으로는 이를 제어할 수 없습니다.

이런 상황을 *Covering Condition(포괄 조건)*이라고 부르는데, 쓰레드를 깨워야 할 모든 경우의 수를 포괄하는 조건이 없기 때문입니다.

이 문제에 대한 Lampson과 Redell의 해법은 의외로 간단합니다. 시그널 함수 `pthread_cond_signal()`을 브로드캐스트 함수 `pthread_cond_broadcast()`로 대체하는 것입니다.

```c
void free(void *ptr, int size) {
	Pthread_mutex_lock(&m);
	bytesLeft += size;
	Pthread_cond_broadcast(&c); // 모든 쓰레드를 깨움
	Pthread_mutex_unlock(&m);
}
```

브로드캐스트를 사용하면 대기 중인 모든 쓰레드가 깨어나서 조건을 재검사하게 됩니다. 조건이 만족되지 않으면 해당 쓰레드는 다시 대기 상태로 돌아갑니다.

물론 이는 많은 쓰레드가 불필요하게 깨어나는 오버헤드가 있습니다. 하지만 모든 경우를 포괄할 수 있는 깔끔한 해법입니다.

일반적으로 `signal`을 `broadcast`로 바꿔야만 프로그램이 제대로 동작한다면, 그건 버그일 확률이 높습니다. 앞서 다룬 생산자-소비자 문제에서도 브로드캐스트를 쓸 수는 있었지만 조건 변수를 2개 사용하는 것이 더 나은 해법이었던 것처럼 말이죠.

그러나 위의 메모리 할당 문제처럼 모든 깨움 조건을 명시할 수 없는 경우에는 브로드캐스트가 최선의 선택이 될 수 있습니다.

## 요약

이 글에서는 락 못지않게 중요한 동기화 도구인 컨디션 변수에 대해 알아보았습니다.

프로그램의 상태가 기대와 다를 때 쓰레드를 대기시키는 컨디션 변수는 동기화 문제를 우아하게 해결하는 강력한 도구입니다.
특히 생산자-소비자 문제나 포괄 조건 문제 등에서 컨디션 변수의 진가를 발휘합니다.

컨디션 변수를 사용할 때는 다음 사항들을 명심합시다.

- 조건 검사는 항상 `while`문으로 할 것
- Mesa semantics의 한계를 이해할 것
- 시그널 대상을 명확히 할 것
- 포괄 조건이 필요할 땐 주저 없이 브로드캐스트를 사용할 것

병행 프로그래밍은 어렵지만, 이런 기본기를 잘 익혀두면 동기화 문제에 휘둘리지 않고 능숙하게 멀티 쓰레드 프로그램을 작성할 수 있을 것입니다.
