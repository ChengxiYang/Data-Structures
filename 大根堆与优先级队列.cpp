// 大根堆与优先级队列.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <stack>
#include <vector>
using std::cin;
using std::cout;
using std::iterator;
using std::stack;
using std::vector;

template <class T>
class maxHeap
{
protected:
	vector<T> heap;
	int heapSize;
public:
	maxHeap();
	maxHeap(int heapSize_);
	maxHeap(const maxHeap<T> & theMaxHeap);
	bool swap(int m, int n); //Swap value of node i and node j.
	void levelTraversal() const;
	void preTraversal() const;
	void inTraversal() const;
	void postTraversal() const;
	bool push(const T & theElment);
	T top() const;
	void pop();
	void initialize(const vector<T> & vec); //Convert a complete binary tree into a maxheap.
	template <class T>
	friend void heapSort(vector<T> & vec);
};

template <class T>
maxHeap<T>::maxHeap()
{ //Make sure that a max heap generated has a null element before first node.
  //Using heap[1:heapSize] to store all data of max heap.
	heapSize = 0;
	T nullElement;
	cout << "Enter a null element for node i = 0 : ";
	cin >> nullElement;
	heap.push_back(nullElement);
}

template <class T>
maxHeap<T>::maxHeap(int heapSize_)
{
	heapSize = heapSize_;
	heap.reserve(heapSize + 1); //Does not work?
	//vector<T>::iterator it; //Why this statement can not pass complilation？
	T nullElement; 
	cout << "Enter a null element for node i = 0 : ";
	cin >> nullElement;
	heap.push_back(nullElement);
	for (int i = 1; i <= heapSize; ++i)
	{
		T element;
		cout << "Enter element for node i = " << i << " of maxheap: ";
		cin >> element;
		//heap.insert(heap.begin() + i, element);
		heap.push_back(element);
	}
}

template <class T>
maxHeap<T>::maxHeap(const maxHeap<T> & theMaxHeap)
{
	heapSize = theMaxHeap.heapSize;
	heap.reserve(heapSize + 1);
	for (int i = 0; i <= heapSize; ++i)
		heap.push_back(theMaxHeap.heap[i]);
	//heap[i] = theMaxHeap.heap[i]; //Consider why this statement goes wrong?
	//The null element at start position also was copied.
}

template <class T>
bool maxHeap<T>::swap(int m, int n)
{
	if (m < 1 || m > heapSize || n < 1 || n > heapSize)
		return false;
	T temp1 = heap[m];
	T temp2 = heap[n];
	heap.erase(heap.begin() + m);
	heap.insert(heap.begin() + m, temp2);
	heap.erase(heap.begin() + n);
	heap.insert(heap.begin() + n, temp1);
	return true;
}

template <class T>
void maxHeap<T>::levelTraversal() const
{
	for (int i = 1; i <= heapSize; ++i)
		cout << heap[i] << ", ";
	cout << '\n';
}

template <class T>
void maxHeap<T>::preTraversal() const
{
	if (heap.empty())
	{
		cout << "The maxheap is empty!" << '\n';
		return;
	}
	stack<int> nodeStack;
	int curNode = 1;
	int lastNode;
	while (curNode <= heapSize || !nodeStack.empty())
	{
		while (curNode <= heapSize)
		{
			cout << heap[curNode] << ", ";
			nodeStack.push(curNode);
			curNode *= 2;
		}
		lastNode = nodeStack.top();
		nodeStack.pop();
		curNode = lastNode * 2 + 1;
	}
	cout << '\n';
	return;
}

template <class T>
void maxHeap<T>::inTraversal() const
{
	if (heap.empty())
	{
		cout << "The maxheap is empty!" << '\n';
		return;
	}
	stack<int> nodeStack;
	int curNode = 1;
	int lastNode;
	while (curNode <= heapSize || !nodeStack.empty())
	{
		while (curNode <= heapSize)
		{
			nodeStack.push(curNode);
			curNode *= 2;
		}
		lastNode = nodeStack.top();
		nodeStack.pop();
		cout << heap[lastNode] << ", ";
		curNode = lastNode * 2 + 1;
	}
	cout << '\n';
	return;
}

template <class T>
void maxHeap<T>::postTraversal() const
{
	stack<int> nodeStack;
	int curNode = 1;
	int lastNode = 0; //Using lastNode to denote node just visited.
	while (curNode <= heapSize)
	{
		nodeStack.push(curNode);
		curNode *= 2;
	}
	while (!nodeStack.empty())
	{
		curNode = nodeStack.top();
		nodeStack.pop();
		if (2 * curNode + 1 > heapSize || 2 * curNode + 1 == lastNode)
		{
			cout << heap[curNode] << ", ";
			lastNode = curNode;
		}
		else
		{
			nodeStack.push(curNode);
			curNode = 2 * curNode + 1;
			while (curNode <= heapSize)
			{
				nodeStack.push(curNode);
				curNode *= 2;
			}
		}
	}
	cout << '\n';
	return;
}

