#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

/* Cherche la commande dans le PATH */
char *find_path(char *cmd)
{
    static char full_path[1024];
    char *path;
    char *tmp;
    char *dir;

    if (strchr(cmd, '/') != NULL)
    {
        if (access(cmd, X_OK) == 0)
            return cmd;
        return NULL;
    }

    path = getenv("PATH");
    if (!path)
        return NULL;

    tmp = malloc(strlen(path) + 1);
    if (!tmp)
        return NULL;
    strcpy(tmp, path);

    dir = strtok(tmp, ":");
    while (dir)
    {
        strcpy(full_path, dir);
        strcat(full_path, "/");
        strcat(full_path, cmd);
        if (access(full_path, X_OK) == 0)
        {
            free(tmp);
            return full_path;
        }
        dir = strtok(NULL, ":");
    }
    free(tmp);
    return NULL;
}

/* Découpe la ligne en mots */
char **split_line(char *line)
{
    char **args;
    char *word;
    int i;

    args = malloc(sizeof(char *) * 64);
    if (!args)
        return NULL;

    i = 0;
    word = strtok(line, " \t\n");
    while (word && i < 63)
    {
        args[i++] = word;
        word = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
    return args;
}

/* Builtins : cd, pwd, env, exit */
int do_builtin(char **args)
{
    char cwd[1024];
    char *path;
    int i;

    if (strcmp(args[0], "exit") == 0)
        exit(0);

    if (strcmp(args[0], "pwd") == 0)
    {
        if (getcwd(cwd, sizeof(cwd)))
            printf("%s\n", cwd);
        else
            perror("pwd");
        return 1;
    }

    if (strcmp(args[0], "env") == 0)
    {
        i = 0;
        while (environ[i])
            printf("%s\n", environ[i++]);
        return 1;
    }

    if (strcmp(args[0], "cd") == 0)
    {
        if (!args[1] || strcmp(args[1], "~") == 0)
            path = getenv("HOME");
        else
            path = args[1];

        if (path && chdir(path) != 0)
            perror("cd");
        return 1;
    }
    return 0;
}

int main(void)
{
    char line[4096];
    char **args;
    char *exec_path;
    int pid;

    while (1)
    {
        printf("?> ");
        if (!fgets(line, sizeof(line), stdin))
            break;

        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '\0')
            continue;

        args = split_line(line);
        if (!args || !args[0])
        {
            free(args);
            continue;
        }

        if (do_builtin(args))
        {
            free(args);
            continue;
        }

        exec_path = find_path(args[0]);
        if (!exec_path)
        {
            perror(args[0]);
            free(args);
            continue;
        }

        pid = fork();
        if (pid == 0)
        {
            execve(exec_path, args, environ);
            perror("execve");
            exit(1);
        }
        waitpid(pid, NULL, 0);
        free(args);
    }
    return 0;
}
