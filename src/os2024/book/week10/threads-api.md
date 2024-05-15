# 쓰레드 API

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

위에 예제는 쓰레드를 생성하는 법을 보았습니다. 다른 쓰레드가 작업을 완료할 때까지 기다려야 한다면 뭔가 특별한 조치를 해야 합니다. POSIX 쓰레드에서는 `는 pthread_join()`을 부르는 것입니다.
```
int pthread_join(pthread_t thread, void **value_ptr);
```

이 루틴은 두 개의 인자를 받습니다. 첫 번째 `pthread_t` 타입 인자는 어떤 쓰레드를 기다리려고 하는지 명시된는데 이 변수는 쓰레드 생성 루틴에 의해 초기화됩니다 (자료구조에 대한 포인터를 `pthread_create()`의 인자로 전달하여). 이 구조체를 보관해 놓으면, 그 쓰레드가 끝나기를 기다릴 때 사용할 수 있습니다.

두 번째 인자는 반환 값에 대한 포인터입니다. 루틴이 임의의 데이터 타입을 반환할 수 있기 때문에 `void`에 대한 포인터 타입으로 정의됩니다. `pthread_join()` 루틴은 전달된 인자의 값을 변경하기 때문에 값을 전달하는 것이 아니라 그 값에 대한 포인터를 전달해야 합니다.

또 다른 예제를 살펴봅시다. 
```c
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

typedef struct __myarg_t {
    int a;
    int b;
} myarg_t;

typedef struct __myret_t {
    int x;
    int y;
} myret_t;

void *mythread(void *arg) {
    myarg_t *m = (myarg_t *) arg;
    printf(“%d %d\n ”, m−>a, m−>b);
    myret_t *r = malloc(sizeof(myret_t));
    r−>x = 1;
    r−>y = 2;
    return (void *) r;
}

int
main(int argc, char *argv[]) {
    int rc;
    pthread_t p;
    myret_t *m;

    myarg_t args;
    args.a = 10;
    args.b = 20;
    Pthread_create(&p, NULL, mythread, &args);
    Pthread_join(p, (void **) &m);
    printf(“returned %d %d\n ”, m−>x, m−>y);
    return 0;
}
```

이 코드에서는 이전 예제와 마찬가지로 쓰레드를 생성하여 `myarg_t` 데이터 타입을 통하여 2개의 인자를 전달했습니다. 반환 값으로 `myret_t` 타입이 사용됩니다. 메인 쓰레드는 `pthread_join()` 루틴 안에서 기다리는 중입니다. 쓰레드가 실행을 마치면 메인 쓰레드가 리턴하고, 종료한 쓰레드가 반환한 값, 즉 `myret_t` 안에 들어 있는 무슨 값이든 접근할 수 있게 됩니다.

이 예제에서 주목할 만한 몇 가지가 있습니다. 

먼저 여러 인자를 한 번에 전달하기 위해 묶고 해체하는 불편한 과정을 항상 해야 하는 것은 아닙니다. 예를 들면 인자가 없는 쓰레드를 생성할 때에는 `NULL`을 전달하여 쓰레드를 생성할 수도 있습니다. 비슷한 예로 반환 값이 필요 없다면 `pthread_join()`에 `NULL`을 전달할 수도 있습니다.

두 번째는 값 하나만 (예, int 값) 전달해야 한다면 인자를 전달하기 위해 묶을 필요가 없습니다.

그 예가 있습니다. 
```
void *mythread(void *arg) {
    int m = (int) arg;
    printf(“%d\n ”, m);
    return (void *) (arg + 1);
}
int main(int argc, char *argv[]) {
    pthread_t p;
    int rc, m;
    Pthread_create(&p, NULL, mythread, (void *) 100);
    Pthread_join(p, (void **) &m);
    printf(“returned %d\n ”, m);
    return 0;
}
```
이 경우에 인자와 반환 값을 구조체로 묶을 필요가 없기때문에 간단하게 할 수 있습니다.

세 번째로 유의해야 할 것은 쓰레드에서 값이 어떻게 반환되는지에 대해 각별한 신경을 써야 한다는 것입니다.

