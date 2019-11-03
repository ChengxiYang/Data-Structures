// AVL������.cpp : Defines the entry point for the console application.
/*	1.AVL�����������������죺ͨ������һϵ��AVLTreeNode���󣬳�ʼ����key, value, left, right, parent��Ա��
      �����͹�����һ��AVL������������ڵ��ƽ������δ��ʼ����ʵ���Ͻڵ��ƽ������Ҳֻ����AVL����������״��ȷ����
	2.��ƽ�����ӵĶ��壬 AVL�������ڵ��ƽ�����ӿ�ͨ����Ա����int height(AVLTreeNode<K, V> *pNode)��á�
	3.AVL��������̬���빹�죺ָ��������ڵ�Ĺؼ��ֺ�ֵ����Ϊ��������insert()������ͨ�������ز��ϵ���insert()����
	  ��̬������AVL��������
	4.AVL�������������ڴ洢�ֵ䣬�Ҳ��ң����룬ɾ���ڵ�ʱ�临�ӶȾ��ȶ���O(log(N))��
*/
#include "stdafx.h"
#include <iostream>
#include <queue>
#include <stack>
using std::cin;
using std::cout;
using std::freopen;
using std::queue;
using std::stack;

template <class K, class V>
struct AVLTreeNode
{
public:
	K key;
	V value;
	int bf; //�ڵ�ƽ�����Ӷ���Ϊ�ڵ��������߶ȼ�ȥ�ڵ��������߶�, bf = -1, 0, 1
	AVLTreeNode<K, V> *left;
	AVLTreeNode<K, V> *right;
	AVLTreeNode<K, V> *parent;
	AVLTreeNode(const K &theKey, const V &theValue)
	{
		key = theKey;
		value = theValue;
		bf = 0;
		left = NULL;
		right = NULL;
		parent = NULL;
	}
};

template <class K, class V>
class AVLSearchTree
{
protected:
	AVLTreeNode<K, V> *root;
	int node_num;
protected:
	int height(AVLTreeNode<K, V> *pNode) const; //����pNodeΪ����Ӧ�����ĸ߶�
	void nodeBF(AVLTreeNode<K, V> *pNode)
	{ pNode->bf = height(pNode->left) - height(pNode->right); }
	//���½ڵ�pNode��ƽ������
	void pathBF(AVLTreeNode<K, V> *pNode);
	//���½ڵ�pNode�����ڵ�֮�����нڵ��ƽ������
	void visitNode(AVLTreeNode<K, V> *pNode) const
	{ cout << "(" << pNode->key << ", " << pNode->value << ")\n"; }
	void LLrotation(AVLTreeNode<K, V> *pA);
	void LRrotation(AVLTreeNode<K, V> *pA);
	void RLrotation(AVLTreeNode<K, V> *pA);
	void RRrotation(AVLTreeNode<K, V> *pA);
	void RBrotation(AVLTreeNode<K, V> *pA); //bf(B) = 0, balance
	void RProtation(AVLTreeNode<K, V> *pA); //bf(B) = +1, positive
	void RNrotation(AVLTreeNode<K, V> *pA); //bf(B) = -1, negative
	void LBrotation(AVLTreeNode<K, V> *pA);
	void LProtation(AVLTreeNode<K, V> *pA);
	void LNrotation(AVLTreeNode<K, V> *pA);
public:
	AVLSearchTree() { root = NULL; node_num = 0; }
	void height() const; //�����ĸ߶�
	void inTraversal() const; //�������AVL�������Թؼ�������ʽ������нڵ�
	void levelTraversal() const;
	AVLTreeNode<K, V> * find(const K &theKey) const;
	void insert(const K &theKey, const V &theValue);
	void erase(const K &theKey);
};

template <class K, class V>
int AVLSearchTree<K, V>::height(AVLTreeNode<K, V> *pNode) const
{ //�ݹ��������ڽڵ����϶�(>10)������»ᷢ��ջ���, �ʿ��Ƿǵݹ�������
	if (pNode == NULL)
		return 0;
	int tree_height = 0;
	queue<AVLTreeNode<K, V> *> nodeQueue;
	nodeQueue.push(pNode);
	while (!nodeQueue.empty())
	{
		++tree_height;
		int level_node = nodeQueue.size(); //����ǰ��Ľڵ���
		int num = 0;
		while (num < level_node)
		{ //����k��Ľڵ�ȫ����������ʱ����k+1��Ľڵ�ȫ���������
			AVLTreeNode<K, V> *curNode = nodeQueue.front();
			nodeQueue.pop();
			++num;
			if (curNode->left != NULL)
				nodeQueue.push(curNode->left);
			if (curNode->right != NULL)
				nodeQueue.push(curNode->right);
		}
	}
	return tree_height;
}

