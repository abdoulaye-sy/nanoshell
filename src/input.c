/* 
** On inclut le header du projet
*/
#include "nanoshell.h"

/* 
** Cette fonction lit l'entrée utilisateur
*/
char *read_input(void)
{
    /* 
    ** Buffer temporaire pour stocker
    ** ce que l'utilisateur tape
    */
    char buffer[1024];

    /* 
    ** Nombre de caractères lus
    */
    int bytes;

    /* 
    ** Chaîne finale à retourner
    */
    char *line;

    /* 
    ** Compteur pour la copie
    */
    int i;

    /* 
    ** read lit depuis le clavier
    ** 0 = entrée standard
    */
    bytes = read(0, buffer, 1023);

    /* 
    ** Si bytes est 0 ou moins,
    ** l'utilisateur a fait Ctrl-D
    */
    if (bytes <= 0)
        return (NULL);

    /* 
    ** On enlève le '\n'
    ** et on termine la chaîne
    */
    buffer[bytes - 1] = '\0';

    /* 
    ** On réserve de la mémoire
    */
    line = malloc(bytes);

    /* 
    ** Vérification de l'allocation
    */
    if (!line)
        return (NULL);

    /* 
    ** Copie caractère par caractère
    */
    i = 0;
    while (buffer[i])
    {
        line[i] = buffer[i];
        i++;
    }

    /* 
    ** Fin de la chaîne
    */
    line[i] = '\0';

    /* 
    ** On retourne la ligne lue
    */
    return (line);
}
