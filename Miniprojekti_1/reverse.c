#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "reverse.h"

// tarkastaa argumentit argv ja määrittelee niiden perusteella tuloste - ja syötevirrat
void assign_fds(FILE **in, FILE **out, int argc, char *argv[])
{
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }
    
    if (argc == 3) {

        if (!strcmp(argv[1], argv[2])) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }

        if ( (*in = fopen(argv[1], "r")) == NULL )
            file_open_err(argv[1]);
        if ( (*out = fopen(argv[2], "w")) == NULL )
            file_open_err(argv[2]);
    } 
    else if (argc == 2) {
        if ( (*in = fopen(argv[1], "r")) == NULL )
            file_open_err(argv[1]);
        if ( (*out = fdopen(STDOUT_FILENO, "w")) == NULL )
            file_open_err("stdout");
    } 
    else {
        if ( (*in = fdopen(STDIN_FILENO, "r")) == NULL)
            file_open_err("stdin");
        if ( (*out = fdopen(STDOUT_FILENO, "w")) == NULL )
            file_open_err("stdout");
        
        fprintf(stdout, "reading from stdin\nPress ctrl+D when you wish to stop\n");
    }
}

// lukee input-virrasta FILE *in syötettä rivi kerrallaan ja lisää ne listaan. Palauttaa osoitteen luotuun listaan.
struct line *read_to_list(FILE *in)
{
    struct line *head = NULL;
    ssize_t n = 0;
    size_t len = 0;
    
    while ( n != -1 ) {
        char *content = NULL;
        n = getline(&content, &len, in);
        if (n != -1) {
            head = append_to_head(head, content);
        } else {
            if (errno == ENOMEM) {
                fprintf(stderr, "getline error with memory allocation at read_to_list\n");
                free_list(head);
                free(content);
                exit(1);
            }
            free(content);
        }
    }
    return head;
}

// varaa keosta muistia line-rakenteelle. Asettaa rakenteen char *content-jäsenmuuttujan osoittamaan parametrina annettuun osoitteeseen. Palauttaa rakenteelle varatun muistipaikan osoitteen
struct line *allocate_node(char *content)
{
    struct line *new_line = NULL;
    new_line = (struct line *) malloc(sizeof(struct line));
    if (new_line == NULL) {
        fprintf(stderr, "malloc failed\n");
        return NULL; // palauttaa NULL, jotta append_to_head voi yrittää vapauttaa listan ennen poistumista
    }
    new_line->content = content;
    new_line->next = NULL;
    return new_line;
}

// lisää uuden solmun listan struct line *head alkuun. Alustaa uuden solmun content-jäsenmuuttujan osoitteen annetulla osoitteella char *content. Palauttaa listan uuden pään.
struct line *append_to_head(struct line *head, char *content)
{
    struct line *new_node = allocate_node(content);
    if (new_node == NULL) {
        free_list(head);
        exit(1);
    }
    if (head == NULL) {
        head = new_node;
    } else {
        new_node->next = head;
        head = new_node;
    }
    return head;
}

// kirjoittaa osoitteesta struct line *head alkavan listan sisällön virtaan FILE *out
void write_list(struct line *head, FILE *out)
{
    struct line *ptr = head;
    if (ptr == NULL) {
        fprintf(stderr, "reverse: input and output file must differ\n");
        exit(1);
    }

    while (ptr != NULL) {
        fprintf(out, "%s", ptr->content);
        ptr = ptr->next;
    }
}

// vapauttaa osoitteesta struct line *head alkavan listan käyttämän muistin
void free_list(struct line *head)
{
    struct line *ptr = head;
    if (ptr == NULL) {
        return;
    }
    while (ptr != NULL) {
        head = head->next;
        free(ptr->content);
        free(ptr);
        ptr = head;
    }
    head = NULL;
}