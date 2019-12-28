// ͼ---�ڽӾ�������.cpp : Defines the entry point for the console application.
/* 1. �����ڽӾ�����������ͼ����Ȩ����ͼ�� ��Ȩ����ͼ����Ȩ����ͼ����Ȩ����ͼ����Ȩ����ͼ����һ���ͼ����������
      ͼ����Ϊ��Ȩ����ͼ�������������ˣ���������������Ƶ�˼·�������ɴӼ�Ȩ����ͼ������������ͼ�ࡣ�������
	  ������ͼ�Ļ��������뷽���������һ��������graph��Ϊ���ࡣ
   2. ͼG(V, E)�Ľڵ���������ʾ��|V| = n����n���ڵ���Ϊ1, 2, 3, ..., n����ƽṹ��edge<T>��ʾͼ�ı�, precursor
      ��ʾ�ߵ�ǰ����successor��ʾ�ߵĺ�̡�T��ʾ�ߵ�Ȩ���������ͣ�������Ȩͼ�� TΪbool���ͣ� weight = true��ʾ����
	  �ڵ�֮����ڱߣ� weight = false��ʾ�����ڵ�֮���ޱߣ� no_edge = false; ���ڼ�Ȩͼ�� TΪint��double���ͣ� 
	  no_edgeȡֵӦ�ڱ߼�Ȩ��ֵ��Χ֮�⡣һ��ͼ������������ڵ��������޸ģ�����ͨ��������ɾ�ߵķ����޸ı�����
   3. ͼG(V, E)�Ƿ������������ڽӾ����Ƿ�Գ��ϡ���ͼG������ͼ���ڽӾ���һ���Գƣ���ͼG������ͼ���ڽӾ����ǶԳ�
      ���󣬼�����ͼ����Ϊ���������ͼ��ע�⣬ͼG�����ڻ����������ߣ�����ڽӾ���Խ�Ԫȫ��Ϊno_edge��
*/
#include "stdafx.h"
#include <iostream>
#include <iterator>
#include <queue>
#include <string>
#include <stack>
#include <vector>


using std::cout;
using std::freopen;
using std::iterator;
using std::priority_queue;
using std::queue;
using std::string;
using std::stack;
using std::vector;

const double NOEDGE = -200.0;
const double MAXWEIGHT = 100.0;
const double MINWEIGHT = -100.0;

template <class T>
struct edge
{
	int precursor;
	int successor;
	T weight;
	edge(int i, int j, T theWeight)
	{ precursor = i; successor = j; weight = theWeight; }
	bool operator<(const edge<T> &the_edge) const
	{ return weight > the_edge.weight; } //���ڶ���С���ѵĶ�����
};

template <class T>
class graph
{
public:
	virtual ~graph() {};
	virtual int numberOfVertices() const = 0;
	virtual int numberOfEdges() const = 0;
	virtual bool isEdge(int, int) const = 0; //��������֮���Ƿ���ڱ�
	virtual void insertEdge(edge<T> *) = 0;
	virtual void eraseEdge(int, int) = 0;
	virtual int degree(int) const = 0; //����ͼ����Ķ���
	virtual int inDegree(int) const = 0; //����ͼ��������
	virtual int outDegree(int) const = 0; //����ͼ����ĳ���
	virtual bool directed() const = 0;
	virtual bool weighted() const = 0;
};

template <class T>
class adjacencyWDgraph : public graph<T>
{
protected:
	int vertex_num;
	int edge_num;
	T **adj; //��ʾ�ɱ��ڽӾ���, TΪ���ͻ򸡵���
	T no_edge; //��ʾ�����ڵı�
protected:
	bool checkVertex(int v) const
	{ if (v < 1 || v > vertex_num) return false;
	  else return true; }
public:
	adjacencyWDgraph(int vertex_num = 0, T no_edge = NOEDGE);
	adjacencyWDgraph(const adjacencyWDgraph<T> &source_graph);
	~adjacencyWDgraph();
	int numberOfVertices() const { return vertex_num; }
	int numberOfEdges() const { return edge_num; }
	bool directed() const { return true; }
	bool weighted() const { return true; }
	bool isEdge(int i, int j) const;
	void insertEdge(edge<T> *theEdge);
	void insertEdge(int precursor, int successor, T weight);
	void eraseEdge(int start, int end);
	int degree(int v) const { return -1; }
	int inDegree(int v) const;
	int outDegree(int v) const;
	void breadthFirstSearch(int v) const;
	void deepthFirstSearch(int v) const;
	void DFSpath(int start, int end) const;
	void greedyPath(int start, int end) const; //��һ���ܸ������Ž�
	adjacencyWDgraph<T> * Dijkstra_shortestPaths(int source) const;
	adjacencyWDgraph<T> * Bellman_Ford_shortestPaths(int source) const; //��˼��һ��
	adjacencyWDgraph<T> * DAG_shortestPaths(int source) const;
	void Kahn_topologicalSort() const;
	void greedyTopoSort() const; //������������
};

template <class T>
class adjacencyWUgraph : public adjacencyWDgraph<T>
{
public:
	adjacencyWUgraph(int vertex_num, T no_edge)
		:adjacencyWDgraph<T>::adjacencyWDgraph(vertex_num, no_edge) {}
	~adjacencyWUgraph() {}
	bool directed() const { return false; }
	void insertEdge(edge<T> *theEdge); 
	void insertEdge(int precursor, int successor, T weight); 
	void eraseEdge(int start, int end);
	int degree(int v) const;
	adjacencyWUgraph<T> * BFS_spanningTree(int start) const;
	adjacencyWUgraph<T> * DFS_spanningTree(int start) const;
	adjacencyWUgraph<T> * Kruskal_spanningTree(int start) const; //�����ش�ȱ��
	adjacencyWUgraph<T> * Prim_spanningTree(int start) const; //�����ش�ȱ��
};

template <class T>
class bipartiteGraph : public adjacencyWUgraph<T>
{
protected:
	int partition_num;
public:
	bipartiteGraph(int vertex_num, int partition_num, T no_edge = NOEDGE)
		:adjacencyWUgraph<T>::adjacencyWUgraph(vertex_num, no_edge), partition_num(partition_num) {}
	~bipartiteGraph() {}
	bool directed() const { return false; }
	void insertEdge(edge<T> *theEdge);
	void eraseEdge(int i, int j);
	void bipartiteCover() const;
};

