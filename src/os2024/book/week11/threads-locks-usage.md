# 락 기반의 병행 자료 구조

병행 자료 구조(Concurrent Data Structures)란 다수의 스레드가 동시에 접근할 수 있는 자료 구조를 말합니다. 이러한 자료 구조는 병행성(Concurrency) 문제를 해결하기 위해 락(Lock)을 사용하여 데이터의 일관성(Consistency)을 유지합니다. 락 기반의 병행 자료 구조는 스레드 간의 데이터 접근 충돌을 방지하고, 데이터의 정확성을 보장하는 데 매우 중요한 역할을 합니다.

병행성이란 여러 작업이 동시에 진행되는 것을 말하며, 멀티스레드 환경에서 흔히 발생합니다. 이때 공유 자원에 대한 접근을 적절히 제어하지 않으면 데이터 불일치, 데이터 손상 등의 문제가 발생할 수 있습니다. 락은 이러한 문제를 해결하기 위해 사용되는 동기화 메커니즘으로, 한 번에 하나의 스레드만 공유 자원에 접근할 수 있도록 제어합니다.

아래에서는 대표적인 락 기반의 병행 자료 구조인 병행 카운터, 병행 연결 리스트, 병행 큐, 병행 해시 테이블에 대해 자세히 설명하겠습니다.

## 병행 카운터(Concurrent Counter)

병행 카운터는 다수의 스레드가 동시에 값을 증가시키거나 감소시킬 수 있는 카운터입니다. 이때 가장 중요한 점은 여러 스레드가 동시에 카운터 값을 변경할 때도 그 값이 정확하게 유지되어야 한다는 것입니다.

병행 카운터는 일반적으로 락을 사용하여 구현됩니다. 카운터의 값을 읽거나 쓸 때 락을 획득하여 다른 스레드의 접근을 막고, 연산이 끝나면 락을 해제합니다. 이를 통해 카운터 값의 일관성을 보장할 수 있습니다.

```c
pthread_mutex_t lock;
int counter = 0;

void increment() {
    pthread_mutex_lock(&lock);  // 락 획득
    counter++;                  // 카운터 증가
    pthread_mutex_unlock(&lock);  // 락 해제
}

void decrement() {
    pthread_mutex_lock(&lock);  // 락 획득
    counter--;                  // 카운터 감소
    pthread_mutex_unlock(&lock);  // 락 해제
}
```

위 코드에서 `pthread_mutex_t` 타입의 `lock` 변수가 락의 역할을 합니다. `increment()`와 `decrement()` 함수에서는 락을 획득한 후 카운터 값을 변경하고, 마지막에 락을 해제합니다.

이러한 락을 이용한 구현은 비교적 간단하고 데이터의 일관성을 보장할 수 있다는 장점이 있습니다. 하지만 모든 연산에 락을 사용하므로 성능 저하가 발생할 수 있고, 특히 다수의 스레드가 동시에 접근할 때는 락 경합(Lock Contention)으로 인해 효율이 크게 떨어질 수 있습니다.

병행 카운터는 웹 서버에서 활성 사용자 수를 추적하거나, 데이터베이스에서 특정 이벤트 발생 횟수를 기록하는 등 다양한 응용 프로그램에서 사용됩니다.

## 병행 연결 리스트(Concurrent Linked List)

병행 연결 리스트는 다수의 스레드가 동시에 노드를 추가하거나 삭제할 수 있는 연결 리스트입니다. 이때 락을 사용하여 스레드 간의 노드 접근을 안전하게 제어합니다.

병행 연결 리스트는 노드별로 락을 사용하거나, 전체 리스트에 대한 하나의 락을 사용할 수 있습니다. 노드별 락을 사용하면 리스트의 다른 부분을 동시에 수정할 수 있어 성능이 향상될 수 있지만, 구현이 복잡해지고 데드락(Deadlock)이나 라이브락(Livelock)과 같은 문제가 발생할 수 있습니다.

```c
typedef struct Node {
    int data;
    struct Node* next;
    pthread_mutex_t lock;
} Node;

Node* head = NULL;
pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;

void add_node(int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = value;
    pthread_mutex_init(&new_node->lock, NULL);

    pthread_mutex_lock(&list_lock);  // 리스트 락 획득
    new_node->next = head;
    head = new_node;
    pthread_mutex_unlock(&list_lock);  // 리스트 락 해제
}

void remove_node(int value) {
    pthread_mutex_lock(&list_lock);  // 리스트 락 획득
    Node* current = head;
    Node* prev = NULL;

    while (current != NULL) {
        if (current->data == value) {
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            pthread_mutex_destroy(&current->lock);
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }
    pthread_mutex_unlock(&list_lock);  // 리스트 락 해제
}
```

