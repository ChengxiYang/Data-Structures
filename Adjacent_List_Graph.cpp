// Adjacent_List_Graph.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include <iostream>
//#include <limits>
#include <queue>
#include <string>
#include <stack>

using std::cout;
using std::queue;
using std::string;
using std::stack;

//#define _GRAPH_1_
#define _GRAPH_2_
const int BUFFER = 10;
//const int INFINITE = ox3f3f3f3f;

class Exception
{
public:
	Exception(const string &info) : info{ info } {}
	void print_exception() { cout << info << "\n"; }
protected:
	string info;
};

template <class T>
struct node
{
	int vertex_index; //即successor节点的编号
	T weight; //边(precursor, successor)的权重
	node<T> *p_next;
	node(int vertex_index, T weight, node<T> *p_next = NULL)
		: vertex_index{ vertex_index }, weight{ weight }, p_next{ p_next } {}
};

template <class T>
class adj_list
{
public:
	adj_list(int precursor) : precursor{ precursor }, adj_size{ 0 }, p_front{ NULL } {}
	~adj_list();
	bool empty() { return p_front == NULL; }
	int size() const { return adj_size; }
	void insert_node(int successor, T weight); //邻接链表中各节点按其在图中的编号升序排序
	void erase_node(int successor);
	node<T> * begin() { return p_front; }
	class iterator
	{
	public:
		iterator() { p_node = NULL; }
		void operator=(node<T> *p_node) { this->p_node = p_node; }
		node<T> operator*()
		{
			if (p_node != NULL) return *p_node;
			else throw Exception("Null Pointer!");
		}
		void operator++() { p_node = p_node->p_next; }
		bool operator==(node<T> *p_node) { return this->p_node == p_node; }
		bool operator!=(node<T> *p_node) { return this->p_node != p_node; }
	protected:
		node<T> *p_node;
	};
protected:
	int precursor; //precursor节点的邻接链表
	int adj_size; //precursor节点的出度
	node<T> *p_front;
};

template <class T>
struct edge
{
	int precursor;
	int successor;
	T weight;
	edge() {}
	edge(int pre, int suc, T wei)
		: precursor{pre}, successor{suc}, weight{wei} {}
	//void operator=(const edge<T> &edge_);
};

template <class T>
void swap(edge<T> *p_a, edge<T> *p_b)
{ //T should be class like edge<int> or edge<double>
	edge<T> temp = *p_a;
	*p_a = *p_b;
	*p_b = temp;
}

template <class T>
int partite(edge<T> *p_arr, int begin, int end)
{
	int i = begin - 1, j = begin;
	edge<T> pivot = p_arr[end];
	for (; j < end; ++j)
		if (p_arr[j].weight <= pivot.weight)
		{
			++i;
			swap(p_arr + i, p_arr + j);
		}
	swap(p_arr + i + 1, p_arr + end);
	return i + 1;
}

template <class T>
void quick_sort(edge<T> *p_arr, int size)
{
	if (p_arr && (size > 1))
	{
		using std::stack;
		stack<int> arg_stack;
		int low = 0, high = size - 1;
		int mid = partite(p_arr, low, high);
		if ((mid - 1) > low)
		{
			arg_stack.push(low);
			arg_stack.push(mid - 1);
		}
		if ((mid + 1) < high)
		{
			arg_stack.push(mid + 1);
			arg_stack.push(high);
		}
		while (!arg_stack.empty())
		{
			high = arg_stack.top();
			arg_stack.pop();
			low = arg_stack.top();
			arg_stack.pop();
			mid = partite(p_arr, low, high);
			if ((mid - 1) > low)
			{
				arg_stack.push(low);
				arg_stack.push(mid - 1);
			}
			if ((mid + 1) < high)
			{
				arg_stack.push(mid + 1);
				arg_stack.push(high);
			}
		}
	}
}

class UnionFind
{
public:
	UnionFind(int vertex_num);
	int find(int p); //find component id of p
	void union_component(int p, int q); //add connection between p and q
	bool is_connected(int p, int q); //return true if p and q in same component
protected:
	int vertex_num;
	int *p_index;
	int *p_size;
	int component_num;
};