template <class T>
adjacencyWDgraph<T>::adjacencyWDgraph(int vertex_num = 0, T no_edge = NOEDGE)
{
	if (vertex_num < 0)
	{
		cout << "The number of vertex in a graph can not be negative.\n";
		return;
	}
	this->vertex_num = vertex_num;
	this->no_edge = no_edge;
	edge_num = 0;
	adj = new T *[vertex_num + 1];
	adj[0] = NULL;
	for (int i = 1; i <= vertex_num; ++i)
	{
		adj[i] = new T[vertex_num + 1];
		for (int j = 0; j <= vertex_num; ++j)
			adj[i][j] = no_edge;
	}
}

template <class T>
adjacencyWDgraph<T>::adjacencyWDgraph(const adjacencyWDgraph<T> &source_graph)
{
	this->vertex_num = source_graph.vertex_num;
	this->no_edge = source_graph.no_edge;
	this->edge_num = source_graph.edge_num;
	this->adj = new T *[vertex_num + 1];
	adj[0] = NULL;
	for (int i = 1; i <= vertex_num; ++i)
	{
		adj[i] = new T[vertex_num + 1];
		for (int j = 0; j <= vertex_num + 1; ++j)
			adj[i][j] = source_graph.adj[i][j];
	}
}

template <class T>
adjacencyWDgraph<T>::~adjacencyWDgraph()
{
	for (int i = 1; i <= vertex_num; ++i)
	{
		delete[] adj[i];
		adj[i] = NULL;
	}
	delete[] adj;
	adj = NULL;
}

template <class T>
bool adjacencyWDgraph<T>::isEdge(int start, int end) const
{
	if (!checkVertex(start) || !checkVertex(end))
	{
		cout << "Unexisted vertex index.\n";
		return false;
	}
	if (adj[start][end] == no_edge)
		return false;
	else
		return true;
}

template <class T>
void adjacencyWDgraph<T>::insertEdge(edge<T> *theEdge)
{
	int v1 = theEdge->precursor;
	int v2 = theEdge->successor;
	if (!checkVertex(v1) || !checkVertex(v2))
	{
		cout << "There is no such edge in graph.\n";
		return;
	}
	if (adj[v1][v2] == no_edge)
	{
		adj[v1][v2] = theEdge->weight;
		++edge_num;
	}
	else
		adj[v1][v2] = theEdge->weight;
}

template <class T>
void adjacencyWDgraph<T>::insertEdge(int precursor, int successor, T weight)
{
	bool b1 = checkVertex(precursor), b2 = checkVertex(successor);
	if (b1 && b2)
	{
		if (adj[precursor][successor] == no_edge)
		{
			adj[precursor][successor] = weight;
			++edge_num;
		}
		else
			adj[precursor][successor] = weight;
	}
	else
		cout << "Wrong parameter!\n";
}

template <class T>
void adjacencyWUgraph<T>::insertEdge(edge<T> *theEdge)
{
	int v1 = theEdge->precursor; 
	int v2 = theEdge->successor;
	bool b1 = adjacencyWDgraph<T>::checkVertex(v1);
	bool b2 = adjacencyWDgraph<T>::checkVertex(v2);
	if (b1 && b2)
	{
		if (adj[v1][v2] == no_edge)
		{
			adj[v1][v2] = theEdge->weight;
			adj[v2][v1] = theEdge->weight;
			++edge_num;
		}
		else
		{
			adj[v1][v2] = theEdge->weight;
			adj[v2][v1] = theEdge->weight;
		}
	}
	else
		cout << "Wrong parameter!\n";
}

template <class T>
void adjacencyWUgraph<T>::insertEdge(int precursor, int successor, T weight)
{
	bool b1 = adjacencyWDgraph<T>::checkVertex(precursor);
	bool b2 = adjacencyWDgraph<T>::checkVertex(successor);
	if (b1 && b2)
	{
		if (adj[precursor][successor] == no_edge)
		{
			adj[precursor][successor] = weight;
			adj[successor][precursor] = weight;
			++edge_num;
		}
		else
		{
			adj[precursor][successor] = weight;
			adj[successor][precursor] = weight;
		}
	}
	else
		cout << "Wrong parameter!\n";
}

template <class T>
void bipartiteGraph<T>::insertEdge(edge<T> *theEdge)
{
	int v1 = theEdge->precursor;
	int v2 = theEdge->successor;
	bool b1 = adjacencyWDgraph<T>::checkVertex(v1);
	bool b2 = adjacencyWDgraph<T>::checkVertex(v2);
	if (!b1 || !b2)
	{
		cout << "There is no such edge in graph.\n";
		return;
	}
	if ((v1 <= partition_num && v2 > partition_num) || (v1 > partition_num && v2 <= partition_num))
	{
		if (adj[v1][v2] == no_edge)
		{
			adj[v1][v2] = theEdge->weight;
			adj[v2][v1] = theEdge->weight;
			++edge_num;
		}
		else
		{
			adj[v1][v2] = theEdge->weight;
			adj[v2][v1] = theEdge->weight;
		}
	}
	else
		cout << "Invalid parameter!\n";
}

template <class T>
void adjacencyWDgraph<T>::eraseEdge(int start, int end)
{
	bool b1 = checkVertex(start);
	bool b2 = checkVertex(end);
	if (!b1 || !b2)
	{
		cout << "Unexisted vertex index.\n";
		return;
	}
	if (adj[start][end] == no_edge)
	{
		cout << "No edge betwwen vertex " << start << "and vextex " << end;
		cout << '\n';
	}
	else
	{
		adj[start][end] = no_edge;
		--edge_num;
	}
}

