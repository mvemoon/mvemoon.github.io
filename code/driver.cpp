#include "bag2.cpp"
#include <iostream>
using namespace std;


int main(void)
{
	Bag<int> bag = Bag<int>();
	
	cout<<bag.isEmpty()<<endl;

	for (size_t i=0; i<10; i++)
			bag.add(i);
	
	cout<<bag.isEmpty()<<endl;
	cout<<bag.size()<<endl;
	
	return 0;
}
