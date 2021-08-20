template <class T> class Bag
{
public:
	Bag(){}

	void add(T item)
	{
			Node* oldFirst = first;
			first = new Node();
			first->item = item;
			first->next = oldFirst;
	}
		
private:
	struct Node
	{
			T item;
			Node* next = nullptr;
	};

	Node *first = nullptr;


public:
	class Iterator
	{
			friend class Bag;//将bag列为友元类
	public :
			Iterator(){}

			//重载
			//const 成员函数，可以访问类中变量，但不能修改值
			bool operator == (const Iterator &iter)const
			{
					return current == iter.current;
			}

			bool operator !=(const Iterator &iter)const
			{
					return current !=iter.current;
			}

			T& operator *()const
			{
					return current->item;
			}

			Iterator operator ++(int) const
			{
					Iterator temp = *this;
					current = current->next;
					return temp;
			}

			Iterator& operator ++()
			{
					current = current->next;
					return *this;
			}

			bool atEnd() const
			{
					return current == nullptr;
			}

	protected:
			Iterator(Node* p):current(p){}
			
			Node *current = nullptr;
	};


	Iterator begin()
	{
			return Iterator(first);
	}

	Iterator end()
	{
			return Iterator(nullptr);
	}


};


