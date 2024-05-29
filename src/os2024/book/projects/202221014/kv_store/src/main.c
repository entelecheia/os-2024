#include <stdio.h>    // 표준 입출력 라이브러리
#include <stdlib.h>   // 표준 라이브러리 (메모리 할당, 변환 등)
#include <string.h>   // 문자열 처리 라이브러리
#include "kv_store.h" // kv_store의 함수와 구조체 선언 헤더 파일

KeyValue* kv_head = NULL; // Key-Value 저장소의 헤드 포인터

// 명령어를 실행하는 함수
void execute_command(char* command) {
    char* token = strtok(command, ",");         // 명령어와 인자를 ','로 구분
    if (strcmp(token, "cv p") == 0) {           // 명령어가 "cv p"인 경우 (삽입)
        int key = atoi(strtok(NULL, ","));      // 키를 정수로 변환
        char* value = strtok(NULL, ",");        // 값을 문자열로 받음
        if (!insert_kv(&kv_head, key, value)) { // 삽입 실패 시 메시지 출력
            printf("Failed to insert key: %d\n", key);
        }
    } else if (strcmp(token, "cv g") == 0) {    // 명령어가 "cv g"인 경우 (검색)
        int key = atoi(strtok(NULL, ","));      // 키를 정수로 변환
        char* value = get_kv(kv_head, key);     // 키에 해당하는 값을 검색
        if (value) {
            printf("%d,%s\n", key, value);      // 값을 찾으면 출력
        } else {
            printf("K not found (K=%d)\n", key);// 값을 찾지 못하면 메시지 출력
        }
    } else if (strcmp(token, "cv d") == 0) {    // 명령어가 "cv d"인 경우 (삭제)
        int key = atoi(strtok(NULL, ","));      // 키를 정수로 변환
        if (!delete_kv(&kv_head, key)) {        // 삭제 실패 시 메시지 출력
            printf("K not found (K=%d)\n", key);
        }
    } else if (strcmp(token, "cv c") == 0) {    // 명령어가 "cv c"인 경우 (전체 삭제)
        clear_db(&kv_head);                     // 데이터베이스를 초기화
        printf("database is empty\n");
    } else if (strcmp(token, "cv a") == 0) {    // 명령어가 "cv a"인 경우 (전체 출력)
        if (kv_head == NULL) {
            printf("database is empty\n");      // 데이터베이스가 비어있으면 메시지 출력
        } else {
            print_all_kv(kv_head);              // 데이터베이스의 모든 값을 출력
        }
    } else {
        printf("Invalid command\n");            // 알 수 없는 명령어에 대한 메시지 출력
    }
}

// 메인 함수
int main() {
    if (load_db(&kv_head) == -1) {              // 데이터베이스 로드
        printf("Failed to load database\n");    // 로드 실패 시 메시지 출력
    }
    
    char command[256];                          // 명령어를 저장할 배열
    while (1) {
        printf("cv> ");                         // 프롬프트 출력
        if (!fgets(command, 256, stdin)) break; // 사용자로부터 명령어 입력받음
        command[strcspn(command, "\n")] = 0;    // 입력된 명령어에서 개행문자 제거
        execute_command(command);               // 명령어 실행
    }
    
    if (save_db(kv_head) == -1) {               // 데이터베이스 저장
        printf("Failed to save database\n");    // 저장 실패 시 메시지 출력
    }
    
    free_kv(kv_head);                           // 메모리 해제
    return 0;
}
