#include <iostream>
#include <chrono>
#include "Key.h"
#include <string.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include <unordered_map>

using namespace std;

class Hash_Function
{
public:
  long operator()(const Key& key) const
  {
    long index = 0;
    for (int i = 0; i < PW_CHAR_LENGTH; i++)
    {
      index = index * 101 + key.digit[i];
    }
    return index;
  }
};

int main(int argc, char* argv[])
{
  unsigned char buffer[PW_CHAR_LENGTH+1]; //Temporary string buffer
  Key encrypted;  //Encrypted password
  Key zero = {{0}};  //All zero key
  Key table[PW_BIT_COUNT];  

  if (argc != 2)
  {
    cout << "Usage:" << endl << argv[0] << " password < randx.txt" << endl;
    return 1;
  }

  //Convert user defined password to array of digits.
  encrypted = KEYinit((unsigned char *) argv[1]);
  
  //Read table from specified table file
  for (int i{0}; i < PW_BIT_COUNT; ++i)
  {
    scanf("%s", buffer);
    table[i] = KEYinit(buffer);
  }

  auto begin = chrono::high_resolution_clock::now();

  unordered_multimap<Key, Key, Hash_Function> firstHalf;  
  Key current = zero;

  int middleCombs = pow(ALPHA_SIZE, PW_CHAR_LENGTH/2);  
  cout << "Filling up first half... (" << middleCombs << " combinations)" << endl;  
  //Iterate through amount of combinations of half the password
  for(int i = 0; i < middleCombs; i++)
  { 
    Key subset = KEYsubsetsum(current, table);
    firstHalf.insert(make_pair(subset, current));    
    ++current;
  }
  cout << endl;
  
  Key increment = current;  //Key to increment to generate second half combinations
  cout << "Continuing combinations from " << increment << endl;  

  //Calculate amount of combinations needed for second half of table
  int count = pow(ALPHA_SIZE, PW_CHAR_LENGTH - (PW_CHAR_LENGTH/2));  
  cout << "Checking for possible passwords... (Trying " << count << " combinations)" << endl;    
  cout << endl;
  for(int i = 0; i < count; i++)
  {
    //Get crypted difference
    Key diff = encrypted - KEYsubsetsum(current, table);
    
    //Check for difference in first half 
    if (firstHalf.count(diff) >= 1)
    {
      auto range = firstHalf.equal_range(diff);
      cout << "Found possible password: ";
      for (auto i = range.first; i != range.second; ++i)
      {
        cout << current + i->second;
      }
      cout << endl;
    }   
    //Go to next step to check
    current = current + increment;
  }

  cout << endl;  
  auto end = chrono::high_resolution_clock::now();
  cout << "Decryption took "
       << std::chrono::duration_cast<chrono::seconds>(end - begin).count()
       << " seconds." << endl;
  return 0;
}