template <class T>
class Graph
{
public: //以下各成员函数对各种类型的图均适用
	virtual ~Graph() {}
	virtual bool is_directed() const = 0;
	virtual bool is_weighted() const = 0;
	virtual int num_of_vertex() const = 0;
	virtual int num_of_edge() const = 0;
	virtual void add_vertex() = 0; //通过增加新的顶点和边以动态构造图
	virtual void insert_edge(int precursor, int successor, T weight) = 0;
	virtual void erase_edge(int precursor, int successor) = 0;
	virtual bool is_edge(int precursor, int successor) const = 0;
	virtual void breadth_first_search(int start) const = 0;
	virtual void deepth_first_search(int start) const = 0;
	virtual void dfs_path(int start, int end) const = 0;
	virtual Graph<T> * bfs_tree(int start) const = 0; //返回抽象基类指针以实现多态
	virtual Graph<T> * dfs_tree(int start) const = 0; //bfs与dfs均可运行在有向图与无向图上
};

template <class T>
class WD_Graph : public Graph<T>
{
public:
	WD_Graph(int vertex_num = 0);
	~WD_Graph();
	bool is_directed() const { return true; }
	bool is_weighted() const { return true; }
	int num_of_vertex() const { return vertex_num; }
	int num_of_edge() const { return edge_num; }
	int in_degree(int vertex) const;
	int out_degree(int vertex) const;
	void add_vertex();
	void insert_edge(int precursor, int successor, T weight);
	void erase_edge(int precursor, int successor);
	bool is_edge(int precursor, int successor) const;
	void breadth_first_search(int start) const;
	void deepth_first_search(int start) const;
	void dfs_path(int start, int end) const;
	void Kahn_topo_sort() const;
	Graph<T> * bfs_tree(int start) const;
	Graph<T> * dfs_tree(int start) const;
protected:
	bool check_vertex(int vertex) const
	{
		if (1 <= vertex && vertex <= vertex_num) return true;
		else return false;
	}
protected:
	int vertex_num;
	int capacity;
	int edge_num;
	adj_list<T> **adj;
};

template <class T>
class WU_Graph : public WD_Graph<T>
{
public:
	WU_Graph(int vertex_num = 0) : WD_Graph<T>::WD_Graph(vertex_num) {}
	~WU_Graph() {} //并不需要显式调用基类析构函数
	bool is_directed() const { return false; }
	bool is_weighted() const { return true; }
	int degree(int vertex) const;
	void insert_edge(int precursor, int successor, T weight);
	void erase_edge(int precursor, int successor);
	WU_Graph<T> * Kruskal_min_tree() const;
	WU_Graph<T> * Prim_min_tree(int start) const;
};

template <class T>
adj_list<T>::~adj_list()
{
	if (adj_size == 0)
		return;
	else
	{
		node<T> *p_delete = p_front;
		node<T> *p_to_delete = NULL;
		while (p_delete != NULL)
		{
			p_to_delete = p_delete->p_next;
			delete p_delete;
			p_delete = p_to_delete;
		}
		adj_size = 0;
		p_front = NULL;
	}
}

template <class T>
void adj_list<T>::insert_node(int successor, T weight)
{
	if (adj_size == 0)
	{
		p_front = new node<T>(successor, weight);
		++adj_size;
	}
	else
	{
		node<T> *p_current = p_front;
		node<T> *p_previous = NULL;
		while (p_current != NULL)
		{
			if (p_current->vertex_index < successor)
			{
				p_previous = p_current;
				p_current = p_current->p_next;
			}
			else
				break;
		}
		if (p_current != NULL)
		{
			if (p_current->vertex_index > successor)
			{
				p_previous->p_next = new node<T>(successor, weight);
				(p_previous->p_next)->p_next = p_current;
				++adj_size;
			}
			else //等价于p_current->vertex_index == successor
				throw Exception("Existed Edge!");
		}
		else
		{ //边界情况：在邻接链表尾部插入新节点
			p_previous->p_next = new node<T>(successor, weight);
			(p_previous->p_next)->p_next = NULL;
			++adj_size;
		}
	}
}