template <class K, class V>
void AVLSearchTree<K, V>::height() const
{
	cout << height(root) << '\n';
}

template <class K, class V>
void AVLSearchTree<K, V>::inTraversal() const
{
	if (root == NULL)
		return;
	AVLTreeNode<K, V> *current = root;
	AVLTreeNode<K, V> *last = NULL;
	stack<AVLTreeNode<K, V> *> nodeStack;
	while (current != NULL || !nodeStack.empty())
	{
		while (current != NULL)
		{
			nodeStack.push(current);
			current = current->left;
		}
		last = nodeStack.top();
		nodeStack.pop();
		visitNode(last);
		current = last->right;
	}
}

template <class K, class V>
void AVLSearchTree<K, V>::levelTraversal() const
{
	if (root == NULL)
		return;
	AVLTreeNode<K, V> *current = root;
	queue<AVLTreeNode<K, V> *> nodeQueue;
	while (current != NULL)
	{
		visitNode(current);
		//nodeBF(current);
		//cout << current->bf << '\n'; 
		if (current->left != NULL)
			nodeQueue.push(current->left);
		if (current->right != NULL)
			nodeQueue.push(current->right);
		if (nodeQueue.empty())
			return;
		current = nodeQueue.front();
		nodeQueue.pop();
	}
}

template <class K, class V>
AVLTreeNode<K, V> * AVLSearchTree<K, V>::find(const K &theKey) const
{
	if (root == NULL)
	{
		cout << "The AVL search tree contains no node.\n";
		return NULL;
	}
	AVLTreeNode<K, V> *pNode = root;
	while (pNode != NULL)
	{
		if (theKey == pNode->key)
			return pNode;
		else
		{
			if (theKey < pNode->key)
				pNode = pNode->left;
			else
				pNode = pNode->right;
		}	
	}
	return NULL;
}

template <class K, class V>
void AVLSearchTree<K, V>::LLrotation(AVLTreeNode<K, V> *pA)
{ //pAָ������������ĸ��ڵ�
	AVLTreeNode<K, V> *pB = pA->left;
	pA->left = pB->right;
	if (pB->right != NULL)
		(pB->right)->parent = pA;
	pB->right = pA;
	AVLTreeNode<K, V> *pP = pA->parent; //pA�ڵ�ĸ��ڵ�pP
	if (pP == NULL)
	{ //pA�����ĸ��ڵ㣬���ֱ߽������������
		root = pB;
		pB->parent = NULL;
		pA->parent = pB;
	}
	else
	{
		if (pA->key < pP->key) //�޸�pP�ڵ���Ӧ��ָ����
		{
			pP->left = pB;
			pB->parent = pP;
			pA->parent = pB;
		}
		else
		{
			pP->right = pB;
			pB->parent = pP;
			pA->parent = pB;
		}
	}
}

template <class K, class V>
void AVLSearchTree<K, V>::LRrotation(AVLTreeNode<K, V> *pA)
{
	AVLTreeNode<K, V> *pB = pA->left;
	AVLTreeNode<K, V> *pC = pB->right;
	pA->left = pC->right;
	if (pC->right != NULL)
		(pC->right)->parent = pA;
	pB->right = pC->left;
	if (pC->left != NULL)
		(pC->left)->parent = pB;
	pC->left = pB;
	pB->parent = pC;
	pC->right = pA;
	AVLTreeNode<K, V> *pP = pA->parent; 
	if (pP == NULL)
	{
		root = pC;
		pC->parent = NULL;
		pA->parent = pC;
	}
	else
	{
		if (pA->key < pP->key)
		{
			pP->left = pC;
			pC->parent = pP;
			pA->parent = pC;
		}
		else
		{
			pP->right = pC;
			pC->parent = pP;
			pA->parent = pC;
		}
	}
}

