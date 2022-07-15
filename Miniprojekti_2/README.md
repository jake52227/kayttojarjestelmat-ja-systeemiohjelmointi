## Miniprojekti 2

### Tekijä ja päivämäärä:
Jaakko Pyrhönen 4.7.2022

### Projektin tarkoitus
Henkilökohtainen projekti kurssille käyttöjärjestelmät ja systeemiohjelmointi. Projektissa on luotu erilaisia utiliteettifunktioita kuten 
Unix:n cat-funktion kopio my-cat.

### Projektissa käytetyt työkalut
- Käyttöjärjestelmä: Ubuntu 20.04.04
- Kieli: C, ei noudata standardia
- kääntäjä: gcc 9.4.0

### Kääntäminen:
Projektin ohjelmat kääntyvät hakemistossa olevalla Makefile:lla: `cd` hakemistoon ja kirjoita `make`.

### Käyttö

#### my-cat
My-cat lukee tiedostosta rivin kerrallaan ja tulostaa ne näytölle. My-cat:lle voi antaa useamman tiedoston, jolloin ne tulostetaan järjestyksessä.
```bash
./my-cat file1 file2 ...
```
#### my-grep 
My-grep käy läpi sille argumenttina annetut tiedostot ja tulostaa jokaisen rivin, jolta löytyy argumenttina annettu termi pattern.
```bash
./my-grep pattern file1 file2 ...
```
my-grep voidaan kutsua myös pelkällä patternilla, jolloin syöte luetaan virrasta stdin:
```bash
./my-grep pattern
```

#### my-zip
My-zip käy läpi sille argumenttina annetut tiedostot ja kirjoittaa niistä run-length encoding:ia käyttäen yhden pötkön virtaan stdout.
```bash
./my-zip file1 file2 ...
```
#### my-unzip
My-unzip toimii päinvastoin kuin my-zip. Se käy läpi sille annetut RLE-koodatut tiedostot ja dekoodaa ne. Tulos kirjoitetaan virtaan stdout.
```bash
./my-unzip file1 file2 ...
```

### Toimintaperiaate
#### my-cat
my-cat vain lukee merkkejä `fgets`-funktiota käyttäen ja tulostaa ne.

#### my-grep
my-grep lukee rivejä `getline`-funktiolla ja käyttää hyväkseen `strstr`-funktiota tarkistaakseen löytyykö riviltä pattern osamerkkijonona.

#### my-zip & my-unzip
my-zip ja my-unzip käyttävät oheista rakennetta säilyttämään tietoja kirjoitettavista ja luettavista asioista:
```c
struct item
{
    int32_t consecutive;
    int8_t character;
};
```
`consecutive` kuvaa montako kertaa merkki `character` on ilmennyt. Jäsenmuuttujien koot ovat määrätyt 32- ja 8-bittiä.

my-zip lukee syötettä merkki kerrallaan ja pitää kirjaa siitä montako kertaa sama merkki on ilmennyt. Tämän perusteella kirjoitetaan tulos, kun
luettu merkki eroaa edellisestä.

my-unzip toimii tietysti päinvastoin ja lukee syötettä 5 tavua kerrallaan tallentaen tuloksen rakenteeseen `item` ja sitten kirjoittaen
`consecutive` verran merkkejä `character`.


