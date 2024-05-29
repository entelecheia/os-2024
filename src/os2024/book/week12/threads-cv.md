# 컨디션 변수[^ypilseong]

[^ypilseong]: [양필성](https://github.com/ypilseong)

우리가 배운 _‘락’_ 하나만 가지고는 제대로 병행 프로그램을 작성할 수 없습니다. 쓰레드가 계속 진행하기 전에 특정 조건이 만족되었는지 검사가 필요한 경우가 있습니다. 예를 들면 자식 쓰레드가 작업을 끝냈는지 여부를 알 필요가 있습니다. 이런 걸 어떻게 구현할 수 있을까요?

```c
volatile int done = 0;

void *child(void *arg) {
	printf(“child\n ”);
	done = 1;
	return NULL;
}

int main(int argc, char *argv[]) {
	printf(“parent: begin\n ”);
	pthread_t c;
	Pthread_create(&c, NULL, child, NULL);
	while (done == 0)
	; // spin
	printf(“parent: end\n ”);
	return 0;
}
```

이렇게 공유 변수로 구현할 수 있다 하지만 부모 쓰레드가 `spin` 하면서 자원을 낭비하고 있습니다. 이 방법 대신 부모 쓰레드가 특정 조건이 만족될때까지 잠자면서 기다리는 것이 더 좋습니다.

## 정의와 루틴들

조건이 참이 될 때까지 기다리기 위해 컨디션 변수를 활용할 수 있다습니다. 션 변수는 일종으 큐 자료 구조로서, 어떤 실행의 상태 (또는 어떤 조건) 가 원하는 것과 다를 때 참이 되기를 기다리며 스레드가 대기할 수 있는 큐입니다. 다른 쓰레드가 상태를 변경시켰을 때, 대기 중이던 쓰레드를 깨우고, 계속 진행할 수 있도록 합니다.

`pthread_cond_t c;` 라고 써서 `c`가 컨디션 변수가 되도록 선언하고 초기화합니다. 컨디션 변수에는 `wait()` 과 `signal()` 이라는 두 가지 연산이 존재합니다.

`wait()` 은 쓰레드가 스스로를 잠재우기 위해 호출하는 것이고, `signal()`은 쓰레드가 무엇인가를 변경했기 때문에 조건이 참이 되기를 기다리며 잠자고 있던 쓰레드를 깨울 때 호출합니다.

```c
pthread_cond_wait(pthread_cond_t *c, pthread_mutex_t *m);
pthread_cond_signal(pthread_cond_t *c);
```

`wait()`에서 유의할 점은 `mutex`를 매개변수로 사용한다는 것입니다. 호출될 때 `mutex`는 잠겨있었다고 가정하자. `wait()`의 역할은 락을 해제하고 호출한 쓰레드를 재우는 것입니다. 어떤 다른 쓰레드가 시그널을 보내서 쓰레드가 깨어나면, `wait()`에서 리턴하기 전에 락을 재획득해야 한다.

즉, 조건이 만족되어 잠에서 깨어났더라고 락을 획득하지 못하면 다시 잠에 드는 것입니다. 이렇게 복잡한 이유는 쓰레드가 스스로를 재우려고 할 때, 경쟁 조건의 발생을 방지하기 위해서입니다.

이해를 돕기 위해 예제를 살펴보자.

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
	printf(“child\n ”);
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
	printf(“parent: begin\n ”);
	pthread_t p;
	Pthread_create(&p, NULL, child, NULL);
	thr_join();
	printf(“parent: end\n ”);
	return 0;
}
```

```notice
부모 쓰레드가 조건을 검사할 때 `if`문이 아니라 `while`문을 사용합니다.
```

### 첫 번째 케이스

부모 쓰레드가 자식 쓰레드를 생성하고, 계속 실행하며 `thr_join()`을 호출하고, 자식 쓰레드가 끝나기를 기다리는 경우입니다. 이 경우 부모 쓰레드가 락을 획득하고 자식이 끝났는지 검사한 후에 자식이 끝나지 않았으므로 `wait()`을 호출하여 스스로를 잠재우고, 락을 해제합니다. 자식 쓰레드가 추후 실행되어 `thr_exit()`을 호출하여 부모 쓰레드를 깨웁니다. 호출했던 `wait()`에서 락을 획득한 채로 리턴하여 부모 쓰레드가 실행되고, 락을 해제한 후 종료됩니다.

### 두 번째 케이스

자식 쓰레드가 생성되면서 즉시 실행되고, `done` 을 1로 설정하고, 자고 있는 쓰레드를 깨우기 위해 시그널을 보냅니다. 하지만 자고 있는 쓰레드가 없기 때문에 그냥 리턴합니다. 그 후 부모 쓰레드가 실행되고 `thr_join()`을 호출하고 `done`이 1이므로 바로 리턴합니다.

`thr_exit()`, `thr_join()`의 중요성을 이해할 수 있도록 몇 가지 구현의 방식을 살펴보자.

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

이런 식으로 작성된 경우는 두 번째 케이스, 자식 쓰레드가 생성된 즉시 실행되어 `thr_exit()`을 호출하는 경우에 제대로 작동하지 않습니다. 자식 프로세스가 시그널을 보내지만, 깨울 쓰레드가 없어 리턴됩니다. 부모 쓰레드는 `wait()`을 호출하고 거기서 멈춰있게 됩니다.

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

이런 식으로 작성된 경우, 경쟁 조건이 발생합니다. 부모 쓰레드가 `thr_join()`을 호출하고 나서 `done`이 `0`인 것을 확인하고 `wait()`을 호출하기 직전에 인터럽트에 걸려 자식 쓰레드가 실행되었다고 해보자. 자식 쓰레드는 `done`을 `1`로 변경하고 시그널을 보내지만 대기 중인 쓰레드가 없습니다. 다시 부모 쓰레드가 실행되면, `wait()`을 호출하고 잠에 들지만 아무도 깨워 줄 수 없습니다.

## 생산자/소비자 (유한 버퍼) 문제

다음으로 살펴볼 동기화 문제는 Dijkstra가 처음 제시한 생산자/소비자(producer/consumer) 문제입니다. 유한 버퍼(bounded 버퍼) 문제로도 알려져 있습니다. `lock` 대신 일반화된 세마포어를 발명하게 된 이유가 이 문제 때문입니다.

여러 개의 생산자 쓰레드와 소비자 쓰레드가 있다고 하자. 생산자는 데이터를 만들어 버퍼에 넣고, 소비자는 버퍼에서 데이터를 꺼내어 사용합니다. 이러한 관계는 실제로 시스템에서 자주 일어납니다. 예를 들어 멀티 쓰레드 웹 서버의 경우 생산자는 HTTP 요청을 작업 큐 (유한 버퍼) 에 넣고, 소비자 쓰레드는 이 큐에서 요청을 꺼내어 처리합니다.

`grep foo file.txt | wc -l`와 같은 문장처럼 파이프 명령으로 한 프로그램의 결과를 다른 프로그램에게 전달할 때도 유한 버퍼를 사용합니다. UNIX 쉘은 출력 결과를 _UNIX 파이프_ 라는 곳으로 전송합니다. 파이프의 한쪽 끝에는 `wc` 프로세스의 표준 입력과 연결되어 있습니다. `grep` 프로세스가 생산자가 되고 `wc` 프로세스가 소비자가 됩니다.

유한 버퍼는 공유 자원이고, 경쟁 조건의 발생을 방지하기 위해 동기화가 필요합니다. 한 개의 정수를 사용하고, 공유 버퍼에 값을 넣는 함수, 값을 꺼내는 함수 두 개가 있습니다.

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
		printf(“%d\n ”, tmp);
	}
}
```

