// https://en.wikipedia.org/wiki/Maximum_flow_problem
// Пользуясь методом потенциалов, написать и исследовать программу
// поиска максимального однородного потока из заданной 
// вершины в заданную на взвешенном ориентированном графе.

#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <clocale>
#include <functional> 
#include <cctype>
#include <locale>
#include <assert.h>

using namespace std;

template<typename T> class square;
template<typename T> class graph;
template<typename T> class maximum_flow_problem;


enum t_ask_mode {
	NOASK = 0,
	ASK = 1
};
enum t_trace_mode {
	NOTRACE = 0,
	TRACE = 1
};
t_ask_mode ask_mode = NOASK;
t_trace_mode trace_mode = TRACE;



// класс квадратной матрицы
template<typename T> class square : public std::vector<std::vector<T>>
{
public:
	square()
	{
	}

	square(int n)
	{
		assign_default(n);
	}

	square(square &s)
	{
		int n=s.size();
		resize(n);
		for(int i=0;i<n;i++) at(i).assign(s[i].begin(),s[i].end());
	}

	~square()
	{
	}

	void assign_default(int n)
	{
		resize(n);
		for(int i=0;i<n;i++) at(i).assign(n,(T)0);
	}	
};


// класс графа, задаваемого матрицей смежности
template<typename T> class graph 
{
public:
	square<T> _weights; // матрица весов
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
		_weights(g._weights),
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

template<typename T> class maximum_flow_problem
{
public:
	// алгоритм построения вектора потенциалов из узла s в остальные узлы
	// петли игнорируются
	// Метод потенциалов гарантирует определение кратчайших путей из выбранной вершины во все остальные
	void build_potential_from(std::vector<T> &p, std::vector<bool> &e, int s, graph<T> &g)
	{
		int n = g.size();
		T weight;
		bool exists;

		assert(s<n);

		for(int i=0; i<n ; i++)
		{
			for(int j=0; j<n ; j++)
			{
				g.get(i,j,weight,exists);
				if(exists && weight<=(T)0)
					throw "Вес каждой дуги должен быть положительным";
			}
		}

		p.assign(g._weights[s].begin(),g._weights[s].end());
		e.assign(g._exists[s].begin(),g._exists[s].end());

		p[s]=(T)0;
		e[s]=true;

		for(bool changed=( n > 1 );changed;)
		{
			changed=false;
			for(int i=0; i<n ; i++) if(e[i])
			{
				for(int j=0; j<n ; j++)
				{
					g.get(i,j,weight,exists);
					if(j!=s && i!=j && exists)
					{
						if(e[j])
						{
							T pij = p[i]+weight;
							if(p[j]>pij)
							{
								p[j]=pij;
								changed=true;
							}
						}
						else
						{
							e[j]=true;
							p[j]=p[i]+weight;
							changed=true;
						}
					}
				}
			}
		}
	}

	// алгоритм построения вектора доступных узлов из узла k
	void check_path_from(std::vector<bool> &e, int k, graph<T> &g)
	{
		int n = g.size();
		T weight;
		bool exists;

		assert(k<n);

		e.assign(n,false);
		e[k]=true;
		int count=1;
		for(bool changed=( count != n );changed;)
		{
			for(int j=0; j<n ; j++)
			{
				for(int i=0; i<n && !e[j]; i++) 
				{
					if(e[i]) {
						g.get(i,j,weight,exists);
						e[j]=e[j]||exists;
					}
				}
				int count2 = 0;
				for(int i=0; i<n ; i++) if(e[i]) count2++;
				changed=(count!=count2)&&(count2!=n);
				count=count2;
			}
		}
	}

	// Поиск кратчайшего пути 
	// формируется исключение при отсутствии пути
	// возвращает длину пути
	int find_minimal_path(std::vector<int> &path, int s, int t, graph<T> &g)
	{
		int n = g.size();
		std::vector<T> p;
		std::vector<T> p2;
		std::vector<bool> e;
		T weight;
		bool exists;

		assert(s<n);
		assert(t<n);

		build_potential_from(p,e,s,g);
		if(!e[t]) throw "Нет пути";

		T pt = p[t];
		path.clear();
		path.push_back(s);
		while(p[s]!=pt)
		{
			for(int k=0; k<n; k++)
			{
				g.get(s,k,weight,exists);
				if(k!=s && exists && p[k]==p[s]+weight)
				{
					check_path_from(e,k,g);
					if(!e[t]) continue;
					build_potential_from(p2,e,k,g);
					if(!e[t]) continue;
					if(p[s]+weight+p2[t]==pt)
					{
						path.push_back(s=k);
						break;
					}
				}
			}
		}
		return path.size()-1;
	}
	// АЛГОРИТМ ПОИСКА МАКСИМАЛЬНОГО ПОТОКА
	T find_maximum_flow(int s, int t, graph<T> &g)
	{
		int n = g.size();
		T weight;
		bool exists;

		assert(s<n);
		assert(t<n);

		if(s==t) 
			throw "Узлы должны быть различными";

		for(int i=0; i<n ; i++)
		{
			for(int j=0; j<n ; j++)
			{
				g.get(i,j,weight,exists);
				if(exists && weight<=(T)0)
					throw "Вес каждой дуги должен быть положительным";
			}
		}

		graph<T> g1(g);

		T maximum_flow=(T)0;

		while(true)
		{
			try
			{
				for(int i=0; i<n ; i++)
				{
					for(int j=0; j<n ; j++)
					{
						g1.get(i,j,weight,exists);
						g1.set(i,j,weight,exists&&(weight!=(T)0));
					}
				}

				graph<T> g2(g1);

				for(int i=0; i<n ; i++)
				{
					for(int j=0; j<n ; j++)
					{
						g2.get(i,j,weight,exists);
						if(exists) g2.set(i,j,(T)1/weight,exists);
					}
				}

				std::vector<int> path;
				int length = find_minimal_path(path,s,t,g2);

				assert(length>0);

				T minimum_weight;
				g1.get(path[0],path[1],minimum_weight,exists);
				assert(exists);

				for(int i=1;i<length;i++) 
				{
					g1.get(path[i],path[i+1],weight,exists);
					assert(exists);
					if (minimum_weight>weight) minimum_weight=weight;
				}
				for(int i=0;i<length;i++) 
				{
					g1.get(path[i],path[i+1],weight,exists);
					assert(exists);
					g1.set(path[i],path[i+1],weight-minimum_weight,exists);
				}
				maximum_flow += minimum_weight;
			}
			catch (...)
			{
				return maximum_flow;
			}
		}
	}
};


template<typename T> ostream& operator<<(ostream& out, std::vector<T> &vector)
{
	int n = vector.size();
	for(int i=0; i<n ; i++) out << vector[i] << " ";
	return out;
}

template<typename T> ostream& operator<<(ostream& out, graph<T> &g)
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

template<typename T> istream& operator>>(istream& in, graph<T> &g)
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

int main(int argc, char* argv[])
{
	char * input_file_name = NULL;
	char * output_file_name = NULL;

	// Поддержка кириллицы в консоли Windows
	// Функция setlocale() имеет два параметра, первый параметр - тип категории локали, в нашем случае LC_TYPE - набор символов, второй параметр — значение локали. 
	// Вместо второго аргумента можно писать "Russian", или оставлять пустые двойные кавычки, тогда набор символов будет такой же как и в ОС.
	setlocale(LC_ALL, "");

	for(int i=1; i<argc; i++)
	{
		if(strcmp(argv[i],"-help")==0) 
		{
			std::cout << "Задача о максимальном потоке" << std::endl;
			std::cout << "\tСм. https://en.wikipedia.org/wiki/Maximum_flow_problem" << std::endl;
		}
		else if(strcmp(argv[i],"-ask")==0) ask_mode = ASK;
		else if(strcmp(argv[i],"-noask")==0) ask_mode = NOASK;
		else if(strcmp(argv[i],"-trace")==0) trace_mode = TRACE;
		else if(strcmp(argv[i],"-notrace")==0) trace_mode = NOTRACE;
		else if(strcmp(argv[i],"-input")==0) input_file_name = argv[++i];
		else if(strcmp(argv[i],"-output")==0) output_file_name = argv[++i];
	}

	if(input_file_name!=NULL) freopen(input_file_name,"r",stdin);
	if(output_file_name!=NULL) freopen(output_file_name,"w",stdout);

	std::cout << "Вводите строки. " << std::endl
		<< "Первая строка содержит количество узлов" << std::endl
		<< "Последующие строки в формате i j weight" << std::endl
		<< "По окончание ввода введите Ctrl+Z" << std::endl;

	graph<double> g;
	maximum_flow_problem<double> mfp;

	std::cin >> g;
	int n = g.size();

	std::cout << "Граф:" << std::endl;
	std::cout << g << std::endl;

	std::cout << "Кратчайшие пути:" << std::endl;
	for(int i=0; i<n ; i++)
	{
		for(int j=0; j<n ; j++)
		{
			if(i!=j)
			{
				try
				{
					std::vector<int> path;
					int length = mfp.find_minimal_path(path,i,j,g);
					std::cout << path << std::endl;
				}
				catch (...)
				{
				}
			}
		}
	}

	std::cout << "Максимальный однородный поток из заданной вершины в заданную вершину:" << std::endl;
	for(int i=0; i<n ; i++)
	{
		for(int j=0; j<n ; j++)
		{
			if(i!=j)
			{
				try
				{
					double maximum_flow = mfp.find_maximum_flow(i,j,g);
					std::cout << i << " " << j << " "  << maximum_flow << std::endl;
				}
				catch (...)
				{
				}
			}
		}
	}

	getchar();
	getchar();

	return 0;
}

