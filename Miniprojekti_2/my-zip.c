#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

#include "my-zip.h"
#include "common.h"

// sulkee nykyisen virran *in ja avaa samaan osoittimeen virran tiedostoon argv[i]
void switch_file(FILE **in, int i, char *argv[])
{
    if ( fclose(*in) != 0 )
        file_close_err("my-zip");
    *in = NULL;
    if ( (*in = fopen(argv[i], "r")) == NULL )
        file_open_err("my-zip");
}

// yhdistää merkkijonon *second merkkijonon *first perään. Jos *first:n perässä on \n, se korvataan pois
char *merge_strings(char *first, char *second)
{
    int length1 = strlen(first);
    int length2 = strlen(second);
    int total = (length1 + length2 + 1) * sizeof(char);
    if ( (first = (char *) realloc( first, total )) == NULL )
        malloc_err("my-zip"); 
    first[ length1 - 1] = '\0';
    first = strcat(first, second);
    return first;
}

// jos tulee vastaan EOF ja seuraava tiedosto on saatavilla, vaihtaa tiedoston seuraavaan ja palauttaa 1. Muutoin palauttaa 0.
int check_eof(FILE **in, int *i, int argc, char *argv[])
{
    if ( (feof(*in) != 0) && (*i + 1 < argc)) {
        (*i)++;
        switch_file(in, *i, argv);
        return 1;
    }
    return 0;
}

// käy läpi argv tarkennetut tiedostot ja kirjoittaa niistä RLE-koodauksella tuloksen virtaan *out
void zip(int argc, char *argv[], FILE *in)
{
    ssize_t n1 = 0;
    ssize_t n2 = 0;
    size_t len = 0;
    char *line = NULL;
    char *next_line = NULL;
    int i = 1;

    while (i < argc) {

        if ( (in = fopen(argv[i], "r")) == NULL )
            file_open_err("my-zip");

        do {

            n1 = getline(&line, &len, in);
            n2 = getline(&next_line, &len, in);

            /* jos tulee vastaan eof, vaihdetaan tiedostoja, tarkastetaan seuraavan tiedoston ensimmäinen rivi ja yhdistetään rivi aiempaan, jos
               edellisen rivin viimeinen kirjain on sama kuin seuraavan ensimmäinen.
               Seuraava tiedosto saattaa olla vain yhden rivin pituinen, tarkistus on loopissa
            */
            while (check_eof(&in, &i, argc, argv)) {
                n2 = getline(&next_line, &len, in);
                
                if ( (n1 != -1) && (n2 != -1) && ( line[ strlen(line) - 2 ] == next_line[0] ) ) {
                    line = merge_strings(line, next_line);
                    n2 = getline(&next_line, &len, in);
                }
            }


            if (n1 != -1) {
                write_out(line);
            }
            if (n2 != -1) {
                write_out(next_line);
            }
                

            free(line);
            free(next_line);
            line = NULL;
            next_line = NULL;

        } while ( (n2 != -1) && (n1 != -1) );
        
        if (fclose(in) != 0)
            file_close_err("my-zip");
        
        i++;
    }
}


// kirjoittaa rivin *line sisällön kompressoituna RLE-koodauksella virtaan stdout. 
void write_out(char *line)
{
    uint32_t num_chars = 0; // 4-tavua taattu
    int index_of_current = 0;
    
    char ch[2];
    ch[1] = '\0';

    char text[ strlen(line) + 1 ];
    strcpy(text, line);


    while (index_of_current < strlen(text)) {
        ch[0] = text[index_of_current];
        
        if (ch[0] == '\0')
            break; 
        
        num_chars = (uint32_t) strspn(text + index_of_current, ch);     // strspn kertoo montako kertaa sama kirjain on peräkkäin alkaen osoitteesta text + index_of_current
        fwrite( (void *) &num_chars, sizeof(uint32_t), 1, stdout);
        fwrite( (void *) ch, 1, 1, stdout);
        index_of_current += num_chars;
    }
}


int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("my-zip: file1 [file2 ...]\n");
        exit(1);
    }
    FILE *in = NULL;
    
    zip(argc, argv, in);

    return 0;
}