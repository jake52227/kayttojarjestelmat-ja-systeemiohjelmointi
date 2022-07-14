#ifndef WISHCOMMON_H
#define WISHCOMMON_H

#define common_err_msg() do { fprintf(stderr, "An error has occurred\n"); } while(0);
#define mem_alloc_err_msg(spot) do { fprintf(stderr, "error at %s: memory allocation failure\n", spot); } while(0);


// rakenne, joka säilyttää tietoa suoritettavasta komennosta.
struct command {
    char *cmd;          // varsinainen suoritettava ohjelma kuten ls
    char **arguments;   // argumentit ohjelmalle. Ensimmäinen argumentti on ohjelman nimi
    char *output_file;  // minne, jos minnekään, ohjelman lähtö ohjataan
    int *wait;          // Kun ohjelma laitetaan ajoon, kutsutaanko heti waitpid
};


// Vapauttaa rakenteen *cmd varaaman muistin keosta.
void free_cmd(struct command *cmd);

// Käy läpi listan *cmds ja vapauttaa sen varaamat muistin keosta.
void free_cmds(struct command **cmd);

// avaa virran tiedosto-osoittimeen *in tiedostoon file. Jos file == NULL, avaa virran stdin
void open_input_stream(FILE **in, char *file);

// sulkee avatun virran tiedosto-osoittimesta *in
void close_input_stream(FILE **in);

#endif