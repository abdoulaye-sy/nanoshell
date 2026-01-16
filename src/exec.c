#include "nanoshell.h"
#include <sys/wait.h>

/*
** Cette fonction exécute une commande simple
** Exemple : "/bin/ls"
*/
void execute_command(char *cmd)
{
    pid_t pid;

    /* Création d'un nouveau processus */
    pid = fork();

    /* Erreur lors du fork */
    if (pid < 0)
    {
        perror("fork");
        return;
    }

    /* Code du processus enfant */
    if (pid == 0)
    {
        /* Tableau d'arguments pour execve */
        char *args[] = {cmd, NULL};

        /* Exécution de la commande */
        execve(cmd, args, NULL);

        /* Si execve échoue */
        perror("execve");
        exit(1);
    }
    else
    {
        /* Le parent attend la fin de l'enfant */
        waitpid(pid, NULL, 0);
    }
}
