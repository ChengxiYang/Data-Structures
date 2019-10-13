//cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include <queue>
#include <stack>
using std::ostream;
using std::freopen;
using std::cin;
using std::cout;
using std::queue;
using std::stack;

template <typename K, typename V>
struct pair
{
public:
	K key; //Variable key should be constant.
	V value;
public:
	pair() {}
	pair(K theKey, V theValue) { key = theKey; value = theValue; }
	pair(const pair<K, V> & thePair) { key = thePair.key; value = thePair.value; }
	void operator=(const pair<K, V> & thePair)
	{
		key = thePair.key; value = thePair.value;
	}
	template <typename K, typename V>
	friend ostream & operator<<(ostream & out, const pair<K, V> &thePair);
};

template <typename K, typename V>
ostream & operator<<(ostream & out, const pair<K, V> &thePair)
{
	out << "(" << thePair.key << ", " << thePair.value << ")";
	return out;
}

template <typename T>
struct binaryTreeNode
{
public:
	T element;
	binaryTreeNode<T> *left;
	binaryTreeNode<T> *right;
public:
	binaryTreeNode()
	{
		left = right = NULL;
	}
	binaryTreeNode(const T & theElement)
	{
		//element(theElement);
		element = theElement;
		left = right = NULL;
	}
	binaryTreeNode(const T & theElement, const binaryTreeNode<T> *pLeft,
		const binaryTreeNode<T> *pRight)
	{
		//element(theElement);
		element = theElement;
		left = pLeft;
		right = pRight;
	}
	~binaryTreeNode() { left = NULL; right = NULL; }
};

template <typename T>
class binaryTree
{
protected:
	binaryTreeNode<T> *root;
	int numOfNode;
public:
	binaryTree();
	binaryTree(binaryTreeNode<T> *pNode, int num);
	~binaryTree();
	void visitNode(binaryTreeNode<T> *pNode) const;
	void preOrder(binaryTreeNode<T> *pNode) const;
	void inOrder(binaryTreeNode<T> *pNode) const;
	void postOrder(binaryTreeNode<T> *pNode) const;
	void levelOrder(binaryTreeNode<T> *pNode) const;
	int height(binaryTreeNode<T> *pNode) const;
	void preTraversal(binaryTreeNode<T> *pNode) const;
	void inTraversal(binaryTreeNode<T> *pNode) const;
	void postTraversal(binaryTreeNode<T> *pNode) const;
	binaryTreeNode<T> * find(T theElement) const;
};

template <typename K, typename V>
class binarySearchTree : public binaryTree<pair<K, V> >
{
public:
	binarySearchTree() { root = NULL; numOfNode = 0; }
	binarySearchTree(binaryTreeNode<pair<K, V> > *p, int n)
	{ root = p; numOfNode = n; }
	pair<K, V> * find(K theKey) const;
	void insert(const pair<K, V> & thePair);
	bool erase(const K & theKey); //Delete node of keyword equal to theKey.
};

template <typename T>
binaryTree<T>::binaryTree()
{
	root = NULL;
	numOfNode = 0;
}

template <typename T>
binaryTree<T>::binaryTree(binaryTreeNode<T> *pNode, int num)
{
	root = pNode;
	numOfNode = num;
}

template <typename T>
binaryTree<T>::~binaryTree()
{
	cout << "Destructor of class binaTree has been called!\n";
	queue<binaryTreeNode<T> *> nodeQueue;
	nodeQueue.push(root);
	binaryTreeNode<T> *pNode = NULL;
	while (!nodeQueue.empty())
	{
		pNode = nodeQueue.front();
		nodeQueue.pop();
		if (pNode->left != NULL)
			nodeQueue.push(pNode->left);
		if (pNode->right != NULL)
			nodeQueue.push(pNode->right);
		delete pNode;
	}
}

template <typename T>
void binaryTree<T>::visitNode(binaryTreeNode<T> *pNode) const
{
	if (pNode == NULL)
		return;
	cout << pNode->element << ", ";
}

template <typename T>
void binaryTree<T>::preOrder(binaryTreeNode<T> *pNode) const
{
	if (pNode == NULL)
		return;
	else
	{
		visitNode(pNode);
		preOrder(pNode->left);
		preOrder(pNode->right);
	}
}

template <typename T>
void binaryTree<T>::inOrder(binaryTreeNode<T> *pNode) const
{
	if (pNode == NULL)
		return;
	else
	{
		inOrder(pNode->left);
		visitNode(pNode);
		inOrder(pNode->right);
	}
}

