# 쓰레드 API

운영체제가 쓰레드를 생성하고 제어하는 데 어떤 인터페이스를 제공해야 할까?
어떻게 이 인터페이스를 설계해야 쉽고 유용하게 사용할 수 있을까?

## 쓰레드 생성

멀티 쓰레드 프로그램을 작성 시, 가장 먼저할 일은 새로운 쓰레드 생성입니다. 쓰레드 생성을 위해서는 해당 인터페이스다 존재 해야 합니다.

```c
#include <pthread.h>
int
pthread_create(      pthread_t * thread,
               const pthread_attr_t * attr,
                     void * (*start_routine)(void*) ,
                     void * arg);
```

복잡하게 보이지만 실제로는 그리 어렵지 않습니다. `thread, attr, start_routine` 그리고 `arg`라는 4개의 인자가있습니다.

먼저 `thread`는 `pthread_t` 타입 구조체를 가리키는 포인터입니다. 이 구조가 쓰레드와 상호작용하는 데 사용되기 때문에 쓰레드 초기화 시 `pthread_create()`에 이 구조체를 전달합니다.

두 번째 인자 `attr`은 쓰레드의 속성을 지정하는 데 사용합니다. 스택의 크기와 쓰레드의 스케줄링 우선순위 같은 정보를 지정하기 위해서 사용될 수 있습니다. 개별 속성은 `pthread_attr_init()` 함수를 호출하여 초기화합니다. 대부분의 경우에 디폴트 값을 지정하면 충분하고, 간단히 NULL을 전달하기만 하면 됩니다.

세 번째 인자는 인자들 중에 가장 복잡해 보입니다. 이 쓰레드가 실행할 함수를 나타납니다. C 언어에서는 이를 함수 포인터라고 부르고, 다음과 같은 정보가 필요하다고 알려줍니다. 이 함수는 `void *` 타입의 인자 한 개를 전달받고 (함수 이름 `start_routine`다음에 괄호 안에 표시된 것처럼), `void *` (즉, `void` 포인터) 타입의 값을 반환합니다.

마지막으로 네 번째 인자인 `arg`는 실행할 함수에게 전달할 인자를 나타냅니다. 왜 void 포인터 타입이 필요하지라고 질문할 수도 있는데 `void` 포인터를 `start_routine` 함수의 인자로 사용하면, 어떤 데이터 타입도 인자로 전달할 수있고, 반환 값의 타입으로 사용하면 쓰레드는 어떤 타입의 결과도 반환할 수 있습니다.

두 개의 인자를 전달받는 새로운 쓰레드를 생성하는데 두 인자는 우리가 정의한 myarg_t 타입으로 묶여집니다. 쓰레드가 생성되면 전달받은 인자의 타입을 예상하고 있는 타입으로 변환할 수 있고, 원하는 대로 인자를 풀어낼 수 있습니다.

```c
#include <pthread.h>

typedef struct __myarg_t {
    int a;
    int b;
} myarg_t;

void *mythread(void *arg) {
    myarg_t *m = (myarg_t *) arg;
    printf(“%d %d\n ”, m−>a, m−>b);
    return NULL;
}

int
main(int argc, char *argv[]) {
    pthread_t p;
    int rc;

    myarg_t args;
    args.a = 10;
    args.b = 20;
    rc = pthread_create(&p, NULL, mythread, &args);
    ...
}
```

쓰레드를 생성하고 나면, 실행 중인 모든 쓰레드롸 같은 주소 공간에서 실행되는 또 하나의 실행 개체를 보유하게 됩니다.

## 쓰레드 종료

앞서 쓰레드를 생성하는 방법에 대해 알아보았습니다. 하지만 때로는 다른 쓰레드가 작업을 완료할 때까지 기다려야 하는 경우가 있습니다. 이를 위해 POSIX 쓰레드에서는 `pthread_join()`이라는 함수를 제공합니다.

```c
int pthread_join(pthread_t thread, void **value_ptr);
```

이 함수는 두 개의 인자를 받습니다:

