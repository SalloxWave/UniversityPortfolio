#pragma once

#include <iostream>
#include <iomanip>
#include <map>
#include <utility>
#include <unordered_map>
#include <vector>

#define PW_CHAR_LENGTH 6         // number of characters in password
#define CHAR_BIT_COUNT 5         // number of bits per character
#define ALPHA_SIZE (1 << CHAR_BIT_COUNT)  // size of alphabet (32)
#define PW_BIT_COUNT (CHAR_BIT_COUNT * PW_CHAR_LENGTH)   // number of bits per password

#define ALPHABET "abcdefghijklmnopqrstuvwxyz012345"

using namespace std;

/*

****************************************************************
* An extended precision base ALPHA_SIZE integer consisting of PW_CHAR_LENGTH digits.
* An array packaged in a struct for easy memory management and
* pass-by-value.
****************************************************************/


typedef struct {
	unsigned char digit[PW_CHAR_LENGTH];
} Key;

/****************************************************************
* Initialize k from a character string.
* Example: s = "abcdwxyz"  =>  k = 0 1 2 3 22 23 24 25
****************************************************************/
Key  KEYinit(unsigned char s[]);

/****************************************************************
* Add and return the subset of the integers T[i] that are
* indexed by the bits of k. Do sum mod 2^N.
****************************************************************/
Key  KEYsubsetsum(const Key& k, const Key T[PW_BIT_COUNT], vector<Key>&);

int  KEYbit(const Key& k, int i);        // return the ith bit of k
bool operator==(const Key&, const Key&);
bool operator!=(const Key&, const Key&);
bool operator<(const Key&, const Key&);
bool operator>(const Key&, const Key&);
bool operator<=(const Key&, const Key&);
bool operator>=(const Key&, const Key&);
Key  operator+(const Key&, const Key&); // return a + b mod 2^N
Key  operator++(Key&, int);            // postfix increment
Key&  operator++(Key&);            // prefix increment
Key  operator-(const Key&, const Key&); // return a - b mod 2^N
ostream& operator<<(ostream&, const Key&);