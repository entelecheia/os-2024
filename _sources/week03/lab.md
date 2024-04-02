# Lab: 프로세스 생성과 실행 기초

## 실습 목표

이 실습은 다음과 같은 목표를 달성하기 위해 설계되었습니다.

1. fork() 함수를 사용하여 새로운 프로세스를 생성하는 방법을 이해합니다.
2. 부모 프로세스와 자식 프로세스의 실행 흐름을 이해합니다.
3. wait() 함수를 사용하여 부모 프로세스가 자식 프로세스의 종료를 기다리는 방법을 익힙니다.
4. execvp() 함수를 사용하여 새로운 프로그램을 실행하는 방법을 배웁니다.
5. 표준 출력을 파일로 리다이렉션하는 방법을 알아봅니다.

## 실습 내용

이 실습에서는 4개의 C 프로그램 파일(p1.c, p2.c, p3.c, p4.c)과 하나의 Makefile을 사용하여 프로세스 생성과 실행에 대해 학습합니다.

1. p1.c - 프로세스 생성 기초

   - fork() 함수를 사용하여 새로운 자식 프로세스를 생성합니다.
   - 부모 프로세스와 자식 프로세스의 실행 흐름을 확인합니다.
   - 프로세스 ID(PID)를 출력하여 부모 프로세스와 자식 프로세스를 구분합니다.

2. p2.c - 부모 프로세스의 대기

   - fork() 함수를 사용하여 새로운 자식 프로세스를 생성합니다.
   - 자식 프로세스에서 sleep() 함수를 사용하여 일정 시간 동안 대기합니다.
   - 부모 프로세스에서 wait() 함수를 사용하여 자식 프로세스가 종료될 때까지 기다립니다.

3. p3.c - 새로운 프로그램 실행

   - fork() 함수를 사용하여 새로운 자식 프로세스를 생성합니다.
   - 자식 프로세스에서 execvp() 함수를 사용하여 "wc" 프로그램을 실행합니다.
   - execvp() 함수에 전달할 인자들을 설정하여 "p3.c" 파일의 단어 개수를 셉니다.

4. p4.c - 표준 출력 리다이렉션
   - fork() 함수를 사용하여 새로운 자식 프로세스를 생성합니다.
   - 자식 프로세스에서 표준 출력을 닫고 "./p4.output" 파일로 리다이렉션합니다.
   - execvp() 함수를 사용하여 "wc" 프로그램을 실행하고 "p4.c" 파일의 단어 개수를 셉니다.
   - 부모 프로세스에서 자식 프로세스가 종료될 때까지 기다립니다.

Makefile:

- "make all" 명령어를 사용하여 모든 프로그램(p1, p2, p3, p4)을 한 번에 컴파일할 수 있습니다.
- 각 프로그램에 대한 컴파일 규칙이 정의되어 있습니다.
- "make clean" 명령어를 사용하여 생성된 실행 파일을 삭제할 수 있습니다.

실습을 통해 프로세스 생성, 부모-자식 프로세스 간의 실행 흐름, 새로운 프로그램 실행, 표준 출력 리다이렉션 등의 개념을 익힐 수 있습니다. 각 프로그램 파일에는 자세한 주석이 포함되어 있어 코드의 동작을 이해하는 데 도움이 될 것입니다.

## 실습

모든 코드는 [수업 리포](https://github.com/chu-aie/os-2024/blob/main/src/ostep/cpu-api)에서 확인할 수 있습니다.

### p1.c - 프로세스 생성 기초

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    // 현재 프로세스의 PID를 출력합니다.
    printf("hello world (pid:%d)\n", (int) getpid());

    // fork() 함수를 호출하여 새로운 자식 프로세스를 생성합니다.
    int rc = fork();

    if (rc < 0) {
        // fork() 함수 호출이 실패한 경우 에러 메시지를 출력하고 종료합니다.
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // 자식 프로세스인 경우 이 블록이 실행됩니다.
        printf("hello, I am child (pid:%d)\n", (int) getpid());
    } else {
        // 부모 프로세스인 경우 이 블록이 실행됩니다.
        printf("hello, I am parent of %d (pid:%d)\n",
               rc, (int) getpid());
    }

    return 0;
}
```

코드에 대한 설명:

- `#include` 지시문을 사용하여 필요한 헤더 파일을 포함시킵니다.
- `main()` 함수는 프로그램의 진입점입니다.
- `printf()` 함수를 사용하여 현재 프로세스의 PID를 출력합니다.
- `fork()` 함수를 호출하여 새로운 자식 프로세스를 생성합니다.
- `fork()` 함수의 반환값을 `rc` 변수에 저장합니다.
  - 반환값이 음수인 경우 `fork()` 함수 호출이 실패한 것이므로 에러 메시지를 출력하고 프로그램을 종료합니다.
  - 반환값이 0인 경우 현재 실행 중인 프로세스가 자식 프로세스임을 의미합니다. 자식 프로세스에서 실행되는 코드 블록이 실행됩니다.
  - 반환값이 양수인 경우 현재 실행 중인 프로세스가 부모 프로세스임을 의미합니다. 부모 프로세스에서 실행되는 코드 블록이 실행됩니다.
