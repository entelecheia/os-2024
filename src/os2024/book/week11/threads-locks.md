# 락 [^ypilseong]

[^ypilseong]: [양필성](https://github.com/ypilseong)

락(Lock)은 코드의 특정 영역을 감싸서 한 순간에 오로지 한 스레드만 이 영역에 접근할 수 있도록 해주는 동기화 메커니즘입니다. 즉, 락은 여러 스레드가 공유 자원에 동시에 접근하는 것을 제어하여 상호 배제(Mutual Exclusion)를 보장합니다. 이 락이 없으면 멀티 스레드 프로그램에서 경쟁 조건(Race Condition)이 발생하여 예상치 못한 결과를 초래할 수 있습니다.

## 락의 기본 개념

예를 들어, 다음과 같은 공유 자원에 대한 연산이 있다고 가정해봅시다.

```c
balance = balance + 1;
```

이 코드를 임계 영역(Critical Section)이라고 하며, 락을 사용하여 다음과 같이 보호할 수 있습니다.

```c
lock_t mutex; // 전역 변수로 선언된 락
...
lock(&mutex);
balance = balance + 1;
unlock(&mutex);
```

락은 하나의 변수로 표현되며, 사용하기 전에 먼저 선언해야 합니다. 이 락 변수는 두 가지 상태를 가질 수 있습니다.

1. 사용 가능 상태: 어느 스레드도 락을 가지고 있지 않은 상태
2. 사용 중 상태: 임계 영역에서 정확히 하나의 스레드가 락을 획득한 상태

`lock()` 함수는 락을 획득하려고 시도하고, `unlock()` 함수는 획득한 락을 해제합니다. 어떤 스레드가 `lock()` 함수를 호출하여 락을 획득하면, 해당 스레드를 락의 소유자(Owner)라고 부릅니다. 락이 이미 다른 스레드에 의해 사용 중인 경우, `lock()` 함수는 해당 락이 해제될 때까지 대기합니다.

락의 소유자가 `unlock()` 함수를 호출하면 락은 다시 사용 가능한 상태가 됩니다. 만약 어떤 스레드도 해당 락을 기다리고 있지 않다면, 락은 사용 가능한 상태로 유지됩니다.

락은 프로그래머에게 스케줄링에 대한 제어권을 제공하여 특정 코드 영역 내에서 한 번에 하나의 스레드만 실행되도록 보장합니다. 이를 통해 스레드 간의 잘못된 실행 순서로 인한 문제를 예방할 수 있습니다.

## POSIX 스레드 락

POSIX 스레드 라이브러리에서는 락을 뮤텍스(Mutex)라고 부릅니다. 뮤텍스는 "Mutual Exclusion"의 줄임말로, 스레드 간의 상호 배제를 의미합니다.

POSIX 스레드에서 뮤텍스를 사용하는 예제 코드는 다음과 같습니다.

```c
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

Pthread_mutex_lock(&lock); // pthread_mutex_lock()을 위한 래퍼 함수
balance = balance + 1;
Pthread_mutex_unlock(&lock);
```

POSIX 스레드에서는 뮤텍스 변수를 선언하고, 이를 `pthread_mutex_lock()`과 `pthread_mutex_unlock()` 함수에 전달하여 사용합니다. 이를 통해 서로 다른 공유 자원을 보호하기 위해 다양한 뮤텍스를 사용할 수 있습니다.

### 락의 세분성에 따른 전략

락의 세분성(Granularity)에 따라 두 가지 전략으로 나눌 수 있습니다.

1. 거친 락(Coarse-Grained Locking)

   - 하나의 락이 큰 임계 영역을 보호합니다.
   - 구현이 간단하지만, 병렬성이 제한될 수 있습니다.

2. 세밀한 락(Fine-Grained Locking)
   - 여러 개의 락이 각각 작은 임계 영역을 보호합니다.
   - 병렬성이 향상되지만, 데드락(Deadlock) 등의 복잡한 문제가 발생할 수 있습니다.

## 락의 구현

락은 다양한 방식으로 구현될 수 있습니다. 대표적인 방법으로는 인터럽트 비활성화, 테스트 앤 셋(Test-and-Set), 컴페어 앤 스왑(Compare-and-Swap), 페치 앤 애드(Fetch-and-Add) 등이 있습니다. 각각의 방법은 하드웨어 지원 여부와 특성에 따라 선택될 수 있습니다.

## 락의 평가 기준

락의 성능과 효율성을 평가하기 위해 다음과 같은 기준을 고려할 수 있습니다.

1. 상호 배제(Mutual Exclusion)

   - 락의 가장 기본적인 역할로, 한 번에 하나의 스레드만 임계 영역에 접근할 수 있어야 합니다.

2. 공정성(Fairness)

   - 모든 스레드가 락을 공평하게 획득할 수 있어야 합니다. 특정 스레드가 락을 독점하거나 기아 상태에 빠지지 않아야 합니다.

3. 성능(Performance)
   - 락의 사용으로 인한 오버헤드를 최소화해야 합니다. 락의 획득과 해제에 소요되는 시간이 짧아야 하며, 불필요한 대기 시간을 줄여야 합니다.

## 인터럽트 비활성화를 통한 락 구현

초기의 단일 프로세서 시스템에서는 인터럽트를 비활성화하여 상호 배제를 구현했습니다.

```c
void lock() {
    DisableInterrupts();
}

void unlock() {
    EnableInterrupts();
}
```

이 방법은 구현이 간단하지만, 몇 가지 단점이 있습니다.

1. 인터럽트 활성화/비활성화는 커널 모드 권한이 필요하므로, 사용자 코드에서는 사용이 제한됩니다.
2. 멀티프로세서 환경에서는 적용할 수 없습니다. 한 프로세서에서 인터럽트를 비활성화해도 다른 프로세서에서는 여전히 임계 영역에 접근할 수 있기 때문입니다.
3. 장시간 인터럽트를 비활성화하면 중요한 이벤트를 놓칠 수 있습니다.
4. 인터럽트 활성화/비활성화 명령어는 일반 명령어에 비해 실행 시간이 길어 효율성이 떨어집니다.

따라서 인터럽트 비활성화는 제한된 상황에서만 사용되어야 합니다.

## 테스트 앤 셋(Test-and-Set)을 통한 락 구현

테스트 앤 셋(Test-and-Set) 또는 아토믹 익스체인지(Atomic Exchange)는 멀티프로세서 환경에서 락을 구현하기 위한 하드웨어 명령어입니다.

```c
int TestAndSet(int *old_ptr, int new) {
    int old = *old_ptr;
    *old_ptr = new;
    return old;
}
```

`TestAndSet` 함수는 `old_ptr`이 가리키는 메모리 위치의 값을 `new`로 설정하고, 이전 값을 반환합니다. 이 과정은 원자적(Atomic)으로 수행되어 경쟁 조건을 방지합니다.

`TestAndSet`을 사용하여 스핀락(Spinlock)을 구현할 수 있습니다.

```c
typedef struct __lock_t {
    int flag;
} lock_t;

void init(lock_t *lock) {
    lock->flag = 0;  // 0: 사용 가능, 1: 사용 중
}

void lock(lock_t *lock) {
    while (TestAndSet(&lock->flag, 1) == 1)
        ; // 스핀 대기
}

void unlock(lock_t *lock) {
    lock->flag = 0;
}
```

`lock()` 함수는 `TestAndSet`을 사용하여 `flag`가 0일 때 1로 변경하고 락을 획득합니다. 만약 `flag`가 이미 1이라면 0이 될 때까지 스핀 대기합니다.

`unlock()` 함수는 `flag`를 0으로 변경하여 락을 해제합니다.

## 컴페어 앤 스왑(Compare-and-Swap)을 통한 락 구현

컴페어 앤 스왑(Compare-and-Swap)은 또 다른 하드웨어 명령어로, 메모리 위치의 값을 예상 값과 비교하고, 일치하면 새로운 값으로 교체합니다.

```c
int CompareAndSwap(int *ptr, int expected, int new) {
    int actual = *ptr;
    if (actual == expected)
        *ptr = new;
    return actual;
}
```

`CompareAndSwap`을 사용하여 스핀락을 구현할 수 있습니다.

```c
typedef struct __lock_t {
    int flag;
} lock_t;

void init(lock_t *lock) {
    lock->flag = 0;  // 0: 사용 가능, 1: 사용 중
}

void lock(lock_t *lock) {
    while (CompareAndSwap(&lock->flag, 0, 1) == 1)
        ; // 스핀 대기
}

void unlock(lock_t *lock) {
    lock->flag = 0;
}
```

`CompareAndSwap`은 `TestAndSet`과 유사하게 동작하지만, 예상 값과 실제 값을 비교한다는 점에서 더 강력한 기능을 제공합니다.

## 페치 앤 애드(Fetch-and-Add)를 통한 락 구현

페치 앤 애드(Fetch-and-Add)는 메모리 위치의 값을 원자적으로 증가시키고, 이전 값을 반환하는 하드웨어 명령어입니다.

```c
int FetchAndAdd(int *ptr) {
    int old = *ptr;
    *ptr = old + 1;
    return old;
}
```

`FetchAndAdd`를 사용하여 티켓 락(Ticket Lock)을 구현할 수 있습니다.

```c
typedef struct __lock_t {
    int ticket;
    int turn;
} lock_t;

void lock_init(lock_t *lock) {
    lock->ticket = 0;
    lock->turn = 0;
}

void lock(lock_t *lock) {
    int myturn = FetchAndAdd(&lock->ticket);
    while (lock->turn != myturn)
        ; // 스핀 대기
}

void unlock(lock_t *lock) {
    FetchAndAdd(&lock->turn);
}
```

티켓 락은 `ticket`과 `turn` 변수를 사용하여 락을 요청한 순서대로 스레드에게 락을 할당합니다. 이를 통해 공정성을 보장할 수 있습니다.

## 과도한 스핀에 대한 주의사항

스핀락을 사용할 때 주의해야 할 점은 과도한 스핀(Excessive Spinning)입니다. 이는 락을 획득하기 위해 스레드가 무한히 반복하여 CPU 자원을 낭비하는 상황을 말합니다.

과도한 스핀은 다음과 같은 경우에 발생할 수 있습니다.

1. 락의 소유자가 락을 장시간 보유하는 경우
2. 많은 스레드가 락을 경쟁적으로 획득하려는 경우

과도한 스핀을 방지하기 위해서는 락의 보유 시간을 최소화하고, 필요한 경우 스레드 간의 락 양보 메커니즘을 구현해야 합니다.

> 스핀(Spin)은 락을 획득하려는 스레드가 락이 해제될 때까지 반복문을 돌며 계속 시도하는 것을 말합니다. 스핀 중인 스레드는 CPU를 점유하면서 락의 상태를 반복해서 확인합니다. 스핀은 락의 대기 시간이 매우 짧을 것으로 예상되는 경우에 적합합니다.

## 결론

락은 멀티스레드 프로그래밍에서 상호 배제를 보장하기 위한 핵심적인 메커니즘입니다. 다양한 락 구현 방식이 있으며, 각각의 장단점을 고려하여 상황에 맞게 선택해야 합니다. 또한, 락의 사용으로 인한 성능 저하와 과도한 스핀 등의 문제를 인지하고 적절히 대처해야 합니다. 효과적인 락의 사용을 통해 멀티스레드 프로그램의 안정성과 효율성을 높일 수 있습니다.

락은 병렬 프로그래밍에서 매우 중요한 역할을 하지만, 락의 사용이 항상 최선의 선택은 아닙니다. 때로는 락 없이 동기화를 구현하는 것이 더 효율적일 수 있습니다. 이를 위해 락 프리(Lock-Free) 알고리즘이나 대기 없는 동기화(Wait-Free Synchronization) 기법 등을 활용할 수 있습니다.

또한, 락의 사용으로 인한 데드락(Deadlock), 라이브락(Livelock), 우선순위 역전(Priority Inversion) 등의 문제를 인지하고 방지해야 합니다. 이를 위해 락 순서 지정, 타임아웃, 우선순위 상속 등의 기법을 활용할 수 있습니다.

멀티스레드 프로그래밍에서 락은 강력한 도구이지만, 올바르게 사용하지 않으면 오히려 성능 저하와 불안정성을 초래할 수 있습니다. 따라서 락의 동작 원리를 이해하고, 적절한 락 전략을 선택하며, 필요한 경우 락 대신 다른 동기화 기법을 활용하는 것이 중요합니다.

프로그래머는 멀티스레드 환경에서의 동기화 문제를 깊이 이해하고, 다양한 락 메커니즘과 동기화 기법을 상황에 맞게 활용할 수 있어야 합니다. 이를 통해 효율적이고 안정적인 병렬 프로그램을 작성할 수 있습니다.
