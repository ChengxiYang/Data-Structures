// vector������.cpp : Defines the entry point for the console application.
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
	int size; //��̬�����Ԫ�ظ���
	int capacity; //��̬����������� capability > size
	T *p_begin; //ָ��̬�������ʼλ��
	void doubleCapacity(); //ע����������������ı�p_begin
public:
	myVector() { size = 0; capacity = 10; p_begin = new T[10]; }
	myVector(int n);
	~myVector() { if (p_begin) delete p_begin; }
	T * begin() { return p_begin; }
	T * end() { return p_begin + size; }
	int find(T element) const;
	T & operator[](int k);
	void insert(T element); //�ڶ�̬����β�����ϼ���Ԫ��
	void insert(int k, T element);
	bool erase(T element); //ɾ����̬�����е�һ�����ֵ�element
	//bool erase(int k); 
	void eraseByIndex(int k);
	//��̬������Ϊһ����������Ӧ�Ĳ��ң����룬ɾ��Ԫ�ط������Ͼ���ʵ��
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
		//�������������������myVector��������ʵ�����
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
		destination[i] = p_begin[i]; //��TΪ�Զ�������ʱ��Ҫ����operator=����
	delete p_begin;
	p_begin = destination; //һ��doubleCapacity���������ã�p_begin�ı�
	return;
}

template <class T>
int myVector<T>::find(T element) const
{ //���Ҷ�̬�������״γ��ֵ�Ԫ��element�����ҳɹ����������������ɹ�����-1
	if (size == 0) //p_begin��ֵ����ȷ����̬�����Ƿ�Ϊ��
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
		doubleCapacity(); //��֤size < capacity
	p_begin[size++] = element; //������β��������Ԫ��
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
	~p_begin[size - 1]; //������bug
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
{ //����Ϊ���Դ���
	try
	{
		myVector<int> vec;
		myVector<int>::iterator iter;
		iter = vec.begin();
		cout << "Before doubling capacity: " << vec.begin() << '\n';
		for (int i = 0; i <= 15; ++i)
			vec.insert(i);
		cout << "After doubling capacity: " << vec.begin() << '\n';
		iter = vec.begin(); //iter���������������ú���ָ��̬������ʼλ�ã���Ҫ���¶�λ
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

