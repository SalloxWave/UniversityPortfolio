#include <map>
#include <iostream>
#include <vector>
using namespace std;

int main()
{
	map<string, vector<int>> my_map;
	for(int i{0}; i < 10; ++i)
	{
		string v_name = to_string(i+1);
		for (int i {0}; i < 10; ++i)
		{
			vector<int> v;
			v.insert(i);
			my_map.insert(pair<string, vector<int>>(v_name, v));
		}
	}
	return 0;
}