컨디션 변수 하나와 그것과 연결된 `mutex` 락을 사용하는 방식을 먼저 시도해보자.

생산자는 버퍼가 빌 때까지 기다립니다. 소비자도 버퍼가 차기를 기다립니다. 생산자와 소비자가 각 하나씩인 경우에 위의 코드는 정상적으로 동작합니다. 하지만 생산자, 소비자가 두 개 이상씩 존재하는 경우에는 문제가 있습니다.

### 첫 번째 문제

대기 명령 전의 `if` 문과 관련이 있다.

Tc1 과 Tc2 라는 두 개의 소비자가 있고 Tp 라는 생산자가 하나 있다고 가정하자. 소비자 (Tc1)가 먼저 실행됩니다. 락 (c1) 을 획득하고 버퍼를 소비할 수 있는지 검사한다 (c2). 그리고 비어있음을 확인한 후에 대기하며 (c3) 락을 해제합니다. 그리고 생산자 (Tp)가 실행됩니다. 락을 획득하고 (p1) 버퍼가 비었는지 확인합니다 (p2). 비었음을 발견하고, 버퍼를 채웁니다 (p4). 생산자는 버퍼가 가득 찼다는 시그널을 보냅니다 (p5). 대기 중인 첫째 소비자 (Tc1)는 깨어나 준비 큐 (ready queue)로 이동합니다. Tc1 은 이제 실행할 수 있는 상태이지만 아직 실행 상태는 아닙니다. 생산자는 실행을 계속합니다. 버퍼가 차 있으므로 대기 상태로 전이합니다 (p6, p1-p3).