template <class T>
void adjacencyWUgraph<T>::eraseEdge(int start, int end)
{
	bool b1 = adjacencyWDgraph<T>::checkVertex(start);
	bool b2 = adjacencyWDgraph<T>::checkVertex(end);
	if (!b1 || !b2)
	{
		cout << "Unexisted vertex index.\n";
		return;
	}
	if (adj[start][end] == no_edge)
	{
		cout << "No edge betwwen vertex " << start << "and vextex " << end;
		cout << '\n';
	}
	else
	{
		adj[start][end] = no_edge;
		adj[end][start] = no_edge;
		--edge_num;
	}
}

template <class T>
void bipartiteGraph<T>::eraseEdge(int start, int end)
{
	bool b1 = adjacencyWDgraph<T>::checkVertex(start);
	bool b2 = adjacencyWDgraph<T>::checkVertex(end);
	if (!b1 || !b2)
	{
		cout << "Unexisted vertex index.\n";
		return;
	}
	if ((start <= partition_num && end > partition_num) || (start > partition_num && end <= partition_num))
	{
		if (adj[start][end] == no_edge)
		{
			cout << "No edge betwwen vertex " << start << "and vextex " << end;
			cout << '\n';
		}
		else
		{
			adj[start][end] = no_edge;
			adj[end][start] = no_edge;
			--edge_num;
		}
	}
	else
		cout << "Invalid parameter!\n";
}

template <class T>
int adjacencyWUgraph<T>::degree(int v) const
{
	if (adjacencyWDgraph<T>::checkVertex(v))
	{
		int in_degree = 0;
		for (int i = 1; i <= vertex_num; ++i)
			if (adj[v][i] != no_edge)
				++in_degree;
		return in_degree; //ͼ�еĹ����������Ϊ0
	}
	else
	{
		cout << "Vertex " << v << " is not in graph.\n";
		return -1;
	}
}

template <class T>
int adjacencyWDgraph<T>::inDegree(int v) const
{
	if (!directed())
	{
		cout << "The indegree is not defined for undirected graph!\n";
		return -1;
	}
	if (checkVertex(v))
	{
		int in_degree = 0;
		for (int i = 1; i <= vertex_num; ++i)
			if (adj[i][v] != no_edge)
				++in_degree;
		return in_degree; 
	}
	else
	{
		cout << "Vertex " << v << " is not in graph.\n";
		return -1; 
	}
}

template <class T>
int adjacencyWDgraph<T>::outDegree(int v) const
{
	if (!directed())
	{
		cout << "The outdegree is not defined for undirected graph!\n";
		return -1;
	}
	if (checkVertex(v))
	{
		int out_degree = 0;
		for (int i = 1; i <= vertex_num; ++i)
			if (adj[v][i] != no_edge)
				++out_degree;
		return out_degree;
	}
	else
	{
		cout << "Vertex " << v << " is not in graph.\n";
		return -1;
	}
}

template <class T>
void adjacencyWDgraph<T>::breadthFirstSearch(int v) const
{
	if (!checkVertex(v))
	{
		cout << "There is no such vertex in graph!\n";
		return;
	}
	queue<int> vertex_queue;
	vertex_queue.push(v);
	bool *p_visited = new bool[vertex_num + 1];
	for (int i = 0; i <= vertex_num; ++i)
		p_visited[i] = false;
	p_visited[v] = true;
	cout << "Path of breadth first search from " << v << ":\n";
	while (!vertex_queue.empty())
	{
		int j = vertex_queue.front();
		vertex_queue.pop();
		cout << j << " -> ";
		for (int k = 1; k <= vertex_num; ++k)
			if (adj[j][k] != no_edge && !p_visited[k])
			{
				vertex_queue.push(k);
				p_visited[k] = true;
			}
	}
	cout << "\n";
	delete[] p_visited; p_visited = NULL;
}

template <class T>
void adjacencyWDgraph<T>::deepthFirstSearch(int v) const
{
	if (!checkVertex(v))
	{
		cout << "There is no such vertex in graph!\n";
		return;
	}
	stack<int> vertex_stack;
	vertex_stack.push(v);
	bool *p_visited = new bool[vertex_num + 1];
	for (int i = 0; i <= vertex_num; ++i)
		p_visited[i] = false;
	p_visited[v] = true;
	int current = v;
	cout << "Path of deepth first search from " << v << ":\n";
	while (!vertex_stack.empty())
	{
		int j = 1;
		for (; j <= vertex_num; ++j)
			if (adj[current][j] != no_edge && !p_visited[j])
			{
				vertex_stack.push(j);
				p_visited[j] = true;
				cout << current << " -> " << j << ", ";
				current = j;
				break;
			}
		if (checkVertex(j))
			continue; //��ʼ��һ�������������
		else
		{ //��ǰ�ڵ����ڽӽڵ�������ڽӽڵ���ѱ����Ϊ�ѷ��ʣ������
			current = vertex_stack.top();
			vertex_stack.pop(); 
			for (int k = 1; k <= vertex_num; ++k)
				if (adj[current][k] != no_edge && !p_visited[k])
					vertex_stack.push(current);
		}
	} //����ʼ�ڵ������ڽӽڵ�������Ϊ�ѷ��ʣ��ڵ�ջΪ��ʱwhileѭ������
	cout << "\n";
	delete[] p_visited; p_visited = NULL;
}

template <class T>
void adjacencyWDgraph<T>::DFSpath(int start, int end) const
{
	if (!checkVertex(start) || !checkVertex(end))
	{
		cout << "Unexisted vertex!\n";
		return;
	}
	if (start == end)
	{
		cout << "Start vertex is same to end vertex!";
		return;
	}
	stack<int> vertex_stack;
	vertex_stack.push(start);
	bool *p_visited = new bool[vertex_num + 1];
	for (int i = 0; i <= vertex_num; ++i)
		p_visited[i] = false;
	p_visited[start] = true;
	int current = start;
	while (!vertex_stack.empty())
	{
		int j = 1;
		for (; j <= vertex_num; ++j)
			if (adj[current][j] != no_edge && !p_visited[j])
			{
				vertex_stack.push(j);
				p_visited[j] = true;
				current = j;
				break;
			}
		if (j == end)
		{
			int length = vertex_stack.size();
			stack<int> path;
			for (int k = 0; k < length; ++k)
			{
				path.push(vertex_stack.top());
				vertex_stack.pop();
			}
			cout << "DFS path with start " << start << " and end " << end << ":\n";
			while (!path.empty())
			{
				cout << path.top() << " -> ";
				path.pop();
			}
			cout << "\n";
			return;
		}
		else
		{
			if (checkVertex(j))
				continue;
			else
			{
				current = vertex_stack.top();
				vertex_stack.pop();
				for (int k = 1; k <= vertex_num; ++k)
					if (adj[current][k] != no_edge && !p_visited[k])
						vertex_stack.push(current);
			}
		}
	}
	cout << "There is no such path!\n";
	delete[] p_visited; p_visited = NULL;
}

