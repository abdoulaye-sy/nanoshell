/* 
** Ce fichier sert à partager les informations
** entre tous les fichiers .c du projet
*/

/* 
** Protection pour éviter que ce fichier
** soit inclus plusieurs fois
*/
#ifndef NANOSHELL_H
#define NANOSHELL_H

/* 
** On inclut unistd.h pour utiliser
** read() et write()
*/
#include <unistd.h>

/* 
** On inclut stdlib.h pour utiliser
** malloc() et free()
*/
#include <stdlib.h>

/* 
** On inclut stdio.h pour utiliser
** printf() plus tard
*/
#include <stdio.h>

/* 
** Cette fonction affiche le prompt
** du shell (ex: nanoshell$)
*/
void print_prompt(void);

/* 
** Cette fonction lit ce que
** l'utilisateur tape au clavier
** Elle retourne une chaîne de caractères
*/
char *read_input(void);

/* 
** Fin de la protection
*/

/* Exécute une commande simple */
void execute_command(char *cmd);

#endif

