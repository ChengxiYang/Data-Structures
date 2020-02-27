// heap and priority queue.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <string>

using std::cin;
using std::cout;
using std::string;

const int NINF = -0x3f3f3f3f;

class Exception
{
public:
	Exception(const string &info_) { info = info_; }
	void print_info() const { cout << info << "\n"; }
private:
	string info;
};

template <class T>
class max_heap
{
public:
	max_heap();
	~max_heap();
	void build_max_heap(T *p_array, int length); //transform a array into a max heap
	void push_back(const T &ele);
	int size() const { return heap_size; }
	bool empty() const { return heap_size == 0; }
	T top() const { return p_heap[0]; } //equivalent to priority queue
	void pop();
	void heap_sort(T *p_array, int length); //inplace sort
protected:
	inline void swap_key(int i, int j); //swap key between node i and j
	//void swap_key(int i, int j);
	void max_heapify(int i);
	void increase_key(int i, const T &key);
	void expand_capacity(int capa); //expand capacity when heap_size == capacity
protected:
	int heap_size;
	T *p_heap;
	int capacity;
};

template <class T>
max_heap<T>::max_heap()
{
	heap_size = 0;
	capacity = 0x00000010;
	p_heap = new T[capacity];
}

template <class T>
max_heap<T>::~max_heap()
{
	delete []p_heap;
	p_heap = NULL;
}

template <class T>
void max_heap<T>::expand_capacity(int capa)
{ 
	if (capa < heap_size)
		throw Exception("Heap Overflow!");
	capacity = capa; 
	T *p_new = new T[capacity];
	for (int i = 0; i < heap_size; ++i)
		p_new[i] = p_heap[i];
	delete []p_heap;
	p_heap = p_new;
}

template <class T>
void max_heap<T>::swap_key(int i, int j)
{
	T temp = p_heap[i];
	p_heap[i] = p_heap[j];
	p_heap[j] = temp;
}

template <class T>
void max_heap<T>::max_heapify(int i)
{
	int left = 0, right = 0, largest = 0;
	while (i < heap_size)
	{
		//left = (i << 1) & 1; right = (i + 1) << 1; 
		left = 2 * i + 1; right = 2 * i + 2;
		if (left < heap_size && p_heap[left] > p_heap[i])
			largest = left;
		else
			largest = i;
		if (right < heap_size && p_heap[right] > p_heap[largest])
			largest = right;
		if (i == largest)
			break;
		swap_key(i, largest);
		i = largest;
	}
}

template <class T>
void max_heap<T>::increase_key(int i, const T &key)
{
	if (key < p_heap[i])
		throw Exception("Wrong Argument!");
	p_heap[i] = key;
	int cur = i;
	//int parent = (cur - 1) >> 1;
	int parent = (cur - 1) / 2;
	while (parent >= 0)
	{
		if (p_heap[cur] > p_heap[parent])
		{
			swap_key(cur, parent);
			cur = parent;
			//parent = (cur - 1) >> 1;
			parent = (cur - 1) / 2;
		}
		else break;
	}
}

template <class T>
void max_heap<T>::build_max_heap(T *p_array, int length)
{
	if (p_array == NULL || length < 0)
		throw Exception("Invalid Argument!");
	delete []p_heap;
	p_heap = p_array;
	heap_size = length;
	capacity = length;
	//int bound = (heap_size - 2) >> 1; //(heap_size - 2) / 2
	int bound = (heap_size - 2) / 2;
	for (int i = bound; i >= 0; --i)
		max_heapify(i);
}

template<class T>
void max_heap<T>::heap_sort(T * p_array, int length)
{
	build_max_heap(p_array, length);
	for (int i = length - 1; i > 0; --i)
	{
		swap_key(0, i);
		--heap_size;
		max_heapify(0);
	} //using a temporary heap for sorting inputed array
	//when this for-loop finished, the temporary heap remains only one element,
	//and inputed array gets sorted.
}

template <class T>
void max_heap<T>::pop()
{
	if (heap_size == 0)
		throw Exception("Heap Underflow!");
	swap_key(0, heap_size - 1);
	--heap_size;
	max_heapify(0);
}

template <class T>
void max_heap<T>::push_back(const T &ele)
{
	if (capacity == heap_size)
	{
		capacity = 2 * heap_size;
		expand_capacity(capacity);
	}
	++heap_size;
	p_heap[heap_size - 1] = NINF;
	increase_key(heap_size - 1, ele);
}

int main()
{
	try
	{
		max_heap<int> heap_1, heap_2;
		int nums = 1024;
		int *p_arr = new int[nums];
		for (int i = 0; i < nums; ++i)
		{
			p_arr[i] = rand() % 1024 + 1;
			heap_1.push_back(p_arr[i]);
			cout << p_arr[i] << ", ";
		}
		cout << "\n---------\n";
		cout << "heap_1 size: " << heap_1.size() << "\n";
		for (int i = 0; i < nums; ++i)
		{
			cout << heap_1.top() << ", ";
			heap_1.pop();
		}
		cout << "\n---------\n";
		heap_2.heap_sort(p_arr, nums);
		for (int i = 0; i < nums; ++i)
			cout << p_arr[i] << ", ";
		cout << "\n---------\n";
	}
	catch (Exception &running_error)
	{
		running_error.print_info();
	}
    return 0;
}

