#ifndef __GRAPH_H
#define __GRAPH_H

#include "square.h"

// класс графа, задаваемого матрицей смежности
template<typename T> class graph 
{
public:
	square<T> _times; // матрица времени
	square<T> _prices; // матрица стоимостей
	square<bool> _exists; // матрица признака существования пути
	int _n; // количество узлов
public:
	graph()
	{
	}

	graph(int n)
	{
		assign_default(n);
	}

	graph(graph &g):
		_n(g.size()),
		_times(g._times),
		_prices(g._prices),
		_exists(g._exists)
	{
	}

	~graph()
	{
	}

	int size() { return _n; } 

	void assign_default(int n)
	{
		_n = n;
		_times.assign_default(n);
		_prices.assign_default(n);
		_exists.assign_default(n);
	}

	void get(int i, int j, T &time, T &price, bool &exists)
	{
		time=_times[i][j];
		price=_prices[i][j];
		exists=_exists[i][j];
	}

	void set(int i, int j, T time, T price, bool exists)
	{
		_times[i][j]=time;
		_prices[i][j]=price;
		_exists[i][j]=exists;
	}
};

template<typename T> ostream& operator<<(ostream& out, graph<T> &g)
{
	int n = g.size();
	T time;
	T price;
	bool exists;
	out << n << std::endl;
	for(int i=0; i<n ; i++)
	{
		for(int j=0; j<n ; j++)
		{
			g.get(i,j,time,price,exists);
			if(exists) out << i << " " << j << " " << time << " " << price << std::endl;
		}
	}
	return out;
}

template<typename T> istream& operator>>(istream& in, graph<T> &g)
{
	std::string buffer = "";

	int n,i,j;
	T time;
	T price;

	if(std::getline(in, buffer)) {
		std::istringstream ss(buffer);
		ss >> n;
		g.assign_default(n);
	}

	while (std::getline(in, buffer)) {
		if (buffer.size() > 0) {
			std::istringstream ss(buffer);
			ss >> i >> j >> time >> price;
			assert(i<n);
			assert(j<n);
			g.set(i,j,time,price,true);
		}
	}

	return in;
}

#endif // __GRAPH_H
