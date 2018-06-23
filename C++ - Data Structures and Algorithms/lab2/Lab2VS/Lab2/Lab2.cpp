// Lab2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include <chrono>
#include "Key.h"
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <unordered_map>
#include <algorithm>

using namespace std;

//Uppgift, vilka kandidater användes för att få summan till det krypterade lösenordet?
Key encryptPassword(unsigned char password[], Key table[PW_BIT_COUNT], vector<Key>& keys) {
	Key passw_num = KEYinit(password);
	Key encrypted_num = KEYsubsetsum(passw_num, table, keys);
	return encrypted_num;
}

void populateTable(Key table[PW_BIT_COUNT], string fileName) {
	fstream myfile;
	unsigned char buffer[PW_CHAR_LENGTH + 1];
	myfile.open(fileName);
	if (myfile.is_open()) {
		string line;
		for (int i{ 0 }; i < PW_BIT_COUNT; ++i)
		{
			getline(myfile, line);
			memcpy(buffer, line.c_str(), sizeof(buffer)); // <-
			table[i] = KEYinit(buffer);
		}
	}
	else {
		cout << "fail" << endl;
	}
}


//Gets all subsets of input vector, store them in a map with their respective sum as a key.
multimap<Key, vector<Key>> getAllSubsets(vector<Key>& numbers) {
	//Initializing variables
	multimap<Key, vector<Key>> lookUpTable;
	vector< vector<Key> > subsets;
	vector<Key> empty;
	subsets.push_back(empty);

	for (int i = 0; i < numbers.size(); i++)
	{
		//Create tmp vector of current subsets (first iteration {{}})
		vector< vector<Key> > subsetTemp = subsets;

		//Loop tmp vector and push back numbers[i] in each internal vector. 
		//If numbers is {4,5,1,9}, subsetTemp would be {{4}} in the first iteration.
		for (int j = 0; j < subsetTemp.size(); j++) {
			subsetTemp[j].push_back(numbers[i]);
		}

		//Loop the tmp vector, for each internal vector push it back to subsets.
		//Continuing the previous example, after the first iteration subsets would be
		//{{},{4}}, i am basically just adding subets + tmp
		for (int j = 0; j < subsetTemp.size(); j++) {
			subsets.push_back(subsetTemp[j]);
		}

		//{{},{4}} is then copied to the tmp vector again.
		//After the loop tmp vector is now {{5},{4,5}}
		//{5} and {4,5} is pushed back in to subsets.
		//subsets is now {{},{4},{5},{4,5}}
		//tmp copies subsets, inserts the next numbers[i] in to each
		//internal vector: {{1},{4,1},{5,1},{4,5,1}}
		//Each internal vector in tmp is added to subsets:
		//{{},{4},{5},{4,5},{1},{4,1},{5,1},{4,5,1}}
		//....
	}

	//Sums up every vector in subsets and add the sum
	//as a key to the map with the vector as value.
	for (vector<Key> v : subsets) {
		Key sum{ 0 };
		for (Key k : v) {
			sum = sum + k;
		}
		lookUpTable.insert(make_pair(sum, v));
	}

	return lookUpTable;
}

vector<vector<Key>> meetInTheMiddle(multimap<Key, vector<Key>> firstSubset, multimap<Key, vector<Key>> secondSubset, Key encrypted) {
	vector<vector<Key>> solutions;

	vector<Key> second;

	for (auto it = secondSubset.begin(); it != secondSubset.end(); ++it) {
		second.push_back(it->first);
	}

	//Iterates first subset
	for (multimap<Key, vector<Key>> ::iterator it1 = firstSubset.begin(); it1 != firstSubset.end(); it1++) {

		//Calculate difference between encrypted and sum
		Key diff = encrypted - it1->first;

		//If the difference is found in the second subset we know that a solution exists.
		if (binary_search(second.begin(), second.end(), diff)) {
			//cout << endl << diff + it1->first << endl << endl;
			//Find every pair in first subset that corresponds to the current sum.
			pair<multimap<Key, vector<Key>> ::iterator, multimap<Key, vector<Key>> ::iterator> range1 = firstSubset.equal_range(it1->first);

			//Find every pair in first second subset that corresponds to the difference.
			pair<multimap<Key, vector<Key>> ::iterator, multimap<Key, vector<Key>> ::iterator> range2 = secondSubset.equal_range(diff);

			//The vectors of these two combined will yield one permutation of the solution.
			vector<Key> solution;

			for (multimap<Key, vector<Key>>::iterator it = range1.first; it != range1.second; ++it)
			{
				//cout << endl;
				for (Key k : it->second) {
					solution.push_back(k);
				}
			}

			for (multimap<Key, vector<Key>>::iterator it = range2.first; it != range2.second; ++it)
			{
				for (Key k : it->second) {
					solution.push_back(k);
				}
			}

			solutions.push_back(solution);
		}
	}

	return solutions;
}


int main()
{
	string fileName = "rand6.txt";
	unsigned char password[PW_CHAR_LENGTH + 1] = "helloo";
	Key table[PW_BIT_COUNT];
	populateTable(table, fileName);
	vector<Key> used;
	Key encrypted = encryptPassword(password, table, used);

	auto begin = chrono::high_resolution_clock::now();

	cout << encrypted << endl;

	vector<Key> firstHalf(table, table + PW_BIT_COUNT / 2);
	vector<Key> secondHalf(table + PW_BIT_COUNT / 2, table + PW_BIT_COUNT);

	multimap<Key, vector<Key>> subsetSumFirst = getAllSubsets(firstHalf);
	multimap<Key, vector<Key>> subsetSumSecond = getAllSubsets(secondHalf);

	vector<vector<Key>> solutions = meetInTheMiddle(subsetSumFirst, subsetSumSecond, encrypted);

	for (vector<Key> solution : solutions) {
		cout << endl;
		for (Key k : solution) {
			cout << k << endl;
		}
	}

	auto end = chrono::high_resolution_clock::now();
	cout << "Decryption took "
		<< std::chrono::duration_cast<chrono::seconds>(end - begin).count()
		<< " seconds." << endl;

	while (true) {}

    return 0;
}