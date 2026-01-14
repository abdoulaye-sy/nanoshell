/* 
** Protection contre les inclusions multiples
** Si NANOSHELL_H n'existe pas encore, on le crée
*/
#ifndef NANOSHELL_H
#define NANOSHELL_H

/* 
** =============================
**        INCLUDES SYSTEME
** =============================
*/

/* 
** unistd.h donne accès aux fonctions système
** comme read() et write()
*/
#include <unistd.h>

/* 
** stdlib.h donne accès à malloc(), free(), exit()
*/
#include <stdlib.h>

/* 
** stdio.h donne accès à printf()
*/
#include <stdio.h>

/* 
** =============================
**     PROTOTYPES DES FONCTIONS
** =============================
*/

/* 
** Fonction qui affiche le prompt du shell
** Elle ne retourne rien
*/
void print_prompt(void);

/* 
** Fonction qui lit ce que tape l'utilisateur
** Elle retourne une chaîne de caractères
** ou NULL si Ctrl-D est pressé
*/
char *read_input(void);

/* 
** Fin de la protection contre les inclusions multiples
*/
#endif
