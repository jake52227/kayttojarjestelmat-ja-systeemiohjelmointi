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

// käy läpi argv tarkennetut tiedostot ja kirjoittaa niistä RLE-koodauksella tuloksen virtaan stdout
void zip(int argc, char *argv[])
{
    struct item item = {0, -1};
    char character;
    FILE *in = NULL;

    for (int i = 1; i < argc; i++) {
        open_file(&in, "r", "my-zip", argv[i]);
        while ( (character = fgetc(in)) != EOF ) {
            if (item.character == -1)
                item.character = character;

            if (item.character == character) {
                item.consecutive++;
            } else {
                fwrite(&item, 5, 1, stdout);    // kääntäjä lisää paddingia structin perään, joten kirjoitetaan eksplisiittisesti 5 tavua, ei sizeof(struct item)
                item.consecutive = 1;
                item.character = character;
            }
        }
        close_file(&in, "my-zip");
    }

    // jos ollaan poistumassa, mutta kirjoitettavaa on vielä
    if (item.consecutive > 0) {
        fwrite(&item, 5, 1, stdout);
    }
}


int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("my-zip: file1 [file2 ...]\n");
        exit(1);
    }
    
    zip(argc, argv);

    return 0;
}