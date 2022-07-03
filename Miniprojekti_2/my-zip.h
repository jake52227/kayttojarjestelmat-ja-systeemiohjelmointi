#ifndef MYZIP_H
#define MYZIP_H

// sulkee nykyisen virran *in ja avaa samaan osoittimeen virran tiedostoon argv[i]
void switch_file(FILE **in, int i, char *argv[]);
// yhdistää merkkijonon *second merkkijonon *first perään.
char *merge_strings(char *first, char *second);
// jos tulee vastaan EOF ja seuraava tiedosto on saatavilla palauttaa 1. Muutoin palauttaa 0.
int check_eof(FILE **in, int *i, int argc, char *argv[]);
// käy läpi kaikki annetut tiedostot ja tulostaa näytölle niiden run-length-enkoodauksen tuloksen
void zip(int argc, char *argv[], FILE *in);
// kirjoittaa rivin *line sisällön kompressoituna RLE-koodauksella virtaan *out.
void write_out(char *line);

#endif