#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>

#include "wish.h"
#include "wish-common.h"
#include "wish-builtin.h"


// varaa muistia uudelle struct command rakenteelle. Alustaa kaikki jäsenmuuttujat paitsi muuttujan wait arvoon NULL. Wait aseteaan arvoon 1. Palauttaa rakenteen tai NULL virheen sattuessa.
struct command *allocate_cmd()
{
    struct command *cmd = (struct command *) malloc(sizeof(struct command));
    if (cmd == NULL) {
        mem_alloc_err_msg("allocate_cmd"); 
        return NULL;
    }

    cmd->cmd = NULL;
    cmd->arguments = NULL;
    cmd->output_file = NULL;
    cmd->wait = (int *) malloc(sizeof(int));
    if (cmd->wait == NULL) {
        mem_alloc_err_msg("allocate_cmd"); 
        return NULL;
    }
    *cmd->wait = 1;
    return cmd;
}


// varaa muistia rakenteelle cmd ja alustaa sen merkkijonon line määrittämillä arvoilla. Palauttaa rakenteen tai NULL virheen sattuessa.
struct command *init_cmd(char *line, struct command *cmd)
{
    int i = 0;
    int size = 1;       // muuttuja, joka pitää kirjaa argumenttien määrästä
    char *token = NULL;
    char *saveptr = NULL;

    cmd = NULL;
    cmd = allocate_cmd(); 

    char delim[] = {' ','\t', '\0'};

    token = strtok_r(line, delim, &saveptr);
  
    cmd->cmd = trim(token); // trim poistaa argumenteista turhan whitespacen
    if (cmd->cmd == NULL) {
        free_cmd(cmd);
        return NULL;
    } 

    // argumentit
    do {
        size++;
        
        if ( (cmd->arguments = (char **) realloc( cmd->arguments, size * sizeof(char **))) == NULL ) {
            mem_alloc_err_msg("init_cmd");
            return NULL;
        }

        cmd->arguments[i] = trim(token);
        if (cmd->arguments[i] != NULL)
            i++;

    } while ( (token = strtok_r(NULL, delim, &saveptr)) != NULL );
     
    cmd->arguments[i] = NULL;

    return cmd;
}


// Parsii tiedostonimen merkkijonosta token. Ignooraa whitespacen. Palauttaa tiedostonimen merkkijonon tai NULL virheen sattuessa.
char *parse_filename(char *token)
{
    if (token == NULL)
        return NULL;

    char *filename = (char *) malloc(sizeof(char) * strlen(token));
    if (filename == NULL) {
        mem_alloc_err_msg("parse_filename");
        return NULL;
    }
    
    int j = 0;
    for (int i = 0; i < strlen(token); i++) {

        if ( (i != 0) && (i != strlen(token) - 1) && (isspace(token[i]))) {
            // liian monta tiedostoa '>' jälkeen, palauta NULL merkiksi virheestä
            free(filename);
            return NULL;
        }

        if (!isspace(token[i])) {
            filename[j] = token[i];
            j++;
        }
    }

    filename[j] = '\0';

    // ei mitään '>' merkin jälkeen, palauta NULL merkiksi virheestä 
    if (strlen(filename) == 0) {
        free(filename);
        filename = NULL;
    }

    return filename;
}


// Lähde oheiselle funktiolle käyttäjän Adam Rosenfield vastaus: https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way . Funktiota on muokattu hieman.
/* Käy läpi merkkijonon to_trim. Jos koko merkkijono on pelkkää whitespacea, palauttaa NULL. Muutoin varaa keosta muistia uudelle merkkijonolle, joka on kopio merkkijonosta to_trim ilman whitespacea edessä tai lopussa 
 ja palauttaa osoitteen tämän merkkijonon alkuun */
char *trim(char *to_trim)
{
    char *str = to_trim; 
    char *end;

    while ( isspace((unsigned char)*str) ) 
        str++;

    if (*str == '\0') { // merkkijono pelkkää whitespacea. Palauta NULL merkiksi virheestä
        return NULL;
    } 

    end = str + strlen(str) - 1;
    while( (end > str) && isspace( (unsigned char)*end ) ) end--;

    end[1] = '\0';

    // varataan muistia uuteen osoitteeseen ja kopioidaan haluttu osa sinne
    char *result = (char *) malloc(sizeof(char) * (end - str + 2));
    if (result == NULL) {
        mem_alloc_err_msg("trim");
        return NULL;
    }
    strcpy(result, str);

    return result;
}


void handle_loop_err(struct command **cmds, char *line, int i)
{
    common_err_msg();
    free(line);
    cmds[i] = NULL;
    free_cmds(cmds);
}

// Lukee rivin virrasta in ja parsii sen sisällön listaan rakenteita command. Virheen sattuessa palauttaa NULL. Muutoin palauttaa kyseisen listan.
struct command **parse_command(FILE **in)
{
    struct command **cmds = NULL;
    int i = 0;
    size_t len = 0;
    ssize_t n = -1;
    int inside_redirection_loop = 0; // inside- muuttujat pitävät kirjaa while-looppien kierroksista. 
    int inside_command_loop = 0;

    char *line = NULL;
    
    char *saveptr = NULL;
    char *saveptr2 = NULL;

