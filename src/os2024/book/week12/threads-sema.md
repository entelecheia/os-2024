# 세마포어


다양한 범주의 병행성 문제 해결을 위해서는 락과 조건 변수 둘 다 필요합니다.


```notice
핵심 질문 : 세마포어를 어떻게 사용하는가 락과 컨디션 변수 대신에 세마포어를 사용하는 방법은 무엇인가? 세마포어의 정의는 무엇인가? 이진 세마포어는 무엇인가? 락과 컨디션 변수를 사용하여 세마포어를 만드는 것이 가능한가? 그 반대로 세마포어를 사용하여 락과 조건 변수를 만드는 것이 가능한가?
```


## 세마포어: 정의


세마포어는 정수 값을 갖는 객체로서 두 개의 루틴으로 조작할 수 있습니다. POSIX 표준에서 이 두 개의 루틴은 `sem_wait()`, `sem_post()`이다. 세마포어는 초깃값에 의해 동작이 결정되기 때문에 사용 전에 초기화를 해야 합니다.
```sh
#include <semaphore.h>
sem_t s;
sem_init(&s, 0, 1);
```
세마포어 `s`를 선언 후 3번째 인자로 1을 전달하여 세마포어의 값을 1로 초기화합니다. `sem_init()`의 두 번째 인자는 모든 예제에서 0입니다. 이 값은 같은 프로세스 내의 쓰레드 간에 세마포어를 공유한다는 뜻입니다. 다른 값을 사용하는 예시 (다른 프로세스간 동기화 제공) 는 docs를 읽어보자.

초기화된 후에는 `sem_wait()`, `sem_post()`라는 함수를 호출하여 세마포어를 다룰 수 있습니다.


```sh
int sem_wait(sem_t *s) {
	decrement the value of semaphore s by one;
	wait if value of semaphore s is negative;
}

int sem_post(sem_t *s) {
	increment the value of semaphore s by one;
	if there are one or more threads waiting, wake one;
}
```
`sem_wait()` 함수는 즉시 리턴하거나 (세마포어의 값이 1 이상이면), 아니면 해당 세마포어 값이 1 이상이 될 때까지 호출자를 대기시킵니다. 다수의 쓰레드들이 `sem_wait()`을 호출할 수 있기 때문에, 대기큐에는 다수의 쓰레드가 존재할 수 있습니다. 대기하는 법에는 회전과 재우기 두 가지가 있다는 것을 기억하자.
`sem_post()` 함수는 대기하지 않습니다. 세마포어 값을 증가시키고 대기 중인 쓰레드 중 하나를 깨웁니다.
세마포어가 음수라면 그 값은 현재 대기 중인 쓰레드의 개수와 같습니다.
이 두 개의 함수는 atomic하게 실행된다고 가정합니다.


## 이진 세마포어 (락)


우리가 처음으로 세마포어를 적용할 곳은 ‘락’입니다.
```sh
sem_t m;
sem_init(&m, 0, X); // X로 세마포어 초기화하기.
sem_wait(&m);
// 임계 영역
sem_post(&m);
```
`X`는 어떤 값이 되어야 할까? 조금만 생각해봐도 1이 되어야 한다는 사실을 알 수 있습니다.

쓰레드가 2개라고 가정해보자. 첫 번째 쓰레드가 세마포어 값을 1 감소시켜 0으로 만듭니다. 세마포어의 값이 음수가 아니므로 리턴하고 진행합니다. 첫 번째 쓰레드가 임계 영역에 있을 때 두 번째 쓰레드가 `sem_wait()`을 호출하여 임계 영역 진임을 시도한다면 어떻게 될까?

이 경우 두 번째 쓰레드가 세마포어 값을 -1로 감소시키고, 대기에 들어갑니다. 첫 번째 쓰레드가 다시 실행되면 `sem_post()`를 호출하고 세마포어 값을 0으로 증가시키고, 잠자던 두 번째 쓰레드를 깨웁니다.락은 두 개의 상태 (사용 가능, 사용 중) 만 존재하므로 *이진 세마포어 (binary semaphore)* 라고도 불립니다.

