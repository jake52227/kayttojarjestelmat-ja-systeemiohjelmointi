#ifndef WISH_H
#define WISH_H

#include "wish-common.h"


// varaa muistia uudelle struct command rakenteelle. Alustaa kaikki jäsenmuuttujat paitsi wait arvoon NULL. Wait aseteaan arvoon 1. Palauttaa rakenteen tai NULL virheen sattuessa.
struct command *allocate_cmd();

// varaa muistia rakenteelle cmd ja alustaa sen merkkijonon line määrittämillä arvoilla. Palauttaa rakenteen tai NULL virheen sattuessa.
struct command *init_cmd(char *line, struct command *cmd);

// Parsii tiedostonimen merkkijonosta token. Ignooraa whitespacen. Palauttaa tiedostonimen merkkijonon tai NULL virheen sattuessa.
char *parse_filename(char *token);

/* Käy läpi merkkijonon to_trim. Jos koko merkkijono on pelkkää whitespacea, palauttaa NULL. Muutoin varaa keosta muistia uudelle merkkijonolle, joka on kopio merkkijonosta to_trim ilman whitespacea edessä tai lopussa 
 ja palauttaa osoitteen tämän merkkijonon alkuun */
char *trim(char *to_trim);

// Lukee rivin virrasta in ja parsii sen sisällön listaan rakenteita command. Virheen sattuessa palauttaa NULL. Muutoin palauttaa kyseisen listan.
struct command **parse_command(FILE **in);

// kutsuu execv rakenteen command *cmd määrittämälle komennolle.
void execute_command(char *exec_path, struct command *cmd);

// Tarkistaa tuleeko rakenteen command *cmd lähtö ohjata tiedostoon. Jos pitää, sulkee virran stdout, avaa tiedoston ja kopioi tiedostokuvaimen virran stderr päälle.
void check_output(int *fd, struct command *cmd);

// käy läpi listan **cmds kommenot ja luo kullekin lapsiprosessin, joka kutsuu execute_command vastaavalle komennolle.
void execute_commands(char **path, struct command **cmds, FILE **in);




#endif