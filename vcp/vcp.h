// Задача о вершинном покрытии
#ifndef __VCP_H
#define __VCP_H

#include <algorithm> //Обозначаем, что будем использовать алгоритмы
#include <vector>

using namespace std;

#include "symmetric_matrix.h"
#include "disordered_graph.h"


class vertex_cover_problem
{
	bool trace;
private:
	// Класс множества решений (плана)
	class branch_and_bounds_item
	{
		std::vector<bool> value0; // Нижняя граница множества решений (плана)
		std::vector<bool> value1; // Верхняя граница множества решений (плана)

	public:
		std::vector<bool> &value() 
		{ 
			assert(value0==value1);
			return value0; 
		}

		branch_and_bounds_item(int n)	
		{
			value0.assign(n,false);
			value1.assign(n,true);
		}

		branch_and_bounds_item(std::vector<bool> &v0, std::vector<bool> &v1) :
			value0(v0),
			value1(v1)
		{
		}


		branch_and_bounds_item(const branch_and_bounds_item &di) : 
			value0(di.value0),
			value1(di.value1)
		{
		}

		~branch_and_bounds_item()	
		{
		}

		void split(int i, std::vector<branch_and_bounds_item> &results, disordered_graph &g)
		{
			assert(value0[i]==false);
			assert(value1[i]==true);

			std::vector<bool> v0(value0);
			std::vector<bool> v1(value1);
			v1[i]=false;
			v0[i]=true;
			branch_and_bounds_item di0(value0,v1);
			branch_and_bounds_item di1(v0,value1);
			if(di0.valid(g)) results.push_back(di0);
			if(di1.valid(g)) results.push_back(di1);
		}

		// Нижняя граница множества решений (плана)
		int low(disordered_graph &g)
		{
			int count=std::count (value0.begin(), value0.end(), true);
			std::vector<bool> v;
			forward(v,value0,g);
			if(std::count (v.begin(), v.end(), true)<g.size()) count++;
			return count;
		}

		// Верхняя граница множества решений (плана)
		int high(disordered_graph &g)
		{
			return std::count (value1.begin(), value1.end(), true);
		}

		friend ostream& operator<<(ostream& out, branch_and_bounds_item &item)
		{
			out << "(" << item.value0  << "-" << item.value1 << ")";
			return out;
		}

	private:

		// Проверка достижимости всех вершин графа
		bool valid(disordered_graph &g)
		{
			std::vector<bool> v;
			forward(v,value1,g);
			return std::count (v.begin(), v.end(), true)==g.size();
		}

		// список вершин достижимых за один шаг
		static void forward(std::vector<bool> &v, std::vector<bool> &value, disordered_graph &g)
		{
			int n=g.size();
			int i=0;

			v.clear();
			for(int i=0;i<n;i++)
			{
				bool b=value[i];
				for(int j=0;j<n&&!b;j++) if(value[j]) 
				{
					g.get(i,j,b);
				}
				v.push_back(b);
			}
		}
	};

public:

	vertex_cover_problem(bool trace):trace(trace)
	{
	}

	~vertex_cover_problem()
	{
	}

	// поиск минимального покрытия
	// алгоритм ветвей и границ
	void find(std::vector<std::vector<bool>> &solutions, disordered_graph &graph)
	{
		disordered_graph g(graph);

		// Удаляем петли
		int n = g.size();
		for(int i=0;i<n;i++) g.set(i,i,false);

		std::vector<branch_and_bounds_item> prev;
		std::vector<branch_and_bounds_item> next;
		branch_and_bounds_item root(n);
		prev.push_back(root);
		for(int k=0;k<n;k++)
		{
			if(trace) cout << __FUNCTION__ << ":" << k << std::endl;

			next.clear();

			for(std::vector<branch_and_bounds_item>::iterator iterator=prev.begin();
				iterator<prev.end();
				iterator++)
			{
				iterator->split(k,next,g);
			}

			if(trace) cout << "split " << prev.size() << "->" << next.size() << std::endl;

			prev.clear();

			std::vector<int> index;
			std::vector<int> low;
			std::vector<int> high;
			int count=0;
			for(std::vector<branch_and_bounds_item>::iterator iterator=next.begin();
				iterator<next.end();
				iterator++)
			{
				int l = iterator->low(g);
				int h = iterator->high(g);
				low.push_back(l);
				high.push_back(h);
				index.push_back(count++);

				// if(trace) cout << *iterator << " low=" << l << " high=" << h << std::endl;
			}

			// http://stackoverflow.com/questions/8340569/stdvector-and-stdmin-behavior
			int min_high = *std::min_element(high.begin(), high.end());
			int max_low = *std::max_element(low.begin(), low.end());
			if(trace) cout << " min_high=" << min_high << " max_low=" << max_low << " count=" << count << std::endl;

			while(max_low>min_high)
			{
				for(int i=count;i-->0;) 
				{
					if(low[i]<=min_high) continue;
					count--;
					low[i]=low[count];
					high[i]=high[count];
					index[i]=index[count];
					index.pop_back();
					high.pop_back();
					low.pop_back();
				}
				min_high = *std::min_element(high.begin(), high.end());
				max_low = *std::max_element(low.begin(), low.end());
				if(trace) cout << " min_high=" << min_high << " max_low=" << max_low<< " count=" << count << std::endl;
			}

			assert(max_low<=min_high);
			assert((k<n)||(max_low==min_high));

			for(int i=count;i-->0;)
			{
				prev.push_back(next[index[i]]);
			}

			assert(prev.size()>0);
		}

		solutions.clear();
		for(std::vector<branch_and_bounds_item>::iterator iterator=prev.begin();
			iterator<prev.end();
			iterator++)
		{
			solutions.push_back(iterator->value());
		}
	}
};
#endif // __VCP_H
