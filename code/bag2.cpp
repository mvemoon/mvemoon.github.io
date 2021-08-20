//用栈实现bag,关键是迭代器的实现。
//Private : T item, ptr first, counter
//method : add isEmpty size
//嵌套结构或类： 在类声明中声明的结构、类或枚举被称为是嵌套在类中，其作用域为整个类。这种声明不会创建数据对象，而只是指定了可以在类中使用的类型。

#include <cstddef>

template <class T> class Bag
{
public:
		Bag()
		{
			num = 0;
			first = nullptr;
		}
		
		void add(T item)
		{
			Node* old_first = first;
			Node* first = new Node();
			first->item = item;
			first->ptr = old_first;
			num++;
		}

		bool isEmpty()
		{
			return num;
		}

		size_t size()
		{
			return num;
		}

private:
		struct Node
		{
			T item;
			struct Node* ptr;
		};

		size_t num;
		Node* first;
public: 
};
