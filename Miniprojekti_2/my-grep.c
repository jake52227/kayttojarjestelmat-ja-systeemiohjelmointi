#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "common.h"

// lukee virrasta *in rivin kerrallaan ja etsii rivistä substring:iä *pattern. Jos substring löytyy, tulostaa kyseisen rivin näytölle. Jos "pattern" on tyhjä, tulostetaan kaikki
void read_and_check(FILE *in, const char *pattern)
{
    ssize_t n = 0;
    size_t len = 0;
    char match_all = 0;

    if (strlen(pattern) == 0) {
        match_all = 1;
    }

    while ( n != -1 ) {
        char *line = NULL;
        n = getline(&line, &len, in);
        if (n != -1) {
            if (!match_all) {
                if (strstr(line, pattern) != NULL) // strstr hoitaa patternin etsimisen
                    printf("%s", line);
            } else {
                printf("%s", line);
            }
            free(line);
        } else {
            if (errno == ENOMEM) {
                fprintf(stderr, "read_and_check: getline error with memory allocation\n");
                free(line);
                exit(1);
            }
            free(line);
        }
    }
}

int main(int argc, char *argv[])
{
    FILE *in = NULL;
    const char *pattern = NULL;
    
    if (argc < 2) {
        fprintf(stderr, "my-grep: searchterm [file ...]\n");
        exit(1);
    }

    pattern = argv[1];

    // jos annettu vain etsittävä pattern, luetaan stdin-virrasta
    if (argc == 2) {
        in = stdin;
        read_and_check(in, pattern);
    } else {
        // muutoin käydään läpi annetut tiedostot
        for (int i = 2; i < argc; i++) {
            if ( (in = fopen(argv[i], "r") ) == NULL)
                file_open_err("my-grep");

            read_and_check(in, pattern);
            
            if (fclose(in) != 0)
                file_close_err("my-grep");
            
            in = NULL;
        }
    }

    return 0;
}