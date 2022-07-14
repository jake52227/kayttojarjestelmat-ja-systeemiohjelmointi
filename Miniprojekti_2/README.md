## Miniprojekti 2

### Tekijä ja päivämäärä:
Jaakko Pyrhönen 4.7.2022

### Projektin tarkoitus
Henkilökohtainen projekti kurssille käyttöjärjestelmät ja systeemiohjelmointi. Projektissa on luotu erilaisia utiliteettifunktioita kuten 
Unix:n cat-funktion kopio my-cat.

### Kääntäminen:
Projektin ohjelmat kääntyy hakemistossa olevalla Makefile:lla: `cd` hakemistoon ja kirjoita `make`. Kääntäjänä on käytetty GNU compiler:ia. 

### Projektissa käytetyt työkalut
- Käyttöjärjestelmä: Ubuntu 20.04.04
- Kieli: C, ei noudata standardia
- kääntäjä: gcc 9.4.0

### my-cat
My-cat lukee tiedostosta rivin kerrallaan ja tulostaa ne näytölle. My-cat:lle voi antaa useamman tiedoston, jolloin ne tulostetaan järjestyksessä.
```bash
./my-cat file1 file2 ...
```
### my-grep 
My-grep käy läpi sille argumenttina annetut tiedostot ja tulostaa jokaisen rivin, jolta löytyy argumenttina annettu termi pattern.
```bash
./my-grep pattern file1 file2 ...
```
my-grep voidaan kutsua myös pelkällä patternilla, jolloin syöte luetaan virrasta stdin:
```bash
./my-grep pattern
```

### my-zip
My-zip käy läpi sille argumenttina annetut tiedostot ja kirjoittaa niistä run-length encoding:ia käyttäen yhden pötkön virtaan stdout.
```bash
./my-zip file1 file2 ...
```
### my-unzip
My-unzip toimii päinvastoin kuin my-zip. Se käy läpi sille annetut RLE-koodatut tiedostot ja dekoodaa ne. Tulos kirjoitetaan virtaan stdout.
```bash
./my-unzip file1 file2 ...
```
