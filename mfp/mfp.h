#ifndef __MFP_H
#define __MFP_H

#include "square_matrix.h"
#include "weighted_graph.h"

template<typename T> class maximum_flow_problem
{
public:
	// алгоритм построения вектора потенциалов из узла s в остальные узлы
	// петли игнорируются
	// Метод потенциалов гарантирует определение кратчайших путей из выбранной вершины во все остальные
	void build_potential_from(std::vector<T> &p, std::vector<bool> &e, int s, weighted_graph<T> &g)
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
	void check_path_from(std::vector<bool> &e, int k, weighted_graph<T> &g)
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
	int find_minimal_path(std::vector<int> &path, int s, int t, weighted_graph<T> &g)
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
	T find_maximum_flow(int s, int t, weighted_graph<T> &g)
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

		weighted_graph<T> g1(g);

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

				weighted_graph<T> g2(g1);

				for(int i=0; i<n ; i++)
				{
					for(int j=0; j<n ; j++)
					{
						g2.get(i,j,weight,exists);
						if(exists) g2.set(i,j,(T)1/weight,exists);
					}
				}

				std::vector<int> path;
				int length = find_minimal_path(path,s,t,g2); // формируется исключение при отсутствии пути

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
#endif // __MFP_H
