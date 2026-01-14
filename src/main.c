/* 
** On inclut le header du projet
** pour avoir accès aux prototypes
*/
#include "nanoshell.h"

/* 
** =============================
**        FONCTION MAIN
** =============================
*/
int main(void)
{
    /* 
    ** Pointeur qui contiendra la ligne
    ** tapée par l'utilisateur
    */
    char *line;

    /* 
    ** Boucle infinie du shell
    ** Le shell tourne tant qu'on ne quitte pas
    */
    while (1)
    {
        /* 
        ** Affiche le prompt "nanoshell$ "
        */
        print_prompt();

        /* 
        ** Lit l'entrée utilisateur
        ** Retourne NULL si Ctrl-D
        */
        line = read_input();

        /* 
        ** Si line est NULL, l'utilisateur a appuyé sur Ctrl-D
        */
        if (line == NULL)
        {
            /* 
            ** On affiche un retour à la ligne
            ** pour que la sortie soit propre
            */
            write(1, "\n", 1);

            /* 
            ** On sort de la boucle infinie
            */
            break;
        }

        /* 
        ** Libère la mémoire allouée
        ** pour éviter les fuites mémoire
        */
        free(line);
    }

    /* 
    ** Fin normale du programme
    */
    return (0);
}
