#include "Key.h"

// convert from char to integer 0-31
Key KEYinit(unsigned char s[]) {
	Key k;
	for (int i{ 0 }; i < PW_CHAR_LENGTH; ++i)
		for (int j{ 0 }; j < ALPHA_SIZE; ++j)
			if (s[i] == ALPHABET[j])
				k.digit[i] = j;
	return k;
}

// compute sum of subset of T[i] for which ith bit of k is 1

//Arg1. k is the candidate key. Initially {0,0,0,0,0}.
//Arg2. T[PW_BIT_COUNT] contains the input table:
//
// 13157
// 60123
// 78123
// 12345
//
// etc ...
Key KEYsubsetsum(const Key& k, const Key T[PW_BIT_COUNT], vector<Key>& subsets) {
	Key sum = { { 0 } };
	for (int i{ 0 }; i < PW_BIT_COUNT; ++i)
		//Add sum from table row which has 1
		if (KEYbit(k, i)) {
			sum = sum + T[i];
			subsets.push_back(T[i]);
			cout << setw(2) << i << " "; // for debugging
			cout << T[i] << " <- " << endl;        // for debugging
		}
		else {
			cout << setw(2) << i << " "; // for debugging
			cout << T[i] << endl;         // for debugging
		}
	return sum;
}

// return the ith bit of Key k
int KEYbit(const Key& k, int i) {
	return (k.digit[i / CHAR_BIT_COUNT] >> (CHAR_BIT_COUNT - 1 - i % CHAR_BIT_COUNT)) & 1;
}

bool operator==(const Key& k1, const Key& k2)
{
	for (int i = 0; i < PW_CHAR_LENGTH; i++) {
		if (k1.digit[i] != k2.digit[i])
			return false;
	}
	return true;
}

bool operator!=(const Key& k1, const Key& k2)
{
	return !(k1 == k2);
}

bool operator<(const Key& k1, const Key& k2)
{
	int i;
	for (i = 0; i < PW_CHAR_LENGTH; i++) {
		if (k1.digit[i] != k2.digit[i])
		{
			if (k1.digit[i] < k2.digit[i])
				return true;
			return false;
		}
	}
	return false;
}

bool operator>(const Key& k1, const Key& k2)
{
	return k2 < k1;
}

bool operator<=(const Key& k1, const Key& k2)
{
	return !(k1 > k2);
}

bool operator>=(const Key& k1, const Key& k2)
{
	return !(k1 < k2);
}

// k = lhs + rhs mod 2^N
Key operator+(const Key& lhs, const Key& rhs)
{
	Key c = { { 0 } };
	int carry{ 0 };
	for (int i{ PW_CHAR_LENGTH - 1 }; i >= 0; --i) {
		c.digit[i] = (lhs.digit[i] + rhs.digit[i] + carry) % ALPHA_SIZE;
		carry = (lhs.digit[i] + rhs.digit[i] + carry) >= ALPHA_SIZE;
	}
	return c;
}

Key& operator++(Key& k)
{
	int i{ PW_CHAR_LENGTH - 1 };
	unsigned char one{ 1 };
	unsigned char dig{ 0 };
	int carry;
	dig = (k.digit[i] + one) % ALPHA_SIZE;
	carry = (k.digit[i] + one) >= ALPHA_SIZE;
	k.digit[i] = dig;
	for (i = PW_CHAR_LENGTH - 2; i >= 0; --i) {
		if (!carry)
			break;
		dig = (k.digit[i] + carry) % ALPHA_SIZE;
		carry = (k.digit[i] + carry) >= ALPHA_SIZE;
		k.digit[i] = dig;
	}
	return k;
}

Key operator++(Key& k, int)
{
	Key temp = k;
	++k;
	return temp;
}

// k = lhs - rhs mod 2^N
Key operator-(const Key& lhs, const Key& rhs)
{
	Key c = { { 0 } };
	int carry{ 0 };

	// if lhs >= rhs the following will clearly work
	// if lhs < rhs we will have carry == -1 after the for loop
	// but this is ok since we are working mod 2^N
	for (int i{ PW_CHAR_LENGTH - 1 }; i >= 0; --i) {
		int t = (int(lhs.digit[i]) - int(rhs.digit[i]) + carry);
		if (t < 0) {
			carry = -1;
			c.digit[i] = (unsigned char)(t + ALPHA_SIZE) % ALPHA_SIZE;
		}
		else {
			carry = 0;
			c.digit[i] = (unsigned char)(t) % ALPHA_SIZE;
		}
	}

	return c;
}

ostream& operator<<(ostream& os, const Key& k)
{
	for (auto i : k.digit)
		os << ALPHABET[i];
	os << "  ";
	for (int i : k.digit)
		os << setw(2) << i << " ";
	os << "  ";
	for (int i{ 0 }; i < PW_BIT_COUNT; i++)
		os << KEYbit(k, i);

	return os;
}