template <class K, class V>
void AVLSearchTree<K, V>::RLrotation(AVLTreeNode<K, V> *pA)
{
	AVLTreeNode<K, V> *pB = pA->right;
	AVLTreeNode<K, V> *pC = pB->left;
	pA->right = pC->left;
	if (pC->left != NULL)
		(pC->left)->parent = pA;
	pB->left = pC->right;
	if (pC->right != NULL)
		(pC->right)->parent = pB;
	pC->left = pA;
	pC->right = pB;
	pB->parent = pC;
	AVLTreeNode<K, V> *pP = pA->parent;
	if (pP == NULL)
	{
		root = pC;
		pC->parent = NULL;
		pA->parent = pC;
	}
	else
	{
		if (pA->key < pP->key)
		{
			pP->left = pC;
			pC->parent = pP;
			pA->parent = pC;
		}
		else
		{
			pP->right = pC;
			pC->parent = pP;
			pA->parent = pC;
		}
	}
}

template <class K, class V>
void AVLSearchTree<K, V>::RRrotation(AVLTreeNode<K, V> *pA)
{
	AVLTreeNode<K, V> *pB = pA->right;
	pA->right = pB->left;
	if (pB->left != NULL)
		(pB->left)->parent = pA;
	pB->left = pA;
	AVLTreeNode<K, V> *pP = pA->parent; 
	if (pP == NULL)
	{
		root = pB;
		pB->parent = NULL;
		pA->parent = pB;
	}
	else
	{
		if (pA->key < pP->key)
		{
			pP->left = pB;
			pB->parent = pP;
			pA->parent = pB;
		}
		else
		{
			pP->right = pB;
			pB->parent = pP;
			pA->parent = pB;
		}
	}
}

template <class K, class V>
void AVLSearchTree<K, V>::RBrotation(AVLTreeNode<K, V> *pA)
{
	AVLTreeNode<K, V> *pB = pA->left;
	pA->left = pB->right;
	if (pB->right != NULL) //�ϸ��������ڵ�B�������ӽڵ������������һ����Ϊ��
		(pB->right)->parent = pA; //��ֹ����Ĵ���
	pB->right = pA;
	AVLTreeNode<K, V> *pP = pA->parent;
	if (pP == NULL)
	{
		root = pB;
		pB->parent = NULL;
		pA->parent = pB;
	}
	else
	{
		if (pA->key < pP->key)
		{
			pP->left = pB;
			pB->parent = pP;
			pA->parent = pB;
		}
		else
		{
			pP->right = pB;
			pB->parent = pP;
			pA->parent = pB;
		}
	}
}

template <class K, class V>
void AVLSearchTree<K, V>::RProtation(AVLTreeNode<K, V> *pA)
{
	AVLTreeNode<K, V> *pB = pA->left;
	pA->left = pB->right; //�ڵ�B�����ӽڵ�һ����Ϊ�գ����ӽڵ����Ϊ��
	if (pB->right != NULL)
		(pB->right)->parent = pA;
	pB->right = pA;
	AVLTreeNode<K, V> *pP = pA->parent;
	if (pP == NULL)
	{
		root = pB;
		pB->parent = NULL;
		pA->parent = pB;
	}
	else
	{
		if (pA->key < pP->key)
		{
			pP->left = pB;
			pB->parent = pP;
			pA->parent = pB;
		}
		else
		{
			pP->right = pB;
			pB->parent = pP;
			pA->parent = pB;
		}
	}
}

template <class K, class V>
void AVLSearchTree<K, V>::RNrotation(AVLTreeNode<K, V> *pA)
{
	AVLTreeNode<K, V> *pB = pA->left;
	AVLTreeNode<K, V> *pC = pB->right;
	pA->left = pC->right;
	if (pC->parent != NULL)
		(pC->right)->parent = pA;
	pB->right = pC->left;
	if (pC->left != NULL)
		(pC->left)->parent = pB;
	pC->left = pB;
	pB->parent = pC;
	pC->right = pA;
	AVLTreeNode<K, V> *pP = pA->parent;
	if (pP == NULL)
	{
		root = pC;
		pC->parent = NULL;
		pA->parent = pC;
	}
	else
	{
		if (pA->key < pP->key)
		{
			pP->left = pC;
			pC->parent = pP;
			pA->parent = pC;
		}
		else
		{
			pP->right = pC;
			pC->parent = pP;
			pA->parent = pC;
		}
	}
}