위 코드에서는 연결 리스트 전체에 대한 하나의 락(`list_lock`)을 사용하여 노드 추가와 삭제 연산을 제어합니다. 노드 추가 시에는 새로운 노드를 생성하고 락을 초기화한 후, 리스트 락을 획득하여 새 노드를 리스트의 head에 추가합니다. 노드 삭제 시에는 리스트 락을 획득한 후 리스트를 탐색하여 해당 노드를 찾아 제거합니다.

병행 연결 리스트는 운영체제의 스케줄러, 메모리 할당기, 네트워크 프로토콜 스택 등 다양한 시스템 소프트웨어에서 사용됩니다. 예를 들어, 스케줄러는 실행 중인 프로세스나 스레드의 목록을 관리하기 위해 병행 연결 리스트를 활용할 수 있습니다.

## 병행 큐(Concurrent Queue)

병행 큐는 다수의 스레드가 동시에 요소를 삽입하거나 제거할 수 있는 큐입니다. 병행 큐는 생산자-소비자 문제(Producer-Consumer Problem)를 해결하기 위해 자주 사용되며, 락을 통해 데이터의 일관성을 유지합니다.

병행 큐는 삽입 연산과 제거 연산에 대해 별도의 락을 사용할 수 있습니다. 이를 통해 삽입과 제거가 동시에 이루어질 수 있어 성능이 향상됩니다. 하지만 락 사용으로 인한 성능 저하가 발생할 수 있으며, 특히 큐의 길이가 길어질수록 락 경합이 심해질 수 있습니다.

```c
typedef struct QueueNode {
    int data;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} Queue;

void enqueue(Queue* q, int value) {
    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    new_node->data = value;
    new_node->next = NULL;

    pthread_mutex_lock(&q->lock);  // 큐 락 획득
    if (q->rear == NULL) {
        q->front = new_node;
        q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
    pthread_cond_signal(&q->cond);  // 대기 중인 스레드에 시그널 전송
    pthread_mutex_unlock(&q->lock);  // 큐 락 해제
}

int dequeue(Queue* q) {
    pthread_mutex_lock(&q->lock);  // 큐 락 획득
    while (q->front == NULL) {
        pthread_cond_wait(&q->cond, &q->lock);  // 조건 변수를 사용하여 대기
    }
    QueueNode* temp = q->front;
    int value = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    pthread_mutex_unlock(&q->lock);  // 큐 락 해제
    return value;
}
```

위 코드에서는 큐 전체에 대한 하나의 락(`lock`)과 조건 변수(`cond`)를 사용합니다. `enqueue()` 함수에서는 큐 락을 획득한 후 새로운 노드를 큐의 rear에 추가하고, 대기 중인 스레드에 시그널을 보냅니다. `dequeue()` 함수에서는 큐 락을 획득하고, 큐가 비어 있는 경우 조건 변수를 사용하여 대기합니다. 큐에 요소가 있으면 front 노드를 제거하고 해당 값을 반환합니다.

병행 큐는 로그 시스템, 작업 스케줄러, 메시지 전달 시스템 등 생산자-소비자 패턴이 필요한 다양한 응용 프로그램에서 사용됩니다.

## 병행 해시 테이블(Concurrent Hash Table)

병행 해시 테이블은 다수의 스레드가 동시에 키-값 쌍을 삽입, 삭제 또는 조회할 수 있는 해시 테이블입니다. 병행 해시 테이블은 성능 향상을 위해 버킷(Bucket) 단위로 락을 사용할 수 있습니다.

각 버킷에 대해 개별적인 락을 사용하면 서로 다른 버킷에 대한 연산이 동시에 이루어질 수 있어 성능이 크게 향상됩니다. 하지만 락의 개수가 많아질수록 메모리 오버헤드가 증가하고 구현이 복잡해질 수 있습니다.