## 컨디션 변수로서의 세마포어


어떤 조건이 참이 되기를 기다리기 위해 현재 쓰레드를 멈출 때에도 세마포어는 유용하게 사용됩니다.
```sh
sem_t s;

void *child(void *arg) {
	printf(“child\n ”);
	sem_post(&s); // 시그널 전달: 동작 끝
	return NULL;
}

int main(int argc, char *argv[]) {
	sem_init(&s, 0, X); // X의 값은 무엇이 되어야 할까?
	printf(“parent: begin\n ”);
	pthread_t c;
	Pthread_create(c, NULL, child, NULL);
	sem_wait(&s); // 자식을 여기서 대기
	printf(“parent: end\n ”);
	return 0;
}
```
다음과 같은 실행 결과를 얻고자 합니다.
```sh
parent: begin
child
parent: end
```
세마포어를 이용하여 어떻게 이 효과를 만들 수 있을까? 답은 의외로 간단합니다. 코드에서 볼 수 있듯이, 부모 프로세스는 자식 프로세스 생성 후 `sem_wait()`를 호출하여 자식의 종료를 대기합니다. 자식은 `sem_post()`를 호출하여 종료되었음을 알립니다.

중요한 질문이 있습니다. 세마포어 값을 무엇으로 초기화할까?

정답은 0입니다.

다음과 같은 두 가지 경우를 통해 그 이유를 알아보자.

1. 자식 프로세스 생성 후, 아직 자식 프로세스가 실행을 시작하지 않은 경우입니다 (준비 큐에만 들어 있고 실행 중이 아니다). 이 경우 자식이 `sem_post()`를 호출하기 전에 부모가 `sem_wait()`를 호출할 것입니다. 부모 프로세스는 자식이 실행될 때까지 대기해야 합니다. 이를 위해서는 `wait()` 호출 전에 세마포어 값이 0보다 같거나 작아야 합니다. 때문에 0이 초기값이 되어야 합니다. 부모가 실행되면 세마포어 값을 감소시키고 (-1로) 대기합니다. 자식이 실행되었을 때 `sem_post()`를 호출하여 세마포어의 값을 0으로 증가시킨 후 부모를 깨훕니다. 그러면 부모는 `sem_wait()`에서 리턴을 하여 프로그램을 종료시킵니다.

2. 부모 프로세스가 `sem_wait`()를 호출하기 전에 자식 프로세스의 실행이 종료된 경우입니다. 이 경우, 자식이 먼저 `sem_post()`를 호출하여 세마포어의 값을 0에서 1로 증가시킵니다. 부모가 실행할 수 있는 상황이 되면 `sem_wait()` 를 호출합니다. 세마포어 값이 1인 것을 발견할 것입니다. 부모는 세마포어 값을 0으로 감소시키고 `sem_wait()`에서 대기 없이 리턴합니다. 이 방법 역시 의도한 결과를 만들어냅니다.


## 생산자/소비자 (유한 버퍼) 문제


이 문제를 해결하기 위해 `empty`와 `full`이라는 두 개의 세마포어를 사용합니다.
```sh
int buffer[MAX];
int fill = 0;
int use = 0;

void put(int value) {
	buffer[fill] = value; // f1
	fill = (fill + 1) % MAX; // f2
}

int get() {
	int tmp = buffer[use]; // g1
	use = (use + 1) % MAX; // g2
	return tmp;
}
sem_t empty;
sem_t full;

void *producer(void *arg) {
	int i;
	for (i = 0; i < loops; i++) {
		sem_wait(&empty); // P1
		put(i); // P2
		sem_post(&full); // P3
	}
}

void *consumer(void *arg) {
	int i, tmp = 0;
	while (tmp != −1) {
		sem_wait(&full); // C1
		tmp = get(); // C2
		sem_post(&empty); // C3
		printf(“%d\n ”, tmp);
	}
}

int main(int argc, char *argv[]) {
	// . . .
	sem_init(&empty, 0, MAX); // empty는 MAX
	sem_init(&full, 0, 0); // full은 0
	// . . .
}
```


