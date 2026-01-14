#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h> // Pour strlen, strcpy, strtok, strcmp, etc.

// Fonction pour découper la ligne en arguments
// Elle remplace les espaces par des '\0' et remplie un tableau de pointeurs
void parse_input(char *line, char **argv, int max_args) {
    int i = 0;
    char *token = strtok(line, " \t\n"); //strtok : découpe la chaîne aux espaces (" "), tabulations ("\t") et retours à la ligne ("\n")
    while (token != NULL && i < max_args - 1) { //Boucle qui récupère tous les tokens un à un
        argv[i] = token;                        //strtok(NULL, ...) : continue la découpe à partir du dernier séparateur trouvé
        i++;                                    //Stocke chaque token dans argv[i]
        token = strtok(NULL, " \t\n");
    }
    argv[i] = NULL;  // Termine le tableau avec NULL
}

char *find_in_path(char *command) { // Fonction pour chercher un exécutable dans le PATH
    char *path_env = getenv("PATH"); // Retourne le chemin complet si trouvé, sinon NULL
    if (path_env == NULL)
        return NULL;
    
    // Copier PATH car strtok le modifie
    char *path_copy = malloc(strlen(path_env) + 1); // malloc alloue de la mémoire pour une copie de PATH
    strcpy(path_copy, path_env); // strcpy : copie PATH dans cette nouvelle zone (car strtok détruit la chaîne qu'il traite)
    
    char *dir = strtok(path_copy, ":"); // Découpe PATH par les : (séparateur entre répertoires)
    while (dir != NULL) {
        // Construire le chemin complet: /usr/bin/ls par exemple
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command); //snprintf : construit le chemin complet
                                                                        //Exemple : "/usr/local/bin" + "/" + "ls" → "/usr/local/bin/ls"
        if (access(full_path, X_OK) == 0) { //access X_OK vérifie si le fichier existe et est exécutable (0 si OK, -1 sinon)
            char *result = malloc(strlen(full_path) + 1);
            strcpy(result, full_path); // Alloue et retourne le chemin complet trouvé
            free(path_copy); // free : libère la copie de PATH qui n'est plus utile
            return result;
        }
        dir = strtok(NULL, ":");
    }
    
    free(path_copy);
    return NULL;
}

// Fonction pour gérer les commandes internes (builtins)
// Retourne 1 si c'est un builtin, 0 sinon
int handle_builtin(char **argv) {
    if (argv[0] == NULL) // Si l'utilisateur appuie sur Entrée sans rien taper, argv[0] est NULL
        return 1;  // Ligne vide, ne rien faire
    
    // Builtin: exit
    if (strcmp(argv[0], "exit") == 0) { // strcmp : compare deux chaînes (retourne 0 si égales)
        exit(0); // exit(0) : quitter le shell avec code 0 (succès)
    }
    
    // Builtin: pwd
    if (strcmp(argv[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) // getcwd : "get current working directory" = récupère le répertoire courant
            printf("%s\n", cwd); // L'affiche et retourne 1 (traité comme builtin)
        return 1;
    }
    
    // Builtin: cd
    if (strcmp(argv[0], "cd") == 0) {
        char *target = NULL;
        
        if (argv[1] == NULL) // Teste les 4 cas : cd, cd ~, cd .., cd somedir
            target = getenv("HOME");  // cd tout court = aller au HOME
        else if (strcmp(argv[1], "~") == 0)
            target = getenv("HOME");  // cd ~ = aller au HOME
        else if (strcmp(argv[1], "..") == 0)
            target = ".."; // cd .. = répertoire parent
        else
            target = argv[1]; // cd somedir = somedir
        
        if (target != NULL) {
            if (chdir(target) != 0) //chdir : "change directory" = change le répertoire courant
                printf("nanoshell: cannot change directory to %s\n", target);
        }
        return 1; // Retourne 0 si OK, -1 sinon
    }
    
    // Builtin: env
    if (strcmp(argv[0], "env") == 0) {
        extern char **environ; // environ : tableau global contenant toutes les variables d'environnement
        for (int i = 0; environ[i] != NULL; i++) {
            printf("%s\n", environ[i]); // Affiche chaque ligne jusqu'à trouver NULL
        }
        return 1;
    }
    
    return 0;  // Pas un builtin
}

// Fonction main - la boucle principale
int main(void) {
    char line[1024 * 4] = {0};
    char *argv[512] = {NULL}; // Tableau pour stocker max 512 arguments
    char *env[] = {NULL};
    int pid = -1;
    
    while (1) {
        printf("?> ");
        fflush(stdout);  // fflush force l'affichage immédiat du prompt (important avant fgets)
        
        if (fgets(line, sizeof(line), stdin) == NULL)
            break;  // Si l'utilisateur fait Ctrl+D (EOF), fgets retourne NULL → quitter
        
        parse_input(line, argv, 512); // Découpe la ligne en arguments

        if (argv[0] == NULL) // Si ligne vide, passer à la prochaine itération
            continue;
        
        if (handle_builtin(argv)) // Si c'est un builtin, il est exécuté ici et on continue
            continue;
                                    // Sinon on va chercher la commande externe
        char *command_path = NULL;
        
        if (strchr(argv[0], '/') != NULL) { // strchr cherche s'il y a un / dans le nom de la commande
            command_path = argv[0]; // Si oui, chemin (absolu ou relatif) → l'utiliser tel quel
        } else {
            command_path = find_in_path(argv[0]); // Si non, chercher dans le PATH
        }
        
        if (command_path != NULL) { // Si commande trouvée, l'exécuter
            pid = fork(); // fork() : créer un processus enfant
            if (pid == 0) {
                execve(command_path, argv, env); // pid == 0 : on est dans l'enfant → exécuter la commande
                perror("execve"); // perror : affiche le message d'erreur si execve échoue
                exit(-1);
            } else if (pid > 0) {
                waitpid(pid, NULL, 0); // pid > 0 : on est dans le parent → attendre que l'enfant finisse
            }
            
            if (strchr(argv[0], '/') == NULL)
                free(command_path); // Si la commande venait du PATH (malloc), libérer la mémoire
        } else {
            printf("nanoshell: weird, %s is not here... :/\n", argv[0]); // Message d'erreur si commande non trouvée
        }
    }
    
    return 0;
}
