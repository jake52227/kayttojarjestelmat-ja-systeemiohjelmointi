#ifndef COMMON_H
#define COMMON_H

// hyödyllisiä asioita, joita käytetään kaikissa ohjelmissa

#define file_open_err(program_name) do {fprintf(stderr, "%s: cannot open file\n", program_name); exit(1);} while(0);
#define file_close_err(program_name) do {fprintf(stderr, "%s: failed to close file\n", program_name); exit(1);} while(0);


void open_file(FILE **fptr, char *mode, char *program_name, char *filename)
{
    if ( (*fptr = fopen(filename, mode)) == NULL ) {
        file_open_err(program_name);
    }
}


void close_file(FILE **fptr, char *program_name)
{
    if ( fclose(*fptr) != 0 )
        file_close_err(program_name);
    *fptr = NULL;
}


#endif