생산자와 소비자 쓰레드가 각 하나씩 있고 CPU도 하나인 상황에 대해 살펴보자. 소비자가 먼저 실행했다고 가정하면 소비자 쓰레드가 그림에서 `C1` 라인에 먼저 도달하여 `sem_wait(&full)`을 호출합니다. 변수`full`의 값은 0으로 초기화되었기 때문에 해당 명령으로 인해 `full`의 값은 -1로 감소되고, 소비자는 대기합니다. 다른 쓰레드가 `sem_post()`를 호출해서 `full` 변수가 증가하기를 기다려야 합니다.

그런 이후에 생산자 쓰레드가 실행하여 P1 라인에서 `sem_wait(&empty)` 루틴을 호출합니다. `empty` 변수가 `MAX` (이 경우에는 1)로 설정되었기 때문에 소비자와 다르게 생산자는 다음 문장을 계속 실행합니다. `empty` 변수는 감소하여 0이 되고 생산자가 데이터 값을 버퍼의 첫 번째 공간에 넣습니다 (P2 라인). 그런 후에 생산자는 P3 라인의 `sem_post(&full)`를 호출하여 `full` 세마포어의 값을 -1에서 0으로 변경하고 소비자 쓰레드를 깨웁니다 (대기 상태에서 준비 상태로 바뀝니다).

만약 `MAX` 값이 1보다 크고 생산자와 소비자 쓰레드들이 여러 개 있다면? 경쟁 조건이 발생할 것입니다. `put()`, `get()`에서 경쟁 조건이 발생합니다.

두 개의 생산자 Pa와 Pb가 있는데, 두 쓰레드가 `put()` 을 거의 동시에 호출하였다고 해 보자. Pa가 먼저 실행되어서 버퍼에 첫 공간에 값을 넣기 시작합니다 (f1 라인에서 `fill = 0`이다). Pa 쓰레드가 `fill` 카운터 변수를 1로 변경하기 전에 인터럽트가 걸렸습니다. 생산자 Pb가 실행되고 f1 라인에서 마찬가지로 버퍼의 첫 번째 공간에 데이터를 삽입합니다. Pa가 기록한 이전의 값은 새로운 값으로 대체됩니다.

### 상호 배제 추가
버퍼를 채우고 버퍼에 대한 인덱스를 증가하는 동작은 임계 영역이기 때문에 신중하게 처리해야 합니다. 지금까지 배운 이진 세마포어와 몇 개의 락을 추가하여 해결해보자.
```sh
sem_t empty;
sem_t full;
sem_t mutex;

void *producer(void *arg) {
	int i;
	for (i = 0; i < loops; i++) {
		sem_wait(&mutex); // P0
		sem_wait(&empty); // P1
		put(i); // P2
		sem_post(&full); // P3
		sem_post(&mutex); // P4
	}
}

void *consumer(void *arg) {
	int i, tmp = 0;
	while (tmp != −1) {
		sem_wait(&mutex); // C0
		sem_wait(&full); // C1
		tmp = get(); // C2
		sem_post(&empty); // C3
		sem_post(&mutex); // C4
		printf(“%d\n ”, tmp);
	}
}

int main(int argc, char *argv[]) {
	// . . .
	sem_init(&empty, 0, MAX); // empty는 MAX
	sem_init(&full, 0, 0); // full은 0
	sem_init(&mutex,0 ,1); // lock은 1로 초기화
	// . . .
}
```


이렇게 하면 교착 상태가 발생합니다.

생산자와 소비자 쓰레드가 각 하나씩 있다고 하자. 소비자가 먼저 실행이 되었습니다. `mutex`(c0 라인)를 획득하고 `full` 변수에 대하여 `sem_wait()`(c1 라인)를 호출합니다. 아직 데이터가 없기 때문에 소비자는 대기해야 하고 CPU를 양보해야 합니다.