```c
#define HASH_SIZE 101

typedef struct HashNode {
    int key;
    int value;
    struct HashNode* next;
    pthread_mutex_t lock;
} HashNode;

typedef struct {
    HashNode* buckets[HASH_SIZE];
    pthread_mutex_t table_lock;
} HashTable;

unsigned int hash(int key) {
    return key % HASH_SIZE;
}

void insert(HashTable* table, int key, int value) {
    unsigned int index = hash(key);
    pthread_mutex_lock(&table->table_lock);  // 테이블 락 획득

    HashNode* new_node = (HashNode*)malloc(sizeof(HashNode));
    new_node->key = key;
    new_node->value = value;
    pthread_mutex_init(&new_node->lock, NULL);

    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;

    pthread_mutex_unlock(&table->table_lock);  // 테이블 락 해제
}

int lookup(HashTable* table, int key) {
    unsigned int index = hash(key);
    pthread_mutex_lock(&table->table_lock);  // 테이블 락 획득

    HashNode* current = table->buckets[index];
    while (current != NULL) {
        if (current->key == key) {
            pthread_mutex_unlock(&table->table_lock);  // 테이블 락 해제
            return current->value;
        }
        current = current->next;
    }

    pthread_mutex_unlock(&table->table_lock);  // 테이블 락 해제
    return -1; // 키를 찾지 못한 경우
}
```

위 코드에서는 해시 테이블 전체에 대한 하나의 락(`table_lock`)을 사용합니다. `insert()` 함수에서는 테이블 락을 획득한 후 새로운 노드를 해당 버킷에 추가합니다. `lookup()` 함수에서는 테이블 락을 획득하고 해당 버킷을 탐색하여 키에 해당하는 값을 반환합니다.

병행 해시 테이블은 데이터베이스 시스템, 캐시 시스템, 네트워크 라우팅 테이블 등 다양한 응용 프로그램에서 사용됩니다. 예를 들어, 웹 서버의 세션 관리 시스템에서는 세션 정보를 병행 해시 테이블에 저장하여 여러 스레드가 동시에 세션 정보를 효율적으로 조회하고 갱신할 수 있습니다.

## 락 기반 병행 자료 구조의 장단점

락 기반의 병행 자료 구조는 다음과 같은 장단점을 가지고 있습니다.

장점:

- 구현이 비교적 간단하고 직관적입니다.
- 데이터의 일관성을 보장할 수 있습니다.
- 다양한 자료 구조에 적용할 수 있습니다.

단점:

- 락 사용으로 인한 성능 저하가 발생할 수 있습니다.
- 락 경합이 발생하면 효율이 크게 떨어질 수 있습니다.
- 데드락이나 라이브락 등의 문제가 발생할 수 있습니다.
- 락의 개수가 많아질수록 메모리 오버헤드가 증가합니다.

따라서 락 기반의 병행 자료 구조를 사용할 때는 락의 granularity를 적절히 조절하고, 불필요한 락 사용을 최소화하며, 데드락 등의 문제를 예방하기 위한 주의가 필요합니다.

## 락 없는 병행 자료 구조

락 기반의 병행 자료 구조가 가진 단점을 해결하기 위해 락 없는(Lock-Free) 병행 자료 구조가 제안되었습니다. 락 없는 병행 자료 구조는 락을 사용하지 않고 아토믹 연산(Atomic Operation)을 활용하여 병행성을 제어합니다.

대표적인 락 없는 병행 자료 구조로는 다음과 같은 것들이 있습니다.

- 락 없는 스택(Lock-Free Stack)
- 락 없는 큐(Lock-Free Queue)
- 락 없는 해시 테이블(Lock-Free Hash Table)
- 락 없는 연결 리스트(Lock-Free Linked List)

락 없는 병행 자료 구조는 락 사용에 따른 성능 저하를 방지하고, 데드락 등의 문제를 해결할 수 있다는 장점이 있습니다. 하지만 구현이 복잡하고 아토믹 연산의 사용으로 인한 오버헤드가 발생할 수 있다는 단점도 있습니다.

## 결론

락 기반의 병행 자료 구조는 멀티스레드 환경에서 데이터의 일관성을 유지하기 위해 널리 사용되는 방법입니다. 병행 카운터, 병행 연결 리스트, 병행 큐, 병행 해시 테이블 등 다양한 자료 구조에 락을 적용하여 스레드 간의 동기화를 제어할 수 있습니다.

하지만 락 사용에 따른 성능 저하와 락 경합, 데드락 등의 문제를 인지하고 적절히 대처해야 합니다. 이를 위해 락의 granularity를 조절하고, 불필요한 락 사용을 최소화하며, 데드락 예방 기법을 활용하는 것이 중요합니다.

또한, 락 없는 병행 자료 구조와 같은 대안적인 방법을 고려하여 상황에 맞는 최적의 솔루션을 선택해야 합니다.

병행 프로그래밍에서 자료 구조의 선택과 구현은 매우 중요한 문제입니다. 락 기반의 병행 자료 구조와 락 없는 병행 자료 구조의 특성을 잘 이해하고, 적재적소에 활용함으로써 효율적이고 안정적인 병행 프로그램을 작성할 수 있을 것입니다.
