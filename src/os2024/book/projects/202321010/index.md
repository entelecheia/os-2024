# reverse 프로젝트

## 프로젝트의 목적
역순 프로그램의 목표는 지정된 입력 파일에서 데이터를 읽어 역순으로 만드는 것입니다. 

## 주요 기능

- 입력
- 출력
- 역순
- 오류 출력

## 사용 기술

파일 읽기 및 쓰기 함수
- fopen(): 파일을 엽니다.
- fclose(): 파일을 닫습니다.
- fgets(): 파일에서 한 줄씩 읽어옵니다.
- fprintf(): 파일에 서식화된 데이터를 씁니다.

연결 리스트 관련 함수
- malloc(): 메모리를 동적으로 할당합니다.
- free(): 동적으로 할당된 메모리를 해제합니다.

문자열 처리 함수
- strcmp(): 두 문자열을 비교합니다.
- strcpy(): 문자열을 복사합니다.
- strlen(): 문자열의 길이를 반환합니다.

## 진행계획
﻿
- 요구사항 분석: 문제 이해와 프로그램 요구사항을 정확히 이해한다.
- 설계: 프로그램의 기능을 분석하고, 모듈화하여 설계한다. 각각의 실행 방법에 따라 동작을 구분한다.
- 구현: 설계한 내용을 바탕으로 프로그램을 구현한다. 파일 입출력, 명령줄 인수 처리 등을 구현한다.
- 테스트: 다양한 입력 조건에 대해 프로그램을 테스트하여 정확성을 확인한다.
- 디버깅: 프로그램의 오류를 찾고 수정한다.

## 코드
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

void reverse_lines(FILE *input, FILE *output) {
    char **lines = NULL;
    size_t lines_count = 0;
    size_t lines_capacity = 0;
    char buffer[MAX_LINE_LENGTH];

    // Read all lines from input
    while (fgets(buffer, sizeof(buffer), input)) {
        if (lines_count >= lines_capacity) {
            lines_capacity = lines_capacity ? lines_capacity * 2 : 1;
            lines = realloc(lines, lines_capacity * sizeof(char*));
            if (!lines) {
                perror("Unable to allocate memory");
                exit(EXIT_FAILURE);
            }
        }
        lines[lines_count] = strdup(buffer);
        if (!lines[lines_count]) {
            perror("Unable to allocate memory");
            exit(EXIT_FAILURE);
        }
        lines_count++;
    }

    // Print lines in reverse order
    for (int i = lines_count - 1; i >= 0; i--) {
        fputs(lines[i], output);
        free(lines[i]);
    }
    free(lines);
}

int main(int argc, char *argv[]) {
    FILE *input = stdin;
    FILE *output = stdout;

    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (!input) {
            perror("Error opening input file");
            return EXIT_FAILURE;
        }
    }

    if (argc > 2) {
        output = fopen(argv[2], "w");
        if (!output) {
            perror("Error opening output file");
            if (input != stdin) fclose(input);
            return EXIT_FAILURE;
        }
    }

    reverse_lines(input, output);

    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return EXIT_SUCCESS;
}
```

## 코드 설명

- 파일 열기

명령 줄 인수로 입력 파일과 출력 파일을 지정할 수 있습니다.
지정되지 않으면 표준 입력(stdin)과 표준 출력(stdout)을 사용합니다.

- 줄 읽기 및 저장

fgets로 입력 파일의 모든 줄을 읽어 동적 배열에 저장합니다.
배열 크기가 부족하면 realloc으로 확장합니다.

- 역순 출력
  
저장된 줄들을 배열의 마지막 줄부터 첫 번째 줄까지 순회하며 출력 파일에 씁니다.
for 루프를 사용하여 배열의 끝에서부터 시작하여 첫 번째 줄까지 fputs 함수로 출력합니다

