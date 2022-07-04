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
        open_file(&in, "r", "my-cat", argv[i]);
        write_out(in);
        close_file(&in, "my-cat");
    }

    return 0;
}