- 프로그램이 정상적으로 종료되면 0을 반환합니다.

이 코드는 `fork()` 함수를 사용하여 새로운 자식 프로세스를 생성하고, 부모 프로세스와 자식 프로세스에서 각각 다른 메시지를 출력하는 예제입니다.

### p2.c - 부모 프로세스의 대기

```c
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
```

코드에 대한 설명:

- 필요한 헤더 파일들을 포함시킵니다.
- `main()` 함수는 프로그램의 진입점입니다.
- `printf()` 함수를 사용하여 현재 프로세스의 PID를 출력합니다.
- `fork()` 함수를 호출하여 새로운 자식 프로세스를 생성합니다.
- `fork()` 함수의 반환값을 `rc` 변수에 저장합니다.
  - 반환값이 음수인 경우 `fork()` 함수 호출이 실패한 것이므로 에러 메시지를 출력하고 프로그램을 종료합니다.
  - 반환값이 0인 경우 현재 실행 중인 프로세스가 자식 프로세스임을 의미합니다. 자식 프로세스에서 실행되는 코드 블록이 실행됩니다.
    - 자식 프로세스는 자신의 PID를 출력하고, `sleep()` 함수를 사용하여 1초 동안 대기합니다.
  - 반환값이 양수인 경우 현재 실행 중인 프로세스가 부모 프로세스임을 의미합니다. 부모 프로세스에서 실행되는 코드 블록이 실행됩니다.
    - 부모 프로세스는 `wait()` 함수를 사용하여 자식 프로세스가 종료될 때까지 기다립니다.
    - `wait()` 함수의 반환값은 종료된 자식 프로세스의 PID입니다.
    - 부모 프로세스는 자신의 PID, 자식 프로세스의 PID, 그리고 `wait()` 함수의 반환값을 출력합니다.
- 프로그램이 정상적으로 종료되면 0을 반환합니다.

이 코드는 `fork()` 함수를 사용하여 자식 프로세스를 생성하고, 자식 프로세스에서는 1초 동안 대기한 후 종료되며, 부모 프로세스에서는 자식 프로세스가 종료될 때까지 기다리는 예제입니다.

### p3.c - 새로운 프로그램 실행

```c
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
        myargs[0] = strdup("wc");    // 실행할 프로그램: "wc" (단어 개수 세기)
        myargs[1] = strdup("p3.c");  // 인자: 개수를 셀 파일 이름
        myargs[2] = NULL;            // 배열의 끝을 표시

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
```

코드에 대한 설명:

- 필요한 헤더 파일들을 포함시킵니다.
- `main()` 함수는 프로그램의 진입점입니다.
- `printf()` 함수를 사용하여 현재 프로세스의 PID를 출력합니다.
- `fork()` 함수를 호출하여 새로운 자식 프로세스를 생성합니다.
- `fork()` 함수의 반환값을 `rc` 변수에 저장합니다.
  - 반환값이 음수인 경우 `fork()` 함수 호출이 실패한 것이므로 에러 메시지를 출력하고 프로그램을 종료합니다.
  - 반환값이 0인 경우 현재 실행 중인 프로세스가 자식 프로세스임을 의미합니다. 자식 프로세스에서 실행되는 코드 블록이 실행됩니다.
    - 자식 프로세스는 자신의 PID를 출력합니다.
    - `execvp()` 함수에 전달할 인자들을 저장할 배열 `myargs`를 생성합니다.
    - `strdup()` 함수를 사용하여 문자열을 복제하고 `myargs` 배열에 저장합니다.
    - `execvp()` 함수를 호출하여 "wc" 프로그램을 실행합니다. 이때 `myargs` 배열을 인자로 전달합니다.
    - `execvp()` 함수 호출 이후의 코드는 실행되지 않습니다.
  - 반환값이 양수인 경우 현재 실행 중인 프로세스가 부모 프로세스임을 의미합니다. 부모 프로세스에서 실행되는 코드 블록이 실행됩니다.
    - 부모 프로세스는 `wait()` 함수를 사용하여 자식 프로세스가 종료될 때까지 기다립니다.
    - `wait()` 함수의 반환값은 종료된 자식 프로세스의 PID입니다.
    - 부모 프로세스는 자신의 PID, 자식 프로세스의 PID, 그리고 `wait()` 함수의 반환값을 출력합니다.
