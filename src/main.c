/* 
** On inclut le header du projet
** pour connaître les fonctions
*/
#include "nanoshell.h"

/* 
** Fonction principale du programme
*/
int main(void)
{
    /* 
    ** Cette variable va contenir
    ** ce que l'utilisateur tape
    */
    char *line;

    /* 
    ** Boucle infinie :
    ** le shell ne s'arrête jamais
    ** sauf si on le quitte
    */
    while (1)
    {
        /* 
        ** On affiche le prompt
        */
        print_prompt();

        /* 
        ** On lit l'entrée utilisateur
        */
        line = read_input();

        /* 
        ** Si line est NULL,
        ** l'utilisateur a fait Ctrl-D
        */
        if (line == NULL)
        {
            /* 
            ** On saute une ligne
            ** avant de quitter
            */
            write(1, "\n", 1);

            /* 
            ** On sort de la boucle
            */
            break;
        }

        /* 
        ** On libère la mémoire
        ** utilisée pour la ligne
        */
       /* Si l'utilisateur tape quelque chose */
if (line[0] != '\0')
{
    execute_command(line);
}

/* Libération mémoire */
free(line);

    }

    /* 
    ** Fin normale du programme
    */
    return (0);
}