template <class T>
void adj_list<T>::erase_node(int successor)
{
	if (adj_size == 0)
		throw Exception("Empty List!");
	else
	{
		node<T> *p_current = p_front;
		node<T> *p_previous = NULL;
		while (p_current != NULL)
		{
			if (successor != p_current->vertex_index)
			{ //注意if表达式中等号所对应的情况
				p_previous = p_current;
				p_current = p_current->p_next;
			}
			else
				break;
		}
		if (p_current != NULL)
		{
			if (p_previous == NULL)
			{ //恰好删除唯一的节点情形
				delete p_current;
				p_current = NULL;
				p_front = NULL;
				adj_size = 0;
			}
			else
			{
				p_previous->p_next = p_current->p_next;
				delete p_current;
				p_current = NULL;
				--adj_size;
			}
		}
		else
			throw Exception("Unexisted Edge!");
	}
}

UnionFind::UnionFind(int vertex_num)
{
	this->vertex_num = vertex_num;
	component_num = vertex_num;
	p_index = new int[vertex_num + 1];
	p_size = new int[vertex_num + 1];
	p_index[0] = -1; p_size[0] = 0;
	for (int i = 1; i <= vertex_num; ++i)
	{
		p_index[i] = i;
		p_size[i] = 1;
	}
}

int UnionFind::find(int p)
{
	while (p != p_index[p])
	{
		p_index[p] = p_index[p_index[p]]; //path-compression
		p = p_index[p];
	}
	return p;
}

bool UnionFind::is_connected(int p, int q)
{
	return find(p) == find(q);
}

void UnionFind::union_component(int p, int q)
{
	int root_p = find(p);
	int root_q = find(q);
	if (root_p == root_q)
		return;
	if (p_size[p] < p_size[q])
	{
		p_index[root_p] = root_q;
		p_size[root_q] += p_size[root_p];
	}
	else
	{
		p_index[root_q] = root_p;
		p_size[root_p] += p_size[root_q];
	}
	--component_num;
}

template <class T>
WD_Graph<T>::WD_Graph(int vertex_num = 0)
{
	if (vertex_num < 0)
		throw Exception("Invalid Parameter!");
	adj = new adj_list<T> *[vertex_num + BUFFER + 1]; //BUFFER部分为空余，留作增加新节点时用
	adj[0] = NULL;
	for (int vertex = 1; vertex <= vertex_num; ++vertex)
		adj[vertex] = new adj_list<T>(vertex);
	for (int rest = vertex_num + 1; rest <= capacity; ++rest)
		adj[rest] = NULL;
	this->vertex_num = vertex_num;
	this->capacity = vertex_num + BUFFER;
	edge_num = 0;
}

template <class T>
WD_Graph<T>::~WD_Graph()
{
	for (int vertex = 1; vertex <= vertex_num; ++vertex)
	{
		adj[vertex]->~adj_list(); //邻接链表对象指针显式调用析构函数
		adj[vertex] = NULL; //~adj_list<T>()也正常运行？
	}
	delete[] adj;
	adj = NULL;
}

template <class T>
int WD_Graph<T>::in_degree(int vertex) const
{
	if (check_vertex(vertex))
	{
		int in_degree_num = 0;
		for (int v = 1; v <= vertex_num; ++v)
		{
			if (v != vertex)
			{
				adj_list<T>::iterator citer;
				citer = adj[v]->begin();
				while (citer != NULL)
				{
					if ((*citer).vertex_index != vertex)
						++citer;
					else
					{
						++in_degree_num;
						break;
					}
				}
			}
		}
		return in_degree_num;
	}
	else
		throw Exception("Invalid Parameter!");
}

template <class T>
int WD_Graph<T>::out_degree(int vertex) const
{
	if (check_vertex(vertex))
		return adj[vertex]->size();
	else
		throw Exception("Invalid Parameter!");
}

template <class T>
int WU_Graph<T>::degree(int vertex) const
{
	if (check_vertex(vertex))
		return adj[vertex]->size();
	else
		throw Exception("Invalid Parameter!");
}

template <class T>
void WD_Graph<T>::add_vertex()
{
	if (vertex_num < capacity)
		adj[vertex_num] = new adj_list<T>(++vertex_num);
	else
	{
		capacity = capacity + BUFFER;
		adj_list<T> **p_temp = new adj_list<T> *[capacity];
		for (int vertex = 0; vertex <= vertex_num; ++vertex)
			p_temp[vertex] = adj[vertex];
		for (int rest = vertex_num + 1; rest <= capacity; ++rest)
			p_temp[rest] = NULL;
		p_temp[vertex_num] = new adj_list<T>(++vertex_num);
		delete[] adj; adj = p_temp; p_temp = NULL;
	}
}