template <class T>
void adjacencyWDgraph<T>::greedyPath(int start, int end) const
{
	if (!checkVertex(start) || !checkVertex(end))
	{
		cout << "Wrong parameter!\n";
		return;
	}
	if (start == end)
	{
		cout << "Start vertex is same to end vertex!";
		return;
	}
	queue<int> vertex_queue;
	vertex_queue.push(start);
	bool *p_label = new bool[vertex_num + 1];
	for (int i = 0; i <= vertex_num; ++i)
		p_label[i] = false;
	p_label[start] = true;
	int current = start;
	while (current != end)
	{
		int reach = 0;
		T last = MAXWEIGHT + 1; //�ٶ���Ȩ�����ֵΪ10.0
		int j = 1;
		for (; j <= vertex_num; ++j)
			if (adj[current][j] != no_edge && !p_label[j])
				if (adj[current][j] < last)
				{
					last = adj[current][j];
					reach = j;
				}
		if (reach == 0)
		{
			cout << "No path!\n";
			delete[] p_label; p_label = NULL;
			return;
		}
		p_label[reach] = true;
		vertex_queue.push(reach);
		current = reach;
	}
	cout << "The path of minimal weight:\n";
	while (!vertex_queue.empty())
	{
		cout << vertex_queue.front() << " -> ";
		vertex_queue.pop();
	}
	cout << "\n";
	delete[] p_label; p_label = NULL;
}

template <class T>
adjacencyWDgraph<T> * adjacencyWDgraph<T>::Dijkstra_shortestPaths(int source) const
{
	T infinite = 2 * MAXWEIGHT * edge_num; //��һ���㹻��������������
	int *p_precursor = new int[vertex_num + 1]; //ʵ��������p_precursor��ʾ���ɵ����·����
	T *p_distance = new T[vertex_num + 1]; //ÿ���ڵ㵽Դ������·������
	bool *p_label = new bool[vertex_num + 1];
	for (int i = 0; i <= vertex_num; ++i)
	{
		p_precursor[i] = 0;
		p_distance[i] = infinite;
		p_label[i] = false;
	}
	p_distance[source] = 0;
	adjacencyWDgraph<T> shortest_path_tree(vertex_num, no_edge);
	struct vertex_distance
	{
		int vertex;
		T distance;
		vertex_distance(int vertex, T distance) { this->vertex = vertex; this->distance = distance; }
		bool operator<(const vertex_distance &compare) const
		{ return distance > compare.distance; } //const vertex_distance<T> &compare����������
	};
	priority_queue<vertex_distance> vertex_heap; //ÿ���ڵ��distance������С���ȶ���
	vertex_distance current(source, 0);
	vertex_heap.push(current);
	while (!vertex_heap.empty())
	{
		current = vertex_heap.top();
		vertex_heap.pop();
		p_label[current.vertex] = true;
		for (int j = 1; j <= vertex_num; ++j)
			if (adj[current.vertex][j] != no_edge && !p_label[j])
			{
				if (p_distance[j] > (p_distance[current.vertex] + adj[current.vertex][j]))
				{
					p_distance[j] = p_distance[current.vertex] + adj[current.vertex][j];
					p_precursor[j] = current.vertex;
				}
			}
		while (!vertex_heap.empty())
			vertex_heap.pop(); //�����С���ȶ���
		for (int v = 1; v <= vertex_num; ++v)
		{
			if (!p_label[v] && p_precursor[v] != 0)
			{ //���¼�������һ���ɳڲ�����δ����ǵĽڵ㣬��֤�ڵ�����С���ȶ��в��ظ�����
				//current(v, p_distance[v]);
				current.vertex = v; current.distance = p_distance[v];
				vertex_heap.push(current);
			}
		}
	}
	for (int i = 1; i <= vertex_num; ++i)
		if (p_precursor[i] != 0)
		{
			int i_pre = p_precursor[i];
			edge<T> current_edge(i_pre, i, adj[i_pre][i]);
			shortest_path_tree.insertEdge(&current_edge);
			cout << "The minimal distance of " << i << ": " << p_distance[i];
			cout << "\n";
		}
	delete[] p_precursor; p_precursor = NULL;
	delete[] p_distance; p_distance = NULL;
	delete[] p_label; p_label = NULL;
	return &shortest_path_tree; 
}

