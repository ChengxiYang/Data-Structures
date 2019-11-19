// vector迭代器.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include <string>

using std::cout;
using std::string;

class myException
{
protected:
	string info;
public:
	myException(string info) { this->info = info; }
	void print() { cout << info << "\n"; }
};

template <class T>
class myVector
{
protected:
	int size; //动态数组的元素个数
	int capacity; //动态数组的容量， capability > size
	T *p_begin; //指向动态数组的起始位置
	void doubleCapacity(); //注意容量倍增方法会改变p_begin
public:
	myVector() { size = 0; capacity = 10; p_begin = new T[10]; }
	myVector(int n);
	~myVector() { if (p_begin) delete p_begin; }
	T * begin() { return p_begin; }
	T * end() { return p_begin + size; }
	int find(T element) const;
	T & operator[](int k);
	void insert(T element); //在动态数组尾部不断加入元素
	void insert(int k, T element);
	bool erase(T element); //删除动态数组中第一个出现的element
	//bool erase(int k); 
	void eraseByIndex(int k);
	//动态数组作为一种容器，相应的查找，加入，删除元素方法以上均已实现
	class iterator
	{
	protected:
		T *position;
	public:
		iterator() { position = NULL; }
		void operator=(T *p) { position = p; }
		T operator*() { return *position; }
		T * operator++(int) { T *p_temp = position; ++position; return p_temp; }
		T * operator++() { return ++position; }
		T * operator--(int) { T *p_temp = position; --position; return p_temp; }
		T * operator--() { return --position; }
		T * operator+(int k) { return position + k; }
		bool operator==(T *p) { if (position == p) return true; else return false; }
		bool operator!=(T *p) { if (position != p) return true; else return false; }
		//以上运算符函数定义了myVector的随机访问迭代器
	};
};

template <class T>
myVector<T>::myVector(int n)
{
	if (n < 1)
		throw myException("Illegal argument!");
	size = 0;
	capacity = n;
	p_begin = new T[n];
}

template <class T>
void myVector<T>::doubleCapacity()
{
	capacity *= 2;
	T *destination = new T[capacity];
	for (int i = 0; i < size; ++i)
		destination[i] = p_begin[i]; //当T为自定义类型时，要定义operator=函数
	delete p_begin;
	p_begin = destination; //一旦doubleCapacity函数被调用，p_begin改变
	return;
}

template <class T>
int myVector<T>::find(T element) const
{ //查找动态数组中首次出现的元素element，查找成功返回其索引，不成功返回-1
	if (size == 0) //p_begin的值不能确定动态数组是否为空
		throw myException("Empty vector!");
	T *current = p_begin;
	for (int i = 0; i < size; ++i)
	{
		if (*current == element)
			return i;
		++current;
	}
	cout << "Element does not exsit!\n";
	return -1;
}

template <class T>
T & myVector<T>::operator[](int k)
{
	if (size == 0)
		throw myException("Empty vector!");
	if (k < 0 || k >= size)
		throw myException("Parameter exceeds boundary!");
	T *current = p_begin + k;
	return *current;
}

template <class T>
void myVector<T>::insert(T element)
{
	int capa = capacity - 1;
	if (size == capa)
		doubleCapacity(); //保证size < capacity
	p_begin[size++] = element; //在容器尾部插入新元素
}

template <class T>
void myVector<T>::insert(int k, T element)
{
	if (k < 0 || k > size)
		throw myException("Parameter exceeds boundary!");
	if (size == 0)
	{
		p_begin[0] = element;
		++size;
		return;
	}
	int capa = capacity - 1;
	if (size == capa)
		doubleCapacity();
	int i = size - 1;
	while (i >= k)
	{
		p_begin[i + 1] = p_begin[i];
		--i;
	}
	p_begin[k] = element;
	++size;
	return;
}

template <class T>
bool myVector<T>::erase(T element)
{
	if (size == 0)
		throw myException("Empty vector!");
	int index = find(element);
	if (index == -1)
	{
		cout << "Unexsited element!\n";
		return false;
	}
	for (int i = index + 1; i < size; ++i)
		p_begin[i - 1] = p_begin[i];
	~p_begin[size - 1]; //可能有bug
	--size;
	return true;
}

template <class T>
void myVector<T>::eraseByIndex(int k)
{
	if (size == 0)
		throw myException("Empty vector!");
	if (k < 0 || k >= size)
		throw myException("Illegal parameter!");
	for (int i = k + 1; i < size; ++i)
		p_begin[i - 1] = p_begin[i];
	~p_begin[size - 1];
	--size;
	return;
}

int main()
{ //以下为测试代码
	try
	{
		myVector<int> vec;
		myVector<int>::iterator iter;
		iter = vec.begin();
		cout << "Before doubling capacity: " << vec.begin() << '\n';
		for (int i = 0; i <= 15; ++i)
			vec.insert(i);
		cout << "After doubling capacity: " << vec.begin() << '\n';
		iter = vec.begin(); //iter在容量倍增被调用后不再指向动态数组起始位置，需要重新定位
		for (; iter != vec.end(); ++iter)
			cout << *iter << ", ";
		cout << "\n";
		cout << vec.find(1) << "\n";
		vec.erase(1);
		vec.eraseByIndex(10);
		iter = vec.begin();
		for (; iter != vec.end(); ++iter)
			cout << *iter << ", ";
		cout << "\n";
		vec.insert(10, 100);
		iter = vec.begin();
		for (; iter != vec.end(); ++iter)
			cout << *iter << ", ";
		cout << "\n";
		vec.erase(100);
		iter = vec.begin();
		for (; iter != vec.end(); ++iter)
			cout << *iter << ", ";
		cout << "\n";
		vec[0] = 101;
		cout << vec[0] << "\n";
		vec[2] = 103;
		vec[5] = 105;
		iter = vec.begin();
		for (; iter != vec.end(); ++iter)
			cout << *iter << ", ";
		cout << "\n";
		int a = vec[1];
		cout << a << "\n";
		vec[20] = 120;
	}
	catch (myException &my_exception)
	{
		my_exception.print();
	}
    return 0;
}