template <class T>
void WD_Graph<T>::insert_edge(int precursor, int successor, T weight)
{
	if (check_vertex(precursor) && check_vertex(successor))
	{ //在边不存在的情况下才能加入边，insert_node函数已考虑了这种情况
		adj[precursor]->insert_node(successor, weight);
		++edge_num;
	}
	else
		throw Exception("Invalid Parameter!");
}

template <class T>
void WU_Graph<T>::insert_edge(int precursor, int successor, T weight)
{
	if (check_vertex(precursor) && check_vertex(successor))
	{  //在边不存在的情况下才能加入边，insert_node函数已考虑了这种情况
		adj[precursor]->insert_node(successor, weight);
		adj[successor]->insert_node(precursor, weight);
		++edge_num;
	}
	else
		throw Exception("Invalid Parameter!");
}

template <class T>
void WD_Graph<T>::erase_edge(int precursor, int successor)
{
	if (check_vertex(precursor) && check_vertex(successor))
	{ //在边存在的情况下才能删除边，erase_node函数已考虑了这种情况
		adj[precursor]->erase_node(successor);
		--edge_num;
	}
	else
		throw Exception("Invalid Parameter!");
}

template <class T>
void WU_Graph<T>::erase_edge(int precursor, int successor)
{
	if (check_vertex(precursor) && check_vertex(successor))
	{ //在边存在的情况下才能删除边，erase_node函数已考虑了这种情况
		adj[precursor]->erase_node(successor);
		adj[successor]->erase_node(precursor);
		--edge_num;
	}
	else
		throw Exception("Invalid Parameter!");
}

template <class T>
bool WD_Graph<T>::is_edge(int precursor, int successor) const
{
	if (!check_vertex(precursor) || !check_vertex(successor))
		throw Exception("Invalid Parameter!");
	adj_list<T>::iterator citer;
	citer = adj[precursor]->begin();
	while (citer != NULL)
	{
		if ((*citer).vertex_index != successor)
			++citer;
		else
			break;
	}
	if (citer != NULL)
		return true;
	else
		return false;
}


template <class T>
void WD_Graph<T>::breadth_first_search(int start) const
{
	if (!check_vertex(start))
		throw Exception("Invalid Parameter!");
	bool *p_label = new bool[vertex_num + 1];
	for (int v = 0; v <= vertex_num; ++v)
		p_label[v] = false;
	queue<int> vertex_queue;
	vertex_queue.push(start);
	p_label[start] = true;
	int cur_vertex = 0; //初始化步骤
	cout << "Breadth First Search From: " << start << "\n";
	while (!vertex_queue.empty())
	{
		cur_vertex = vertex_queue.front();
		vertex_queue.pop();
		adj_list<T>::iterator citer;
		citer = adj[cur_vertex]->begin();
		while (citer != NULL)
		{
			int adj_vertex = (*citer).vertex_index;
			if (!p_label[adj_vertex])
			{
				vertex_queue.push(adj_vertex);
				p_label[adj_vertex] = true;
				cout << cur_vertex << " -> " << adj_vertex << ", ";
			}
			++citer;
		}
		cout << "\n";
	}
	delete[] p_label; p_label = NULL;
}

template <class T>
void WD_Graph<T>::deepth_first_search(int start) const
{
	if (!check_vertex(start))
		throw Exception("Invalid Parameter!");
	bool *p_label = new bool[vertex_num + 1];
	for (int v = 0; v <= vertex_num; ++v)
		p_label[v] = false;
	stack<int> vertex_stack;
	vertex_stack.push(start);
	p_label[start] = true;
	int cur_vertex = 0; //初始化步骤
	cout << "Deepth First Search From:" << start << "\n";
	while (!vertex_stack.empty())
	{
		cur_vertex = vertex_stack.top();
		if (!adj[cur_vertex]->empty())
		{
			adj_list<T>::iterator citer;
			citer = adj[cur_vertex]->begin();
			int adj_vertex = 0;
			while (citer != NULL)
			{
				adj_vertex = (*citer).vertex_index;
				if (!p_label[adj_vertex])
				{ //进行深度优先搜索
					vertex_stack.push(adj_vertex);
					p_label[adj_vertex] = true;
					cout << cur_vertex << " -> " << adj_vertex << ", ";
					break;
				}
				++citer;
			}
			if (citer == NULL) //当前顶点所有邻接顶点均已被标记过，采取回退
				vertex_stack.pop();
		}
		else //当前顶点无邻接顶点，采取回退
			vertex_stack.pop();
	}
	cout << "\n";
	delete[] p_label; p_label = NULL;
}