- 프로그램이 정상적으로 종료되면 0을 반환합니다.

이 코드는 `fork()` 함수를 사용하여 자식 프로세스를 생성하고, 자식 프로세스에서는 `execvp()` 함수를 사용하여 "wc" 프로그램을 실행하여 "p3.c" 파일의 단어 개수를 세는 예제입니다. 부모 프로세스는 자식 프로세스가 종료될 때까지 기다립니다.

### p4.c - 표준 출력 리다이렉션

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
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

        // 표준 출력(stdout)을 닫습니다.
        close(STDOUT_FILENO);

        // "./p4.output" 파일을 생성하고 열어서 표준 출력으로 리다이렉션합니다.
        open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

        // "wc" 프로그램을 실행하기 위한 인자들을 설정합니다.
        char *myargs[3];
        myargs[0] = strdup("wc");    // 실행할 프로그램: "wc" (단어 개수 세기)
        myargs[1] = strdup("p4.c");  // 인자: 개수를 셀 파일 이름
        myargs[2] = NULL;            // 배열의 끝을 표시

        // execvp() 함수를 호출하여 "wc" 프로그램을 실행합니다.
        execvp(myargs[0], myargs);
    }
    else
    {
        // 부모 프로세스인 경우 이 블록이 실행됩니다.

        // 자식 프로세스가 종료될 때까지 기다립니다.
        int wc = wait(NULL);

        // 자식 프로세스가 정상적으로 종료되었는지 확인합니다.
        assert(wc >= 0);
    }

    return 0;
}
```

코드에 대한 설명:

- 필요한 헤더 파일들을 포함시킵니다.
- `main()` 함수는 프로그램의 진입점입니다.
- `fork()` 함수를 호출하여 새로운 자식 프로세스를 생성합니다.
- `fork()` 함수의 반환값을 `rc` 변수에 저장합니다.
  - 반환값이 음수인 경우 `fork()` 함수 호출이 실패한 것이므로 에러 메시지를 출력하고 프로그램을 종료합니다.
  - 반환값이 0인 경우 현재 실행 중인 프로세스가 자식 프로세스임을 의미합니다. 자식 프로세스에서 실행되는 코드 블록이 실행됩니다.
    - `close()` 함수를 사용하여 표준 출력(stdout)을 닫습니다.
    - `open()` 함수를 사용하여 "./p4.output" 파일을 생성하고 열어서 표준 출력으로 리다이렉션합니다.
    - `execvp()` 함수에 전달할 인자들을 저장할 배열 `myargs`를 생성합니다.
    - `strdup()` 함수를 사용하여 문자열을 복제하고 `myargs` 배열에 저장합니다.
    - `execvp()` 함수를 호출하여 "wc" 프로그램을 실행합니다. 이때 `myargs` 배열을 인자로 전달합니다.
  - 반환값이 양수인 경우 현재 실행 중인 프로세스가 부모 프로세스임을 의미합니다. 부모 프로세스에서 실행되는 코드 블록이 실행됩니다.
    - 부모 프로세스는 `wait()` 함수를 사용하여 자식 프로세스가 종료될 때까지 기다립니다.
    - `wait()` 함수의 반환값은 종료된 자식 프로세스의 상태 정보입니다.
    - `assert()` 함수를 사용하여 자식 프로세스가 정상적으로 종료되었는지 확인합니다.
- 프로그램이 정상적으로 종료되면 0을 반환합니다.

이 코드는 `fork()` 함수를 사용하여 자식 프로세스를 생성하고, 자식 프로세스에서는 표준 출력을 "./p4.output" 파일로 리다이렉션한 후 `execvp()` 함수를 사용하여 "wc" 프로그램을 실행하여 "p4.c" 파일의 단어 개수를 세는 예제입니다. 부모 프로세스는 자식 프로세스가 종료될 때까지 기다리고, 자식 프로세스의 종료 상태를 확인합니다.