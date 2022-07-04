#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "common.h"

// jokainen kirjaus koostuu 4 tavun luvusta ja yhdestä 8 bitin ASCII-kirjaimesta
struct item
{
    int32_t consecutive;
    int8_t character;
};

// lukee parametrina annetut RLE-zipatut tiedostot ja unzippaa ne tulostaen tuloksen
void unzip(int argc, char *argv[])
{
    FILE *in = NULL;
    struct item item = {0, 0};
    
    for (int i = 1; i < argc; i++) {
        open_file(&in, "r", "my-unzip", argv[i]);
        
        while (feof(in) == 0) {
            fread(&item, 5, 1, in);

            // jos ei ilmennyt eof tai virheitä, tulostetaan luetun kokonaisluvun verran kirjaimia
            if ( (feof(in) == 0) && (ferror(in) == 0)) {
                for (int j = 0; j < item.consecutive; j++) {
                    printf("%c", item.character);
                }
            }
        }
        close_file(&in, "my-unzip");
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