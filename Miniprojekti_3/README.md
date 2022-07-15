## Miniprojekti 3

### Tekijä ja päivämäärä:
Jaakko Pyrhönen 14.7.2022

### Projektin tarkoitus
Henkilökohtainen projekti kurssille käyttöjärjestelmät ja systeemiohjelmointi. Projektissa on luotu Unix shell nimeltään wish.

### Projektissa käytetyt työkalut
- Käyttöjärjestelmä: Ubuntu 20.04.04
- Kieli: C, ei noudata mitään tiettyä standardia
- kääntäjä: gcc 9.4.0

### Kääntäminen:
Projekti kääntyy hakemistossa olevalla Makefile:lla: `cd` hakemistoon ja kirjoita `make`.

### Käyttö
wish voidaan kutsua kahdella tavalla:

Interaktiivinen moodi, jossa wish ottaa vastaan käyttäjän komentoja:
```bash
./wish
```

ja batch-moodi, jossa wish käy läpi tiedostoon kirjoitettuja komentoja:
```bash
./wish file
```

### Sisäänrakennetut funktiot

wish:llä on kolme sisäänrakennettua funktiota:

`cd` vaihtaa hakemistoa:
```
wish> cd directory
```
`exit` sulkee wishin:
```
wish> exit
```
`path` tarkentaa hakemistopolun, josta wish etsii suoritettavia ohjelmia. Path ottaa yhden tai useamman argumentin:
```
wish> path
```
tai
```
wish> path dirpath1 dirpath2 ...
```
Oletuksena path-muuttujaan on asetettu `/bin`

### Uudelleenohjaus ja samanaikaiset komennot
Wishin ajamien komentojen tulokset voidaan ohjata tiedostoon oheisella tavalla:
```
wish> command > file
```
komentoja voi suorittaa myös samanaikaisesti. Esimerkiksi:
```
wish> sleep 5 & sleep 5 & sleep 5
```
, joka odottaisi yhteensä noin 5 sekuntia.

### Toimintaperiaate
Yleisesti ottaen periaate on tämä:
- wish lukee syötteestä rivin käyttäen `getline`-funktiota
- luettu rivi jaetaan osiin erikoissymbolien '&' ja '>' perusteella
- Jokainen osa käydään läpi ja siltä poimitaan tiedot joko komentoa varten tai tiedostonimeä varten. Yksittäisen komennon tiedot tallennetaan
oheiseen rakenteeseen:
```c
struct command {
    char *cmd;
    char **arguments;
    char *output_file;
    int *wait;
};
``` 
- `cmd` on varsinainen suoritettava ohjelma/komento kuten `ls` esimerkiksi
- `arguments` säilyttää kyseisen komennon argumentteja, joista ensimmäinen on sama kuin `cmd`
- `output_file` on joko `NULL` tai sitten tiedosto, jonne ajettavan ohjelman lähtö tulee ohjata. Jos `output_file` on `NULL`, oletusvirta on `stdout`
- `wait` määrittää laitetaanko komennon suorittava lapsiprosessi odottamaan heti vai laitetaanko mahdollisesti muita samaan aikaan suoritettavia
komentoja ajoon heti perään muissa prosesseissa.

- Kaikki riviltä löydetyt komennon tallennetaan listaan yllä näkyvän mukaisia rakenteita.
- Ei-sisäänrakennetut Komennot suoritetaan luomalla jokaiselle oma lapsiprosessi `fork`-systeemikutsulla. Lapsiprosessi kutsuu `execv`-systeemikutsua.


