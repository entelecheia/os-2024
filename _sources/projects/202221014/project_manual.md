## 1. 디렉토리 구조

```
/home/qhddybuntu/workspace/projects/os-2024/book/projects/202221014
└── kv_store
    ├── src
    │   ├── main.c
    │   ├── kv_store.c
    │   └── kv_store.h
    ├── build
    ├── database.txt
    └── Makefile
```

### 1.1 디렉토리 생성

```bash
cd /home/qhddybuntu/workspace/projects/os-2024/book/projects/202221014
mkdir kv_store
cd kv_store
mkdir src build
touch src/main.c src/kv_store.h src/kv_store.c database.txt Makefile
```

## 2. 프로그램 작성

> main.c

```c
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
```

> kv_store.c

```c
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
```

> kv_store.h

```c
#ifndef KV_STORE_H
#define KV_STORE_H

#define DATABASE_FILE "database.txt" // 데이터베이스 파일 이름
#define MAX_KEY 100                  // 최대 키 값
#define MAX_VALUE 100                // 최대 값 길이

// Key-Value 쌍을 저장하는 구조체
typedef struct KeyValue {
    int key;                         // 키 값
    char value[MAX_VALUE];           // 값
    struct KeyValue* next;           // 다음 Key-Value 쌍을 가리키는 포인터
} KeyValue;

// 함수 선언
KeyValue* create_kv(int key, const char* value);   // Key-Value 쌍 생성
void free_kv(KeyValue* head);                      // Key-Value 쌍 메모리 해제
int insert_kv(KeyValue** head, int key, const char* value); // Key-Value 쌍 삽입
char* get_kv(KeyValue* head, int key);             // 키 값에 해당하는 값 검색
int delete_kv(KeyValue** head, int key);           // Key-Value 쌍 삭제
void clear_db(KeyValue** head);                    // 데이터베이스 초기화
void print_all_kv(KeyValue* head);                 // 모든 Key-Value 쌍 출력
int load_db(KeyValue** head);                      // 데이터베이스 로드
int save_db(KeyValue* head);                       // 데이터베이스 저장

#endif
```

```makefile
## 컴파일러 설정
CC = gcc                            ## 사용할 컴파일러
CFLAGS = -Wall -Wextra -std=c99     ## 컴파일 옵션: 모든 경고 표시, C99 표준 사용

## 소스 파일 및 오브젝트 파일 설정
SRC = src/main.c src/kv_store.c     ## 소스 파일 목록
OBJ = $(SRC:.c=.o)                  ## 오브젝트 파일 목록 (.c를 .o로 대체)

## 타겟 설정
TARGET = kv_store                   ## 생성할 실행 파일 이름

## 기본 타겟
all: $(TARGET)

## 실행 파일 생성 규칙
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

## 오브젝트 파일 생성 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

## clean 타겟: 생성된 파일들 삭제
clean:
	rm -f $(OBJ) $(TARGET)

## .PHONY 타겟: 실제 파일이 아닌 타겟 설정
.PHONY: all clean
```

## 3. 빌드 및 실행

```bash
## 프로젝트 디렉토리 이동
cd /home/qhddybuntu/workspace/projects/os-2024/book/projects/202221014/kv_store

## Makefile를 사용하여 프로젝트 빌드
make

## 프로그램 실행
./kv_store
```

## 4. 실행 예제

**4.1 Key-Value 쌍 삽입**

```bash
cv p,1,Gyubeom
cv p,8,Apple
cv p,50,Melon
```

**4.2 키 값 검색**

```bash
cv g,1
1,Gyubeom
cv g,8
8,Apple
cv g,50
50,Melon
```

**4.3 Key-Value 쌍 삭제**

```bash
cv d,8
cv g,8
K not found (K=8)
```

**4.4 모든 Key-Value 쌍 출력**

```bash
cv a
1,Gyubeom
50,Melon
```

**4.5 추가 Key-Value 쌍 삽입 및 출력**

```bash
cv p,2,Banana
cv p,3,Grape
cv a
1,Gyubeom
2,Banana
3,Grape
50,Melon
```

**4.6 데이터베이스 지우기 및 확인**

```bash
cv c
database is empty
cv a
database is empty
```

**4.7 프로그램 종료**

```bash
exit
```

### 전체 예제 세션

```bash
cv> cv p,1,Gyubeom
cv> cv p,8,Aplle
cv> cv p,50,Melon
cv> cv g,1
10,Gyubeom
cv> cv g,8
8,Aplle
cv> cv g,30
30,Incheon
cv> cv d,20
cv> cv g,20
K not found (K=20)
cv> cv a
10,Seoul
30,Incheon
cv> cv c
database is empty
cv> cv a
database is empty
cv> cv p,40,Gwangju
cv> cv a
40,Gwangju
cv> exit
```
