/**********************************************************************
 *  Kn�cka l�senord readme.txt
 **********************************************************************/

 Ungef�rligt antal timmar spenderade p� labben (valfritt): >20 h

/**********************************************************************
 *  Ge en h�gniv�beskrivning av ditt program decrypt.c.
 **********************************************************************/
En symboltabell beskriven nedanför skapas. Sedan itereras resten av alla 
kombinationer där kombinationens krypterade värde subtraheras från det 
krypterade lösenordet. Skillnaden kollas sedan om den finns som nyckel 
i första tabellen. Om den gör det kan vi kombinera nuvarandra kombination 
med värdet från första tabellen som hade hittade nyckeln för att få 
ett möjligt lösenord.
(Anledningen att det är möjligt att dela tabellen på mitten utgår ifrån att 
001 000 + 000 101 = 001 101, alltså aab aaa + aaa bab = aab bab.)


/**********************************************************************
 *  Beskriv symboltabellen du anv�nt f�r decrypt.c.
 **********************************************************************/
Alla kombinationer på högra sidan av lösenordslängden genereras där 
kombinationens resulat vid kryptering (KEYsubsetsum) sparas som nyckel.
Värdet är kombinationen som gav resultatet, vilket skulle kunna vara flera.

/**********************************************************************
 *  Ge de dekrypterade versionerna av alla l�senord med 8 och 10
 *  bokst�ver i uppgiften du lyckades kn�ca med DIN kod.
 **********************************************************************/

8 bokst�ver         10 bokst�ver
-----------         ------------
congrats            completely
youfound            unbreakabl
theright            cryptogram
solution            ormaybenot

/****************************************************************************
 *  Hur l�ng tid anv�nder brute.c f�r att kn�cka l�senord av en viss storlek?
 *  Ge en uppskattning markerad med en asterisk om det tar l�ngre tid �n vad
 *  du orkar v�nta. Ge en kort motivering f�r dina uppskattningar.
 ***************************************************************************/
Antal kombinationer är storleken på alfabetet upphöjt i antal bokstäver, alltså
R^C. Detta leder till en ökning av 32 för varje extra bokstav.
Char     Brute     
--------------
 4       < 1 sekund
 5       25 sekunder   
 6       1108 sekunder
 8       *1 134 592 sekunder (1108 * 32 * 32)


/******************************************************************************
 *  Hur l�ng tid anv�nder decrypt.c f�r att kn�cka l�senord av en viss storlek?
 *  Hur mycket minne anv�nder programmet?
 *  Ge en uppskattning markerad med en asterisk om det tar l�ngre tid �n vad
 *  du orkar v�nta. Ge en kort motivering f�r dina uppskattningar.
 ******************************************************************************/
Antal kombinationer som lagras är storleken på alfabetet upphöjt i antal bokstäver 
genom 2, alltså R^(C/2). Detta leder till en ökning av 32 för varannan extra bokstav.
Antal operationer vi använder är 2^(N/2 + 1) vilket leder till en ökning av 2^5 
för varje bokstav.
Char    Tid (sekunder)    Minne (bytes)
----------------------------------------
6       < 1 sekund        ~2.2 * 10^6 B
8       3 sekunder        ~5.62 * 10^7
10      136 sekunder      ~1.8 * 10^9 B
12      4352              *5.76 * 10^10 B (1.8 * 10^9 * 32)
 
/*************************************************************************
 * Hur m�nga operationer anv�nder brute.c f�r ett N-bitars l�senord? 
 * Hur m�nga operationer anv�nder din decrypt.c f�r ett N-bitars l�senord?
 * Anv�nd ordo-notation.
 Brute:
 O(2^N)

 Decrypt:
 Jämna tal: 2^(N/2) * 2 -> O(2^(N/2 + 1))
 (Ojämna tal: O(2^(N/2) + 2^(N-(N/2))), (OBS: Heltalsdivision))
 C = 5
 32^C/2 + 32^C/2+1 = 32^2 + 32^3
 2^25/2 + 2^
 *************************************************************************/
