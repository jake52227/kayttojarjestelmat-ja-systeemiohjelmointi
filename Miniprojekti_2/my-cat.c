#include <stdio.h>
#include <stdlib.h>

#include "common.h"

void write_out(FILE *in)
{
    char buf[1024];
    while ( (fgets(buf, 1024 * sizeof(char), in)) != NULL )
        printf("%s", buf);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        exit(0);
    }

    FILE *in = NULL;

    for (int i = 1; i < argc; i++) {
        if ( (in = fopen(argv[i], "r") ) == NULL)
            file_open_err("my-cat");
        
        write_out(in);
        
        if (fclose(in) != 0)
            file_close_err("my-cat");
        
        in = NULL;
    }

    return 0;
}