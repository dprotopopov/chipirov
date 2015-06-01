// «адача о вершинном покрытии
#ifndef __VCP_H
#define __VCP_H

#include <algorithm> //ќбозначаем, что будем использовать алгоритмы
#include <vector>

using namespace std;

#include "symmetric_matrix.h"
#include "disordered_graph.h"


class vertex_cover_problem
{
	bool trace;
private:
	//  ласс множества решений (плана)
	class branch_and_bounds_item
	{
	public:
		std::vector<bool> value; // множество решений (план)

		branch_and_bounds_item()	
		{
		}

		branch_and_bounds_item(std::vector<bool> &v) :
			value(v)
		{
		}

		branch_and_bounds_item(std::vector<bool> &v, bool append) :
			value(v)
		{
			value.push_back(append);
		}

		branch_and_bounds_item(const branch_and_bounds_item &di) : 
			value(di.value)
		{
		}

		~branch_and_bounds_item()	
		{
		}

		void split(std::vector<branch_and_bounds_item> &results, disordered_graph &g)
		{
			branch_and_bounds_item di0(value,false);
			branch_and_bounds_item di1(value,true);

			if(di0.valid(g)) results.push_back(di0);
			if(di1.valid(g)) results.push_back(di1);
		}

		// Ќижн€€ граница множества решений (плана)
		int low(disordered_graph &g)
		{
			std::vector<bool> value1;
			other(value1,g);
			
			return std::count (value.begin(), value.end(), true)
				+((std::count (value1.begin(), value1.end(), true)>0)?1:0);
		}

		// ¬ерхн€€ граница множества решений (плана)
		int high(disordered_graph &g)
		{
			int n=g.size();
			int k=value.size();
			assert(k<=n);
			return std::count (value.begin(), value.end(), true)+(n-k);
		}

		friend ostream& operator<<(ostream& out, branch_and_bounds_item &item)
		{
			out << "(" << item.value << ")";
			return out;
		}

	private:

		// ѕроверка достижимости всех вершин графа
		// (добавл€ем в план все оставшиес€ вершины и провер€ем)
		bool valid(disordered_graph &g)
		{
			int n=g.size();
			int k=value.size();
			
			for(int i=0;i<k;i++)
			{
				bool b=value[i];
				for(int j=0;j<k&&!b;j++)
				{
					g.get(i,j,b);
					b=b&&value[j];
				}
				for(int j=k;j<n&&!b;j++)
				{
					g.get(i,j,b);
				}
				if(!b) return false;
			}

			return true;
		}

		// вектор оставшихс€ вершин
		// (исключаем достижимые вершины из оставшегос€ единичного вектора)
		void other(std::vector<bool> &r, disordered_graph &g)
		{
			int n=g.size();
			int k=value.size();
			bool b;
			r.assign(n-k, true);
			for(int i=0;i<k;i++) if(value[i])
			{
				for(int j=k;j<n;j++) if(r[j-k])
				{
					g.get(i,j,b);
					r[j-k]=!b;
				}
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

	// поиск минимального покрыти€
	// алгоритм ветвей и границ
	void find(std::vector<std::vector<bool>> &solutions, disordered_graph &graph)
	{
		disordered_graph g(graph);

		// ”дал€ем петли
		int n = g.size();
		for(int i=0;i<n;i++) g.set(i,i,false);

		std::vector<branch_and_bounds_item> prev;
		std::vector<branch_and_bounds_item> next;
		branch_and_bounds_item root;
		prev.push_back(root);
		for(int k=0;k<n;k++)
		{
			if(trace) cout << __FUNCTION__ << ":" << k << std::endl;

			next.clear();

			for(std::vector<branch_and_bounds_item>::iterator iterator=prev.begin();
				iterator<prev.end();
				iterator++)
			{
				iterator->split(next,g);
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
			solutions.push_back(iterator->value);
		}
	}
};
#endif // __VCP_H