template <class T>
adjacencyWDgraph<T> * adjacencyWDgraph<T>::Bellman_Ford_shortestPaths(int source) const
{
	T infinite = 2 * MAXWEIGHT * edge_num; 
	int *p_precursor = new int[vertex_num + 1]; 
	T *p_distance = new T[vertex_num + 1]; 
	for (int i = 0; i <= vertex_num; ++i)
	{
		p_precursor[i] = 0;
		p_distance[i] = infinite;
	}
	p_distance[source] = 0;
	adjacencyWDgraph<T> shortest_path_tree(vertex_num, no_edge);
	queue<int> vertex_queue;
	int current;
	bool *p_label = new bool[vertex_num + 1];
	for (int round = 0; round < vertex_num; ++round)
	{
		vertex_queue.push(source);
		current = source;
		for (int n = 0; n <= vertex_num; ++n)
			p_label[n] = false;
		p_label[current] = true;
		while (!vertex_queue.empty())
		{
			current = vertex_queue.front();
			vertex_queue.pop();
			for (int j = 1; j <= vertex_num; ++j)
				if (adj[current][j] != no_edge)
				{
					if (!p_label[j])
					{ //��һ��ѭ���У�ÿ���ɵ��ﶥ�������ֻ����һ��
						vertex_queue.push(j);
						p_label[j] = true;
					}
					if (p_distance[j] > (p_distance[current] + adj[current][j]))
					{ //��һ��ѭ���У�ÿ�����ɳ���ֻ�ɳ�һ��
						p_distance[j] = (p_distance[current] + adj[current][j]);
						p_precursor[j] = current;
					}
				}
		}
	}
	vertex_queue.push(source);
	current = source;
	for (int n = 0; n <= vertex_num; ++n)
		p_label[n] = false;
	p_label[current] = true;
	while (!vertex_queue.empty())
	{
		current = vertex_queue.front();
		vertex_queue.pop();
		for (int j = 1; j <= vertex_num; ++j)
			if (adj[current][j] != no_edge)
			{
				if (!p_label[j])
				{ 
					vertex_queue.push(j);
					p_label[j] = true;
				}
				if (p_distance[j] > (p_distance[current] + adj[current][j]))
				{ 
					cout << "No shortest path tree!\n";
					return NULL;
				}
			}
	}
	for (int i = 1; i <= vertex_num; ++i)
		if (p_precursor[i] != 0)
		{
			int i_pre = p_precursor[i];
			edge<T> current_edge(i_pre, i, adj[i_pre][i]);
			shortest_path_tree.insertEdge(&current_edge);
			cout << "The minimal distance of " << i << ": " << p_distance[i];
			cout << "\n";
		}
	if (p_precursor)
	{
		delete[] p_precursor; p_precursor = NULL;
	}
	delete[] p_distance; p_distance = NULL;
	delete[] p_label; p_label = NULL;
	return &shortest_path_tree;
}

template <class T>
adjacencyWDgraph<T> * adjacencyWDgraph<T>::DAG_shortestPaths(int source) const
{
	queue<int> no_incoming;
	bool *p_label = new bool[vertex_num + 1];
	for (int i = 0; i <= vertex_num; ++i)
		p_label[i] = false;
	for (int i = 1; i <= vertex_num; ++i)
		if (inDegree(i) == 0)
		{
			no_incoming.push(i);
			p_label[i] = true;
		}
	T **adj_copy = new T *[vertex_num + 1]; 
	adj_copy[0] = NULL;
	for (int i = 1; i <= vertex_num; ++i)
	{
		adj_copy[i] = new T[vertex_num + 1];
		for (int j = 0; j <= vertex_num; ++j)
			adj_copy[i][j] = adj[i][j];
	}
	queue<int> topo_sequence;
	while (!no_incoming.empty())
	{
		int current = no_incoming.front();
		no_incoming.pop();
		topo_sequence.push(current);
		for (int k = 1; k <= vertex_num; ++k)
			if (adj_copy[current][k] != no_edge)
				adj_copy[current][k] = no_edge;
		for (int vertex = 1; vertex <= vertex_num; ++vertex)
		{
			int in_degree = 0;
			for (int n = 1; n <= vertex_num; ++n)
				if (adj_copy[n][vertex] != no_edge)
					++in_degree;
			if (in_degree == 0 && !p_label[vertex])
			{
				no_incoming.push(vertex);
				p_label[vertex] = true;
			} 
		}
	}
	if (topo_sequence.size() != vertex_num)
	{
		cout << "The algorithm does not work for this graph!\n";
		return NULL;
	}
	T infinite = 2 * MAXWEIGHT * edge_num; 
	int *p_precursor = new int[vertex_num + 1]; 
	T *p_distance = new T[vertex_num + 1]; 
	for (int i = 0; i <= vertex_num; ++i)
	{
		p_precursor[i] = 0;
		p_distance[i] = infinite;
	}
	p_distance[source] = 0;
	adjacencyWDgraph<T> shortest_path_tree(vertex_num, no_edge);
	int current = topo_sequence.front();
	while (current != source)
	{
		topo_sequence.pop();
		current = topo_sequence.front();
	}
	while (!topo_sequence.empty())
	{
		current = topo_sequence.front();
		for (int v = 1; v <= vertex_num; ++v)
			if (adj[current][v] != no_edge)
				if (p_distance[v] > (p_distance[current] + adj[current][v]))
				{
					p_distance[v] = p_distance[current] + adj[current][v];
					p_precursor[v] = current;
				}
		topo_sequence.pop();
	}
	for (int i = 1; i <= vertex_num; ++i)
		if (p_precursor[i] != 0)
		{
			int i_pre = p_precursor[i];
			edge<T> current_edge(i_pre, i, adj[i_pre][i]);
			shortest_path_tree.insertEdge(&current_edge);
			cout << "The minimal distance of " << i << ": " << p_distance[i];
			cout << "\n";
		}
	for (int i = 1; i <= vertex_num; ++i)
	{
		delete[] adj_copy[i];
		adj_copy[i] = NULL;
	}
	delete[] adj_copy; adj_copy = NULL;
	delete[] p_precursor; p_precursor = NULL;
	delete[] p_distance; p_distance = NULL;
	delete[] p_label; p_label = NULL;
	return &shortest_path_tree;
}

