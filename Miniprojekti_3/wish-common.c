#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "wish-common.h"

// Vapauttaa rakenteen *cmd varaaman muistin keosta.
void free_cmd(struct command *cmd)
{
    if (cmd == NULL)
        return;
    
    if (cmd->cmd != NULL)
        free(cmd->cmd);

    if (cmd->output_file != NULL)
        free(cmd->output_file);

    if (cmd->wait != NULL)
        free(cmd->wait);

    if (cmd->arguments != NULL) {
        for (int i = 0; cmd->arguments[i] != NULL; i++) {
            free(cmd->arguments[i]);
        }
        
        free(cmd->arguments);
    }

    free(cmd);
}

// Käy läpi listan *cmds ja vapauttaa sen varaamat muistin keosta.
void free_cmds(struct command **cmds)
{
    if (cmds == NULL)
        return;

    for (int i = 0; cmds[i] != NULL; i++ ) {
        free_cmd(cmds[i]);
        cmds[i] = NULL;
    }
    
    free(cmds);
}

// avaa virran tiedosto-osoittimeen *in tiedostoon file. Jos file == NULL, avaa virran stdin
void open_input_stream(FILE **in, char *file)
{
    if (file == NULL) {
        *in = stdin;
    } else {
        if ( (*in = fopen(file, "r")) == NULL) {
            fprintf(stderr, "error: failed to open file\n");
            exit(1);
        }
    }
}


// sulkee avatun virran tiedosto-osoittimesta *in
void close_input_stream(FILE **in)
{
    if (*in != stdin) {
        fclose(*in);
    }
}