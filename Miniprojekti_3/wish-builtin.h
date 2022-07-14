#ifndef WISHBUILTIN_H
#define WISHBUILTIN_H

#include "wish-common.h"

// tarkistaa löytyykö listasta **cmds wish-shellin sisäänrakennettuja komentoja. Jos löytyy, kutsuu vastaavaa funktiota.
int execute_builtin(char **paths, struct command **cmds, FILE **in);

// alustaa path-muuttujan char *paths sisältämään polun /bin
void init_path(char **paths);

// tarkistaa löytyykö komentoa cmd paths-muuttujan määräämistä poluista. Jos löytyy, palauttaa polun tiedostoon, josta cmd löytyy. Muutoin palauttaa NULL
char *check_path(char *paths, char *cmd);

// vaihtaa hakemiston rakenteen cmd arguments-jäsenmuuttujan toisen argumentin cmd->arguments[1] määräämään hakemistoon
void cd(struct command *cmd);

/* Konkatenoi osoitteesta *paths löytyvään merkkijoon rakenteen cmd arguments-jäsenmuuttujassa tarkennetut merkkijonot alkaen indeksistä 1 ja lisäten jokaisen väliin kaksoispisteen erottimena.
Kaikki mikä osoitteessa *paths on tätä ennen, ylikirjoitetaan.*/
void path(char **paths, struct command *cmd);

#endif