template <typename T>
void binaryTree<T>::postOrder(binaryTreeNode<T> *pNode) const
{
	if (pNode == NULL)
		return;
	else
	{
		postOrder(pNode->left);
		postOrder(pNode->right);
		visitNode(pNode);
	}
}

template <typename T>
void binaryTree<T>::levelOrder(binaryTreeNode<T> *pNode) const
{
	if (pNode == NULL)
		return;
	queue<binaryTreeNode<T> *> nodeQueue;
	binaryTreeNode<T> *p = pNode;
	while (p != NULL)
	{
		visitNode(p);
		if (p->left != NULL)
			nodeQueue.push(p->left);
		if (p->right != NULL)
			nodeQueue.push(p->right);
		if (nodeQueue.empty())
			return;
		p = nodeQueue.front();
		nodeQueue.pop();
	}
}

template <typename T>
int binaryTree<T>::height(binaryTreeNode<T> *pNode) const
{
	//int h;
	if (pNode == NULL)
		return 0;
	else
	{
		int hl = height(pNode->left);
		int hr = height(pNode->right);
		if (hl > hr)
			//h = hl + 1;
			return ++hl;
		else
			//h = hr + 1;
			return ++hr;
		//return h;
	}
}

template <typename T>
void binaryTree<T>::preTraversal(binaryTreeNode<T> *pNode) const
{
	if (pNode == NULL)
		return;
	stack<binaryTreeNode<T> *> nodeStack;
	binaryTreeNode<T> *topNode;
	binaryTreeNode<T> *curNode = pNode;
	while (curNode != NULL || !nodeStack.empty())
	{
		while (curNode != NULL)
		{
			visitNode(curNode);
			nodeStack.push(curNode);
			curNode = curNode->left;
		}
		topNode = nodeStack.top();
		nodeStack.pop();
		curNode = topNode->right;
	}
	return;
}

template <typename T>
void binaryTree<T>::inTraversal(binaryTreeNode<T> *pNode) const
{
	if (pNode == NULL)
		return;
	stack<binaryTreeNode<T> *> nodeStack;
	binaryTreeNode<T> *topNode;
	binaryTreeNode<T> *curNode = pNode;
	while (curNode != NULL || !nodeStack.empty())
	{
		while (curNode != NULL)
		{
			nodeStack.push(curNode);
			curNode = curNode->left;
		}
		topNode = nodeStack.top();
		nodeStack.pop();
		visitNode(topNode);
		curNode = topNode->right;
	}
}

template <typename T>
void binaryTree<T>::postTraversal(binaryTreeNode<T> *pNode) const
{
	if (pNode == NULL)
		return;
	stack<binaryTreeNode<T> *> nodeStack;
	binaryTreeNode<T> *lastNode = NULL;	//The node has visited most recently.
	binaryTreeNode<T> *curNode = pNode;
	while (curNode != NULL)
	{
		nodeStack.push(curNode);
		curNode = curNode->left;
	}
	while (!nodeStack.empty())
	{
		curNode = nodeStack.top();
		nodeStack.pop();
		if (curNode->right == NULL || curNode->right == lastNode)
		{ //The condition that a node can be visited.
			visitNode(curNode);
			lastNode = curNode;
		}
		else
		{
			nodeStack.push(curNode);
			curNode = curNode->right;
			while (curNode != NULL)
			{
				nodeStack.push(curNode);
				curNode = curNode->left;
			}
		}
	}
	return;
}

template <typename T>
binaryTreeNode<T> * binaryTree<T>::find(T theElemnt) const
{
	queue<binaryTreeNode<T> *> nodeQueue;
	nodeQueue.push(root);
	binaryTreeNode<T> *pNode = NULL;
	while (!nodeQueue.empty())
	{
		pNode = nodeQueue.front();
		if (pNode->element == theElemnt)
		{
			cout << "Node successfully finded!\n";
			return pNode;
		}
		else
		{
			nodeQueue.pop();
			if (pNode->left != NULL)
				nodeQueue.push(pNode->left);
			if (pNode->right != NULL)
				nodeQueue.push(pNode->right);
		}
	}
	cout << "Failure to find node!\n";
	return NULL;
} 

