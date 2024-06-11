#include "kv_store.h" // kv_store의 함수와 구조체 선언 헤더 파일
#include <stdio.h>    // 표준 입출력 라이브러리
#include <stdlib.h>   // 표준 라이브러리 (메모리 할당, 변환 등)
#include <string.h>   // 문자열 처리 라이브러리

// Key-Value 쌍을 생성하는 함수
KeyValue* create_kv(int key, const char* value) {
    KeyValue* new_kv = (KeyValue*)malloc(sizeof(KeyValue)); // 새로운 Key-Value 쌍을 위한 메모리 할당
    if (new_kv) {                                           // 메모리 할당 성공 시
        new_kv->key = key;                                  // 키 값 설정
        strncpy(new_kv->value, value, MAX_VALUE);           // 값 설정
        new_kv->next = NULL;                                // 다음 포인터 초기화
    }
    return new_kv;                                          // 생성된 Key-Value 쌍 반환
}

// Key-Value 쌍을 메모리에서 해제하는 함수
void free_kv(KeyValue* head) {
    KeyValue* tmp;
    while (head) {                                          // 리스트의 모든 요소를 순회하며
        tmp = head;                                         // 현재 요소를 임시 변수에 저장
        head = head->next;                                  // 다음 요소로 이동
        free(tmp);                                          // 현재 요소 메모리 해제
    }
}

// Key-Value 쌍을 리스트에 삽입하는 함수
int insert_kv(KeyValue** head, int key, const char* value) {
    KeyValue* new_kv = create_kv(key, value);               // 새로운 Key-Value 쌍 생성
    if (!new_kv) return 0;                                  // 생성 실패 시 0 반환

    new_kv->next = *head;                                   // 새로운 요소의 다음 포인터를 현재 헤드로 설정
    *head = new_kv;                                         // 헤드를 새로운 요소로 업데이트
    return 1;                                               // 삽입 성공 시 1 반환
}

// 키 값에 해당하는 값을 검색하는 함수
char* get_kv(KeyValue* head, int key) {
    while (head) {                                          // 리스트의 모든 요소를 순회하며
        if (head->key == key) return head->value;           // 키 값이 일치하면 값을 반환
        head = head->next;                                  // 다음 요소로 이동
    }
    return NULL;                                            // 키 값이 없으면 NULL 반환
}

// 키 값에 해당하는 Key-Value 쌍을 삭제하는 함수
int delete_kv(KeyValue** head, int key) {
    KeyValue *tmp = *head, *prev = NULL;                    // 현재 요소와 이전 요소를 위한 포인터
    while (tmp) {                                           // 리스트의 모든 요소를 순회하며
        if (tmp->key == key) {                              // 키 값이 일치하면
            if (prev) {                                     // 이전 요소가 있으면
                prev->next = tmp->next;                     // 이전 요소의 다음 포인터를 현재 요소의 다음 포인터로 설정
            } else {                                        // 이전 요소가 없으면 (헤드인 경우)
                *head = tmp->next;                          // 헤드를 현재 요소의 다음 요소로 업데이트
            }
            free(tmp);                                      // 현재 요소 메모리 해제
            return 1;                                       // 삭제 성공 시 1 반환
        }
        prev = tmp;                                         // 이전 요소를 현재 요소로 업데이트
        tmp = tmp->next;                                    // 다음 요소로 이동
    }
    return 0;                                               // 키 값이 없으면 0 반환
}

// 데이터베이스를 초기화하는 함수
void clear_db(KeyValue** head) {
    free_kv(*head);                                         // 모든 요소 메모리 해제
    *head = NULL;                                           // 헤드를 NULL로 설정
}

// 모든 Key-Value 쌍을 출력하는 함수
void print_all_kv(KeyValue* head) {
    while (head) {                                          // 리스트의 모든 요소를 순회하며
        printf("%d,%s\n", head->key, head->value);          // 키 값과 값을 출력
        head = head->next;                                  // 다음 요소로 이동
    }
}

// 데이터베이스를 파일에서 로드하는 함수
int load_db(KeyValue** head) {
    FILE* file = fopen(DATABASE_FILE, "r");                 // 데이터베이스 파일 열기
    if (!file) return -1;                                   // 파일 열기 실패 시 -1 반환

    int key;
    char value[MAX_VALUE];
    while (fscanf(file, "%d,%99s\n", &key, value) != EOF) { // 파일에서 키 값과 값을 읽어와 리스트에 삽입
        insert_kv(head, key, value);
    }
    fclose(file);                                           // 파일 닫기
    return 0;                                               // 로드 성공 시 0 반환
}

// 데이터베이스를 파일에 저장하는 함수
int save_db(KeyValue* head) {
    FILE* file = fopen(DATABASE_FILE, "w");                 // 데이터베이스 파일 쓰기 모드로 열기
    if (!file) return -1;                                   // 파일 열기 실패 시 -1 반환

    while (head) {                                          // 리스트의 모든 요소를 순회하며
        fprintf(file, "%d,%s\n", head->key, head->value);   // 키 값과 값을 파일에 저장
        head = head->next;                                  // 다음 요소로 이동
    }
    fclose(file);                                           // 파일 닫기
    return 0;                                               // 저장 성공 시 0 반환
}