template <class T>
void adjacencyWDgraph<T>::Kahn_topologicalSort() const
{
	if (!directed())
	{
		cout << "The topological sequence is not defined for undirected graph!\n";
		return;
	}
	queue<int> no_incoming; //��ǰͼ�����Ϊ0����ļ���
	bool *p_label = new bool[vertex_num + 1];
	for (int i = 0; i <= vertex_num; ++i)
		p_label[i] = false;
	for (int i = 1; i <= vertex_num; ++i)
		if (inDegree(i) == 0)
		{
			no_incoming.push(i);
			p_label[i] = true;
		} //��ʼ��no_incoming����Ǽ���Ķ���
	T **adj_copy = new T *[vertex_num + 1]; //�Ƿ�����ٽ�һ���Ż�����һ���Ʋ���S(N),O(N)��������N^2
	adj_copy[0] = NULL;
	for (int i = 1; i <= vertex_num; ++i)
	{
		adj_copy[i] = new T[vertex_num + 1];
		for (int j = 0; j <= vertex_num; ++j)
			adj_copy[i][j] = adj[i][j];
	}
	queue<int> topo_sequence;
	while (!no_incoming.empty())
	{
		int current = no_incoming.front();
		no_incoming.pop();
		topo_sequence.push(current);
		for (int k = 1; k <= vertex_num; ++k)
			if (adj_copy[current][k] != no_edge)
				adj_copy[current][k] = no_edge;
		for (int vertex = 1; vertex <= vertex_num; ++vertex)
		{
			int in_degree = 0;
			for (int n = 1; n <= vertex_num; ++n)
				if (adj_copy[n][vertex] != no_edge)
					++in_degree;
			if (in_degree == 0 && !p_label[vertex])
			{
				no_incoming.push(vertex);
				p_label[vertex] = true;
			} //���µ����Ϊ0�Ķ������no_incoming�в����
		}
	}
	if (topo_sequence.size() == vertex_num)
	{
		cout << "Corresponded topological sequence generated from this graph:\n";
		while (!topo_sequence.empty())
		{
			cout << topo_sequence.front() << " -> ";
			topo_sequence.pop();
		}
		cout << "\n";
	}
	else //ͼ�д��ڻ�����
		cout << "No topological sequence for this graph!\n";
	for (int i = 1; i <= vertex_num; ++i)
	{
		delete[] adj_copy[i];
		adj_copy[i] = NULL;
	}
	delete[] adj_copy; adj_copy = NULL;
	delete[] p_label; p_label = NULL;
}

template <class T>
void adjacencyWDgraph<T>::greedyTopoSort() const
{
	queue<int> vertex_queue; //�洢��������
	bool *p_label = new bool[vertex_num + 1];
	for (int i = 0; i <= vertex_num; ++i)
		p_label[i] = false;
	int before = 0;
	int after = 1;
	while (before != after)
	{
		before = vertex_queue.size();
		int k = 1;
		for (; k <= vertex_num; ++k)
			if (!p_label[k])
			{
				int j = 1;
				for (; j <= vertex_num; ++j)
					if (adj[j][k] != no_edge && !p_label[j])
						break;
				if (j <= vertex_num)
					continue;
				else
				{
					vertex_queue.push(k);
					p_label[k] = true;
					break;
				}
			}
		after = vertex_queue.size();
	}
	if (vertex_queue.size() == vertex_num)
	{
		cout << "Topological sequence generated from this graph:\n";
		while (!vertex_queue.empty())
		{
			cout << vertex_queue.front() << " -> ";
			vertex_queue.pop();
		}
		cout << "\n";
	}
	else
		cout << "No topological sequence for this graph!\n";
	delete[] p_label; p_label = NULL;
}

template<class T>
adjacencyWUgraph<T> *adjacencyWUgraph<T>::BFS_spanningTree(int start) const
{
	if (!adjacencyWDgraph<T>::checkVertex(start))
	{
		cout << "Wrong parameter!\n";
		return NULL;
	}
	queue<int> vertex_queue;
	vertex_queue.push(start);
	bool *p_visited = new bool[vertex_num + 1];
	for (int i = 0; i <= vertex_num; ++i)
		p_visited[i] = false;
	p_visited[start] = true;
	adjacencyWUgraph<T> spanning_tree(vertex_num, no_edge);
	int tree_edge = 0;
	while (!vertex_queue.empty())
	{
		int current = vertex_queue.front();
		vertex_queue.pop();
		for (int j = 1; j <= vertex_num; ++j)
			if (adj[current][j] != no_edge && !p_visited[j])
			{
				vertex_queue.push(j);
				edge<T> new_edge(current, j, adj[current][j]);
				spanning_tree.insertEdge(&new_edge);
				++tree_edge;
				p_visited[j] = true;
			}
	}
	delete[] p_visited; p_visited = NULL;
	if (tree_edge == (vertex_num - 1))
	{
		cout << "spanning tree:\n";
		spanning_tree.adjacencyWDgraph<T>::breadthFirstSearch(start);
		return &spanning_tree;
	}
	else
	{
		cout << "No spanning tree!\n";
		return NULL;
	}
}

template<class T>
adjacencyWUgraph<T> * adjacencyWUgraph<T>::DFS_spanningTree(int start) const
{
	if (!adjacencyWDgraph<T>::checkVertex(start))
	{
		cout << "Wrong parameter!\n";
		return NULL;
	}
	stack<int> vertex_stack;
	vertex_stack.push(start);
	bool *p_visited = new bool[vertex_num + 1];
	for (int i = 0; i <= vertex_num + 1; ++i)
		p_visited[i] = false;
	p_visited[start] = true;
	int current = start;
	adjacencyWUgraph<T> spanning_tree(vertex_num, no_edge);
	int tree_edge = 0;
	while (!vertex_stack.empty())
	{
		int j = 1;
		for (; j <= vertex_num; ++j)
			if (adj[current][j] != no_edge && !p_visited[j])
			{
				vertex_stack.push(j);
				edge<T> new_edge(current, j, adj[current][j]);
				spanning_tree.insertEdge(&new_edge);
				++tree_edge;
				p_visited[j] = true;
				current = j;
				break;
			}
		if (checkVertex(j))
			continue;
		else
		{
			current = vertex_stack.top();
			vertex_stack.pop();
			for (int k = 1; k <= vertex_num; ++k)
				if (adj[current][k] != no_edge && !p_visited[k])
					vertex_stack.push(current);
		}
	}
	delete[] p_visited; p_visited = NULL;
	if (tree_edge == (vertex_num - 1))
	{
		cout << "spanning tree:\n";
		spanning_tree.adjacencyWDgraph<T>::breadthFirstSearch(start);
		return &spanning_tree;
	}
	else
	{
		cout << "No spanning tree!\n";
		return NULL;
	}
}

