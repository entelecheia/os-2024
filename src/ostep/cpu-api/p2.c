#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

        // 1초 동안 대기합니다.
        sleep(1);
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
