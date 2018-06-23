/**********************************************************************
 *  M�nsterigenk�nning readme.txt
 **********************************************************************/

 Ungef�rligt antal timmar spenderade p� labben (valfritt):

/**********************************************************************
 *  Empirisk    Fyll i tabellen nedan med riktiga k�rtider i sekunder
 *  analys      n�r det k�nns vettigt att v�nta p� hela ber�kningen.
 *              Ge uppskattningar av k�rtiden i �vriga fall.
 *
 **********************************************************************/
    
      N       brute       sortering
 ----------------------------------
    150       30          25
    200       66          57
    300       242         58
    400       516         85
    800       3976        422
   1600       31575       1342
   3200       243128      5604
   6400       1872086     24226
  12800       14415069    104689


/**********************************************************************
 *  Teoretisk   Ge ordo-uttryck f�r v�rstafallstiden f�r programmen som
 *  analys      en funktion av N. Ge en kort motivering.
 *
 **********************************************************************/

Brute: O(N^4)
Motivering: I värsta fall hittar den samma lutning alltid vilket gör att for-loopen i if-satsen alltid körs.

Sortering: O(N^2(1 + log(N)))
Motivering: Vi tittade på vår kod och fick fram:
N*(N + N*log(N) + N) = N*(2N +Nlog(N)) = 2N^2 + N^2log(N)
N^2 + N^2*log(N) = N^2(1 + log(N))
Notera att sorteringen som STL-biblioteket använder sig har tidskomplexitet O(N*log(N)) (troligen används quicksort)
