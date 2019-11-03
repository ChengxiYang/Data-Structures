// AVL搜索树.cpp : Defines the entry point for the console application.
/*	1.AVL搜索树暴力方法构造：通过声明一系列AVLTreeNode对象，初始化其key, value, left, right, parent成员，
      这样就构造了一棵AVL搜索树。但其节点的平衡因子未初始化，实际上节点的平衡因子也只能由AVL搜索树的形状来确定。
	2.由平衡因子的定义， AVL搜索树节点的平衡因子可通过成员函数int height(AVLTreeNode<K, V> *pNode)求得。
	3.AVL搜索树动态插入构造：指定待插入节点的关键字和值，作为参数调用insert()方法，通过这样地不断调用insert()方法
	  动态生成了AVL搜索树。
	4.AVL搜索树可以用于存储字典，且查找，插入，删除节点时间复杂度均稳定在O(log(N))。
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
	int bf; //节点平衡因子定义为节点左子树高度减去节点右子树高度, bf = -1, 0, 1
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
	int height(AVLTreeNode<K, V> *pNode) const; //求以pNode为根相应子树的高度
	void nodeBF(AVLTreeNode<K, V> *pNode)
	{ pNode->bf = height(pNode->left) - height(pNode->right); }
	//更新节点pNode的平衡因子
	void pathBF(AVLTreeNode<K, V> *pNode);
	//更新节点pNode到根节点之间所有节点的平衡因子
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
	void height() const; //求树的高度
	void inTraversal() const; //中序遍历AVL搜索树以关键字升序方式输出所有节点
	void levelTraversal() const;
	AVLTreeNode<K, V> * find(const K &theKey) const;
	void insert(const K &theKey, const V &theValue);
	void erase(const K &theKey);
};

template <class K, class V>
int AVLSearchTree<K, V>::height(AVLTreeNode<K, V> *pNode) const
{ //递归求树高在节点数较多(>10)的情况下会发生栈溢出, 故考虑非递归求树高
	if (pNode == NULL)
		return 0;
	int tree_height = 0;
	queue<AVLTreeNode<K, V> *> nodeQueue;
	nodeQueue.push(pNode);
	while (!nodeQueue.empty())
	{
		++tree_height;
		int level_node = nodeQueue.size(); //树当前层的节点数
		int num = 0;
		while (num < level_node)
		{ //当第k层的节点全部弹出队列时，第k+1层的节点全部进入队列
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
{ //pA指向待调整子树的根节点
	AVLTreeNode<K, V> *pB = pA->left;
	pA->left = pB->right;
	if (pB->right != NULL)
		(pB->right)->parent = pA;
	pB->right = pA;
	AVLTreeNode<K, V> *pP = pA->parent; //pA节点的父节点pP
	if (pP == NULL)
	{ //pA是树的根节点，这种边界情况单独处理
		root = pB;
		pB->parent = NULL;
		pA->parent = pB;
	}
	else
	{
		if (pA->key < pP->key) //修改pP节点相应的指针域
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
	if (pB->right != NULL) //严格来讲，节点B的左右子节点在这种情况下一定不为空
		(pB->right)->parent = pA; //防止意外的错误
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
	pA->left = pB->right; //节点B的左子节点一定不为空，右子节点可能为空
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
	if (pB->left != NULL) //实际上节点B的左子树一定不为空
		(pB->left)->parent = pA; //以防意外
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
{ //回溯法
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
		{ /* 若AVL搜索树中存在节点key值与待插入节点key值相同，则以待插入节点theValue值替换该节点value值，
			 插入节点过程结束 */
			current->value = theValue;
			return;
		}
		else
		{
			last = current;
			last->bf = height(last->left) - height(last->right);
			//记录从根节点到新插入节点之间所有节点的平衡因子
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
	++node_num; //先以搜索树的方式插入新节点后再考虑平衡性质。
	if (nodeStack.empty())
	{ //修改新插入节点到根节点之间节点的平衡因子, 其余节点的平衡因子不改变。
		pathBF(last);
		return;
	}
	else
	{
		AVLTreeNode<K, V> *pa = nodeStack.top();
		if (pa->bf == 1)
		{ //L型不平衡
			if (pa->key < theKey)
			{ //插入在A节点的右子树中不会破坏平衡性
				pathBF(last);
				return; 
			}
			else
			{
				AVLTreeNode<K, V> *pb = pa->left;
				if (pb->key > theKey)
				{ //LL型不平衡
					pa->bf = 0;
					LLrotation(pa);
					AVLTreeNode<K, V> *curNode = pb;
					while (curNode->key != theKey)
					{ //更新LL旋转后新子树根节点pb到插入节点的平衡因子
						nodeBF(curNode);
						if (curNode->key > theKey)
							curNode = curNode->left;
						else
							curNode = curNode->right;
					}
					return;
				}
				else
				{ //LR型不平衡
					AVLTreeNode<K, V> *pc = pb->right;
					AVLTreeNode<K, V> *curNode = pc;
					while (curNode->key != theKey)
					{ //更新LR旋转后新子树根节点pc到插入节点的平衡因子
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
		{ //R型不平衡
			if (pa->key > theKey)
			{
				pathBF(last);
				return;
			}
			else
			{
				AVLTreeNode<K, V> *pb = pa->right;
				if (pb->key < theKey)
				{ //RR型不平衡
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
				{ //RL型不平衡
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
{ //先以二叉搜索树的方式删除节点，再调整树的平衡
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
	{ //待删除节点是根节点的边界情况单独处理
		if (p_delete->left == NULL && p_delete->right == NULL)
		{
			delete p_delete;
			root = NULL; //删除树中唯一的根节点后，root必须设置为NULL，否则root为野指针将引发异常
			--node_num;
			return;
		}
		else
		{
			if (p_delete->left == NULL)
			{ //由AVL搜索树平衡性的限制，这种情况右子树只能有一个节点
				root = p_delete->right;
				(p_delete->right)->parent = NULL;
				delete p_delete;
				--node_num;
				return;
			}
			else
			{
				if (p_delete->right == NULL)
				{ //同样由平衡性的限制，左子树只能有一个节点
					root = p_delete->left;
					(p_delete->left)->parent = NULL;
					delete p_delete;
					--node_num;
					return;
				}
				else
				{ //转化为删除非根节点处理
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
	} //下面代码处理删除非根节点的情形
	AVLTreeNode<K, V> *pq = p_delete->parent;
	if (p_delete->left == NULL && p_delete->right == NULL)
	{ //待删除节点是叶节点
		if (p_delete->key < pq->key)
			pq->left = NULL;
		else
			pq->right = NULL;
		delete p_delete; //节点pDelete所有数据全部析构
	}
	else
	{
		if (p_delete->left == NULL)
		{ //待删除节点左子树为空，右子树不为空
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
			{ //待删除节点左子树不为空，右子树为空
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
			{ //待删除节点左右子树均不为空
				AVLTreeNode<K, V> *current = p_delete->left;
				AVLTreeNode<K, V> *left_max = NULL; //节点p_delete左子树的最大关键字节点
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
	} //下面的代码开始调整树的平衡
	--node_num;
	AVLTreeNode<K, V> *p_node = pq;
	AVLTreeNode<K, V> *pa = NULL; //pa总是指向bf为2或-2的节点
	while (p_node != NULL)
	{
		nodeBF(p_node);
		if (p_node->bf == 2 || p_node->bf == -2)
		{
			pa = p_node;
			if (pa->bf == -2)	//pa与pq可能是同一节点
			{ //已删除节点在A节点的左子树中，L型不平衡
				AVLTreeNode<K, V> *pb = pa->right;
				if (pb->bf == 0) //L0型不平衡
				{
					LBrotation(pa);
					return; //一次LB旋转即可恢复整棵树的平衡
				}
				if (pb->bf == 1)
				{ //LP不平衡
					AVLTreeNode<K, V> *pc = pb->left;
					LProtation(pa);	//LP旋转使子树的高度减少1
					p_node = pc->parent;
					continue; //继续上溯调整LP旋转带来的新不平衡节点
				}
				if (pb->bf == -1)
				{
					LNrotation(pa);
					p_node = pb->parent;
					continue;
				}
			}
			if (pa->bf == 2)
			{ //已删除节点在A节点的右子树中，R型不平衡
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
{ //以下为测试代码
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

