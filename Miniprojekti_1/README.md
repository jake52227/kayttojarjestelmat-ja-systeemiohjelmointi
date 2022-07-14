## Miniprojekti 1

### Tekijä ja päivämäärä:
Jaakko Pyrhönen 28.6.2022

### Projektin tarkoitus
Henkilökohtainen projekti kurssille käyttöjärjestelmät ja systeemiohjelmointi 

### Mitä ohjelma tekee
Ohjelma lukee syötettä joko tiedostosta tai suoraan käyttäjän näppäiltynä rivi kerrallaan ja tulostaa lopuksi syötteen toisinpäin käyttäjän tarkentamaan paikkaan.

### Projektissa käytetyt työkalut
- Käyttöjärjestelmä: Ubuntu 20.04.04
- Kieli: C, ei noudata standardia
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