1. `thread`: 기다릴 쓰레드의 ID입니다. 이 값은 `pthread_create()` 호출 시 초기화됩니다.
2. `value_ptr`: 쓰레드의 반환값을 받을 포인터의 포인터입니다. 쓰레드 함수가 `void *`를 반환하므로, 이를 받기 위해 이중 포인터가 사용됩니다.

### 예제

```c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct {
  int a;
  int b;
} myarg_t;

typedef struct {
  int x;
  int y;
} myret_t;

void *mythread(void *arg) {
  myarg_t *m = (myarg_t *) arg;
  printf("%d %d\n", m->a, m->b);

  myret_t *r = malloc(sizeof(myret_t));
  r->x = 1;
  r->y = 2;
  return (void *) r;
}

int main() {
  pthread_t p;
  myarg_t args = { 10, 20 };
  myret_t *retval;

  pthread_create(&p, NULL, mythread, &args);
  pthread_join(p, (void **) &retval);

  printf("returned %d %d\n", retval->x, retval->y);
  free(retval);

  return 0;
}
```

이 예제에서는 `myarg_t`를 통해 쓰레드 함수에 인자를 전달하고, `myret_t`를 통해 반환값을 받습니다. main 함수에서는 `pthread_join()`을 호출하여 쓰레드가 종료될 때까지 기다립니다. 쓰레드가 종료되면 반환값을 `retval`을 통해 받고, 이를 출력합니다.

### 주의사항

- 인자나 반환값이 단순한 값(예: 정수)이라면 구조체를 사용하지 않고 직접 전달할 수 있습니다.
- 반환값은 동적 할당된 메모리여야 합니다. 스택에 할당된 변수의 주소를 반환하면 안됩니다.
- 쓰레드를 생성하고 바로 `pthread_join()`을 호출하는 것은 비효율적입니다. 이런 경우라면 그냥 함수를 호출하는 것이 더 나을 수 있습니다.

### 용어 설명

- 동적 할당(Dynamic Allocation): 프로그램 실행 중에 메모리를 할당하는 것을 말합니다. C에서는 `malloc()` 함수를 사용합니다.
- 스택(Stack): 함수 호출 시 지역 변수와 함수 인자가 저장되는 메모리 영역입니다. 함수가 종료되면 자동으로 해제됩니다.

## 락(Lock)

쓰레드의 생성과 조인 다음으로 POSIX 쓰레드 라이브러리가 제공하는 가장 유용한 기능은 락을 통한 상호 배제(mutual exclusion)입니다. 이는 여러 쓰레드가 공유 자원에 동시에 접근하는 것을 막기 위해 사용됩니다. 이를 위해 다음과 같은 두 함수가 쌍으로 제공됩니다.

```c
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

이 함수들은 사용하기 쉽습니다. 공유 자원에 접근하기 전에 `pthread_mutex_lock()`을 호출하여 락을 획득하고, 접근이 끝나면 `pthread_mutex_unlock()`을 호출하여 락을 해제합니다.

```c
pthread_mutex_t lock;

pthread_mutex_lock(&lock);
// 공유 자원 접근
x = x + 1;
pthread_mutex_unlock(&lock);
```

`pthread_mutex_lock()`이 호출되었을 때, 다른 쓰레드가 이미 락을 획득한 상태라면 해당 쓰레드는 락이 해제될 때까지 대기합니다. 락을 획득한 쓰레드만이 `pthread_mutex_unlock()`을 호출할 수 있습니다.

### 락의 초기화

락을 사용하기 전에는 반드시 초기화를 해야 합니다. POSIX 쓰레드에서는 두 가지 방법으로 락을 초기화할 수 있습니다.

1. 정적 초기화: `PTHREAD_MUTEX_INITIALIZER` 매크로를 사용합니다.

   ```c
   pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
   ```

2. 동적 초기화: `pthread_mutex_init()` 함수를 사용합니다.
   ```c
   int rc = pthread_mutex_init(&lock, NULL);
   assert(rc == 0); // 초기화 성공 여부를 확인
   ```

동적 초기화를 사용한 경우, 락 사용이 끝나면 `pthread_mutex_destroy()`를 호출하여 락을 해제해야 합니다.

### 에러 처리

`pthread_mutex_lock()`과 `pthread_mutex_unlock()` 함수는 실패할 수 있습니다. 따라서 반환값을 확인하여 에러 처리를 해야 합니다. 이를 간편하게 하기 위해 래퍼 함수를 사용할 수 있습니다.

```c
void Pthread_mutex_lock(pthread_mutex_t *mutex) {
  int rc = pthread_mutex_lock(mutex);
  assert(rc == 0);
}
```

### 기타 락 관련 함수

POSIX 쓰레드는 `pthread_mutex_lock()`과 `pthread_mutex_unlock()` 외에도 다음과 같은 락 관련 함수를 제공합니다.

```c
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_timedlock(pthread_mutex_t *mutex,
                            struct timespec *abs_timeout);
