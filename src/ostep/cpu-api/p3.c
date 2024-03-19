#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    // 현재 프로세스의 PID를 출력합니다.
    printf("hello world (pid:%d)\n", (int)getpid());

    // fork() 함수를 호출하여 새로운 자식 프로세스를 생성합니다.
    int rc = fork();

    if (rc < 0)
    {
        // fork() 함수 호출이 실패한 경우 에러 메시지를 출력하고 종료합니다.
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        // 자식 프로세스인 경우 이 블록이 실행됩니다.
        printf("hello, I am child (pid:%d)\n", (int)getpid());

        // execvp() 함수에 전달할 인자들을 저장할 배열을 생성합니다.
        char *myargs[3];
        myargs[0] = strdup("wc");   // 실행할 프로그램: "wc" (단어 개수 세기)
        myargs[1] = strdup("p3.c"); // 인자: 개수를 셀 파일 이름
        myargs[2] = NULL;           // 배열의 끝을 표시

        // execvp() 함수를 호출하여 "wc" 프로그램을 실행합니다.
        execvp(myargs[0], myargs);

        // execvp() 함수 호출 이후의 코드는 실행되지 않습니다.
        printf("this shouldn't print out");
    }
    else
    {
        // 부모 프로세스인 경우 이 블록이 실행됩니다.

        // 자식 프로세스가 종료될 때까지 기다립니다.
        int wc = wait(NULL);

        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
               rc, wc, (int)getpid());
    }

    return 0;
}
