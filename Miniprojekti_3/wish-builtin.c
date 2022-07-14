
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "wish-common.h"
#include "wish-builtin.h"


// tarkistaa löytyykö listasta **cmds wish-shellin sisäänrakennettuja komentoja. Jos löytyy, kutsuu vastaavaa funktiota.
int execute_builtin(char **paths, struct command **cmds, FILE **in)
{
    for (int i = 0; cmds[i] != NULL; i++) {
        
        struct command *cmd = cmds[i];

        if (cmd->cmd != NULL) {
            if (strcmp(cmd->cmd, "exit") == 0) {

                if (cmd->arguments[1] != NULL) {
                    fprintf(stderr, "error: exit does not take arguments\n");
                    return 1;
                }

                close_input_stream(in); 
                free_cmds(cmds);
                free(*paths);
                exit(0);
            }

            if (strcmp(cmd->cmd, "cd") == 0) {
                cd(cmd);
                return 1;
            }

            if (strcmp(cmd->cmd, "path") == 0) {
                path(paths, cmd);
                return 1;
            }
        }


    }

    return 0;
}

// alustaa path-muuttujan char *paths sisältämään polun /bin
void init_path(char **paths)
{
    char *args[] = {"path", "/bin", NULL};
    struct command init = { "path", args };
    path(paths, &init);    
}


// tarkistaa löytyykö komentoa cmd paths-muuttujan määräämistä poluista. Jos löytyy, palauttaa polun tiedostoon, josta cmd löytyy. Muutoin palauttaa NULL
char *check_path(char *paths, char *cmd)
{
    
    if (cmd == NULL)
        return NULL;

    if (paths == NULL) {
        return NULL;
    } 

    char *cmd_path = NULL;
    char *strtok_ptr = NULL;

    strtok_ptr = strtok(paths, ":");
    if (strtok_ptr == NULL) {
        common_err_msg();
        return NULL;
    }

    do {

        cmd_path = strdup(strtok_ptr);
        if (cmd_path == NULL) {
            common_err_msg();
            return NULL;
        }

        cmd_path = realloc( cmd_path, (strlen(cmd_path) + strlen(cmd) + 2) * sizeof(char) );
        if (cmd_path == NULL) {
            mem_alloc_err_msg("check_path");
            return NULL;
        }
        cmd_path = strcat(cmd_path, "/");
        cmd_path = strcat(cmd_path, cmd);

        if (access(cmd_path, X_OK) == 0) 
            break;


        free(cmd_path);
        cmd_path = NULL;

    } while ( (strtok_ptr = strtok(NULL, ":")) != NULL );
    

    return cmd_path;
}


// vaihtaa hakemiston rakenteen cmd arguments-jäsenmuuttujan toisen argumentin cmd->arguments[1] määräämään hakemistoon
void cd(struct command *cmd)
{
    int i;
    for (i = 1; cmd->arguments[i] != NULL; i++);
    if ( i != 2 ) {
        fprintf(stderr, "error: cd takes one argument\n");
        return;
    }

    if ( chdir(cmd->arguments[1]) == -1 ) {
        if (errno == EACCES) 
            fprintf(stderr, "error: permission denied\n");
        
        if (errno == ENONET) 
            fprintf(stderr, "error: given pathname does not exist\n");
        
        if (errno == ENOTDIR) 
            fprintf(stderr, "error: given path contains a non-directory file\n");

        if (errno == ENAMETOOLONG)
            fprintf(stderr, "error: path too long\n");

    }
}


/* Konkatenoi osoitteesta *paths löytyvään merkkijoon rakenteen cmd arguments-jäsenmuuttujassa tarkennetut merkkijonot alkaen indeksistä 1 ja lisäten jokaisen väliin kaksoispisteen erottimena.
Kaikki mikä osoitteessa *paths on tätä ennen, ylikirjoitetaan.*/
void path(char **paths, struct command *cmd)
{
    if (*paths != NULL) {
        free(*paths);
    }
    *paths = NULL;

    if (cmd != NULL) {
        if (cmd->arguments != NULL) {
            for (int i = 1; cmd->arguments[i] != NULL; i++) { // käydään läpi argumenttina annetut polut

                if (*paths == NULL) {
                    
                    *paths = strdup(cmd->arguments[i]);
                    *paths = realloc( *paths, ( strlen(*paths)  + 2 ) * sizeof(char) );
                    if (*paths == NULL) {
                        mem_alloc_err_msg("path");
                        return;
                    }
                    *paths = strcat(*paths, ":");   // erotellaan eri polut toisistaan kaksoispisteellä

                } else {
                    *paths = realloc( *paths, ( strlen(*paths) + strlen(cmd->arguments[i]) + 2 ) * sizeof(char) );
                    if (*paths == NULL) {
                        mem_alloc_err_msg("path");
                        return;
                    }
                    *paths = strcat(*paths, cmd->arguments[i]);
                    *paths = strcat(*paths, ":");
                }
            }
        }
    }
}
