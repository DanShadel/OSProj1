
#include <iostream>
#include <vector>
#include <algorithm> //sort
#include <time.h>
#include <stdlib.h>
using namespace std;
int main()
{
	vector<int> test(10,0);
	test.reserve(10);
	for (int i =0; i < 10; i++)
	{
		cout << "index #" << i << ": " << test.at(i) << endl;
	}
	return 0;
}