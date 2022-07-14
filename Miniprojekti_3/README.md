## Miniprojekti 3

### Tekijä ja päivämäärä:
Jaakko Pyrhönen 14.7.2022

### Projektin tarkoitus
Henkilökohtainen projekti kurssille käyttöjärjestelmät ja systeemiohjelmointi. Projektissa on luotu Unix shell nimeltään wish.

### Projektissa käytetyt työkalut
- Käyttöjärjestelmä: Ubuntu 20.04.04
- Kieli: C, ei noudata standardia
- kääntäjä: gcc 9.4.0

### Kääntäminen:
Projektin ohjelmat kääntyvät hakemistossa olevalla Makefile:lla: `cd` hakemistoon ja kirjoita `make`.

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

