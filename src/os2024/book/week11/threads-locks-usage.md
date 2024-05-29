# 락 기반의 병행 자료 구조

병행 자료 구조는 다수의 스레드가 동시에 접근할 수 있는 자료 구조를 말합니다. 이러한 자료 구조는 병행성 문제를 해결하기 위해 락을 사용하여 일관성을 유지합니다. 락 기반의 병행 자료 구조는 스레드가 데이터에 접근할 때 충돌을 방지하고, 데이터의 일관성을 유지하는 데 중요한 역할을 합니다. 아래에서는 락 기반의 병행 자료 구조의 예로 병행 카운터, 병행 연결 리스트, 병행 큐, 병행 해시 테이블을 설명합니다.

## 병행 카운터

병행 카운터는 다수의 스레드가 동시에 값을 증가시키거나 감소시킬 수 있는 카운터입니다. 병행 카운터에서 중요한 점은 동시에 여러 스레드가 값을 변경할 때도 카운터의 값이 정확하게 유지되어야 한다는 것입니다.

- **락을 이용한 구현**: 병행 카운터는 락을 사용하여 스레드가 카운터의 값을 읽고 쓸 때 다른 스레드가 접근하지 못하도록 합니다. 예를 들어, 카운터를 증가시키는 연산에서는 먼저 락을 획득하고, 값을 증가시킨 후 락을 해제합니다.
- **장점**: 구현이 비교적 간단하며, 락을 사용하여 데이터 일관성을 보장할 수 있습니다.
- **단점**: 모든 접근에 대해 락을 사용하므로 성능이 저하될 수 있습니다. 특히, 많은 스레드가 동시에 접근할 때 락 경합이 발생하여 효율이 떨어질 수 있습니다.

### 병행 카운터 예시 코드

```c
pthread_mutex_t lock;
int counter = 0;

void increment() {
    pthread_mutex_lock(&lock);
    counter++;
    pthread_mutex_unlock(&lock);
}

void decrement() {
    pthread_mutex_lock(&lock);
    counter--;
    pthread_mutex_unlock(&lock);
}
```