여기서 중요한 것은 소비자가 아직도 락을 획득하고 있다는 것입니다. 생산자가 실행됩니다. 실행이 가능하면 데이터를 생성하고 소비자 쓰레드를 깨울 것입니다. 불행하게도 이 쓰레드는 먼저 `mutex` 세마포어에 대해서 `sem_wait()`를 실행합니다(p0 라인). 이미 락은 소비자가 획득한 상태이기 때문에 생산자 역시 대기에 들어갑니다.

순환 고리가 생겼습니다. 소비자는 `mutex`를 갖고 있으면서 다른 `full` 시그널을 발생시키기를 대기하고 있습니다. `full` 시그널을 발생시켜야 하는 생산자는 `mutex`에서 대기중입니다. 생산자와 소비자가 서로를 기다립니다. 전형적인 교착 상태입니다.

### 제대로 된 해법


이렇게 락의 범위를 줄여야 합니다. 뮤텍스가 임계 영역만 감싸도록 바꿔보자.
```sh
sem_t empty;
sem_t full;
sem_t mutex;

void *producer(void *arg) {
	int i;
	for (i = 0; i < loops; i++) {
		sem_wait(&empty); // p1
		sem_wait(&mutex); // p1.5 
		put(i); // p2
		sem_post(&mutex); // p2.5
		sem_post(&full); // p3
	}
}

void *consumer(void *arg) {
	int i;
	for (i = 0; i < loops; i++) {
		sem_wait(&full); // c1
		sem_wait(&mutex); // c1.5
		int tmp = get(); // c2
		sem_post(&mutex); // c2.5
		sem_post(&empty); // c3
		printf(“%d\n ”, tmp);
	}
}

int main(int argc, char *argv[]) {
	// . . .
	sem_init(&empty, 0, MAX); 
	sem_init(&full, 0, 0); 
	sem_init(&mutex, 0,1); 
	// . . .
}
```
멀티 쓰레드 프로그램에서 사용 가능한 유한 버퍼를 만들었습니다.


## Reader-Writer 락


삽입 연산은 리스트의 상태를 변경하고 (전통적인 임계 영역 보호 방식으로 해결 가능합니다), 검색은 자료 구조를 단순히 읽기만 합니다. 삽입 연산이 없다는 보장만 된다면 다수의 검색 작업을 동시에 수행할 수 있습니다. 이와 같은 경우를 위해 만들어진 락이 *reader-writer* 락이다
```sh
typedef struct _rwlock_t {
	sem_t lock; // 이진 세마포어 (기본 락)
	sem_t writelock; // 하나의 쓰기 또는 다수의 읽기 락을 위한 락
	int readers; // 임계 영역 내에 읽기를 수행중인 reader의 수
} rwlock_t;

void rwlock_init(rwlock_t *rw) {
	rw−>readers = 0;
	sem_init(&rw−>lock, 0, 1);
	sem_init(&rw−>writelock, 0, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw) {
	sem_wait(&rw−>lock); // readers를 변경하기 위해 이진 세마포어 사용
	rw−>readers++;
	if (rw−>readers == 1)
		sem_wait(&rw−>writelock); // 읽기용 락이 writelock 획득
	sem_post(&rw−>lock);
}

void rwlock_release_readlock(rwlock_t *rw) {
	sem_wait(&rw−>lock);
	rw−>readers−−;
	if (rw−>readers == 0)
		sem_post(&rw−>writelock); // 마지막으로 읽기용 락이 writelock 해제
	sem_post(&rw−>lock);
}

void rwlock_acquire_writelock(rwlock_t *rw) {
	sem_wait(&rw−>writelock);
}

void rwlock_release_writelock(rwlock_t *rw) {
	sem_post(&rw−>writelock);
}
```