template <typename K, typename V>
pair<K, V> * binarySearchTree<K, V>::find(K theKey) const
{
	binaryTreeNode<pair<K, V> > *pNode = root;
	while (pNode != NULL)
	{
		if (pNode->element.key == theKey)
			return &(pNode->element);
		else
		{
			if (pNode->element.key > theKey)
				pNode = pNode->left;
			else
				pNode = pNode->right;
		}
	}
	if (pNode == NULL)
		return NULL; 
}

template <typename K, typename V>
void binarySearchTree<K, V>::insert(const pair<K, V> & thePair)
{
	if (root == NULL)
		root = new binaryTreeNode<pair<K, V> >(thePair); //Should write code of operator= ?
	else
	{
		binaryTreeNode<pair<K, V> > *pNode = root;
		binaryTreeNode<pair<K, V> > *lastNode = NULL; //Trace node visited most recently.
		while (pNode != NULL && (pNode->element).key != thePair.key)
		{
			lastNode = pNode;
			if ((pNode->element).key > thePair.key)
				pNode = pNode->left;
			else
				pNode = pNode->right;
		}
		if (pNode != NULL)
			(pNode->element).value = thePair.value;

		else
		{
			if (lastNode->left == NULL)
				lastNode->left = new binaryTreeNode<pair<K, V> >(thePair);
			else
				lastNode->right = new binaryTreeNode<pair<K, V> >(thePair);
		}
	}
	++numOfNode;
	return;
}

template <typename K, typename V>
bool binarySearchTree<K, V>::erase(const K & theKey)
{
	binaryTreeNode<pair<K, V> > *pNode = root;
	binaryTreeNode<pair<K, V> > *lastNode = NULL;
	while (pNode != NULL)
	{
		if ((pNode->element).key == theKey)
			break;
		else
		{
			lastNode = pNode;
			if ((pNode->element).key > theKey)
				pNode = pNode->left;
			else
				pNode = pNode->right;
		}
	}
	if (pNode == NULL)
	{
		cout << "No node of keyword: " << theKey << '\n';
		return false;
	}
	//if (lastNode == NULL) //The node to be deleted is root of binary search tree.
	else
	{
		if (pNode->left == NULL && pNode->right == NULL)
		{
			if (lastNode == NULL) //The node to be deleted is root of binary search tree.
				root = NULL;
			else
			{
				if (lastNode->left == pNode)
					lastNode->left = NULL;
				else
					lastNode->right = NULL;
			}
			delete pNode;
		}
		else
		{
			if (pNode->left == NULL)
			{
				if (lastNode == NULL)
					root = pNode->right;
				else
				{
					if (lastNode->left == pNode)
						lastNode->left = pNode->right;
					else
						lastNode->right = pNode->right;
				}
				delete pNode;
			}
			else
			{
				if (pNode->right == NULL)
				{
					if (lastNode == NULL)
						root = pNode->left;
					else
					{
						if (lastNode->left == pNode)
							lastNode->left = pNode->left;
						else
							lastNode->right = pNode->left;
					}
					delete pNode;
				}
				else
				{
					binaryTreeNode<pair<K, V> > *pMax = pNode->left;
					binaryTreeNode<pair<K, V> > *lastMax = NULL;
					binaryTreeNode<pair<K, V> > *pLastMax = NULL;
					while (pMax != NULL)
					{
						pLastMax = lastMax;
						lastMax = pMax;
						pMax = pMax->right;
					}
					pNode->element = lastMax->element;
					if (pLastMax != NULL)
						pLastMax->right = lastMax->left;
					delete lastMax; 
					//For this situation, it does not matter wheather pNode is root or not.
					//The node of mininum key value in right subtree of pNode could also be used.
				}
			}
		}
		--numOfNode;
		return true;
	}
}