template<class T>
adjacencyWUgraph<T>* adjacencyWUgraph<T>::Kruskal_spanningTree(int start) const
{
	if (!adjacencyWDgraph<T>::checkVertex(start))
	{
		cout << "Wrong parameter!\n";
		return NULL;
	}
	vector<edge<T> > edge_vector;
	priority_queue<edge<T> > edge_min_heap;
	for (int i = 1; i <= vertex_num; ++i)
		for (int j = i + 1; j <= vertex_num; ++j)
			if (adj[i][j] != no_edge)
			{
				edge<T> cur_edge(i, j, adj[i][j]);
				edge_vector.push_back(cur_edge);
			}
	vector<edge<T> >::iterator iter = edge_vector.begin();
	while (!edge_vector.empty())
	{
		edge_min_heap.push(edge_vector.front());
		edge_vector.erase(iter);
		iter = edge_vector.begin();
	}
	while (!edge_min_heap.empty())
	{
		edge_vector.push_back(edge_min_heap.top());
		edge_min_heap.pop();
	}
	adjacencyWUgraph<T> minimal_tree(vertex_num, no_edge);
	bool *p_label = new bool[vertex_num + 1];
	for (int i = 0; i <= vertex_num; ++i)
		p_label[i] = false;
	edge<T> initial_edge = edge_vector.front();
	vector<edge<T> >::iterator current = edge_vector.begin();
	edge_vector.erase(current);
	minimal_tree.insertEdge(&initial_edge);
	p_label[initial_edge.precursor] = true;
	p_label[initial_edge.successor] = true;
	while (!edge_vector.empty())
	{
		current = edge_vector.begin();
		for (; current != edge_vector.end(); ++current)
		{
			bool b1 = p_label[current->precursor];
			bool b2 = p_label[current->successor];
			if ((b1 || b2) && !(b1 && b2))  //�������
				break;
		}
		if (current != edge_vector.end())
		{
			edge_vector.erase(current);
			edge<T> cur_edge(current->precursor, current->successor, current->weight);
			minimal_tree.insertEdge(&cur_edge); //���ܽ���������Ϊ����
			p_label[current->precursor] = true;
			p_label[current->successor] = true;
		}
		else
		{
			delete[] p_label; p_label = NULL;
			if (minimal_tree.edge_num == (edge_num - 1))
			{
				cout << "minimal spanning tree:\n";
				minimal_tree.adjacencyWDgraph<T>::breadthFirstSearch(start);
				return &minimal_tree;
			}
			else
			{
				cout << "No minimal spanning tree!\n";
				return NULL;
			}
		}
	}
	cout << "The graph is unconnected!\n";
	delete[] p_label; p_label = NULL;
	return NULL;
}

template<class T>
adjacencyWUgraph<T>* adjacencyWUgraph<T>::Prim_spanningTree(int start) const
{
	if (!adjacencyWDgraph<T>::checkVertex(start))
	{
		cout << "Wrong parameter!\n";
		return NULL;
	}
	vector<edge<T> > edge_vector;
	priority_queue<edge<T> > edge_min_heap;
	for (int i = 1; i <= vertex_num; ++i)
		for (int j = i + 1; j <= vertex_num; ++j)
			if (adj[i][j] != no_edge)
			{
				edge<T> cur_edge(i, j, adj[i][j]);
				edge_vector.push_back(cur_edge);
			}
	vector<edge<T> >::iterator iter = edge_vector.begin();
	while (!edge_vector.empty())
	{
		edge_min_heap.push(edge_vector.front());
		edge_vector.erase(iter);
		iter = edge_vector.begin();
	}
	while (!edge_min_heap.empty())
	{
		edge_vector.push_back(edge_min_heap.top());
		edge_min_heap.pop();
	}
	adjacencyWUgraph<T> minimal_tree(vertex_num, no_edge);
	bool *p_label = new bool[vertex_num + 1];
	for (int i = 0; i <= vertex_num; ++i)
		p_label[i] = false;
	p_label[start] = true;
	vector<edge<T> >::iterator current = edge_vector.begin();
	while (!edge_vector.empty())
	{
		current = edge_vector.begin();
		for (; current != edge_vector.end(); ++current)
		{
			bool b1 = p_label[current->precursor];
			bool b2 = p_label[current->successor];
			if ((b1 || b2) && !(b1 && b2))
				break;
		}
		if (current != edge_vector.end())
		{
			edge_vector.erase(current);
			edge<T> cur_edge(current->precursor, current->successor, current->weight);
			minimal_tree.insertEdge(&cur_edge);
			p_label[current->precursor] = true;
			p_label[current->successor] = true;
		}
		else
		{
			delete[] p_label; p_label = NULL;
			if (minimal_tree.edge_num == (edge_num - 1))
			{
				cout << "minimal spanning tree:\n";
				minimal_tree.adjacencyWDgraph<T>::breadthFirstSearch(start);
				return &minimal_tree;
			}
			else
			{
				cout << "No minimal spanning tree!\n";
				return NULL;
			}
		}
	}
	cout << "The graph is unconnected!\n";
	delete[] p_label; p_label = NULL;
	return NULL;
}

template <class T>
void bipartiteGraph<T>::bipartiteCover() const
{
	int *pa_cover = new int[partition_num + 1];
	bool *pa_label = new bool[partition_num + 1];
	pa_cover[0] = 0; pa_label[0] = false;
	for (int i = 1; i <= partition_num; ++i)
	{
		pa_cover[i] = degree(i);
		pa_label[i] = false;
	}
	int b_num = vertex_num - partition_num;
	bool *pb_label = new bool[b_num + 1];
	for (int i = 0; i <= b_num; ++i)
		pb_label[i] = false;
	struct vertex_cover
	{
		int vertex;
		int cover_num;
		vertex_cover(int vertex, int cover_num) 
		{ this->vertex = vertex; this->cover_num = cover_num; }
		bool operator<(const vertex_cover &obj)
		{ return cover_num < obj.cover_num; }
	};
	priority_queue<vertex_cover> cover_heap;
	for (int i = 1; i <= partition_num; ++i)
	{
		vertex_cover a_cover(i, pa_cover[i]);
		cover_heap.push(a_cover);
	}
	int b_labeled = 0;
	while (b_labeled != b_num && !cover_heap.empty())
	{
		vertex_cover current = cover_heap.top();
		pa_label[current.vertex] = true;
		cover_heap.pop();
		for (int b = partition_num + 1; b <= vertex_num; ++b)
			if (adj[current.vertex][b] != no_edge && !pb_label[b - partition_num])
			{
				pb_label[b - partition_num] = true;
				++b_labeled;
			}
		for (int a = 1; a <= partition_num; ++a)
			if (!pa_label[a])
				for (int b = partition_num + 1; b <= vertex_num; ++b)
					if (adj[a][b] != no_edge && pb_label[b - partition_num])
						--pa_cover[a];
		while (!cover_heap.empty())
			cover_heap.pop();
		for (int a = 1; a <= partition_num; ++a)
			if (!pa_label[a] && pa_cover[a] > 0)
			{
				vertex_cover new_cover(a, pa_cover[a]);
				cover_heap.push(new_cover);
			}
	}
	if (b_labeled == b_num)
	{
		cout << "The cover set exsits.\n";
		for (int i = 1; i <= partition_num; ++i)
			if (pa_label[i])
				cout << "vertex: " << i << ", num of coverage: " << pa_cover[i] << "\n";
	}
	else
		cout << "Do not find solution!\n";
}

