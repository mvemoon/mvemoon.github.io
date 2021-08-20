/*
 *Implementaiton of the Queue with the single linkedList
 *FIFO first in, first out
 * */

#include <cstddef>
template <class T> class Queue
{
public:
	Queue()
	{
		first = nullptr;
		last = nullptr;
		num = 0;
	}

	bool isEmpty()
	{
		return num == 0;
	}

	size_t size()
	{
		return num;
	}

	T peek()
	{
		/*Ruturns the item least added
		 *
		 * @return the item least added to the queue
		 * @throw  throw expception when the queue is empty
		 * */

		//TODO
		return first->item;
	}

	void enqueue(T item)
	{
		/**
		 *Adds the item
		 *@param item : the item to be added
		 */
		last = new Node();
		last->next = nullptr;
		last->item = item;
		if(num == 0) first = last;
		num++;
	}

	T dequeue()
	{
		/*
		 *Removes the first item
		 *
		 * @returns the first item
		 * @throws  when the queue is empty
		 **/

		//TODO
		T out = first->item;
		first = first->next;
		num--;
		return out;
	}

private:
	struct Node
	{
		T item;
		struct Node* next;
	};

	Node* first;
	Node* last;
	size_t num;
};
