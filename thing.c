#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


extern char **environ;

char *find_path(char *cmd)
{
    static char full_path[1024];
    char *path_env;
    char *path_copy;
    char *dir;

    if (strchr(cmd, '/') != NULL) {
        if (access(cmd, X_OK) == 0)
            return cmd;
        return NULL;
    }
    path_env = getenv("PATH");
    if (!path_env)
        return NULL;
    path_copy = strdup(path_env);
    dir = strtok(path_copy, ":");
    while (dir) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path;
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}

char **split_args(char *line)
{
    static char *argv[64];
    int i = 0;
    char *token;

    token = strtok(line, " \t\n");
    while (token && i < 63) {
        argv[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    argv[i] = NULL;
    return argv;
}

int do_builtin(char **argv)
{
    char cwd[1024];
    char *path;
    int i;

    if (strcmp(argv[0], "exit") == 0)
        exit(0);
    if (strcmp(argv[0], "pwd") == 0) {
        if (getcwd(cwd, sizeof(cwd)))
            printf("%s\n", cwd);
        return 1;
    }
    if (strcmp(argv[0], "env") == 0) {
        i = 0;
        while (environ[i])
            printf("%s\n", environ[i++]);
        return 1;
    }
    if (strcmp(argv[0], "cd") == 0) {
        if (argv[1] == NULL || strcmp(argv[1], "~") == 0)
            path = getenv("HOME");
        else
            path = argv[1];
        if (path)
            chdir(path);
        return 1;
    }
    return 0;
}

int main(void) {
    char line[1024 * 4] = {0};
    char **argv;
    char *exec_path;
    int pid = -1;

    while (1) {
        printf("?> ");                      //Début du prompt
        fgets(line, 1024 * 4, stdin);       //Equ. input

        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '\0')
            continue;

        argv = split_args(line);
        if (!argv[0])
            continue;

        if (do_builtin(argv))
            continue;

        exec_path = find_path(argv[0]);
        if (!exec_path) {
            printf("nanoshell: weird, %s is not here... :/\n", argv[0]);
            continue;
        }

        pid = fork();
        if (pid == 0) {
            execve(exec_path, argv, environ);
            exit(-1);
        }
        waitpid(pid, NULL, 0);
    }

    return 0;
}



