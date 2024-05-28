# 컴파일러 설정
CC = gcc                            # 사용할 컴파일러
CFLAGS = -Wall -Wextra -std=c99     # 컴파일 옵션: 모든 경고 표시, C99 표준 사용

# 소스 파일 및 오브젝트 파일 설정
SRC = src/main.c src/kv_store.c     # 소스 파일 목록
OBJ = $(SRC:.c=.o)                  # 오브젝트 파일 목록 (.c를 .o로 대체)

# 타겟 설정
TARGET = kv_store                   # 생성할 실행 파일 이름

# 기본 타겟
all: $(TARGET)

# 실행 파일 생성 규칙
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# 오브젝트 파일 생성 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# clean 타겟: 생성된 파일들 삭제
clean:
	rm -f $(OBJ) $(TARGET)

# .PHONY 타겟: 실제 파일이 아닌 타겟 설정
.PHONY: all clean
