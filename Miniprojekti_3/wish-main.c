#include <stdio.h>
#include <stdlib.h>

#include "wish.h"
#include "wish-common.h"
#include "wish-builtin.h"


int main(int argc, char *argv[])
{
    struct command **cmds = NULL;

    FILE* in = NULL;
    char batch = 0;
    char *paths = NULL;

    if (argc < 2) {
        open_input_stream(&in, NULL);       // jos ei tiedostoa, avataan stdin
    } else if (argc == 2) {
        open_input_stream(&in, argv[1]);    // muutoin avataan tiedosto ja asetetaan muuttuja batch = true
        batch = 1;
    } else {
        fprintf(stderr, "error: too many arguments. Specify either none or a file\n");
        exit(1);
    }

    init_path(&paths);  // alustetaan path-muuttuja hakemistolla /bin
 
    while (1) {         // ikilooppi, jossa wish ottaa vastaan komentoja
        if (!batch)
            printf("wish> ");

        cmds = parse_command(&in);
        if (cmds == NULL) {
            if ( feof(in) != 0 ) {
                close_input_stream(&in);
                if (paths != NULL)
                    free(paths);
                exit(0);
            }

            continue;
        } 
        
        if (!execute_builtin(&paths, cmds, &in)) // tarkistetaan onko annettu komento sisäänrakennettu 
            execute_commands(&paths, cmds, &in);
        
        if (cmds != NULL)
            free_cmds(cmds);
        cmds = NULL;
    }

    if (paths != NULL)
        free(paths);
    
    close_input_stream(&in);

    return 0;
}