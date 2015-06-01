#ifndef __WEIGHTED_GRAPH_H
#define __WEIGHTED_GRAPH_H

#include "square_matrix.h"

// класс графа, задаваемого матрицей смежности
// Взвешенный граф — граф, каждому ребру которого поставлено в соответствие некое значение (вес ребра).
template<typename T> class weighted_graph 
{
public:
	square_matrix<T> _weights; // матрица весов
	square_matrix<bool> _exists; // матрица признака существования пути
	int _n; // количество узлов
public:
	weighted_graph()
	{
	}

	weighted_graph(int n)
	{
		assign_default(n);
	}

	weighted_graph(weighted_graph &g):
		_n(g.size()),
		_weights(g._weights),
		_exists(g._exists)
	{
	}

	~weighted_graph()
	{
	}

	int size() { return _n; } 

	void assign_default(int n)
	{
		_n = n;
		_weights.assign_default(n);
		_exists.assign_default(n);
	}

	void get(int i, int j, T &weight, bool &exists)
	{
		weight=_weights[i][j];
		exists=_exists[i][j];
	}

	void set(int i, int j, T weight, bool exists)
	{
		_weights[i][j]=weight;
		_exists[i][j]=exists;
	}
};

template<typename T> ostream& operator<<(ostream& out, weighted_graph<T> &g)
{
	int n = g.size();
	T weight;
	bool exists;
	out << n << std::endl;
	for(int i=0; i<n ; i++)
	{
		for(int j=0; j<n ; j++)
		{
			g.get(i,j,weight,exists);
			if(exists) out << i << " " << j << " " << weight << std::endl;
		}
	}
	return out;
}

template<typename T> istream& operator>>(istream& in, weighted_graph<T> &g)
{
	std::string buffer = "";

	int n,i,j;
	T weight;

	if(std::getline(in, buffer)) {
		std::istringstream ss(buffer);
		ss >> n;
		g.assign_default(n);
	}

	while (std::getline(in, buffer)) {
		if (buffer.size() > 0) {
			std::istringstream ss(buffer);
			ss >> i >> j >> weight;
			assert(i<n);
			assert(j<n);
			g.set(i,j,weight,true);
		}
	}

	return in;
}

#endif // __WEIGHTED_GRAPH_H
