# 세마포어

다양한 병행성 문제를 해결하기 위해서는 락(Lock)과 조건 변수(Condition Variable) 둘 다 필요합니다. 하지만 세마포어(Semaphore)라는 추상화된 개념을 사용하면 이 둘을 대신할 수 있습니다.

```{note}
핵심 질문 : 세마포어를 어떻게 사용하는가? 락과 조건 변수 대신에 세마포어를 사용하는 방법은 무엇인가? 세마포어의 정의는 무엇인가? 이진 세마포어는 무엇인가? 락과 조건 변수를 사용하여 세마포어를 구현하는 것이 가능한가? 그 반대로 세마포어를 사용하여 락과 조건 변수를 구현하는 것이 가능한가?
```

## 세마포어의 정의

세마포어는 음이 아닌 정수 값을 갖는 객체로서 두 가지 연산(`wait`와 `post`)을 통해 조작할 수 있습니다. POSIX 표준에서는 이 연산들을 각각 `sem_wait()`와 `sem_post()`로 정의하고 있습니다. 세마포어는 초기값에 따라 동작이 결정되기 때문에 사용 전에 반드시 초기화를 해야 합니다.

```c
#include <semaphore.h>

sem_t s;
sem_init(&s, 0, 1); // 세마포어 s를 1로 초기화
```

위 코드에서 `sem_init()` 함수의 두 번째 인자 0은 세마포어가 쓰레드 간에 공유됨을 의미합니다. 만약 프로세스 간 공유가 필요하다면 0이 아닌 값을 사용해야 합니다.

초기화 후에는 `sem_wait()`와 `sem_post()` 함수를 호출하여 세마포어를 조작할 수 있습니다.

```c
int sem_wait(sem_t *s) {
    // 세마포어 s의 값을 1 감소
    // 세마포어 s의 값이 음수가 되면 해당 쓰레드는 블록됨
}

int sem_post(sem_t *s) {
    // 세마포어 s의 값을 1 증가
    // 블록된 쓰레드가 있다면 그 중 하나를 깨움
}
```

`sem_wait()` 함수는 세마포어의 값이 양수일 때 즉시 리턴하고, 그렇지 않으면 세마포어의 값이 양수가 될 때까지 호출한 쓰레드를 블록시킵니다.
`sem_post()` 함수는 세마포어의 값을 증가시키고 블록된 쓰레드가 있다면 그 중 하나를 깨웁니다.
이 두 함수는 원자적(atomic)으로 동작한다고 가정합니다.

## 이진 세마포어 (Binary Semaphore)

이진 세마포어는 락(Lock)과 동일한 개념입니다. 이진 세마포어의 값은 0 또는 1입니다.

```c
sem_t lock;
sem_init(&lock, 0, 1); // lock을 1로 초기화

sem_wait(&lock); // 락 획득
// 임계 영역(Critical Section)
sem_post(&lock); // 락 해제
```

이진 세마포어를 사용하여 상호 배제(Mutual Exclusion)를 달성할 수 있습니다. 한 쓰레드가 `sem_wait()`를 호출하여 세마포어의 값을 0으로 만들면, 다른 쓰레드들은 `sem_wait()`에서 블록됩니다. 임계 영역에 있는 쓰레드가 `sem_post()`를 호출하여 세마포어의 값을 1로 만들어야 다른 쓰레드가 임계 영역에 진입할 수 있습니다.

## 세마포어를 조건 변수로 사용하기

세마포어는 쓰레드 간 동기화를 위해서도 사용될 수 있습니다. 한 쓰레드가 특정 조건을 만족할 때까지 다른 쓰레드를 기다리게 하는데 사용할 수 있습니다.

```c
sem_t cond;

void *child(void *arg) {
    printf("Child\n");
    sem_post(&cond); // 시그널을 보냄
    return NULL;
}

int main(int argc, char *argv[]) {
    sem_init(&cond, 0, 0); // cond를 0으로 초기화
    printf("Parent: begin\n");
    pthread_t c;
    pthread_create(c, NULL, child, NULL);
    sem_wait(&cond); // 자식 쓰레드를 기다림
    printf("Parent: end\n");
    return 0;
}
```

위 코드에서 부모 쓰레드는 자식 쓰레드가 `sem_post()`를 호출하여 시그널을 보낼 때까지 `sem_wait()`에서 블록됩니다. 이는 조건 변수의 `wait()`와 `signal()` 연산과 유사합니다.