```
void *mythread(void *arg) {
    myarg_t *m = (myarg_t *) arg;
    printf(“%d %d\n ”, m−>a, m−>b);
    myret_t r; // 스택에 할당했다. 이러면 안 된다!
    r.x = 1;
    r.y = 2;
    return (void *) &r;
}
```
이 코드에 `mythread`의 스택에 변수 `r`이 할당되어 있습니다. 이 값은 쓰레드가 리턴할 때 자동적으로 해제됩니다 (어쨌건, 이게 스택이 사용하기 쉬운 이유이다). 현재 해제된 변수를 가리키는 포인터를 반환하는 것은 온갖 종류의 좋지 않은 결과를 가져옵니다. 분명 반환받았다고 생각하는 값을 출력해 보면 (꼭 그런 건 아니지만) 놀라게 될 것입니다.

마지막으로, `pthread_create()`를 사용하여 쓰레드를 생성하고 직후에 `pthread_join()`을 호출한다는 것은 쓰레드를 생성하는 아주 이상한 방법입니다. 사실 작을 똑같이 할 수 있는 더 쉬운 방법이 있습니다. 이를 프로시저 호출(procedure call)이라고 부릅니다.

## 락

쓰레드의 생성과 조인 다음으로 POSIX 쓰레드 라이브러리가 제공하는 가장 유용한 함수는 락(lock)을 통한 임계 영역에 대한 상호 배제 기법입니다. 이러한 목적을 위하여 사용되는 가장 기본적인 루틴은 다음과 같이 쌍으로 이루어져 있습다.

```
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```
이 루틴은 이해하고 사용하기 쉬워야 합니다. 그래야 임계 영역을 원하는 방식으로 동작하도록 보호할 때 사용할 수 있습니다. 다음과 같은 코드를 예상할 수 있습다.
```
pthread_mutex_t lock;
pthread_mutex_lock(&lock);
x = x + 1; // 또는 다른 임계 영역의 코드를 사용할 수 있음
pthread_mutex_unlock(&lock);
```
이 코드가 하고자 하는 바는 다음과 같습니다. `pthread_mutex_lock()`가 호출되었을때 다른 어떤 쓰레드도 락을 가지고 있지 않다면 이 쓰레드가 락을 얻어 임계 영역에 진입합니다. 만약 다른 쓰레드가 락을 가지고 있다면, 락 획득을 시도하는 쓰레드는 락을 얻을 때까지 호출에서 리턴하지 않습니다 (리턴했다면 락을 가지고 있던 쓰레드가 언락(unlonk)을 호출하여 락을 양도했다는 것을 의미합니다). 많은 쓰레드들이 락 획득 함수 에서 대기중 일 수 있습니다. 락을 획득한 쓰레드만이 언락을 호출해야 합니다.

안타깝게도 이 코드는 두 가지 측면애서 올바르게 동작하지 않습니다. 
첫 번째, 초기화를 하지 않았습니다. 올바른 값을 가지고 시작했다는 것을 보장하기 위해 모든 락은 올바르게 초기화되어야 합니다. 그래야 락과 언락을 호출하였을 때 의도한 대로 동작할 수 있습니다.

POSIX 쓰레드를 사용할 때 락을 초기화하는 방법은 두 가지입니다. 
한 가지 방법은 다음과 같이 `PTHREAD_MUTEX_INITIALIZER`를 사용하는 것입다
```
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
```
위 연산은 락을 디폴트 값으로 설정합니다. 동적으로 초기화하는 방법은 (즉, 실행 중에) 다음과 같이 `pthread_mutex_init()`을 호출하는 것입다.
```
int rc = pthread_mutex_init(&lock, NULL);
assert(rc == 0); // 성공했는지 꼭 확인해야 한다!
```
이 루틴의 첫 번째 인자는 락 자체의 주소이고, 반면에 두 번째 인자는 선택 가능한 속성입니다. NULL을 전달하면 디폴트
값을 사용합니다. 정적, 동적 두 방법 모두 사용할 수 있지만 우리는 후자인 동적 방법을 주로 사용합니다. 락 사용이 끝났다면 초기화 API와 상응하는 `pthread_mutex_destroy()`도 호출해야 한다는 것을 주의해야 합니다.