template <class T>
void WD_Graph<T>::dfs_path(int start, int end) const
{
	if (!check_vertex(start) || !check_vertex(end))
		throw Exception("Invalid Parameter!");
	if (start == end)
	{
		cout << "The start and end is same!\n";
		return;
	}
	bool *p_label = new bool[vertex_num + 1];
	for (int v = 0; v <= vertex_num; ++v)
		p_label[v] = false;
	stack<int> vertex_stack;
	vertex_stack.push(start);
	p_label[start] = true;
	int cur_vertex = 0;
	while (!vertex_stack.empty())
	{
		cur_vertex = vertex_stack.top();
		if (cur_vertex == end)
			break;
		if (!adj[cur_vertex]->empty())
		{
			adj_list<T>::iterator citer;
			citer = adj[cur_vertex]->begin();
			while (citer != NULL)
			{
				int adj_vertex = (*citer).vertex_index;
				if (!p_label[adj_vertex])
				{
					vertex_stack.push(adj_vertex);
					p_label[adj_vertex] = true;
					break;
				}
				++citer;
			}
			if (citer == NULL)
				vertex_stack.pop();
		}
		else
			vertex_stack.pop();
	}
	if (!vertex_stack.empty())
	{
		cout << "The path between start and end:\n";
		stack<int> path_stack;
		while (!vertex_stack.empty())
		{
			int temp_vertex = vertex_stack.top();
			vertex_stack.pop();
			path_stack.push(temp_vertex);
		}
		while (!path_stack.empty())
		{
			cout << path_stack.top() << " -> ";
			path_stack.pop();
		}
		cout << "\n";
	}
	else
		cout << "There is no path between start and end!\n";
	delete[] p_label; p_label = NULL;
	return;
}

template <class T>
void WD_Graph<T>::Kahn_topo_sort() const
{
	if (!is_directed())
		throw Exception("Invalid Calling!");
	bool *p_label = new bool[vertex_num + 1];
	for (int vertex = 1; vertex <= vertex_num; ++vertex)
		p_label[vertex] = false;
	queue<int> topo_sequence;
	bool loop_condition = true;
	while (loop_condition)
	{
		int increment = 0;
		for (int vertex = 1; vertex <= vertex_num; ++vertex)
		{
			int dynamic_in_degree = 0;
			if (!p_label[vertex])
			{
				for (int pre = 1; pre <= vertex_num; ++pre)
					if (is_edge(pre, vertex) && !p_label[pre])
						++dynamic_in_degree;
				if (dynamic_in_degree == 0)
				{
					p_label[vertex] = true;
					topo_sequence.push(vertex);
					++increment;
				}
			}
		}
		if (increment == 0) //1.剩余的顶点和边构成的子图中存在环 2.所有顶点均已被标记
			loop_condition = false;
	}
	if (topo_sequence.size() == vertex_num)
	{
		cout << "Topological Sequence:\n";
		while (!topo_sequence.empty())
		{
			cout << topo_sequence.front() << " -> ";
			topo_sequence.pop();
		}
		cout << "\n";
	}
	else
		cout << "No Topological Sequence!\n";
}

template<class T>
Graph<T> * WD_Graph<T>::bfs_tree(int start) const
{
	if (!check_vertex(start))
		throw Exception("Invalid Parameter!");
	bool *p_label = new bool[vertex_num + 1];
	for (int v = 0; v <= vertex_num; ++v)
		p_label[v] = false;
	queue<int> vertex_queue;
	vertex_queue.push(start);
	p_label[start] = true;
	int cur_vertex = 0;
	static WD_Graph<T> bfs_spanning_tree(vertex_num);
	//bfs生成树声明为静态对象以保证在成员函数运行结束时不会析构掉
	Graph<T> *p_tree = &bfs_spanning_tree;
	while (!vertex_queue.empty())
	{
		cur_vertex = vertex_queue.front();
		vertex_queue.pop();
		adj_list<T>::iterator citer;
		citer = adj[cur_vertex]->begin();
		while (citer != NULL)
		{
			int adj_vertex = (*citer).vertex_index;
			if (!p_label[adj_vertex])
			{
				T adj_weight = (*citer).weight;
				vertex_queue.push(adj_vertex);
				p_label[adj_vertex] = true;
				bfs_spanning_tree.insert_edge(cur_vertex, adj_vertex, adj_weight);
			}
			++citer;
		}
	}
	delete[] p_label; p_label = NULL;
	return p_tree;
}