```

- `pthread_mutex_trylock()`: 락이 이미 획득되어 있으면 대기하지 않고 바로 실패를 반환합니다.
- `pthread_mutex_timedlock()`: 지정된 시간까지 락 획득을 시도합니다. 시간 내에 락을 획득하지 못하면 실패를 반환합니다.

이 함수들은 교착 상태(deadlock)를 피하기 위해 사용될 수 있습니다. 하지만 일반적으로는 사용을 권장하지 않습니다.

### 용어 설명

- 락(Lock): 공유 자원에 대한 접근을 동기화하기 위해 사용되는 메커니즘입니다.
- 상호 배제(Mutual Exclusion): 여러 쓰레드가 공유 자원에 동시에 접근하는 것을 막는 것을 말합니다.
- 임계 영역(Critical Section): 공유 자원에 접근하는 코드 영역을 말합니다. 락을 통해 보호됩니다.
- 교착 상태(Deadlock): 두 개 이상의 쓰레드가 서로가 가진 자원을 기다리며 무한히 블록되는 상태를 말합니다.

## 컨디션 변수(Condition Variable)

쓰레드 간 통신을 위해 POSIX 쓰레드 라이브러리가 제공하는 또 다른 중요한 도구는 **컨디션 변수**입니다. 한 쓰레드가 다른 쓰레드로부터 특정 조건이 만족될 때까지 기다리는 상황에서 사용됩니다.

```c
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_signal(pthread_cond_t *cond);
```

컨디션 변수를 사용하려면 반드시 연관된 락(mutex)이 있어야 합니다. 컨디션 변수 함수를 호출하기 위해서는 먼저 이 락을 획득해야 합니다.

### pthread_cond_wait()

`pthread_cond_wait()`는 호출한 쓰레드를 대기 상태로 만들고, 다른 쓰레드로부터 시그널을 기다립니다. 일반적인 사용 패턴은 다음과 같습니다.

```c
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_lock(&lock);
while (ready == 0)
  pthread_cond_wait(&cond, &lock);
pthread_mutex_unlock(&lock);
```

이 코드에서 쓰레드는 `ready` 변수를 확인하고, 그 값이 0이면 `pthread_cond_wait()`를 호출하여 다른 쓰레드로부터 깨워질 때까지 대기합니다.

### pthread_cond_signal()

다른 쓰레드에서 대기 중인 쓰레드를 깨우는 코드는 다음과 같습니다.

```c
pthread_mutex_lock(&lock);
ready = 1;
pthread_cond_signal(&cond);
pthread_mutex_unlock(&lock);
```

여기서 주의할 점은 다음과 같습니다:

1. `pthread_cond_signal()`을 호출하고 `ready` 변수를 변경할 때는 반드시 락을 획득한 상태여야 합니다. 이는 경쟁 조건(race condition)을 방지합니다.

2. `pthread_cond_wait()`는 락을 인자로 받지만, `pthread_cond_signal()`은 컨디션 변수만 인자로 받습니다.

3. 대기 중인 쓰레드에서 조건을 검사할 때는 `if`가 아닌 `while`을 사용해야 합니다. 이는 가짜 깨움(spurious wakeup)에 대비하기 위함입니다.

### 주의사항

컨디션 변수 대신 단순히 플래그 변수를 사용하여 쓰레드 간 통신을 구현하고 싶을 수 있습니다. 예를 들어, 다음과 같이 구현할 수 있습니다.

```c
while (ready == 0)
  ; // busy-wait
