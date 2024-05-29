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