## 생산자-소비자 문제 (Producer-Consumer Problem)

생산자-소비자 문제를 세마포어로 해결하는 방법을 살펴보겠습니다.

```c
#define MAX 10 // 버퍼의 크기

int buffer[MAX];
int fill = 0;
int use = 0;

sem_t empty;
sem_t full;
sem_t mutex;

void put(int value) {
    buffer[fill] = value;
    fill = (fill + 1) % MAX;
}

int get() {
    int tmp = buffer[use];
    use = (use + 1) % MAX;
    return tmp;
}

void *producer(void *arg) {
    for (int i = 0; i < MAX; i++) {
        sem_wait(&empty);
        sem_wait(&mutex);
        put(i);
        sem_post(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *arg) {
    for (int i = 0; i < MAX; i++) {
        sem_wait(&full);
        sem_wait(&mutex);
        int tmp = get();
        sem_post(&mutex);
        sem_post(&empty);
        printf("%d\n", tmp);
    }
}

int main(int argc, char *argv[]) {
    sem_init(&empty, 0, MAX); // 처음에는 모든 버퍼가 비어 있음
    sem_init(&full, 0, 0); // 처음에는 찬 버퍼가 없음
    sem_init(&mutex, 0, 1);

    pthread_t p, c;
    pthread_create(p, NULL, producer, NULL);
    pthread_create(c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    return 0;
}
```

생산자-소비자 문제에는 세 개의 세마포어가 사용됩니다.

- `empty`: 비어있는 버퍼의 개수를 나타냅니다. 초기값은 버퍼의 크기(`MAX`)입니다.
- `full`: 채워진 버퍼의 개수를 나타냅니다. 초기값은 0입니다.
- `mutex`: 버퍼에 대한 상호 배제를 제공합니다. 초기값은 1입니다.

생산자는 `empty` 세마포어를 통해 비어있는 버퍼가 있는지 확인하고, `mutex` 세마포어로 버퍼에 대한 독점적 접근을 보장받습니다. 그 후 버퍼에 데이터를 넣고(`put()`) `mutex`를 해제하고 `full` 세마포어의 값을 증가시킵니다.

소비자는 `full` 세마포어를 통해 소비할 데이터가 있는지 확인하고, `mutex` 세마포어로 버퍼에 대한 독점적 접근을 보장받습니다. 그 후 버퍼에서 데이터를 꺼내고(`get()`) `mutex`를 해제하고 `empty` 세마포어의 값을 증가시킵니다.

이렇게 세마포어를 사용하면 생산자와 소비자가 버퍼에 동시에 접근하는 것을 방지할 수 있고, 생산자는 버퍼가 가득 차면 기다리고 소비자는 버퍼가 비어있으면 기다리게 할 수 있습니다.

## 식사하는 철학자들 문제 (Dining Philosophers Problem)

식사하는 철학자들 문제는 병행성 제어와 동기화를 설명하기 위해 자주 사용되는 예제입니다. 이 문제에서 철학자들은 생각하고 식사하기를 반복합니다. 식사를 하려면 좌우에 있는 포크를 모두 들어야 합니다.

```c
#define N 5 // 철학자의 수

sem_t forks[N]; // 포크를 나타내는 세마포어

void *philosopher(void *arg) {
    int i = *(int *)arg;
    while (1) {
        think();
        sem_wait(&forks[i]); // 왼쪽 포크를 듦
        sem_wait(&forks[(i + 1) % N]); // 오른쪽 포크를 듦
        eat();
        sem_post(&forks[(i + 1) % N]); // 오른쪽 포크를 내려놓음
        sem_post(&forks[i]); // 왼쪽 포크를 내려놓음
    }
}

int main(int argc, char *argv[]) {
    pthread_t philosophers[N];
    int ids[N];

    for (int i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1); // 각 포크는 사용 가능한 상태에서 시작
    }

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}
```

각 철학자는 무한 루프를 돌면서 다음을 반복합니다:

1. 생각한다 (`think()`)
2. 왼쪽 포크를 든다 (`sem_wait(&forks[i])`)
3. 오른쪽 포크를 든다 (`sem_wait(&forks[(i+1)%N])`)
4. 식사를 한다 (`eat()`)
5. 오른쪽 포크를 내려놓는다 (`sem_post(&forks[(i+1)%N])`)
6. 왼쪽 포크를 내려놓는다 (`sem_post(&forks[i])`)