template <class K, class V>
void AVLSearchTree<K, V>::LBrotation(AVLTreeNode<K, V> *pA)
{
	AVLTreeNode<K, V> *pB = pA->right;
	pA->right = pB->left;
	if (pB->left != NULL) //ʵ���Ͻڵ�B��������һ����Ϊ��
		(pB->left)->parent = pA; //�Է�����
	pB->left = pA;
	AVLTreeNode<K, V> *pP = pA->parent;
	if (pP == NULL)
	{
		root = pB;
		pB->parent = NULL;
		pA->parent = pB;
	}
	else
	{
		if (pA->key < pP->key)
		{
			pP->left = pB;
			pB->parent = pP;
			pA->parent = pB;
		}
		else
		{
			pP->right = pB;
			pB->parent = pP;
			pA->parent = pB;
		}	
	}
}

template <class K, class V>
void AVLSearchTree<K, V>::LProtation(AVLTreeNode<K, V> *pA)
{
	AVLTreeNode<K, V> *pB = pA->right;
	AVLTreeNode<K, V> *pC = pB->left;
	pA->right = pC->left;
	if (pC->left != NULL)
		(pC->left)->parent = pA;
	pB->left = pC->right;
	if (pC->right != NULL)
		(pC->right)->parent = pB;
	pC->left = pA;
	pC->right = pB;
	pB->parent = pC;
	AVLTreeNode<K, V> *pP = pA->parent;
	if (pP == NULL)
	{
		root = pC;
		pC->parent = NULL;
		pA->parent = pC;
	}
	else
	{
		if (pA->key < pP->key)
		{
			pP->left = pC;
			pC->parent = pP;
			pA->parent = pC;
		}
		else
		{
			pP->right = pC;
			pC->parent = pP;
			pA->parent = pC;
		}
	}
}

template <class K, class V>
void AVLSearchTree < K, V > ::LNrotation(AVLTreeNode<K, V> *pA)
{
	AVLTreeNode<K, V> *pB = pA->right;
	pA->right = pB->left;
	if (pB->left != NULL)
		(pB->left)->parent = pA;
	pB->left = pA;
	AVLTreeNode<K, V> *pP = pA->parent;
	if (pP == NULL)
	{
		root = pB;
		pB->parent = NULL;
		pA->parent = pB;
	}
	else
	{
		if (pA->key < pP->key)
		{
			pP->left = pB;
			pB->parent = pP;
			pA->parent = pB;
		}
		else
		{
			pP->right = pB;
			pB->parent = pP;
			pA->parent = pB;
		}
	}
}

template <class K, class V>
void AVLSearchTree<K, V>::pathBF(AVLTreeNode<K, V> *pNode)
{ //���ݷ�
	if (pNode == NULL)
		return;
	AVLTreeNode<K, V> *curNode = pNode;
	while (curNode != NULL)
	{ 
		curNode->bf = height(curNode->left) - height(curNode->right);
		curNode = curNode->parent;
	}
}

