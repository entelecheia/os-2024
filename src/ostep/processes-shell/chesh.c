#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LEN 512
#define MAX_ARGS 10
#define DELIM " \t\r\n\a"
#define PATH_MAX 1024

void chesh_loop(void);
char *chesh_read_line(void);
char **chesh_split_line(char *line);
int chesh_execute(char **args);
int chesh_launch(char **args);
int chesh_exit(char **args);
int chesh_cd(char **args);
int chesh_path(char **args);

char *builtin_str[] = {
    "exit",
    "cd",
    "path"};

int (*builtin_func[])(char **) = {
    &chesh_exit,
    &chesh_cd,
    &chesh_path};

int chesh_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

int main(int argc, char **argv)
{
    // 셸 실행
    chesh_loop();
    return EXIT_SUCCESS;
}

void chesh_loop(void)
{
    char *line;
    char **args;
    int status;

    do
    {
        printf("chesh> ");
        line = chesh_read_line();
        args = chesh_split_line(line);
        status = chesh_execute(args);

        free(line);
        free(args);
    } while (status);
}

char *chesh_read_line(void)
{
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

char **chesh_split_line(char *line)
{
    int bufsize = MAX_ARGS, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "chesh: 메모리 할당 오류\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, DELIM);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                fprintf(stderr, "chesh: 메모리 할당 오류\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int chesh_execute(char **args)
{
    if (args[0] == NULL)
    {
        // 빈 명령어 처리
        return 1;
    }

    for (int i = 0; i < chesh_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    return chesh_launch(args);
}

int chesh_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // 자식 프로세스
        if (execvp(args[0], args) == -1)
        {
            perror("chesh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // fork 실패
        perror("chesh");
    }
    else
    {
        // 부모 프로세스
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int chesh_exit(char **args)
{
    return 0; // 셸 종료
}

int chesh_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "chesh: \"cd\" 명령어는 하나의 인자를 필요로 합니다.\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("chesh");
        }
    }
    return 1;
}

int chesh_path(char **args)
{
    // 여기서 PATH 환경 변수를 설정하는 구현을 추가할 수 있습니다.
    // 이 예제에서는 간단히 인자로 받은 경로를 출력하는 것으로 대체합니다.
    printf("새로운 경로 설정: ");
    for (int i = 1; args[i] != NULL; i++)
    {
        printf("%s ", args[i]);
        // 실제 구현에서는 여기서 환경 변수 PATH를 설정하게 됩니다.
        // 예: setenv("PATH", newPath, 1); // newPath는 사용자가 입력한 새 경로
    }
    printf("\n");
    return 1;
}
