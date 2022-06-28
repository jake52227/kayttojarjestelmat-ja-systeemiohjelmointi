#ifndef REVERSE_H
#define REVERSE_H

#define file_open_err(filename) do {fprintf(stderr, "error: cannot open file '%s'\n", filename); exit(1);} while(0);
#define file_close_err(filename) do {fprintf(stderr, "error trying to close '%s' stream\n", filename); exit(1); } while(0);

struct line {
    char *content;
    struct line *next;
};

// tarkastaa argumentit argv ja määrittelee niiden perusteella tuloste - ja syötevirrat
void assign_fds(FILE **in, FILE **out, int argc, char *argv[]);
// lukee input-virrasta FILE *in syötettä rivi kerrallaan ja lisää ne listaan. Palauttaa osoitteen luotuun listaan.
struct line *read_to_list(FILE *in);
// varaa keosta muistia line-rakenteelle. Asettaa rakenteen char *content-jäsenmuuttujan osoittamaan parametrina annettuun osoitteeseen. Palauttaa varatun muistipaikan osoitteen
struct line *allocate_node(char *content);
// lisää uuden solmun listan struct line *head alkuun. Alustaa uuden solmun content-jäsenmuuttujan osoitteen annetulla osoitteella char *content. Palauttaa listan uuden pään.
struct line *append_to_head(struct line *head, char *content);
// kirjoittaa osoitteesta struct line *head alkavan listan sisällön virtaan FILE *out
void write_list(struct line *head, FILE *out);
// vapauttaa osoitteesta struct line *line alkavan listan käyttämän muistin
void free_list(struct line *head);



#endif