이 코드는 간단합니다. 자료 구조를 “갱신”하려면 새로운 동기화 연산 쌍을 사용합니다. 락을 획득하기 위해서는 `rwlock_acquire_writelock()`을 사용하고 해제하기 위해서 `rwlock_release_writelock()`을 사용합니다. 내부적으로는 `writelock` 세마포어를 사용하여 하나의 쓰기 쓰레드만이 락을 획득할 수 있도록 하여, 임계 영역 진임 후에 자료 구조를 갱신합니다.

읽기 락을 획득시 읽기 쓰레드(reader)가 먼저 락을 획득하고 읽기 중인 쓰레드의 수를 표현하는 `reader` 변수를 증가시킵니다. 첫 번째 읽기 쓰레드가 읽기 락을 획득할 때 중요한 과정이 있습니다. 읽기 락을 획득시 `writelock` 세마포어에 대해 `sem_wait()`을 호출하여 쓰기 락을 함께 획득합니다. 획득한 쓰기 락은 읽기 락을 해제할 때 `sem_post()`로 다시 해제합니다.

이 과정을 통해서 읽기 락을 획득하고 난 후, 다른 읽기 쓰레드들이 읽기 락을 획득할 수 있도록 합니다. 다만, 쓰기 락을 획득하려는 쓰기 쓰레드 (writer)들은 모든 읽기 쓰레드가 끝날 때까지 대기하여야 합니다. 임계 영역을 빠져나오는 마지막 읽기 쓰레드가 “`writelock`”에 대한 `sem_post()`를 호출하여 대기 중인 쓰기 쓰레드가 락을 획득할 수 있도록 합니다.

이 방식은 몇가지 단점이 존재합니다.

쓰기 쓰레드에게 불리한 방식입니다. 쓰기 쓰레드에게 기아 현상이 발생할 수 있습니다. 쓰기 우선 정책 등을 사용해서 기아 현상을 줄일 수 있습니다.


```sh
typedef struct _rwlock_t {
    sem_t lock; // 기본 락
    sem_t writelock; // 쓰기 또는 읽기 락
    sem_t writer_sem; // 쓰기 작업을 위한 세마포어
    int readers; // 읽기 작업 수
    int waiting_writers; // 대기 중인 쓰기 작업 수
} rwlock_t;

void rwlock_init(rwlock_t *rw) {
    rw->readers = 0;
    rw->waiting_writers = 0;
    sem_init(&rw->lock, 0, 1);
    sem_init(&rw->writelock, 0, 1);
    sem_init(&rw->writer_sem, 0, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw) {
    sem_wait(&rw->writer_sem); // 쓰기 작업이 대기 중일 경우 차단
    sem_wait(&rw->lock);
    rw->readers++;
    if (rw->readers == 1) {
        sem_wait(&rw->writelock);
    }
    sem_post(&rw->lock);
    sem_post(&rw->writer_sem);
}

void rwlock_release_readlock(rwlock_t *rw) {
    sem_wait(&rw->lock);
    rw->readers--;
    if (rw->readers == 0) {
        sem_post(&rw->writelock);
    }
    sem_post(&rw->lock);
}

void rwlock_acquire_writelock(rwlock_t *rw) {
    sem_wait(&rw->writer_sem);
    rw->waiting_writers++;
    sem_post(&rw->writer_sem);

    sem_wait(&rw->writelock);

    sem_wait(&rw->writer_sem);
    rw->waiting_writers--;
    sem_post(&rw->writer_sem);
}

void rwlock_release_writelock(rwlock_t *rw) {
    sem_post(&rw->writelock);
}
```


이렇게 구현하면, 쓰기 작업이 대기 중일 때 새로운 읽기 작업이 락을 획득하지 못하게 됩니다. 이는 `writer_sem` 세마포어를 통해 관리됩니다. 이렇게 하면 쓰기 작업이 대기 중일 때 읽기 작업이 쌓이는 것을 방지할 수 있으므로 쓰기 작업의 기아 현상을 줄일 수 있습니다.
## 식사하는 철학자


![semaphores](./figs/OSTEP_31_Semaphores.jpeg)

