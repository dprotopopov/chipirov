#ifndef __DISORDERED_GRAPH_H
#define __DISORDERED_GRAPH_H

#include "symmetric_matrix.h"

// класс неорентированного графа, задаваемого матрицей смежности
class disordered_graph 
{
public:
	symmetric_matrix<bool> _exists; // матрица признака существования пути
	int _n; // количество узлов
public:
	disordered_graph()
	{
	}

	disordered_graph(int n)
	{
		assign_default(n);
	}

	disordered_graph(disordered_graph &g):
		_n(g.size()),
		_exists(g._exists)
	{
	}

	~disordered_graph()
	{
	}

	int size() { return _n; } 

	void assign_default(int n)
	{
		_n = n;
		_exists.assign_default(n);
	}

	// Удаление всех рёбер сопряжённых с i-той вершиной из графа
	void remove(int i) 
	{
		_exists[i].assign(i+1,false);
	}
	
	void get(int i, int j, bool &exists)
	{
		if(i>j)
			exists=_exists[i][j];
		else
			exists=_exists[j][i];
	}

	void set(int i, int j, bool exists)
	{
		if(i>j)
			_exists[i][j]=exists;
		else
			_exists[j][i]=exists;
	}
};

ostream& operator<<(ostream& out, disordered_graph &g)
{
	int n = g.size();
	bool exists;
	out << n << std::endl;
	for(int i=0; i<n ; i++)
	{
		for(int j=0; j<n ; j++)
		{
			g.get(i,j,exists);
			if(exists) out << i << " " << j << std::endl;
		}
	}
	return out;
}

istream& operator>>(istream& in, disordered_graph &g)
{
	std::string buffer = "";

	int n,i,j;

	if(std::getline(in, buffer)) 
	{
		std::istringstream ss(buffer);
		ss >> n;
		g.assign_default(n);
	}

	while (std::getline(in, buffer)) 
	{
		if (buffer.size() > 0) 
		{
			std::istringstream ss(buffer);
			ss >> i >> j;
			assert(i<n);
			assert(j<n);
			g.set(i,j,true);
		}
	}

	return in;
}

#endif // __DISORDERED_GRAPH_H
