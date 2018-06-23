#include <iostream>
#include <chrono>
#include "Key.h"
#include <string.h>

using namespace std;

//1. Kolla vad alla �verlagringar / funktioner vi inte f�rst�r g�r.
//2. https://www.youtube.com/watch?v=ZIOWy923v2w <- han vet hur man g�r en snabb subset-sum
//3. F�rb�ttra subset sum.
//4. $Profit$

int
main(int argc, char* argv[])
{
	char c = 5;
	cout << c << endl;

  unsigned char buffer[PW_CHAR_LENGTH+1];     // temporary string buffer
  Key candidate = {{0}};         // a password candidate
  Key encrypted;                 // the encrypted password
  Key candenc;                   // the encrypted password candidate
  Key zero = {{0}};              // the all zero key
  Key T[PW_BIT_COUNT];                      // the table T  

  if (argc != 2)
  {
    cout << "Usage:" << endl << argv[0] << " password < rand8.txt" << endl;
    return 1;
  }

  //Convert user defined password to array of digits.
  encrypted = KEYinit((unsigned char *) argv[1]);

  //Read table T
  for (int i{0}; i < PW_BIT_COUNT; ++i)
  {
    scanf("%s", buffer);
    T[i] = KEYinit(buffer);    
  }

  auto begin = chrono::high_resolution_clock::now();

  candenc = KEYsubsetsum(candidate, T);  
  do
  {
   candenc = KEYsubsetsum(candidate, T);
   //cout << candenc << endl;
   if (candenc == encrypted)
     cout << candidate << endl;

   ++candidate;
  } while (candidate != zero);

  auto end = chrono::high_resolution_clock::now();
  cout << "Decryption took "
       << std::chrono::duration_cast<chrono::seconds>(end - begin).count()
       << " seconds." << endl;

  return 0;
}