다섯 명의 “철학자”가 식탁 주위를 둘러앉았습니다. 총 다섯 개의 포크가 철학자와 철학자 사이에 하나씩 놓여 있습니다. 철학자는 식사하는 때가 있고 생각하는 때가 있습니다. 생각 중일 때는 포크가 필요 없습니다. 자신의 왼쪽과 오른쪽에 있는 포크를 들어야 식사를 할 수 있습니다. 이 포크를 잡기 위한 경쟁과 그에 따른 동기화 문제가 병행 프로그래밍에서 다루려는 식사하는 철학자 문제입니다.
```sh
while (1) {
	think();
	getforks();
	eat();
	putforks();
}
```
주요 쟁점은 `getfork()`와 `putfork()`의 루틴을 작성하되 교착 상태의 발생을 방지해야 하고, 어떤 철학자도 못 먹어서 굶주리면 안되며 병행성이 높아야 합니다. 이 문제를 해결해보자.


```sh
int left(int p) { return p; }
int right(int p) { return (p + 1) % 5; }
```
이 문제를 해결하기 위해서 세마포어가 필요합니다. 각 포크마다 한 개씩 총 다섯 개가 있고 `sem_t fork[5]`로 정의합니다.


```sh
void getforks() {
	sem_wait(forks[left(p)]);
	sem_wait(forks[right(p)]);
}

void putforks() {
	sem_post(forks[left(p)]);
	sem_post(forks[right(p)]);
}
```
포크가 필요할 때 하나의 락을 획득합니다. 그리고 식사가 끝나면 순서대로 놓습니다. 이렇게 하면 교착 상태가 발생합니다. 각 철학자가 동시에 자기 왼쪽에 있는 포크를 집으면 평생 기다려야 합니다.

### 해답: 의존성 제거
간단한 해법 중 하나는 최소한 하나의 철학자가 다른 순서로 포크를 집도록 하는 것입니다.


```sh
void getforks() {
	if (p == 4) {
		sem_wait(forks[right(p)]); // 오른쪽 포크 먼저 집도록
		sem_wait(forks[left(p)]);
	} else {
		sem_wait(forks[left(p)]);
		sem_wait(forks[right(p)]);
	}
}
```
이렇게 해서 환형 대기 상태를 끊을 수 있습니다.


## 세마포어 구현


저수준 동기화 기법인 락과 컨디션 변수를 사용하여 우리만의 세마포어를 만들어 보자. *(Zemaphore)*
```sh
typedef struct __Zem_t {
	int value;
	pthread_cond_t cond;
	pthread_mutex_t lock;
} Zem_t;

// 오직 하나의 쓰레드만 이 문장을 호출할 수 있다.
void Zem_init(Zem_t *s, int value) {
	s−>value = value;
	Cond_init(&s−>cond);
	Mutex_init(&s−>lock);
}

void Zem_wait(Zem_t *s) {
	Mutex_lock(&s−>lock);
	while (s−>value <= 0)
		Cond_wait(&s−>cond, &s−>lock);
	s−>value−−;
	Mutex_unlock(&s−>lock);
}

void Zem_post(Zem_t *s) {
	Mutex_lock(&s−>lock);
	s−>value++;
	Cond_signal(&s−>cond);
	Mutex_unlock(&s−>lock);
}
```
Dijkstra가 정의한 세마포어와 여기서 정의한 제마포어 간의 중요한 차이 중 하나는 세마포어의 음수 값이 대기 중인 쓰레드의 수를 나타낸다는 부분입니다. 사실 제마포어에서는 이 값이 0 보다 작을 수가 없습니다. 이 방식이 구현하기도 쉽고 현재 Linux에 구현된 방식이기도 합니다.

세마포어를 사용하여 락과 컨디션 변수를 만드는 것은 매우 까다로운 문제입니다. 


## 요약


세마포어는 병행 프로그램 작성을 위한 박력하고 유연한 기법입니다. 어떤 개발자들은 간단하고 유용하다는 이유 때문에 락과 컨디션 변수가 아닌 세마포어만을 사용하기도 합니다.