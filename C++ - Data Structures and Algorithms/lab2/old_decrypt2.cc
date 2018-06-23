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

long keyToDec(Key key) {
	long dec = 0;
	for (int i{ 0 }; i < PW_BIT_COUNT; i++) {
		int bit = KEYbit(key, i);
		if (bit) {
			dec += pow(2, PW_BIT_COUNT-i-1);
		}
	}
	return dec;
}

//table.front() = move(table.back());
  //table.pop_back();

vector<vector<Key>> subSetSum(vector<Key> table, Key target)
{
  if (target == Key({{0}}))
  {
    return {{}};
  }

  if (table.size() == 0)
  {
    return {};
  }

  vector<vector<Key>> results;

  //Save first element of table before deleting  
  Key first = table[0];

  table.erase(table.begin());

  // //Iterate through all values except first with new target being row above
  for (vector<Key> v : subSetSum(table, target - first))
  { 
    //Combine first and result and add to results
    v.push_back(first);  
    results.push_back(v);    
  }

  for (vector<Key> v : subSetSum(table, target)) {
    results.push_back(v);
  }

  return results;
}

int
main(int argc, char* argv[])
{
  unsigned char buffer[PW_CHAR_LENGTH+1];     // temporary string buffer
  Key candidate = {{0}};         // a password candidate
  Key encrypted;                 // the encrypted password
  Key candenc;                   // the encrypted password candidate
  Key zero = {{0}};              // the all zero key
  Key table[PW_BIT_COUNT];                      // the table T  

  if (argc != 2)
  {
    cout << "Usage:" << endl << argv[0] << " password < rand8.txt" << endl;
    return 1;
  }

  //Convert user defined password to array of digits.
  encrypted = KEYinit((unsigned char *) argv[1]);

  int tableRowLength = PW_BIT_COUNT;
  //Read table from specified table file
  for (int i{0}; i < tableRowLength; ++i)
  {
    scanf("%s", buffer);
    table[i] = KEYinit(buffer);
  }

  auto begin = chrono::high_resolution_clock::now();
  
  vector<Key> vTable(table, table + sizeof table / sizeof table[0]);

  //vector<Key> vTable({table});

  vector<vector<Key>> yo = subSetSum(vTable, encrypted);

  for (auto i : yo)
  { 
    Key sum;
    for(auto j : i)
    {
      sum=sum+j;      
    }
    cout << sum << endl;
  }

  auto end = chrono::high_resolution_clock::now();
  cout << "Decryption took "
       << std::chrono::duration_cast<chrono::seconds>(end - begin).count()
       << " seconds." << endl;

  return 0;
}