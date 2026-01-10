Arkanoid ASCII game 
- Pokretanje - file se preuzme za githuba, otvori u Visual Studio 2017/2019 i pokrene u terminalu.
- Kontrole - tipka A (pomicanje palice lijevo), tipka D (pomicanje palice desno), tipka 1 (Start - pokrece igru), tipka 2 (Rules - otvara pravila igre), tipka 3 (Exit - gasi terminal).
- Igra radi u Windowsu i Linuxu, upotrebljeni odredeni #include-ovi za cross platformu.
- Dodana implementacija u .h i .cpp files
- Napravljena palica s dužinom te loptica 'O', blokovi i dodan tekst "Lives: ", "Blocks Left: "
- Organiziran kod
- Igra se uspješno pokreće, problem iscrtavanja frame-a na frame i zato igra "treperi", RJEŠENJE: smanjiti ugrađenu funkciju Sleep(ms) koja štedi CPU i sinkronizacije, problem se ne može u potpunosti rješiti ali se može minimalizirati.