여기서 포크는 세마포어로 나타내어지며, 각 포크의 초기값은 1입니다 (사용 가능한 상태). 철학자가 포크를 들려면 해당 세마포어에 대해 `sem_wait()`를 호출해야 하고, 이는 포크가 사용 가능해질 때까지 철학자를 블록시킵니다.

하지만 위 코드는 교착상태(Deadlock)에 빠질 수 있습니다. 모든 철학자가 동시에 왼쪽 포크를 들어버리면, 아무도 오른쪽 포크를 들 수 없어 모두 굶어죽게 됩니다. 이를 해결하기 위한 한 가지 방법은 철학자 중 한 명이 포크를 드는 순서를 바꾸는 것입니다.

```c
void *philosopher(void *arg) {
    int i = *(int *)arg;
    while (1) {
        think();
        if (i == N - 1) {
            sem_wait(&forks[(i + 1) % N]); // 마지막 철학자는 오른쪽 포크를 먼저 듦
            sem_wait(&forks[i]);
        } else {
            sem_wait(&forks[i]);
            sem_wait(&forks[(i + 1) % N]);
        }
        eat();
        sem_post(&forks[(i + 1) % N]);
        sem_post(&forks[i]);
    }
}
```

이렇게 하면 순환 대기(Circular Wait) 조건이 깨지므로 교착 상태를 예방할 수 있습니다.

## 세마포어의 구현

세마포어는 보통 더 저수준의 동기화 도구인 락과 조건 변수를 사용하여 구현될 수 있습니다.

```c
typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Semaphore;

void sem_init(Semaphore *sem, int value) {
    sem->value = value;
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
}

void sem_wait(Semaphore *sem) {
    pthread_mutex_lock(&sem->mutex);
    while (sem->value <= 0) {
        pthread_cond_wait(&sem->cond, &sem->mutex);
    }
    sem->value--;
    pthread_mutex_unlock(&sem->mutex);
}

void sem_post(Semaphore *sem) {
    pthread_mutex_lock(&sem->mutex);
    sem->value++;
    pthread_cond_signal(&sem->cond);
    pthread_mutex_unlock(&sem->mutex);
}
```

이 구현에서 세마포어는 정수값(`value`), 뮤텍스(`mutex`), 그리고 조건 변수(`cond`)로 이루어져 있습니다.

- `sem_init()` 함수는 세마포어의 값을 초기화하고, 뮤텍스와 조건 변수를 초기화합니다.

- `sem_wait()` 함수는 먼저 뮤텍스를 획득하고, 세마포어의 값이 0보다 클 때까지 조건 변수에서 기다립니다. 그런 다음 세마포어의 값을 감소시키고 뮤텍스를 해제합니다.

- `sem_post()` 함수는 뮤텍스를 획득하고, 세마포어의 값을 증가시킨 후, 조건 변수에 시그널을 보내서 기다리고 있는 쓰레드 중 하나를 깨웁니다. 그리고 뮤텍스를 해제합니다.

이 구현은 Dijkstra의 원래 세마포어 개념과는 약간 다릅니다. 원래 개념에서는 세마포어의 값이 음수일 수 있으며, 그 절댓값은 세마포어에서 대기 중인 프로세스의 수를 나타냅니다. 하지만 여기서는 세마포어의 값이 음수가 되는 것을 허용하지 않습니다. 이는 POSIX 세마포어의 일반적인 구현 방식입니다.

## 요약

- 세마포어는 정수 값을 가지며 두 가지 연산 `wait`(또는 `P`)와 `post`(또는 `V`)를 지원하는 추상 데이터 타입입니다.
- 세마포어는 `wait` 연산에서 그 값이 0보다 클 때까지 호출 프로세스를 블록시킬 수 있습니다.
- 이진 세마포어는 상호 배제를 달성하는 데 사용될 수 있습니다.
- 세마포어는 조건 변수와 유사하게 쓰레드 간 동기화를 위해 사용될 수 있습니다.
- 생산자-소비자 문제와 식사하는 철학자 문제는 세마포어를 사용하여 해결할 수 있습니다.
- 세마포어는 일반적으로 락과 조건 변수를 사용하여 구현됩니다.

세마포어는 병행 프로그래밍에서 강력하고 유연한 도구입니다. 세마포어만으로 많은 동기화 문제를 해결할 수 있기 때문에, 일부 개발자들은 락과 조건 변수 대신 세마포어를 선호하기도 합니다.