```

하지만 이는 좋지 않은 방법입니다. 첫째, 불필요한 CPU 사이클을 낭비합니다. 둘째, 경쟁 조건과 같은 오류가 발생하기 쉽습니다.

### 용어 설명

- 컨디션 변수(Condition Variable): 쓰레드가 특정 조건을 만족할 때까지 기다릴 수 있게 해주는 동기화 도구입니다.
- 경쟁 조건(Race Condition): 두 개 이상의 쓰레드가 공유 자원에 동시에 접근할 때 발생하는 예측 불가능한 결과입니다.
- 가짜 깨움(Spurious Wakeup): 시그널이 전달되지 않았음에도 쓰레드가 깨어나는 현상입니다. 이는 시스템 구현 상의 한계로 인해 발생할 수 있습니다.
- 바쁜 대기(Busy-wait): 조건을 만족할 때까지 계속해서 루프를 돌며 CPU 자원을 낭비하는 대기 방식입니다.

## 컴파일과 실행

이 장에서 다룬 예제 코드들은 비교적 쉽게 실행할 수 있습니다. 컴파일을 위해서는 다음 두 가지를 해야 합니다.

1. `pthread.h` 헤더 파일을 포함해야 합니다. 이 헤더 파일에는 POSIX 쓰레드 관련 함수와 자료형이 정의되어 있습니다.

2. 컴파일러 옵션에 `-pthread`를 추가해야 합니다. 이는 pthread 라이브러리를 링크하도록 지시하는 역할을 합니다.

예를 들어, `main.c`라는 멀티 쓰레드 프로그램을 컴파일하려면 다음과 같이 명령을 입력하면 됩니다.

```bash
gcc -o main main.c -Wall -pthread
```

`-Wall`은 모든 경고 메시지를 출력하도록 하는 옵션입니다. `main.c`가 `pthread.h`를 포함하고 있다면, 이 명령으로 병렬 프로그램을 성공적으로 컴파일할 수 있습니다. 물론, 프로그램이 의도한 대로 동작하는지는 별개의 문제입니다.

### 용어 설명

- 컴파일(Compile): 소스 코드를 기계어로 번역하는 과정입니다. C 언어에서는 일반적으로 `gcc`나 `clang`과 같은 컴파일러를 사용합니다.

- 링크(Link): 컴파일된 객체 파일들과 라이브러리를 연결하여 실행 가능한 파일을 생성하는 과정입니다.

- 라이브러리(Library): 자주 사용되는 함수나 클래스 등을 모아 둔 코드 묶음입니다. 재사용성과 모듈성을 높이기 위해 사용합니다.

## 요약

이 장에서는 `pthread` 라이브러리를 사용하여 쓰레드를 생성하고, 락을 통해 상호 배제를 구현하며, 컨디션 변수를 이용해 쓰레드 간 통신하는 방법을 배웠습니다. 효율적이고 안정적인 멀티 쓰레드 프로그램을 작성하려면 세심한 주의와 인내가 필요합니다.

마지막으로 몇 가지 팁을 살펴보겠습니다.

- POSIX 쓰레드 API에는 이 장에서 다룬 것 외에도 많은 기능이 있습니다. 더 자세한 정보가 필요하다면 Linux 시스템에서 `man -k pthread` 명령을 실행하여 관련 함수들의 매뉴얼을 확인해 보세요.

- 멀티 쓰레드 프로그래밍의 어려운 점은 API가 아니라 병행성을 올바르게 다루는 사고방식입니다. 계속해서 공부하고 연습하는 것이 중요합니다.

멀티 쓰레드 프로그래밍은 강력한 도구이지만, 잘못 사용하면 오히려 프로그램의 복잡성만 높아질 수 있습니다. 쓰레드를 사용할 때는 항상 신중해야 하며, 경쟁 조건이나 교착 상태와 같은 문제를 피하기 위해 노력해야 합니다.