    char *token = NULL;
    char *token2 = NULL;

    n = getline(&line, &len, *in);
    if (n == -1) {

        if (errno == EINVAL)
            fprintf(stderr, "error: bad arguments to getline at function parse_command\n");
        
        free(line);
        return NULL;
    }

    // erilliset komennot eritellään '&' merkillä. 
    token = strtok_r(line, "&", &saveptr);
    do {
        
        // jos saavutaan tähän looppiin uudestaan, tiedetään, että löytyi '&' - merkki ja kaikki komennot laitetaan ajoon peräkkäin odottamatta
        if (inside_command_loop > 0) {
            *cmds[i - 1]->wait = 0;
        }

        // jokaisen komennon sisällä voi olla uudelleenohjauksia tiedostoon '>' merkillä. 
        token2 = strtok_r(token, ">", &saveptr2);
        do {
            
            // jos saavutaan looppiin taas, tämä tarkoittaa, että kommenon output tulee uudelleenohjata
            if (inside_redirection_loop > 0) {
                // jos täällä on jo käyty, '>' merkkejä on annettu liikaa 
                if (inside_redirection_loop > 1) {
                    handle_loop_err(cmds, line, i);
                    return NULL;
                }
                cmds[i-1]->output_file = parse_filename(token2);
                if (cmds[i-1]->output_file == NULL) {
                    handle_loop_err(cmds, line, i);
                    return NULL;
                }

            } else {
                // muutoin kyseessä on komento, ei tiedosto
                cmds = (struct command **) realloc(cmds, (i + 1) * sizeof(struct command) );
                if (cmds == NULL) {
                    mem_alloc_err_msg("parse_command"); 
                    return NULL;
                }

                cmds[i] = init_cmd(token2, cmds[i]);

                if (cmds[i] != NULL) 
                    i++;
                
            }

            inside_redirection_loop++;

        } while ( (token2 = strtok_r(NULL, ">", &saveptr2)) != NULL );
        
        inside_redirection_loop = 0;
        inside_command_loop++;

    } while ( (token = strtok_r(NULL, "&", &saveptr)) != NULL );

    
    if (inside_command_loop > 1) {
        *cmds[i - 1]->wait = 0;
    }
    

    free(line);
    cmds[i] = NULL;

    return cmds;
}

// functio, joka siivoa jäljet, jos exec epäonnistuu. Vapauttaa keosta varattua muistia ja sulkee tiedostokuvaimen fd ja -osoittimen *in
void handle_exec_fail(int fd, char *exec_path, char **path, struct command **cmds, FILE **in)
{
    if (fd != -1)
        close(fd);           

    if (exec_path != NULL) 
        free(exec_path);

    if (*path != NULL)
        free(*path);
    free_cmds(cmds);
    close_input_stream(in);
    exit(1);
}


// kutsuu execv rakenteen command *cmd määrittämälle komennolle.
void execute_command(char *exec_path, struct command *cmd)
{   
    if (exec_path == NULL)  {
        if (cmd->cmd != NULL) {
            fprintf(stderr, "could not execute command '%s'\n", cmd->cmd);
        } else {
            fprintf(stderr, "could not execute command\n");
        }

    } else {
        execv(exec_path, cmd->arguments); 
        common_err_msg(); 
    }
}

// Tarkistaa tuleeko rakenteen command *cmd sisältämän komennon lähtö ohjata tiedostoon. Jos pitää, sulkee virran stdout, avaa tiedoston ja kopioi tiedostokuvaimen virran stderr päälle.
void check_output(int *fd, struct command *cmd)
{
    if (cmd->output_file != NULL) {
        close(STDOUT_FILENO); 
        *fd = open(cmd->output_file, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
        dup2(*fd, STDERR_FILENO);
    }
}

// käy läpi listan **cmds kommenot ja luo kullekin lapsiprosessin, joka kutsuu execute_command vastaavalle komennolle.
void execute_commands(char **path, struct command **cmds, FILE **in)
{
    if (cmds == NULL)
        return;

    int amount = 0;
    for (int i = 0; cmds[i] != NULL; i++) amount++; // tarkistetaan montako komentoa löytyy

    pid_t pids[amount];

    for (int j = 0; j < amount; j++) { // käydään annetut komennon läpi ja luodaan kullekin oma lapsiprosessi

        pids[j] = fork();
        
        if (pids[j] < 0) {
            fprintf(stderr, "error: fork failed\n");
            return;
        } else if (pids[j] == 0) {
            int fd = -1;
            check_output(&fd, cmds[j]);                     // tarkistetaan minne lähtö kirjoitetaan
            char *expath = check_path(*path, cmds[j]->cmd); // tarkistetaan polku suoritettavaan ohjelmaan
            execute_command(expath, cmds[j]);               // ajetaan ohjelma
            handle_exec_fail(fd, expath, path, cmds, in);   // jos päästään tähän asti, tuli virhe, joka pitää hoitaa
        } else {
            if (*cmds[j]->wait == 1) {                      // tarkistetaan pitääkö odottaa
                waitpid(pids[j], NULL, 0); 
            } 
        } 
    } 
  
    for (int k = 0; k < amount; k++)                        // varmistetaan, että kaikki prosessit ovat valmiita
        waitpid(pids[k], NULL, 0);
}