위 코드의 두 번째 문제는 락과 언락을 호출할 때 에러 코드를 확인하지 않는다는 것입니다. Unix 시스템에서 호출하는 거의 모든 라이브러리 루틴과 마찬가지로 이 루틴들도 실패할 수 있습니다! 당신의 코드가 제대로 에러 코드를 검사하지 않는다면, 코드는 조용히 실패하게 됩니다. 이 경우는 여러 쓰레드가 동시에 임계 영역에 들어갈 수 있습니다.

최소한 래퍼 함수를 사용하여 해당 루틴이 성공적으로 처리되었는지 확인해야 합니다. 
(예)
```
// 이 방법을 써서 코드를 깔끔하게 유지하되 오류가 없는지 확인해야 한다.
// 프로그램이 오류에도 문제가 없었을 때에만 사용하자.
void Pthread_mutex_lock(pthread_mutex_t *mutex) {
    int rc = pthread_mutex_lock(mutex);
    assert(rc == 0);
}
```

락과 언락 루틴 외에 pthread 라이브러리에서 락 관련 루틴들이 더 존재합니다. 특히 관심을 가질 만한 루틴이 2개 더 있습니다.
```c
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_timedlock(pthread_mutex_t *mutex,
                            struct timespec *abs_timeout);
```

이 두 함수는 락을 획득하는 데 사용됩니다. `trylock` 버전은 락이 이미 사용 중이라면 실패 코드를 반환합니다. `timedlock`은 타임아웃이 끝나거나 락을 획득하거나의 두 조건 중 하나가 발생하면 리턴합니다. `timedlock`의 타임아웃을 0으로 설정하면 `trylock`과 동일하게 동작합니다. 이 두 함수는 사용하지 않는 것이 좋습다. 그러나 락 획득 루틴에서 무한정 대기하는 상황을 피하기 위해 사용되기도 합니다. 이러한 경우를 앞으로 (예, 교착 상태를 공부할 때) 보게 될 것입니다.

## 컨디션 변수

쓰레드 라이브러리가 POSIX 쓰레드의 경우에는 확실히, 제공하는 주요한 구성 요소로 **컨디션 변수(condition variable)**가 있습니다. 한 쓰레드가 계속 진행하기 전에 다른 쓰레드가 무언가를 해야 쓰레드 간에 일종의 시그널 교환 메커니즘이 필요합니다. 이런 경우 컨디션 변수가 사용됩니다.
```
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_signal(pthread_cond_t *cond);
```
컨디션 변수 사용을 위해서는 이 컨디션 변수와 연결된 락이 “반드시” 존재해야 합니다. 위의 루틴 중 하나를 호출하기 위해서는 그 락을 갖고 있어야 합니다.

첫 번째 루틴 `pthread_cond_wait()`는 호출 쓰레드를 수면 (sleep) 상태로 만들고 다른 쓰레드로부터의 시그널을 대기합니다. 현재 수면 중인 쓰레드가 관심 있는 무언가가 변경되면 시그널을 보냅니다. 전형적인 용례는 다음과 같습니다.
```c
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

Pthread_mutex_lock(&lock);
while (ready == 0)
    Pthread_cond_wait(&cond, &lock);
Pthread_mutex_unlock(&lock);
```
이 코드에서는 연관된 락과 컨디션 변수를 초기화한 후에 쓰레드는 `ready` 변수가 0인지 검사합니다. `ready` 변수 값이 0이라면, 다른 쓰레드가 깨워줄 때까지 잠들기 위해 대기 루틴을 호출합니다.

다른 쓰레드에서 실행될 잠자는 쓰레드를 깨우는 코드는 다음과 같습니다.
```
Pthread_mutex_lock(&lock);
ready = 1;
Pthread_cond_signal(&cond);
Pthread_mutex_unlock(&lock);
```
이 코드에서 유의할 몇 가지가 있습다

