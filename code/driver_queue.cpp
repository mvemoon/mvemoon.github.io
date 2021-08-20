#include "Queue.cpp"
#include <iostream>

using namespace std;

int main(void)
{
		Queue<char> q = Queue<char>();
		q.enqueue('x');
		cout<<q.size()<<endl;
		cout<<q.peek()<<endl;
		q.dequeue();
		cout<<q.isEmpty()<<endl;

		return 0;
}