![image](https://github.com/chu-aie/os-2024/assets/133697595/d7a6a192-00d7-48a1-b302-904c109febbd)

### 병행 카운터의 적용 사례

병행 카운터는 다양한 응용 프로그램에서 사용됩니다. 예를 들어, 웹 서버에서는 활성 사용자 수를 추적하기 위해 병행 카운터를 사용할 수 있으며, 데이터베이스에서는 특정 이벤트가 발생한 횟수를 기록하는 데 사용할 수 있습니다.

네, 이 사진은 병행 연결 리스트의 락 동작을 설명하는 데 적합합니다. 두 개의 스레드가 각각 노드를 추가하거나 변경하는 과정을 시각적으로 잘 나타내고 있습니다.

따라서, 아래와 같이 이 그림을 사용하여 설명을 보완하면 됩니다:

---

## 병행 연결 리스트

병행 연결 리스트는 다수의 스레드가 동시에 노드를 추가하거나 삭제할 수 있는 연결 리스트입니다. 병행 연결 리스트는 락을 사용하여 스레드가 리스트의 노드를 안전하게 수정할 수 있도록 합니다.

- **락을 이용한 구현**: 병행 연결 리스트는 노드별로 락을 사용하거나, 전체 리스트에 대한 단일 락을 사용할 수 있습니다. 노드별 락을 사용하면 성능을 향상시킬 수 있지만, 구현이 복잡해질 수 있습니다.
- **장점**: 여러 스레드가 동시에 리스트의 다른 부분을 수정할 수 있어 성능이 향상될 수 있습니다.
- **단점**: 락의 범위와 granularity에 따라 구현이 복잡해질 수 있으며, 잘못된 락 사용은 데드락이나 라이브락을 초래할 수 있습니다.

### 병행 연결 리스트 예시 코드

```c
// 락을 이용한 병행 연결 리스트 예시 (C언어)
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

    pthread_mutex_lock(&list_lock);
    new_node->next = head;
    head = new_node;
    pthread_mutex_unlock(&list_lock);
}

void remove_node(int value) {
    pthread_mutex_lock(&list_lock);
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
    pthread_mutex_unlock(&list_lock);
}
```

![image](https://github.com/chu-aie/os-2024/assets/133697595/252042a7-e2e5-4513-b55f-9e8b6070f730)

### 병행 연결 리스트의 적용 사례

병행 연결 리스트는 운영 체제의 스케줄러, 메모리 할당기, 네트워크 프로토콜 스택 등 다양한 시스템 소프트웨어에서 사용됩니다. 예를 들어, 스케줄러는 실행 중인 프로세스나 스레드 목록을 유지하기 위해 병행 연결 리스트를 사용할 수 있습니다.

## 병행 큐

병행 큐는 다수의 스레드가 동시에 요소를 삽입하거나 제거할 수 있는 큐입니다. 병행 큐는 생산자-소비자 문제를 해결하기 위해 자주 사용되며, 락을 사용하여 데이터의 일관성을 보장합니다.

- **락을 이용한 구현**: 병행 큐는 삽입 연산과 제거 연산 각각에 대해 별도의 락을 사용할 수 있습니다. 이렇게 하면 삽입과 제거가 동시에 이루어질 수 있습니다.
- **장점**: 생산자-소비자 패턴에서 효율적으로 사용할 수 있으며, 락을 사용하여 데이터의 일관성을 유지할 수 있습니다.
- **단점**: 락을 사용함으로써 성능 저하가 발생할 수 있으며, 특히 큐의 길이가 길어질수록 락 경합이 심해질 수 있습니다.

### 병행 큐 예시 코드

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

    pthread_mutex_lock(&q->lock);
    if (q->rear == NULL) {
        q->front = new_node;
        q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->lock);
}

int dequeue(Queue* q) {
    pthread_mutex_lock(&q->lock);
    while (q->front == NULL) {
        pthread_cond_wait(&q->cond, &q->lock);
    }
    QueueNode* temp = q->front;
    int value = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    pthread_mutex_unlock(&q->lock);
    return value;
}
```

[병행 큐의 락 동작]

### 병행 큐의 적용 사례

병행 큐는 생산자-소비자 패턴이 필요한 다양한 시스템에서 사용됩니다. 예를 들어, 로그 시스템에서는 여러 스레드가 로그 메시지를 병행 큐에 삽입하고, 별도의 스레드가 큐에서 로그 메시지를 꺼내어 파일에 기록할 수 있습니다.

## 병행 해시 테이블

병행 해시 테이블은 다수의 스레드가 동시에 키-값 쌍을 삽입, 삭제 또는 조회할 수 있는 해시 테이블입니다. 병행 해시 테이블은 성능을 위해 버킷 단위로 락을 사용할 수 있습니다.

- **락을 이용한 구현**: 병행 해시 테이블은 각 버킷에 대해 개별 락을 사용하거나, 필요에 따라 전체 테이블에 대한 락을 사용할 수 있습니다. 버킷 단위 락을 사용하면 성능을 크게 향상시킬 수 있습니다.
- **장점**: 여러 스레드가 동시에 다른 버킷에 접근할 수 있어 성능이 향상될 수 있습니다.
- **단점**: 락의 granularity에 따라 구현이 복잡해질 수 있으며, 적절한 락 관리는 필수적입니다.

### 병행 해시 테이블 예시 코드

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
    pthread_mutex_lock(&table->table_lock);

    HashNode* new_node = (HashNode*)malloc(sizeof(HashNode));


    new_node->key = key;
    new_node->value = value;
    pthread_mutex_init(&new_node->lock, NULL);

    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;

    pthread_mutex_unlock(&table->table_lock);
}

int lookup(HashTable* table, int key) {
    unsigned int index = hash(key);
    pthread_mutex_lock(&table->table_lock);

    HashNode* current = table->buckets[index];
    while (current != NULL) {
        if (current->key == key) {
            pthread_mutex_unlock(&table->table_lock);
            return current->value;
        }
        current = current->next;
    }

    pthread_mutex_unlock(&table->table_lock);
    return -1; // Key not found
}
```

[병행 해시 테이블의 락 동작]

### 병행 해시 테이블의 적용 사례

병행 해시 테이블은 데이터베이스 시스템, 캐시 시스템, 네트워크 라우팅 테이블 등에서 사용됩니다. 예를 들어, 웹 서버의 세션 관리 시스템에서는 각 세션의 정보를 병행 해시 테이블에 저장하여 다수의 스레드가 동시에 세션 정보를 조회하거나 갱신할 수 있습니다.

## 요약

락 기반의 병행 자료 구조는 다수의 스레드가 동시에 접근할 수 있는 자료 구조를 락을 통해 안전하게 구현한 것입니다. 병행 카운터, 병행 연결 리스트, 병행 큐, 병행 해시 테이블 등 다양한 자료 구조가 있으며, 각각의 구현에는 락을 사용하여 데이터 일관성을 보장합니다. 락을 사용함으로써 데이터의 일관성을 유지할 수 있지만, 성능 저하나 락 경합 문제를 피하기 위해 적절한 락 관리가 필수적입니다. 이를 통해 병행성 문제를 해결하고, 멀티스레드 환경에서 안정적으로 동작하는 자료 구조를 설계할 수 있습니다.