첫 번째, 시그널을 보내고 전역 변수 `ready`를 수정할 때 반드시 락을 가지고 있어야 합니다. 이를 통해 경쟁 조건이 발생하지 않는다는 것을 보장합다.

두 번째, 시그널 대기 함수에서는 락을 두 번째 인자로 받고 있지만, 시그널 보내기 함수에서는 조건만을 인자로 받는 것에 유의해야 합다.

마지막으로 매우 중요한 특이 사항이 있습니다. 대기하는 쓰레드가 조건을 검사할 때 `if` 문을 사용하는 대신 `while` 문을 사용한다는 것입니다.

`while` 문을 사용하는 것이 일반적으로 간단하고 안전합니다. `pthread` 라이브러리에서 (실수로 또는 부주의하게) 변수를 제대로 갱신하지 않고 대기하던 쓰레드를 깨울 수 있습니다. 이런 경우 재검사를 하지 않는다면 대기하던 쓰레드는 조건이 변경되지 않았더라도 변경되었다고 생각할 것입니다.

두 쓰레드 간에 시그널을 주고 받아야 할 때, 락과 컨디션 변수를 사용하는 대신 간단한 플래그를 사용하여 구현하고 싶을 것입니다. 예를 들어 앞에서의 대기 코드를 다음과 같이 다시 코딩할 수 있습니다.
```
while (ready == 0)
; // 회전
```
이 코드에 상응하는 시그널 보내기 코드는 다음과 같을 것입니다. 
```
ready = 1;
```
절대로 하면 안 됩니다. 이유는 다음과 같습니다.
첫째, 많은 경우에 이 코드는 성능이 좋지 않습니다. 조건 검사를 위해 오랫동안 반복문을 실행하여 검사하는 것은 CPU 사이클의 낭비를 초래합니다.

둘째, 오류가 발생하기 쉽습니다. 최근 연구 [Xio+10]에 따르면, 쓰레드 간에 동기화를 하기 위해 플래그를 사용할 때 (위의 경우처럼) 놀랄 정도로 실수하기 쉽습니다.

## 컴파일과 실행

이 장에서 사용한 예제 코드들은 상대적으로 작동시키기 쉽습니다. 예제들을 컴파일 하기 위해서는 `pthread.h` 헤더를 포함시켜야 합니다. `-pthread` 플래그를 명령어 링크 옵션 부분에 추가하여 사용하여 `pthread` 라이브러리와 링크할 수 있도록 명시해야 합니다.

예를 들어 간단한 멀티 쓰레드 프로그램을 컴파일하기 위하여 해야 할 일은 다음 명령어를 입력하는 것 뿐입니다.
```
prompt> gcc −o main main.c −Wall −pthread
```
`main.c` 가 `pthread` 헤더를 포함하고 있는 한 병행 프로그램이 성공적으로 컴파일 됩니다. 프로그램의 동작 여부는 늘 그렇듯 전혀 다른 문제입니다

## 요약

쓰레드 생성과 락을 통한 상호 배제의 구현, 컨디션 변수를 이용한 시그널과 대기 등 `pthread` 라이브러리의 기본 지식을 소개했습니다. 강인하고 효율적인 멀티 쓰레드 코드를 작성하기 위해서는 인내와 세심한 주의가 답입니다.

멀티 쓰레드 코드를 작성하는 데 도움될 만한 몇 가지 팁과 함께 이 장을 마치고자 합니다. 
API에는 흥미로운 내용들이 더 많습니다. 좀 더 많은 정보를 원한다면 Linux 시스템에서 `man -k pthread`를 실행시켜 백여 개가 넘는 API들의 인터페이스를 확인해 보기 바랍니다. 하지만 이 장에서 다룬 기본적인 논의로도 정교한 (그리고 올바르고 성능 좋은) 멀티 쓰레드 프로그램을 작성할 수 있어야 합니다. 
쓰레드에 있어 어려운 부분은 API가 아니라 병행 프로그램을 구현하는 정교한 사고 방식입니다. 더 많은 걸 배우기 위해 계속 읽어 나가기 바랍다.
