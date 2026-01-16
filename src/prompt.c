/* 
** On inclut le header du projet
*/
#include "nanoshell.h"

/* 
** Cette fonction affiche le prompt
*/
void print_prompt(void)
{
    /* 
    ** write écrit sur l'écran
    ** 1 = sortie standard (écran)
    */
    write(1, "nanoshell$ ", 11);
}