template<class T>
Graph<T>* WD_Graph<T>::dfs_tree(int start) const
{
	if (!check_vertex(start))
		throw Exception("Invalid Parameter!");
	bool *p_label = new bool[vertex_num + 1];
	for (int v = 0; v <= vertex_num; ++v)
		p_label[v] = false;
	stack<int> vertex_stack;
	vertex_stack.push(start);
	p_label[start] = true;
	int cur_vertex = 0;
	static WD_Graph<T> dfs_spanning_tree(vertex_num);
	Graph<T> *p_tree = &dfs_spanning_tree;
	while (!vertex_stack.empty())
	{
		cur_vertex = vertex_stack.top();
		if (!adj[cur_vertex]->empty())
		{
			adj_list<T>::iterator citer;
			citer = adj[cur_vertex]->begin();
			int adj_vertex = 0;
			while (citer != NULL)
			{
				adj_vertex = (*citer).vertex_index;
				if (!p_label[adj_vertex])
				{ //进行深度优先搜索
					T adj_weight = (*citer).weight;
					vertex_stack.push(adj_vertex);
					p_label[adj_vertex] = true;
					dfs_spanning_tree.insert_edge(cur_vertex, adj_vertex, adj_weight);
					break;
				}
				++citer;
			}
			if (citer == NULL) //当前顶点所有邻接顶点均已被标记过，采取回退
				vertex_stack.pop();
		}
		else //当前顶点无邻接顶点，采取回退
			vertex_stack.pop();
	}
	delete[] p_label; p_label = NULL;
	return p_tree;
}

template <class T>
WU_Graph<T> * WU_Graph<T>::Kruskal_min_tree() const
{
	edge<T> *p_arr = new edge<T>[edge_num];
	int count = 0;
	for (int v = 1; v <= vertex_num; ++v)
	{
		if (!adj[v]->empty())
		{
			adj_list<T>::iterator citer;
			citer = adj[v]->begin();
			int suc = 0; T wei = 0;
			while (citer != NULL)
			{
				suc = (*citer).vertex_index;
				if (v < suc)
				{
					T wei = (*citer).weight;
					edge<T> current(v, suc, wei);
					p_arr[count] = current;
					++count;
				}
				++citer;
			}
		}
	}
	quick_sort(p_arr, edge_num);
	UnionFind forest(vertex_num);
	int e = 0, edge_count = 0;
	int pre = 0, suc = 0; T wei = 0;
	static WU_Graph<T> min_tree(vertex_num);
	WU_Graph<T> *p_min_tree = &min_tree;
	while (edge_count < (vertex_num - 1) && e < edge_num)
	{
		pre = p_arr[e].precursor;
		suc = p_arr[e].successor;
		if (!forest.is_connected(pre, suc))
		{
			wei = p_arr[e].weight;
			min_tree.insert_edge(pre, suc, wei);
			forest.union_component(pre, suc);
			++edge_count;
		}
		++e;
	}
	if (edge_count == (vertex_num - 1))
		return p_min_tree;
	else
	{
		cout << "No minimal spanning tree of this graph:\n";
		return NULL;
	}
}

