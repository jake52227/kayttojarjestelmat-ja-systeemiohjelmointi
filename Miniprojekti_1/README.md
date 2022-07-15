## Miniprojekti 1

### Tekijä ja päivämäärä:
Jaakko Pyrhönen 28.6.2022

### Projektin tarkoitus
Henkilökohtainen projekti kurssille käyttöjärjestelmät ja systeemiohjelmointi 

### Mitä ohjelma tekee
Ohjelma lukee syötettä joko tiedostosta tai suoraan käyttäjän näppäiltynä rivi kerrallaan ja tulostaa lopuksi syötteen toisinpäin käyttäjän tarkentamaan paikkaan.

### Projektissa käytetyt työkalut
- Käyttöjärjestelmä: Ubuntu 20.04.04
- Kieli: C, ei noudata mitään tiettyä standardia
- kääntäjä: gcc 9.4.0

### Kääntäminen
Projekti kääntyy hakemistossa olevalla Makefile:lla: `cd` hakemistoon ja kirjoita `make`.

### Ohjelman käyttö:

Kun ohjelma on käännetty, sen voi ajaa yhdellä kolmesta tavasta:

Lue syöte näppäimistöltä ja kirjoita stdout-virtaan:
```
./reverse
```
tai :
Lue syöte tiedostosta "inputfile" ja kirjoita tulos stdout-virtaan
```
./reverse inputfile
```
tai :
Lue syöte tiedostosta "inputfile" ja kirjoita tulos tiedostoon "outputfile". Jos "outputfile" on jo olemassa, se poistetaan.
```
./reverse inputfile outputfile
```

### Toimintaperiaate
Reverse lukee syötettä `getline`-funktiota käyttäen ja tallentaa rivit linkitettyyn listaan käyttäen oheista rakennetta:
```c
struct line {
    char *content;
    struct line *next;
};
```
Luetut rivit tallennetaan aina listan päähän, jolloin, kun ne tulostetaan, järjestys on käänteinen suhteessa järjestykseen, jossa rivit
luettiin.


### Assumptions and Errors
Projektissa tuli vastata muutamiin oletuksiin ja virhetilanteisiin. Ohessa on esitelty kuinka niihin on pyritty vastaamaan:

- Input is the same as output

Tähän on vastattu vertaamalla syöte- ja tulostustiedostoja
```c
if (!strcmp(argv[1], argv[2])) {
    fprintf(stderr, "reverse: input and output file must differ\n");
    exit(1);
}
```
sekä, kun on aika kirjoittaa, tarkistamalla listan päästä onko listassa mitään kirjoitettavaa:
```c
struct line *ptr = head;
if (ptr == NULL) {
    fprintf(stderr, "reverse: input and output file must differ\n");
    exit(1);
}
```
- String length

Luetun merkkijonon pituutta ei rajoiteta mitenkään, koska kukin rivi luetaan `getline`-funktiolla.

- File length

Kunhan tietokoneessa riittää muistia, tiedosto voi olla miten pitkä tahansa, sillä rivejä luetaan niin kauan, kunnes `getline` palauttaa -1
joko virheestä tai EOF:stä johtuen:
```c
while ( n != -1 ) {
    char *content = NULL;
    n = getline(&content, &len, in);
    if (n != -1) {
        head = append_to_head(head, content);
    ...
```

- Invalid files, malloc fails, error messages

Jokainen tiedosto avataan oheiselle tavalla
```c
if ( (*in = fopen(argv[1], "r")) == NULL )
  file_open_err(argv[1]);
```
, missä makro `file_open_err`:
```c
#define file_open_err(filename) do {fprintf(stderr, "reverse: cannot open file '%s'\n", filename); exit(1);} while(0);
```

Ja mitä muistinvaraukseen tulee, jokaiselle solmulle varataan muistia `allocate_node`-funktiolla, joka palauttaa NULL vain, kun 
malloc epäonnistuu:
```c
struct line *allocate_node(char *content)
{
    struct line *new_line = NULL;
    new_line = (struct line *) malloc(sizeof(struct line));
    if (new_line == NULL) {
        fprintf(stderr, "malloc failed\n");
        return NULL; // palauttaa NULL, jotta append_to_head voi yrittää vapauttaa listan ennen poistumista
    }
...
```


```c
...
struct line *new_node = allocate_node(content);
if (new_node == NULL) {
    free_list(head);
    exit(1);
}
...
```

- Too many arguments passed to program

Tähän on vastattu yksinkertaisesti tarkistamalla komentoriviargumentit
```c
if (argc > 3) {
    fprintf(stderr, "usage: reverse <input> <output>\n");
    exit(1);
}
``` 