template <class T>
bool maxHeap<T>::push(const T & theElement)
{
	heap.push_back(theElement);
	int curNode = ++heapSize;
	while (curNode > 1)
	{
		int parNode = curNode / 2;
		if (heap[parNode] > heap[curNode])
			return true;
		else
		{
			swap(parNode, curNode);
			curNode = parNode;
		}
	}
	if (curNode == 1)
		return true;
}

template <class T>
T maxHeap<T>::top() const
{
	if (heapSize == 0)
	{
		cout << "The maxheap contains no node." << '\n';
		return heap[0];
	}
	return heap[1];
}

template <class T>
void maxHeap<T>::pop()
{ //The node to be poped out is restricted to node i = 1.
	if (heapSize == 0)
		return;
	//Move last node to first node.
	if (heapSize == 1)
	{ //Boundary condition should be treated separately.
		heap.pop_back();
		heapSize = 0;
		cout << "The maxheap contains no node." << '\n';
		return;
	}
	T temp = heap[heapSize--];
	heap.pop_back();
	heap.erase(heap.begin() + 1);
	heap.insert(heap.begin() + 1, temp);
	int curNode = 1;
	int childNode = 2;
	while (childNode <= heapSize)
	{
		if (childNode < heapSize && heap[childNode] < heap[childNode + 1])
			++childNode;
		if (temp < heap[childNode])
		{
			swap(curNode, childNode);
			curNode = childNode;
			childNode *= 2;
		}
		else
			break;
	}
}

template <class T>
void maxHeap<T>::initialize(const vector<T> & vec)
{ //Container vec corresponds to a complete binary tree.
	heapSize = vec.size() - 1;
	for (int i = 1; i <= heapSize; ++i)
		heap.push_back(vec[i]);
	int curNode = heapSize / 2;
	for (; curNode >= 1; --curNode)
	{
		int rootNode = curNode;
		int childNode = rootNode * 2;
		while (childNode <= heapSize)
		{
			if (childNode < heapSize && heap[childNode] < heap[childNode + 1])
				++childNode;
			if (heap[rootNode] > heap[childNode])
				break;
			else
			{
				swap(rootNode, childNode);
				rootNode = childNode;
				childNode *= 2;
			}
		}
	}//When while-loop finished, the sub-tree with root of curNode get adjusted to a sub-maxheap.
}

template <class T>
void heapSort(vector<T> & vec)
{
	int items = vec.size() - 1;
	maxHeap<T> midHeap;
	midHeap.initialize(vec);
	vec.clear();
	vec.push_back(midHeap.heap[0]);
	for (int i = 1; i <= items; ++i)
	{
		T temp = midHeap.top();
		midHeap.pop();
		vec.push_back(temp);
	}
	return;
}

int main()
{ //Following is test code, and it proves the correctness of programme.
	/*int nodes;
	cout << "Enter number of nodes: ";
	cin >> nodes;
	maxHeap<int> tree1(nodes);
	tree1.levelTraversal();
	for (int i = 1; i <= nodes; ++i)
	{
		tree1.pop();
		tree1.levelTraversal();
	}
	tree1.preTraversal();
	tree1.inTraversal();
	tree1.postTraversal();
	maxHeap<char> tree2;
	for (int i = 1; i <= nodes; ++i)
	{
		char temp;
		cout << "Enter a value for maxheap: ";
		cin >> temp;
		tree2.push(temp);
	}
	tree2.levelTraversal();
	tree2.preTraversal();
	tree2.inTraversal();
	tree2.postTraversal();
	maxHeap<int> tree3(tree1);
	maxHeap<int> tree4;
	vector<int> vec;
	int node_num;
	cout << "Enter number of nodes: ";
	cin >> node_num;
	for (int i = 0; i <= node_num; ++i)
		vec.push_back(i);
	tree4.initialize(vec);
	tree4.levelTraversal();
	for (int i = 1; i <= node_num; ++i)
	{
		tree4.pop();
		tree4.levelTraversal();
	}
	vector<int> vec;
	vec.push_back(0);
	int nums;
	cout << "Enter number of integers to be sorted: ";
	cin >> nums;
	for (int i = 1; i <= nums; ++i)
	{
		int temp;
		//cout << "Enter an integer: ";
		temp = rand() % 1000;
		vec.push_back(temp);
	}
	for (int i = 1; i <= nums; ++i)
		cout << vec[i] << ", ";
	cout << "\n------\n";
	heapSort(vec);
	for (int j = 1; j <= nums; ++j)
		cout << vec[j] << ", ";
	cout << "\n------\n";*/
    return 0;
}

