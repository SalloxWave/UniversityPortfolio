#include <iostream>
#include "MinPQ.h"
using namespace std;

// Test program
int main( )
{
  int minItem = 10000;
  int maxItem = 99999;
  MinPQ<int> h;
  int i = 37;
  int x;

  cout << "Begin test... " << endl;

  for(i = 37; i != 0; i = (i + 37) % maxItem)
    {
      if(i >= minItem)
	{
	  h.insert(i);
	}
    }
  for(i = minItem; i < maxItem; ++i)
    {
      h.deleteMin(x);
      
      if(x != i)
	cout << "Oops! " << i << endl;
    }

  h = MinPQ<int>();

  for(i = 37; i != 0; i = (i + 37) % maxItem)
    {
      if(i >= minItem)
	{
    h.toss(i);
	}
    }
  for(i = minItem; i < maxItem; ++i)
    {
      h.deleteMin(x);
      
      if(x != i)
	      cout << "Oops! Error after toss" << i << endl;
    }
  
  cout << "End test... no other output is good" << endl;
  return 0;
}