int main()
{
	typedef edge<double> E;
	typedef adjacencyWDgraph<double> WDG;
	typedef adjacencyWUgraph<double> WUG;
	typedef adjacencyWUgraph<bool> UUG;
	typedef edge<bool> UE;
	freopen("20191208.txt", "w", stdout);
	WDG graph1(5, NOEDGE);
	E g1_edge1_2(1, 2, 6.0); graph1.insertEdge(&g1_edge1_2);
	E g1_edge1_3(1, 3, 7.0); graph1.insertEdge(&g1_edge1_3);
	E g1_edge2_3(2, 3, 8.0); graph1.insertEdge(&g1_edge2_3);
	E g1_edge2_4(2, 4, 5.0); graph1.insertEdge(&g1_edge2_4);
	E g1_edge2_5(2, 5, -4.0); graph1.insertEdge(&g1_edge2_5);
	E g1_edge3_4(3, 4, -3.0); graph1.insertEdge(&g1_edge3_4);
	E g1_edge3_5(3, 5, 9.0); graph1.insertEdge(&g1_edge3_5);
	E g1_edge4_2(4, 2, -2.0); graph1.insertEdge(&g1_edge4_2);
	E g1_edge5_1(5, 1, 2.0); graph1.insertEdge(&g1_edge5_1);
	E g1_edge5_4(5, 4, 7.0); graph1.insertEdge(&g1_edge5_4);
	int n1 = graph1.numberOfVertices();
	for (int i = 1; i <= n1; ++i)
		graph1.Bellman_Ford_shortestPaths(i);
	WDG graph2(5, NOEDGE);
	E g2_edge1_2(1, 2, 10); graph2.insertEdge(&g2_edge1_2); 
	E g2_edge1_3(1, 3, 5); graph2.insertEdge(&g2_edge1_3);
	E g2_edge2_3(2, 3, 2); graph2.insertEdge(&g2_edge2_3);
	E g2_edge2_4(2, 4, 1); graph2.insertEdge(&g2_edge2_4);
	E g2_edge3_2(3, 2, 3); graph2.insertEdge(&g2_edge3_2);
	E g2_edge3_4(3, 4, 9); graph2.insertEdge(&g2_edge3_4);
	E g2_edge3_5(3, 5, 2); graph2.insertEdge(&g2_edge3_5);
	E g2_edge4_5(4, 5, 4); graph2.insertEdge(&g2_edge4_5);
	E g2_edge5_1(5, 1, 7); graph2.insertEdge(&g2_edge5_1);
	E g2_edge5_4(5, 4, 6); graph2.insertEdge(&g2_edge5_4);
	graph2.Dijkstra_shortestPaths(1);
	WDG graph3(6, NOEDGE);
	E g3_edge1_2(1, 2, 5); graph3.insertEdge(&g3_edge1_2);
	E g3_edge1_3(1, 3, 3); graph3.insertEdge(&g3_edge1_3);
	E g3_edge2_3(2, 3, 2); graph3.insertEdge(&g3_edge2_3);
	E g3_edge2_4(2, 4, 6); graph3.insertEdge(&g3_edge2_4);
	E g3_edge3_4(3, 4, 7); graph3.insertEdge(&g3_edge3_4);
	E g3_edge3_5(3, 5, 4); graph3.insertEdge(&g3_edge3_5);
	E g3_edge3_6(3, 6, 2); graph3.insertEdge(&g3_edge3_6);
	E g3_edge4_5(4, 5, -1); graph3.insertEdge(&g3_edge4_5);
	E g3_edge4_6(4, 6, 1); graph3.insertEdge(&g3_edge4_6);
	E g3_edge5_6(5, 6, -2); graph3.insertEdge(&g3_edge5_6);
	graph3.DAG_shortestPaths(2);
	UUG graph4(8, false);
	UE g4_edge1_2(1, 2, true); graph4.insertEdge(&g4_edge1_2);
	UE g4_edge1_3(1, 3, true); graph4.insertEdge(&g4_edge1_3);
	UE g4_edge1_4(1, 4, true); graph4.insertEdge(&g4_edge1_4);
	UE g4_edge2_5(2, 5, true); graph4.insertEdge(&g4_edge2_5);
	UE g4_edge3_5(3, 5, true); graph4.insertEdge(&g4_edge3_5);
	UE g4_edge4_6(4, 6, true); graph4.insertEdge(&g4_edge4_6);
	UE g4_edge4_7(4, 7, true); graph4.insertEdge(&g4_edge4_7);
	UE g4_edge5_8(5, 8, true); graph4.insertEdge(&g4_edge5_8);
	UE g4_edge6_8(6, 8, true); graph4.insertEdge(&g4_edge6_8);
	UE g4_edge7_8(7, 8, true); graph4.insertEdge(&g4_edge7_8);
	UUG *g4_bfs_tree = graph4.BFS_spanningTree(1);
	graph4.DFS_spanningTree(1);
	//graph4.Kruskal_spanningTree(1);
	//graph4.Prim_spanningTree(1);
	//g4_bfs_tree->adjacencyWDgraph<bool>::breadthFirstSearch(1); //�﷨����ȷ
	//g4_bfs_tree->adjacencyWDgraph<bool>::deepthFirstSearch(1); //�﷨����ȷ
	return 0;
}

