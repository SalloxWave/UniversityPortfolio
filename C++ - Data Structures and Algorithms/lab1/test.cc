#include <iostream>
using namespace std;

class A
{
	public:
		int value;

	A(int x)
	{
		value = x;
	}	
};

int main()
{
	A* adam = new A(50);
	cout << A->value << endl;
}