여기에서 문제가 발생합니다. 다른 소비자 (Tc2)가 끼어들어서 실행하면서 버퍼 값을 소비합니다 (c1, c2, c4, c5, c6을 수행, c3은 버퍼가 가득 찼기 때문에 건너뜀). Tc1 이 실행된다고 해보자. 대기에서 리턴하기 전에 락을 획득합니다. 그리고 get()을 호출하지만 (c4) 버퍼는 비었습니다. 코드는 의도한 대로 기능하지 못했습니다. 생산자가 버퍼에 넣어 둔 값을 Tc2 가 끼어들어서 소비하였기 때문에 Tc1 이 비어 있는 버퍼를 읽는 행위를 막았어야 했습니다.

문제의 원인은 *Tc1이 깨어나서 실행되기까지의 사이에 유한 버퍼의 상태가 변경되었기 때문*입니다. 시그널은 쓰레드를 깨우기만 하고, 깨어난 쓰레드가 실제 싱행되는 시점에 그 상태가 유지된다는 보장은 없습니다. 이런 식으로 시그널을 정의하는 것을 *Mesa Semantic*이라 합니다. 대비되는 개념은 Hoare Semantic인데 구현하기는 더 어렵지만 깨어난 즉시 쓰레드가 실행되는 것을 보장합니다.

### 해결 방법: if -> while

`if` 문을 `while` 문으로 변경하면 이 문제를 해결할 수 있습니다.

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
		printf(“%d\n ”, tmp);
	}
}
```

소비자 Tc1 이 깨어나서 (락을 획득한 상태), 즉시 공유 변수의 상태를 재확인 합니다 (c2). 만약 이 시점에 버퍼가 비어 있다면, 소비자는 대기 상태로 돌아갑니다 (c3). 문제가 해결되었습니다. Mesa semantic의 컨디션 변수에서 가장 기본적인 법칙은 언제나 `while` 문을 사용하라는 것입니다.

### 두 번째 문제

이 문제는 소비자 쓰레드 Tc1 과 Tc2 가 먼저 실행한 후에 둘 다 대기 상태에 있을 때 발생합니다 (c3).

생산자가 실행되어 버퍼에 값을 넣고 대기 중인 쓰레드 하나를 깨우고 (Tc1 을 깨웠다고 하자), 자신은 대기합니다. 이제 하나의 소비자 (Tc1)가 실행할 준비가 되었고 조건에 의해 Tc2 와 Tp 는 대기 중입니다. 이제 문제가 발생하도록 만들 것입니다.

소비자 Tc1이 wait()에서 리턴을 받아 깨어나고 (c3) 조건을 재확인합니다 (c2). 버퍼가 차있다는 것을 발견하고 값을 소비합니다 (c4). 이 소비자는 시그널을 전송하여 (c5) 대기중인 쓰레드 중 하나를 깨운웁니다. 이때 어떤 쓰레드를 깨울 것인가?

소비자가 버퍼를 비웠기 때문에 생산자를 당연히 깨워야 합니다. 하지만, 만약 소비자 Tc2 를 깨운다면 (대기 큐가 어떻게 관리되느냐에 따라 당연히 발생할 수 있습니다), 문제가 발생합니다. 소비자 Tc2 가 깨어나면 버퍼가 비어 있다는 것을 발견한 후에 (c2) 다시 대기 상태로 들어갑니다 (c3). 버퍼에 값을 넣어야 하는 생산자 Tp 는 대기 중입니다. 다른 소비자 쓰레드 Tc1 역시 대기 상태에 들어갑니다. 세 개의 쓰레드가 모두 대기 상태입니다.

```notice
시그널을 보내는 것은 꼭 필요하지만 대상이 명확해야 합니다. 소비자는 다른 소비자를 깨울 수 없고 생산자만 깨워야 하며, 반대로 생산자의 경우도 마찬가지입니다.
```

### 단일 버퍼 생산자/소비자 해법

*두 개의 컨디션 변수를 사용*하여 시스템의 상태가 변경되었을 때 깨워야 하는 쓰레드에게만 시그널을 제대로 전달합니다.

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
		printf(“%d\n ”, tmp);
	}
}
```