template <class K, class V>
void AVLSearchTree<K, V>::insert(const K &theKey, const V &theValue)
{
	if (root == NULL)
	{
		root = new AVLTreeNode<K, V>(theKey, theValue);
		++node_num;
		return;
	}
	AVLTreeNode<K, V> *current = root;
	AVLTreeNode<K, V> *last = NULL;
	stack<AVLTreeNode<K, V> *> nodeStack;
	while (current != NULL)
	{
		if (current->key == theKey)
		{ /* ��AVL�������д��ڽڵ�keyֵ�������ڵ�keyֵ��ͬ�����Դ�����ڵ�theValueֵ�滻�ýڵ�valueֵ��
			 ����ڵ���̽��� */
			current->value = theValue;
			return;
		}
		else
		{
			last = current;
			last->bf = height(last->left) - height(last->right);
			//��¼�Ӹ��ڵ㵽�²���ڵ�֮�����нڵ��ƽ������
			if (last->bf == 1 || last->bf == -1)
				nodeStack.push(last);
			if (current->key > theKey)
				current = current->left;
			else
				current = current->right;
		}
	}
	if (last->key > theKey)
	{
		last->left = new AVLTreeNode<K, V>(theKey, theValue);
		(last->left)->parent = last;
	}
	else
	{
		last->right = new AVLTreeNode<K, V>(theKey, theValue);
		(last->right)->parent = last;
	}
	++node_num; //�����������ķ�ʽ�����½ڵ���ٿ���ƽ�����ʡ�
	if (nodeStack.empty())
	{ //�޸��²���ڵ㵽���ڵ�֮��ڵ��ƽ������, ����ڵ��ƽ�����Ӳ��ı䡣
		pathBF(last);
		return;
	}
	else
	{
		AVLTreeNode<K, V> *pa = nodeStack.top();
		if (pa->bf == 1)
		{ //L�Ͳ�ƽ��
			if (pa->key < theKey)
			{ //������A�ڵ���������в����ƻ�ƽ����
				pathBF(last);
				return; 
			}
			else
			{
				AVLTreeNode<K, V> *pb = pa->left;
				if (pb->key > theKey)
				{ //LL�Ͳ�ƽ��
					pa->bf = 0;
					LLrotation(pa);
					AVLTreeNode<K, V> *curNode = pb;
					while (curNode->key != theKey)
					{ //����LL��ת�����������ڵ�pb������ڵ��ƽ������
						nodeBF(curNode);
						if (curNode->key > theKey)
							curNode = curNode->left;
						else
							curNode = curNode->right;
					}
					return;
				}
				else
				{ //LR�Ͳ�ƽ��
					AVLTreeNode<K, V> *pc = pb->right;
					AVLTreeNode<K, V> *curNode = pc;
					while (curNode->key != theKey)
					{ //����LR��ת�����������ڵ�pc������ڵ��ƽ������
						nodeBF(curNode);
						if (curNode->key > theKey)
							curNode = curNode->left;
						else
							curNode = curNode->right;
					}
					nodeBF(pc->left); nodeBF(pc->right);
					return;
				}
			}
		}
		if (pa->bf == -1)
		{ //R�Ͳ�ƽ��
			if (pa->key > theKey)
			{
				pathBF(last);
				return;
			}
			else
			{
				AVLTreeNode<K, V> *pb = pa->right;
				if (pb->key < theKey)
				{ //RR�Ͳ�ƽ��
					pa->bf = 0;
					RRrotation(pa);
					AVLTreeNode<K, V> *curNode = pb;
					while (curNode->key != theKey)
					{
						nodeBF(curNode);
						if (curNode->key < theKey)
							curNode = curNode->right;
						else
							curNode = curNode->left;
					}
					return;
				}
				else
				{ //RL�Ͳ�ƽ��
					AVLTreeNode<K, V> *pc = pb->left;
					RLrotation(pa);
					AVLTreeNode<K, V> *curNode = pc;
					while (curNode->key != theKey)
					{
						nodeBF(curNode);
						if (curNode->key < theKey)
							curNode = curNode->right;
						else
							curNode = curNode->left;
					}
					nodeBF(pc->left); nodeBF(pc->right);
					return;
				}
			}
		}
	}
}

