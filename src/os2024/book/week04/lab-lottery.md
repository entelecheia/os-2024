# Lab: Lottery 스케줄링 알고리즘 구현

## 실습 목표

이 실습은 다음과 같은 목표를 달성하기 위해 설계되었습니다.

1. Lottery 스케줄링 알고리즘의 기본 개념을 이해합니다.
2. 연결 리스트를 사용하여 작업 큐를 구현하는 방법을 배웁니다.
3. 랜덤 번호 생성을 통해 작업을 선택하는 방법을 익힙니다.
4. Makefile을 사용하여 프로그램 빌드 및 실행을 자동화하는 방법을 알아봅니다.

## 실습 내용

이 실습에서는 lottery.c 파일과 Makefile을 사용하여 Lottery 스케줄링 알고리즘을 구현하고 실행해봅니다.

### lottery.c - Lottery 스케줄링 알고리즘 구현

- 연결 리스트를 사용하여 작업 큐를 구현합니다.
- 각 작업에는 티켓 수가 할당되며, 전체 티켓 수는 `gtickets` 변수에 저장됩니다.
- `insert` 함수를 사용하여 새로운 작업을 큐에 추가하고 티켓 수를 할당합니다.
- `print_list` 함수를 사용하여 현재 작업 큐의 상태를 출력합니다.
- `main` 함수에서는 명령행 인자로 시드와 루프 횟수를 받아와 Lottery 스케줄링을 수행합니다.
  - 랜덤 번호를 생성하여 당첨된 티켓을 선택합니다.
  - 선택된 티켓에 해당하는 작업을 찾아 스케줄링합니다.
  - 지정된 루프 횟수만큼 스케줄링을 반복합니다.

### Makefile - 빌드 및 실행 자동화

- `make all` 명령어를 사용하여 `lottery` 프로그램을 빌드합니다.
- `make run` 명령어를 사용하여 빌드된 `lottery` 프로그램을 실행합니다.
  - 시드 값과 루프 횟수를 인자로 전달합니다.
- `make clean` 명령어를 사용하여 빌드된 `lottery` 프로그램을 삭제합니다.

실습을 통해 Lottery 스케줄링 알고리즘의 동작 원리를 이해하고, 연결 리스트를 사용한 작업 큐 구현 방법을 익힐 수 있습니다. 또한, Makefile을 활용하여 프로그램 빌드 및 실행 과정을 자동화하는 방법도 배울 수 있습니다.

## 실습

1. lottery.c 파일을 열어 코드를 이해하고, 주석을 통해 각 부분의 동작을 파악합니다.
2. Makefile을 열어 빌드 및 실행 규칙을 확인합니다.
3. 터미널에서 `make all` 명령어를 입력하여 `lottery` 프로그램을 빌드합니다.
4. 터미널에서 `make run` 명령어를 입력하여 `lottery` 프로그램을 실행합니다.
5. 실행 결과를 통해 Lottery 스케줄링 알고리즘의 동작을 확인합니다.
6. 필요한 경우 `make clean` 명령어를 사용하여 빌드된 프로그램을 삭제합니다.

이 실습을 통해 Lottery 스케줄링 알고리즘의 구현과 동작 원리를 이해하고, 연결 리스트와 Makefile의 활용 방법을 익힐 수 있을 것입니다.

모든 코드는 [수업 리포](https://github.com/entelecheia/os-2024/blob/main/src/ostep/cpu-sched-lottery)에서 확인할 수 있습니다.

### lottery.c - Lottery 스케줄링 알고리즘 구현

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// 전역 티켓 카운트
int gtickets = 0;

struct node_t {
    int tickets;
    struct node_t *next;
};

struct node_t *head = NULL;

void insert(int tickets) {
    // 새로운 노드를 생성하고 메모리 할당
    struct node_t *tmp = malloc(sizeof(struct node_t));
    assert(tmp != NULL);

    // 새로운 노드의 티켓 수를 설정하고 리스트의 헤드에 삽입
    tmp->tickets = tickets;
    tmp->next = head;
    head = tmp;

    // 전역 티켓 카운트 업데이트
    gtickets += tickets;
}

void print_list() {
    struct node_t *curr = head;
    printf("List: ");

    // 리스트를 순회하며 각 노드의 티켓 수를 출력
    while (curr) {
        printf("[%d] ", curr->tickets);
        curr = curr->next;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: lottery <seed> <loops>\n");
        exit(1);
    }

    // 명령행 인자에서 시드와 루프 횟수를 가져옴
    int seed = atoi(argv[1]);
    int loops = atoi(argv[2]);

    // 랜덤 시드 설정
    srandom(seed);

    // 리스트에 작업들을 추가하고 각 작업에 티켓 수를 할당
    insert(50);
    insert(100);
    insert(25);

    // 리스트 출력
    print_list();

    int i;
    for (i = 0; i < loops; i++) {
        int counter = 0;
        // 전체 티켓 수 범위 내에서 랜덤한 당첨 번호 선택
        int winner = random() % gtickets;
        struct node_t *current = head;

        // 티켓 수의 합이 당첨 번호보다 클 때까지 리스트를 순회
        while (current) {
            counter = counter + current->tickets;
            if (counter > winner)
                break; // 당첨자 찾음
            current = current->next;
        }

        // 당첨자 출력 및 스케줄링
        print_list();
        printf("winner: %d %d\n\n", winner, current->tickets);
    }

    return 0;
}
```

이 코드는 lottery 스케줄링 알고리즘을 구현한 예제입니다. lottery 스케줄링은 각 작업에 티켓 수를 할당하고, 랜덤으로 당첨 번호를 선택하여 해당 번호에 해당하는 작업을 실행하는 방식으로 동작합니다.

코드의 주요 동작은 다음과 같습니다:

1. `gtickets` 변수는 전체 티켓 수를 추적하기 위한 전역 변수입니다.
2. `node_t` 구조체는 각 작업을 나타내며, 티켓 수와 다음 노드에 대한 포인터를 가집니다.
3. `insert` 함수는 새로운 작업을 리스트에 삽입하고 전체 티켓 수를 업데이트합니다.
4. `print_list` 함수는 리스트의 모든 작업과 해당 티켓 수를 출력합니다.
5. `main` 함수에서는 명령행 인자로 시드와 루프 횟수를 받아옵니다.
6. 랜덤 시드를 설정하고, 리스트에 작업들을 추가합니다.
7. 지정된 루프 횟수만큼 반복하면서 다음을 수행합니다:
   - 전체 티켓 수 범위 내에서 랜덤한 당첨 번호를 선택합니다.
   - 리스트를 순회하면서 티켓 수의 합이 당첨 번호보다 클 때까지 탐색합니다.
   - 당첨된 작업을 출력하고 스케줄링합니다.
8. 프로그램을 종료합니다.

이 코드는 lottery 스케줄링의 기본 개념을 보여주는 예제로, 실제 시스템에서는 더 복잡한 구현이 필요할 수 있습니다. 하지만 이 예제를 통해 lottery 스케줄링의 기본 아이디어를 이해할 수 있습니다.