int main()
{
	//freopen("20191012output.txt", "w", stdout);
	//Following code constructs a complete binary tree in a simple and direct way.
	binaryTreeNode<char> n1('a');
	binaryTreeNode<char> n2('b');
	binaryTreeNode<char> n3('c');
	n1.left = &n2; n1.right = &n3;
	binaryTreeNode<char> n4('d');
	binaryTreeNode<char> n5('e');
	n2.left = &n4; n2.right = &n5;
	binaryTreeNode<char> n6('f');
	binaryTreeNode<char> n7('g');
	n3.left = &n6; n3.right = &n7;
	binaryTreeNode<char> n8('h');
	binaryTreeNode<char> n9('i');
	n4.left = &n8; n4.right = &n9;
	binaryTreeNode<char> n10('j');
	binaryTreeNode<char> n11('k');
	n5.left = &n10; n5.right = &n11;
	binaryTreeNode<char> n12('l');
	binaryTreeNode<char> n13('m');
	n6.left = &n12; n6.right = &n13;
	binaryTreeNode<char> n14('n');
	n7.left = &n14;	
	binaryTree<char> tree(&n1, 14);
	//Testing code for member function of class binaryTree.
	tree.preOrder(&n1);
	cout << "\n------\n";
	tree.preTraversal(&n1);
	cout << "\n------\n";
	tree.inOrder(&n1);
	cout << "\n------\n";
	tree.inTraversal(&n1);
	cout << "\n------\n";
	tree.postOrder(&n1);
	cout << "\n------\n";
	tree.postTraversal(&n1);
	cout << "\n------\n";
	tree.levelOrder(&n1);
	cout << "\n------\n";
	cout << tree.height(&n1);
	cout << "\n------\n";
	cout << tree.find('g')->element << '\n';
	tree.find('z');
	//tree.~binaryTree();
	//A complete binary search tree and testing code for member function of class binarySearchTree.
	pair<int, char> p1(11, 'a'); binaryTreeNode<pair<int, char> > pn1(p1);
	pair<int, char> p2(7, 'b'); binaryTreeNode<pair<int, char> > pn2(p2);
	pair<int, char> p3(15, 'c'); binaryTreeNode<pair<int, char> > pn3(p3);
	pn1.left = &pn2; pn1.right = &pn3;
	pair<int, char> p4(4, 'd'); binaryTreeNode<pair<int, char> > pn4(p4);
	pair<int, char> p5(9, 'e'); binaryTreeNode<pair<int, char> > pn5(p5);
	pair<int, char> p6(13, 'f'); binaryTreeNode<pair<int, char> > pn6(p6);
	pair<int, char> p7(17, 'g'); binaryTreeNode<pair<int, char> > pn7(p7);
	pn2.left = &pn4; pn2.right = &pn5; pn3.left = &pn6; pn3.right = &pn7;
	pair<int, char> p8(2, 'h'); binaryTreeNode<pair<int, char> > pn8(p8);
	pair<int, char> p9(6, 'i'); binaryTreeNode<pair<int, char> > pn9(p9);
	pair<int, char> p10(8, 'j'); binaryTreeNode<pair<int, char> > pn10(p10);
	pair<int, char> p11(10, 'k'); binaryTreeNode<pair<int, char> > pn11(p11);
	pair<int, char> p12(12, 'l'); binaryTreeNode<pair<int, char> > pn12(p12);
	pair<int, char> p13(14, 'm'); binaryTreeNode<pair<int, char> > pn13(p13);
	pair<int, char> p14(16, 'n'); binaryTreeNode<pair<int, char> > pn14(p14);
	pair<int, char> p15(18, 'o'); binaryTreeNode<pair<int, char> > pn15(p15);
	pn4.left = &pn8; pn4.right = &pn9; pn5.left = &pn10; pn5.right = &pn11;
	pn6.left = &pn12; pn6.right = &pn13; pn7.left = &pn14; pn7.right = &pn15;
	pair<int, char> p16(1, 'p'); binaryTreeNode<pair<int, char> > pn16(p16);
	pair<int, char> p17(3, 'q'); binaryTreeNode<pair<int, char> > pn17(p17);
	pair<int, char> p18(5, 'r'); binaryTreeNode<pair<int, char> > pn18(p18);
	pn8.left = &pn16; pn8.right = &pn17; pn9.left = &pn18;
	binarySearchTree<int, char> searchTree(&pn1, 18);
	cout << "\n------\n";
	searchTree.preOrder(&pn1);
	cout << "\n------\n";
	searchTree.preTraversal(&pn1);
	cout << "\n------\n";
	cout << searchTree.height(&pn1);
	cout << "\n------\n";
	cout << searchTree.find(1)->value << ',';
	cout << searchTree.find(14)->value;
	cout << "\n------\n";
	pair<int, char> ap1(19, '+');
	pair<int, char> ap2(20, '+');
	searchTree.insert(ap1);
	searchTree.levelOrder(&pn1);
	//if (searchTree.erase(11))
	cout << "\n------\n";
	searchTree.insert(ap2);
	searchTree.postTraversal(&pn1);
	cout << "\n------\n";
    return 0;
}

