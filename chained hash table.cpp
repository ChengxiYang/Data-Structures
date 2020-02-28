// chained hash table.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <string>

//#define _TEST_1_
#define _TEST_2_

using std::cout;
using std::string;

const int load_factor = 4;
const int max_key = 0x00001000;

class Exception
{
public:
	Exception(const string &info_ref) { info = info_ref; }
	void print_info() const { cout << info << "\n"; }
private:
	string info;
};

template <class T>
struct element
{ //all data abstracted as value-key pair form
	T value;
	int key; //assume keys of all elements to be non-negative integers
};

template <class T>
struct hash_node
{
	element<T> data;
	hash_node<T> *p_next;
	hash_node() { p_next = NULL; }
};

template <class T>
class hash_table
{
public:
	hash_table(int slot_num = 0x00000010);
	~hash_table();
	void insert(const element<T> &ele_ref);
	element<T> * find(int key) const; //assume keys of all elements in hash table are distinct
	void erase(int key);
protected:
	void expand_table(); //expand hash table when load factor gets much more than 1
	int division_hash(int key) const { return key % slot_num; }
	//int multiplication_hash(int key) {}
protected:
	int slot_num; //number of slots in hash table
	int elem_num; //number of total elements hash table stored
	hash_node<T> **p_list;
};

template <class T>
void hash_table<T>::expand_table()
{
	int old_num = slot_num;
	slot_num = load_factor * slot_num;
	hash_node<T> **p_new = new hash_node<T> *[slot_num];
	for (int k = 0; k < slot_num; ++k)
		p_new[k] = NULL;
	for (int k = 0; k < old_num; ++k)
	{
		if (p_list[k] != NULL)
		{
			hash_node<T> *p_delete = p_list[k];
			hash_node<T> *p_to_delete = NULL;
			while (p_delete != NULL)
			{
				hash_node<T> *p_node = new hash_node<T>;
				p_node->data.key = p_delete->data.key;
				p_node->data.value = p_delete->data.value;
				int new_hash_value = division_hash(p_node->data.key);
				if (p_new[new_hash_value] == NULL)
					p_new[new_hash_value] = p_node;
				else
				{ //using chained method to handle collision
					p_node->p_next = p_new[new_hash_value];
					p_new[new_hash_value] = p_node;
				}
				p_to_delete = p_delete->p_next;
				delete p_delete;
				p_delete = p_to_delete;
			}
		}
		p_list[k] = NULL;
	}
	delete []p_list;
	p_list = p_new;
}

template <class T>
hash_table<T>::hash_table(int slot_num = 0x00000010)
{
	this->slot_num = slot_num;
	p_list = new hash_node<T> *[slot_num];
	for (int k = 0; k < slot_num; ++k)
		p_list[k] = NULL;
	elem_num = 0;
}

template <class T>
hash_table<T>::~hash_table()
{
	for (int k = 0; k < slot_num; ++k)
	{
		hash_node<T> *p_delete = p_list[k];
		hash_node<T> *p_to_delete = NULL;
		while (p_delete != NULL)
		{
			p_to_delete = p_delete->p_next;
			delete p_delete;
			p_delete = p_to_delete;
		}
		p_list[k] = NULL;
	}
	delete []p_list;
}

template<class T>
element<T> * hash_table<T>::find(int key) const
{
	if (key < 0)
		throw Exception("Wrong Argument!");
	int hash_value = this->division_hash(key);
	hash_node<T> *p_node = p_list[hash_value];
	while (p_node != NULL)
	{
		if (p_node->data.key == key)
			return &(p_node->data);
		else
			p_node = p_node->p_next;
	}
	//cout << "Unexisted Element!\n";
	return NULL;
}

template <class T>
void hash_table<T>::insert(const element<T> &ele_ref)
{
	if ((elem_num / slot_num) > load_factor)
		expand_table();
	hash_node<T> *p_node = new hash_node<T>;
	p_node->data.key = ele_ref.key; p_node->data.value = ele_ref.value;
	int hash_value = division_hash(ele_ref.key);
	if (p_list[hash_value] == NULL)
		p_list[hash_value] = p_node;
	else
	{
		p_node->p_next = p_list[hash_value];
		p_list[hash_value] = p_node;
	}
	++elem_num;
}

template <class T>
void hash_table<T>::erase(int key)
{
	if (key < 0)
		throw Exception("Wrong Argument!");
	int hash_value = division_hash(key);
	hash_node<T> *p_node = p_list[hash_value];
	hash_node<T> *p_prev = NULL;
	while (p_node != NULL)
	{
		if (p_node->data.key == key)
			break;
		else
		{
			p_prev = p_node;
			p_node = p_node->p_next;
		}
	}
	if (p_node == NULL)
		cout << "Unexisted Element!\n";
	else
	{
		if (p_prev = NULL)
		{ //boundary situation: the node to be deleted is first node
			p_list[hash_value] = p_node->p_next;
			delete p_node;
			p_node = NULL;
		}
		else
		{
			p_prev->p_next = p_node->p_next;
			delete p_node;
			p_node = NULL;
		}
		--elem_num;
	}
}

char * generate_random_string()
{
	int lower_bound = int('a');
	int upper_bound = int('z');
	int gap = upper_bound - lower_bound;
	int length = rand() % 32 + 1;
	char *p_str = new char[length + 1];
	p_str[length] = ' '; 
	for (int i = 0; i < length; ++i)
		p_str[i] = char(rand() % (gap + 1)  + lower_bound);
	return p_str;
}

int main()
{
	typedef element<char> ele_char;
	typedef element<string> ele_str;
	int size = 0x00000040;
	try
	{
#ifdef _TEST_1_
		hash_table<char> table_1;
		int base = int('0');
		for (int k = 0; k < size; ++k)
		{
			ele_char e; e.key = (rand() % max_key + 0); e.value = char(base + k);
			table_1.insert(e);
		}
		for (int k = 0; k < max_key; ++k)
			if (table_1.find(k) != NULL)
				cout << "(" << k << ", " << table_1.find(k)->value << ")";
		cout << "\n---------\n";
#endif

#ifdef _TEST_2_
		hash_table<string> table_2(32);
		for (int k = 0; k < size; ++k)
		{
			ele_str e; e.key = (rand() % max_key + 0); e.value = generate_random_string();
			table_2.insert(e);
		}
		for (int k = 0; k < max_key; ++k)
			if (table_2.find(k) != NULL)
				cout << "(" << k << ", " << table_2.find(k)->value << ")";
		cout << "\n---------\n";
#endif
	}
	catch (Exception &error_ref)
	{
		error_ref.print_info();
	}
    return 0;
}

