# Lab: 메모리 구조 탐색하기

## 목표

이 실습에서는 C 프로그램에서 메모리의 다양한 영역(코드, 힙, 스택)의 위치를 확인하는 방법을 배웁니다. 또한, Makefile을 사용하여 프로그램을 편리하게 빌드, 실행 및 정리하는 방법도 알아봅니다.

## 준비사항

- C 컴파일러 (gcc)
- 텍스트 에디터 (VS Code, Sublime Text, Vim 등)
- 터미널 (Linux, macOS) 또는 Command Prompt (Windows)

## 실습 내용

### 1. C 프로그램 (`va.c`) 작성

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("location of code : %p\n", main);
    printf("location of heap : %p\n", malloc(100e6));
    int x = 3;
    printf("location of stack: %p\n", &x);
    return 0;
}
```

이 프로그램은 다음과 같은 작업을 수행합니다:

1. 필요한 헤더 파일을 포함시킵니다: 입출력 함수를 위한 `stdio.h`와 메모리 할당 함수를 위한 `stdlib.h`.
2. `main` 함수에서:
   - `main` 함수의 주소를 사용하여 코드 영역의 위치를 출력합니다.
   - `malloc`을 사용하여 大량의 메모리(1억 바이트)를 할당하고, 힙의 위치를 출력합니다.
   - 정수 변수 `x`를 선언하고, `x`의 주소를 사용하여 스택의 위치를 출력합니다.
3. 마지막으로 0을 반환하여 프로그램이 성공적으로 실행되었음을 나타냅니다.

이 프로그램의 목적은 실행 중인 프로그램에서 다양한 메모리 영역(코드, 힙, 스택)의 위치를 보여주는 것입니다.

### 2. Makefile 작성

```makefile
.DEFAULT_GOAL := help

.PHONY: help
help: ## Display this help
	@awk 'BEGIN {FS = ":.*##"; printf "\nUsage:\n make \033[36m<target>\033[0m\n"} /^[a-zA-Z_-]+:.*?##/ { printf " \033[36m%-25s\033[0m %s\n", $$1, $$2 } /^##@/ { printf "\n\033[1m%s\033[0m\n", substr($$0, 5) } ' $(MAKEFILE_LIST)

##@ Build

all: va ## Build all targets

va: ## Build target va
	gcc -o va va.c -Wall

##@ Run

run: ## Run the lottery program
	./va

##@ Clean-up

clean: ## Remove all build artifacts
	rm -f va
```

이 Makefile은 프로그램을 빌드, 실행 및 정리하기 위한 여러 타겟과 명령을 제공합니다:

- `.DEFAULT_GOAL`은 `help`로 설정되어 있어, 타겟 없이 `make`를 실행하면 도움말 메시지가 표시됩니다.
- `help` 타겟은 `awk` 명령을 사용하여 Makefile을 파싱하고, 사용 가능한 타겟과 그에 대한 설명이 포함된 도움말 메시지를 표시합니다.
- `all` 타겟은 `va` 타겟에 의존하며, 모든 타겟(여기서는 `va` 뿐)을 빌드하는 데 사용됩니다.
- `va` 타겟은 `gcc`를 사용하여 `va.c` 파일을 컴파일하고 `va`라는 실행 파일을 생성합니다.
- `run` 타겟은 `va` 프로그램을 실행합니다.
- `clean` 타겟은 생성된 실행 파일 `va`를 제거합니다.

### 3. 프로그램 빌드 및 실행

1. `va.c`와 `Makefile`을 같은 디렉토리에 저장합니다.
2. 터미널을 열고 해당 디렉토리로 이동합니다.
3. 다음 명령을 실행합니다:
   - `make` 또는 `make help`: 사용 가능한 타겟이 포함된 도움말 메시지를 표시합니다.
   - `make all` 또는 `make va`: `va.c` 파일을 컴파일하고 `va` 실행 파일을 생성합니다.
   - `make run`: `va` 프로그램을 실행합니다.
   - `make clean`: `va` 실행 파일을 제거합니다.

## 결론

이 실습을 통해 C 프로그램에서 메모리의 다양한 영역의 위치를 확인하는 방법과 Makefile을 사용하여 프로그램을 편리하게 관리하는 방법을 배웠습니다. 이러한 기술은 C 프로그래밍과 시스템 프로그래밍에서 유용하게 활용될 수 있습니다.
