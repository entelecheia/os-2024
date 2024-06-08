#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LENGTH 256
#define MAX_VALUE_LENGTH 256
#define DATABASE_FILE "database.txt"

// 키-값 쌍을 저장할 구조체 정의
typedef struct KeyValue {
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
    struct KeyValue *next;
} KeyValue;

KeyValue *head = NULL;

// 데이터베이스 파일에서 데이터를 읽어오는 함수
void load_database() {
    FILE *file = fopen(DATABASE_FILE, "r");
    if (file == NULL) return;

    char line[MAX_KEY_LENGTH + MAX_VALUE_LENGTH + 2];
    while (fgets(line, sizeof(line), file)) {
        KeyValue *new_kv = (KeyValue *)malloc(sizeof(KeyValue));
        if (new_kv == NULL) {
            printf("메모리 할당 실패.\n");
            exit(1);
        }
        sscanf(line, "%[^,],%s", new_kv->key, new_kv->value);
        new_kv->next = head;
        head = new_kv;
    }

    fclose(file);
}

// 메모리 내의 데이터를 데이터베이스 파일에 저장하는 함수
void save_database() {
    FILE *file = fopen(DATABASE_FILE, "w");
    if (file == NULL) {
        printf("파일 열기 실패.\n");
        exit(1);
    }

    KeyValue *current = head;
    while (current != NULL) {
        fprintf(file, "%s,%s\n", current->key, current->value);
        current = current->next;
    }

    fclose(file);
}

// 새로운 키-값 쌍을 추가하는 함수
void put(char *key, char *value) {
    KeyValue *new_kv = (KeyValue *)malloc(sizeof(KeyValue));
    if (new_kv == NULL) {
        printf("메모리 할당 실패.\n");
        exit(1);
    }
    strcpy(new_kv->key, key);
    strcpy(new_kv->value, value);
    new_kv->next = head;
    head = new_kv;
    printf("키-값 쌍 추가 (%s, %s)\n", key, value);
}

// 키에 해당하는 값을 검색하는 함수
void get(char *key) {
    KeyValue *current = head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            printf("키-값 쌍 찾음 (%s, %s)\n", key, current->value);
            return;
        }
        current = current->next;
    }
    printf("키 %s 를 찾을 수 없음.\n", key);
}

// 키-값 쌍을 삭제하는 함수
void delete(char *key) {
    KeyValue *current = head;
    KeyValue *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (previous == NULL) {
                head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("키 %s 삭제됨\n", key);
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("키 %s 를 찾을 수 없음.\n", key);
}

// 모든 키-값 쌍을 제거하는 함수
void clear() {
    KeyValue *current = head;
    while (current != NULL) {
        KeyValue *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    printf("모든 키-값 쌍 삭제됨.\n");
}

// 모든 키-값 쌍을 출력하는 함수
void all() {
    KeyValue *current = head;
    while (current != NULL) {
        printf("키: %s, 값: %s\n", current->key, current->value);
        current = current->next;
    }
}

// 명령어를 처리하는 함수
void process_command(char *command) {
    char action;
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];

    if (sscanf(command, "p,%[^,],%s", key, value) == 2) {
        put(key, value);
    } else if (sscanf(command, "g,%s", key) == 1) {
        get(key);
    } else if (sscanf(command, "d,%s", key) == 1) {
        delete(key);
    } else if (strcmp(command, "c") == 0) {
        clear();
    } else if (strcmp(command, "a") == 0) {
        all();
    } else {
        printf("잘못된 명령어\n");
    }
}

// 메인 함수
int main() {
    char command[512];

    load_database(); // 프로그램 시작 시 데이터베이스 로드

    printf("Key-Value store project by ypilseong\n");
    printf("202121010 양필성\n");

    while (1) {
        printf("ysh: ");
        if (fgets(command, sizeof(command), stdin) == NULL) break;
        command[strcspn(command, "\n")] = '\0'; // 개행 문자 제거
        if (strcmp(command, "exit") == 0) break;
        process_command(command);
    }

    save_database(); // 프로그램 종료 시 데이터베이스 저장
    clear(); // 종료 전 메모리 해제
    return 0;
}