template <class K, class V>
void AVLSearchTree<K, V>::erase(const K &theKey)
{ //���Զ����������ķ�ʽɾ���ڵ㣬�ٵ�������ƽ��
	if (root == NULL)
	{
		cout << "The AVL search tree is empty.\n";
		return;
	}
	AVLTreeNode<K, V> *p_delete = find(theKey);
	if (p_delete == NULL)
	{
		cout << "There is no node of key equal to theKey in tree.\n";
		return;
	}
	if (p_delete == root)
	{ //��ɾ���ڵ��Ǹ��ڵ�ı߽������������
		if (p_delete->left == NULL && p_delete->right == NULL)
		{
			delete p_delete;
			root = NULL; //ɾ������Ψһ�ĸ��ڵ��root��������ΪNULL������rootΪҰָ�뽫�����쳣
			--node_num;
			return;
		}
		else
		{
			if (p_delete->left == NULL)
			{ //��AVL������ƽ���Ե����ƣ��������������ֻ����һ���ڵ�
				root = p_delete->right;
				(p_delete->right)->parent = NULL;
				delete p_delete;
				--node_num;
				return;
			}
			else
			{
				if (p_delete->right == NULL)
				{ //ͬ����ƽ���Ե����ƣ�������ֻ����һ���ڵ�
					root = p_delete->left;
					(p_delete->left)->parent = NULL;
					delete p_delete;
					--node_num;
					return;
				}
				else
				{ //ת��Ϊɾ���Ǹ��ڵ㴦��
					AVLTreeNode<K, V> *current = p_delete->left;
					AVLTreeNode<K, V> *left_max = NULL;
					while (current != NULL)
					{
						left_max = current;
						current = current->right;
					}
					p_delete->key = left_max->key;
					p_delete->value = left_max->value;
					p_delete = left_max;
				}
			}
		}
	} //������봦��ɾ���Ǹ��ڵ������
	AVLTreeNode<K, V> *pq = p_delete->parent;
	if (p_delete->left == NULL && p_delete->right == NULL)
	{ //��ɾ���ڵ���Ҷ�ڵ�
		if (p_delete->key < pq->key)
			pq->left = NULL;
		else
			pq->right = NULL;
		delete p_delete; //�ڵ�pDelete��������ȫ������
	}
	else
	{
		if (p_delete->left == NULL)
		{ //��ɾ���ڵ�������Ϊ�գ���������Ϊ��
			if (p_delete->key < pq->key)
			{
				pq->left = p_delete->right;
				(p_delete->right)->parent = pq;
				delete p_delete;
			}
			else
			{
				pq->right = p_delete->right;
				(p_delete->right)->parent = pq;
				delete p_delete;
			}
		}
		else
		{
			if (p_delete->right == NULL)
			{ //��ɾ���ڵ���������Ϊ�գ�������Ϊ��
				if (p_delete->key < pq->key)
				{
					pq->left = p_delete->left;
					(p_delete->left)->parent = pq;
					delete p_delete;
				}
				else
				{
					pq->right = p_delete->left;
					(p_delete->left)->parent = pq;
					delete p_delete;
				}
			}
			else
			{ //��ɾ���ڵ�������������Ϊ��
				AVLTreeNode<K, V> *current = p_delete->left;
				AVLTreeNode<K, V> *left_max = NULL; //�ڵ�p_delete�����������ؼ��ֽڵ�
				while (current != NULL)
				{
					left_max = current;
					current = current->right;
				}
				p_delete->key = left_max->key;
				p_delete->value = left_max->value;
				p_delete = left_max;
				pq = left_max->parent;
				if (p_delete->left == NULL)
				{
					pq->right = NULL;
					delete p_delete;
				}
				else
				{
					pq->right = p_delete->left;
					(p_delete->left)->parent = pq;
					delete p_delete;
				}
			}
		}
	} //����Ĵ��뿪ʼ��������ƽ��
	--node_num;
	AVLTreeNode<K, V> *p_node = pq;
	AVLTreeNode<K, V> *pa = NULL; //pa����ָ��bfΪ2��-2�Ľڵ�
	while (p_node != NULL)
	{
		nodeBF(p_node);
		if (p_node->bf == 2 || p_node->bf == -2)
		{
			pa = p_node;
			if (pa->bf == -2)	//pa��pq������ͬһ�ڵ�
			{ //��ɾ���ڵ���A�ڵ���������У�L�Ͳ�ƽ��
				AVLTreeNode<K, V> *pb = pa->right;
				if (pb->bf == 0) //L0�Ͳ�ƽ��
				{
					LBrotation(pa);
					return; //һ��LB��ת���ɻָ���������ƽ��
				}
				if (pb->bf == 1)
				{ //LP��ƽ��
					AVLTreeNode<K, V> *pc = pb->left;
					LProtation(pa);	//LP��תʹ�����ĸ߶ȼ���1
					p_node = pc->parent;
					continue; //�������ݵ���LP��ת�������²�ƽ��ڵ�
				}
				if (pb->bf == -1)
				{
					LNrotation(pa);
					p_node = pb->parent;
					continue;
				}
			}
			if (pa->bf == 2)
			{ //��ɾ���ڵ���A�ڵ���������У�R�Ͳ�ƽ��
				AVLTreeNode<K, V> *pb = pa->left;
				if (pb->bf == 0)
				{
					RBrotation(pa);
					return;
				}
				if (pb->bf == 1)
				{
					RProtation(pa);
					p_node = pb->parent;
					continue;
				}
				if (pb->bf == -1)
				{
					AVLTreeNode<K, V> *pc = pb->right;
					RNrotation(pa);
					p_node = pc->parent;
					continue;
				}
			}
		}
		p_node = p_node->parent;
	}
}

int main()
{ //����Ϊ���Դ���
	freopen("20191103test.txt", "w", stdout);
	AVLSearchTree<int, char> tree_1;
	for (int i = 0; i < 52; ++i)
	{
		char ele = char(int('A') + i);
		tree_1.insert(i + 1, ele);
	}
	tree_1.inTraversal();
	cout << "\n------\n";
	for (int i = 1; i <= 52; ++i)
	{
		tree_1.erase(i);
		tree_1.inTraversal();
		cout << "\n------\n";
	}
    return 0;
}

