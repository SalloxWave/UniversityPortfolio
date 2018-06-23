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

using SubSetSumTable = vector<pair<int, vector<int>>>;
//using SubSetSumTable = vector<pair<int, map<int, bool>>>;

/*void fillSubSets(int target, vector<vector<Key>>& results, const SubSetSumTable& table)
{
  //index = index - current element

  vector<Key> result;

  int index = 0;
  //Iterate through table backwards
  for(auto row = table.rbegin(); i != table.begin(); --it)
  {    
    index = target - row->first;
    //Is there a true value on index
    if (find(row->second.begin(), row->second.end(), index))
    {
      result.push_back(row->first);
    }
    //Every row above on index is also false
    else { break; }
  }

  if result != empty
    resulsts.push_back(resut);

  

  //Iterate through table backwards (skip last element)    
    //if index - current element < 0
      //continue
    //if value on index is true || index == 0
      //Add element to current set
      //index = index - current element
    //if index == 0
      //Add current set to results
      //Clear current, insert last element
      //index = current element (reset)
  
  //10, 2 >> results
  //10, fillSubSets(2) >> results
  //fillSubSets(10), 2
  //
  for (auto element : result) {
    fillSubSets
  }

  {10, fillSubSets(2)}
  {fillSubSets(10), fillSubSets(2)}
  fillSubSets(7)
  fillSubSets(11)
}*/

vector<vector<Key>> subSetSum(Key table[PW_BIT_COUNT], Key target)
{
  vector<vector<Key>> results;

  //Save first element of table before deleting
  Key first = table.begin();
  table.erase(table.begin());
  //Iterate through all values except first with new target being row above
  for (vector<Key> v : subSetSum(table, target - first))
  {    
    results.reserve(1 + v.size());
    results.insert(results.end(), first.begin(), first.end());
    results.insert(results.end(), v.begin(), v.end());    
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

  subSetSum(table, encrypted);

  /*//Get amount of needed columns in subsetsum table
  long columnLength = keyToDec(encrypted);

  vector<int> indices{ 0 };
  int indices_size = indices.size();

  //Init map
  //vector<map<int, vector,int>>
  //vector<pair<int, vector<int>>> subsetSumTable;
  SubSetSumTable subsetSumTable;
  for(int i = 0; i < PW_BIT_COUNT; i++)
  {
    long index_incr = keyToDec(table[i]);
    //Iterate through last row's true values
		for (int j = 0; j < indices_size; j++) {
      //Calculate where true value in row is
      int next = indices[j] + index_incr;
      cout << next << endl;
			if (next <= columnLength) {
        //Update true value				
				indices.push_back(next);
			}
    }    
    indices_size = indices.size();		

    subsetSumTable.push_back(make_pair(index_incr, indices));
    //subsetSumTable[index_incr] = indices;
  }  

  cout << "Number of true values: " << indices_size << endl;
  sort(indices.begin(), indices.end());
  cout << "Largest true value " << indices[indices_size-1] << endl;
  cout << "Column length " << columnLength << endl;*/
  
  //Target (number of columns) subtracted with value of last row
  //Key index = encrypted - subSumTable.begin()->first;
  //Add first element to set
  //results.push_back(subSumTable.begin()->first);

  //vector<Key> current;
  //vector<vector<Key>> results;
  
  //Index: 3
  //Current: {11,4,3}
  //Results: {11,7}, {11,4,3}

  //index = encrypted - last element
  //Add last element to current set

//index = index - current element

  //Iterate through table backwards (skip last element)    
    //if index - current element < 0
      //continue
    //if value on index is true || index == 0
      //Add element to current set
      //index = index - current element
    //if index == 0
      //Add current set to results
      //Clear current, insert last element
      //index = current element (reset)
  

  /*
  Ta Target - lastrowelement för att få index
if (18-10 == 7)

{11,7}
{11,3,4}
{11,7,3,4}

Lägg till 11
Target = 18-11 = 7 
Gå till sista raden som är true på column (index) 7
7-4 = 3, lägg till 4
Gå till index 3
Gå till sista raden som är true på column (index) 3
Lägg till 3

5-(2) = 3
3-(3) = 0

*/

  /*
  int i = 0;
  do
  {
    i++;    
    candenc = KEYsubsetsum(candidate, T);
    //cout << candenc << endl;
    if (candenc == encrypted)
      cout << candidate << endl;

    ++candidate;
  } while (candidate != zero);

  cout << "Iterations: " << i << endl;*/

  auto end = chrono::high_resolution_clock::now();
  cout << "Decryption took "
       << std::chrono::duration_cast<chrono::seconds>(end - begin).count()
       << " seconds." << endl;

  return 0;
}