template <class T>
WU_Graph<T> * WU_Graph<T>::Prim_min_tree(int start) const
{
	edge<T> *p_arr = new edge<T>[edge_num];
	int count = 0;
	for (int v = 1; v <= vertex_num; ++v)
	{
		if (!adj[v]->empty())
		{
			adj_list<T>::iterator citer;
			citer = adj[v]->begin();
			int suc = 0; T wei = 0;
			while (citer != NULL)
			{
				suc = (*citer).vertex_index;
				if (v < suc)
				{
					T wei = (*citer).weight;
					edge<T> current(v, suc, wei);
					p_arr[count] = current;
					++count;
				}
				++citer;
			}
		}
	}
	quick_sort(p_arr, edge_num);
	bool *p_label = new bool[vertex_num + 1];
	for (int v = 0; v <= vertex_num; ++v)
		p_label[v] = false;
	p_label[start] = true;
	bool *edge_label = new bool[edge_num];
	for (int e = 0; e < edge_num; ++e)
		edge_label[e] = false;
	static WU_Graph<T> min_tree(vertex_num);
	WU_Graph<T> *p_min_tree = &min_tree;
	int u = 0, v = 0, edge_count = 0;
	T wei = 0;
	bool flag = true;
	while (flag && edge_count < (vertex_num - 1))
	{
		flag = false;
		int e = 0;
		while (e < edge_num)
		{
			if (!edge_label[e])
			{
				u = p_arr[e].precursor;
				v = p_arr[e].successor;
				if ((p_label[u] && !p_label[v]) || (!p_label[u] && p_label[v]))
				{
					wei = p_arr[e].weight;
					min_tree.insert_edge(u, v, wei);
					++edge_count;
					flag = true;
					edge_label[e] = true;
					p_label[u] = true; p_label[v] = true;
				}
			}
			++e;
		}
	}
	if (edge_count == (vertex_num - 1))
		return p_min_tree;
	else
	{
		cout << "No minimal spanning tree of this graph:\n";
		return NULL;
	}
}

int main()
{
	typedef Graph<double> G;
	typedef WD_Graph<double> WDG;
	typedef WU_Graph<double> WUG;
	try
	{
#ifdef _GRAPH_1_
		WDG graph1(5);
		graph1.insert_edge(1, 2, 1.0);
		graph1.insert_edge(1, 3, 1.0);
		graph1.insert_edge(2, 4, 1.0);
		graph1.insert_edge(2, 5, 1.0);
		graph1.insert_edge(3, 4, 1.0);
		graph1.insert_edge(3, 5, 1.0);
		/*cout << graph1.num_of_vertex() << "\n";
		cout << graph1.num_of_edge() << "\n";
		for (int i = 1; i <= 5; ++i)
		cout << graph1.out_degree(i) << "\n";
		graph1.insert_edge(4, 5, 1.0);
		cout << graph1.num_of_edge() << "\n";
		graph1.erase_edge(4, 5);
		cout << graph1.num_of_edge() << "\n";
		for (int i = 1; i <= 5; ++i)
		cout << graph1.in_degree(i) << "\n";*/
		graph1.breadth_first_search(1);
		graph1.deepth_first_search(1);
		graph1.dfs_path(1, 5);
		graph1.Kahn_topo_sort();
		G *p_graph = graph1.bfs_tree(1);
		p_graph->breadth_first_search(1);
		p_graph = graph1.dfs_tree(1);
		p_graph->breadth_first_search(1);
#endif

#ifdef _GRAPH_2_
		WUG graph2(8);
		graph2.insert_edge(1, 2, 1.0);
		graph2.insert_edge(1, 5, 1.0);
		graph2.insert_edge(2, 6, 1.0);
		graph2.insert_edge(3, 4, 1.0);
		graph2.insert_edge(3, 6, 1.0);
		graph2.insert_edge(3, 7, 1.0);
		graph2.insert_edge(4, 7, 1.0);
		graph2.insert_edge(4, 8, 1.0);
		graph2.insert_edge(6, 7, 1.0);
		graph2.insert_edge(7, 8, 1.0);
		/*cout << graph2.num_of_vertex() << "\n";
		cout << graph2.num_of_edge() << "\n";
		cout << graph2.num_of_edge() << "\n";
		for (int v = 1; v <= 8; ++v)
			cout << graph2.degree(v) << ", ";
		cout << "******" << "\n";
		for (int u = 1; u <= 8; ++u)
		{
			for (int v = 1; v <= 8; ++v)
				cout << graph2.is_edge(u, v) << ", ";
			cout << "******" << "\n";
		}
		graph2.breadth_first_search(2);*/
		WUG *p_Kruskal_tree = graph2.Kruskal_min_tree();
		p_Kruskal_tree->breadth_first_search(1);
		WUG *p_Prim_tree = graph2.Prim_min_tree(1);
		p_Prim_tree->breadth_first_search(1);

#endif
	}
	catch (Exception running_error)
	{
		running_error.print_exception();
	}
	return 0;
}

