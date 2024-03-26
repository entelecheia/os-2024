#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// 전역 티켓 카운트
int gtickets = 0;

struct node_t
{
    int tickets;
    struct node_t *next;
};

struct node_t *head = NULL;

void insert(int tickets)
{
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

void print_list()
{
    struct node_t *curr = head;
    printf("List: ");

    // 리스트를 순회하며 각 노드의 티켓 수를 출력
    while (curr)
    {
        printf("[%d] ", curr->tickets);
        curr = curr->next;
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
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
    for (i = 0; i < loops; i++)
    {
        int counter = 0;
        // 전체 티켓 수 범위 내에서 랜덤한 당첨 번호 선택
        int winner = random() % gtickets;
        struct node_t *current = head;

        // 티켓 수의 합이 당첨 번호보다 클 때까지 리스트를 순회
        while (current)
        {
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
