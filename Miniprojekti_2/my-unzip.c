#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "common.h"

// lukee parametrina annetut RLE-zipatut tiedostot ja unzippaa ne tulostaen tuloksen
void unzip(int argc, char *argv[])
{
    FILE *in = NULL;
    uint32_t integer;
    char character;
    
    for (int i = 1; i < argc; i++) {
        if ( (in = fopen(argv[i], "r")) == NULL )
            file_open_err("my-unzip");
        
        while (feof(in) == 0) {
            fread(&integer, sizeof(uint32_t), 1, in);   // 4 tavun kokonaisluku
            fread(&character, sizeof(char), 1, in);     // 1 tavun merkki
      
            // jos ei ilmennyt eof tai virheitä, kirjoitetaan luetun kokonaisluvun verran kirjaimia
            if ( (feof(in) == 0) && (ferror(in) == 0)) {
                for (int j = 0; j < integer; j++) {
                    printf("%c", character);
                }
            }
        }
        if (fclose(in) != 0)
            file_close_err("my-unzip");
        in = NULL;
    }
}


int main(int argc, char *argv[])
{   
    if (argc < 2) {
        printf("my-unzip: file1 [file2 ...]\n");
        exit(1);
    }

    unzip(argc, argv);

    return 0;
}