### 최종적인 생산자/소비자 해법

마지막 변경을 통해 병행성을 증가시키고 효율적으로 만들어 보자. 버퍼 공간을 추가하여 대기 상태에 들어가기 전에 여러 값들이 생산될 수 있도록 하는 것, 그리고 마찬가지로 여러 개의 값이 대기 상태 전에 생산될 수 있도록 하는 것입니다.

우선 다음과 같이 버퍼 구조와 `put()`, `get()` 함수를 변경하였습니다.

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
	count−−;
	return tmp;
}
```

생산자와 소비자의 대기 상태 로직도 변경하였습니다.

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
		printf(“%d\n ”, tmp);
	}
}
```

생산자는 모든 버퍼가 현재 가득 차 있다면 대기 상태에 들어가고, 소비자도 모든 버퍼가 비어 있다면 대기에 들어갑니다.

## 컨디션 변수 사용 시 주의점

```notice
팁 : 조건에 while 문을 사용하자 (if 문은 아니다) 멀티 쓰레드 프로그램에서 조건을 검사할 때에는 항상 while 문을 사용하는 것이 옳다. 시그널 전달의 의미에 따라 if 문을 사용하는 것은 맞을 수도 있을 뿐이다. 그러 므로 항상 while 문을 사용하자, 그러면 작성한 코드가 의도한 대로 동작할 것이다. 조건 검사에 while 문을 사용하는 것은 거짓으로 깨운 경우 (spurious wakeup) 에 대처할 수 있도록 해 준다. 어떤 쓰레드 패키지는 구현상의 문제로 하나의 시그널에 의해서 두 개의 쓰레드가 깨어나는 경우도 가능하다. 쓰레드가 조건을 재검사해야 하는 이유는 거짓으로 깨운 경우가 있기 때문이다.
```

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
	bytesLeft −= size;
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

멀티 쓰레드 기반 미모리 할당 라이브러리 예제입니다. 메모리 할당 코드를 호출하면, 공간이 생길 때까지 기다려야 할 수 있습니다. 또 쓰레드가 메모리 반납시 사용 가능한 메모리 공간의 발생을 알리는 시그널을 생성할 수 있습니다. 하지만 이 코드에는 문제가 있다 어떤 쓰레드가 깨어나야 할까요?

쓰레드 Ta는 100을 할당받길 원하고, 쓰레드 Tb는 10을 할당받길 원하는 상태에서, 어떤 쓰레드가 50만큼 메모리를 반환한 경우, Ta가 깨어나면 안 되고 Tb가 깨어나야 합니다. 이런 문제는 두 개의 컨디션 변수를 사용해도 해결할 수 없습니다.

Lampson과 Redell이 제시한 해법은 단순합니다. `pthread_cond_signal()`을 대기 중인 모든 쓰레드를 깨우는 `pthread_cond_broadcast()`로 바꿔서 사용하면 된다. 그렇게 함으로써 깨어나야 할 쓰레드가 있다면 깨어날 수 있도록 합니다. 그렇게 깨어난 쓰레드들은 깨어나서 조건을 재검사하고, 즉시 대기 상태로 다시 들어갑니다.

Lampson과 Redell은 이런 경우를 포함 조건(covering condition)이라고 했습니다. 왜냐하면 (보수적으로) 쓰레드가 깨어나야 하는 모든 경우를 다 포함하기 때문입니다. 불필요하게 많은 쓰레드가 깨어나는 단점이 있습니다. 문맥 전환 오버헤드가 큽니다. 예리한 독자라면 이 방법을 앞에서 사용했을 수도 있다는 것을 알 것입니다 (컨디션 변수를 하나만 사용하는 생산자/소비자 문제를 보자). 하지만 그 경우에는 더 좋은 해법이 있었기 때문에 그 방법을 택했습니다. 일반적으로 시그널을 브로드캐스트 (broadcast)로 바꿨을 때만 프로그램이 동작한다면 아마도 버그가 존재하는 것입니다. 앞서 다룬 메모리 할당 문제의 경우 브로드캐스트를 적용하는 것이 가장 자명한 해법입니다.

## 요약

락 이상으로 중요한 동기화 기법인 컨디션 변수를 소개하였습니다. 프로그램 상태가 기대한 것과 다를 경우 쓰레드가 대기하도록 하여 컨디션 변수는 주요 동기화 문제 몇 가지를 깔끔하게 해결할 수 있습니다. 그 해법에는 그 유명한 생산자/소비자 그리고 포함 조건 문제도 포함합니다.
