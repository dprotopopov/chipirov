#ifndef __AP_H
#define __AP_H

#include "square_matrix.h"
#include "graph.h"
#include "bf.h"

template<typename T> class assignment_problem
{
public:
	bool check_assignment(std::vector<int> &assignment, graph<T> &g)
	{
		int n = g.size();
		T time;
		T price;
		bool exists;

		assert(assignment.size()==n);

		for(int i=0; i<n; i++)
		{
			g.get(i,assignment[i],time,price,exists);
			if(!exists) return false;
		}
		return true;
	}

	// вычисление цены назначения
	// формируется исключение при недопустимом назначении
	void calculate_assignment(std::vector<int> &assignment,T &assignment_time,T &assignment_price, graph<T> &g)
	{
		int n = g.size();

		T time;
		T price;
		bool exists;

		assert(assignment.size()==n);

		if(n==0) {
			assignment_time=(T)0;
			assignment_price=(T)0;
			return;
		}

		g.get(0,assignment[0],assignment_time,assignment_price,exists);
		if(!exists) throw "Недопустимое назначение";

		for(int i=1; i<n; i++)
		{
			g.get(i,assignment[i],time,price,exists);
			if(!exists) throw "Недопустимое назначение";
			if(assignment_time<time) assignment_time=time;
			assignment_price+=price;
		}
	}

	// Поиск назначения с минимальной ценой полным перебором
	// формируется исключение при отсутствии назначения
	// возвращает цену
	void find_optimal_assignment(std::vector<int> &optimal_assignment,T &optimal_time,T &optimal_price,graph<T> &g)
	{
		int n = g.size();
		brute_force bf; // алгоритм полного перебора
		for(bool b=bf.get_first(optimal_assignment,n);b;b=bf.get_next(optimal_assignment))
		{
			try
			{
				calculate_assignment(optimal_assignment,optimal_time,optimal_price,g); // формируется исключение при недопустимом назначении
				std::vector<int> current=optimal_assignment; // текущее назначение
				for(b=bf.get_next(current);b;b=bf.get_next(current))
				{
					try
					{
						T time;
						T price;
						calculate_assignment(current,time,price,g); // формируется исключение при недопустимом назначении
						if(optimal_price>price) {
							optimal_assignment=current;
							optimal_time=time;
							optimal_price=price;						
						}
					}
					catch (...)
					{
						continue;
					}
				}
				return;
			}
			catch (...)
			{
				continue;
			}
		}
		throw "Нет назначения";
	}

	// удаление возможных назначений времени больше заданного
	graph<T> &remove_gt_maximum_time(T maximum_time,graph<T> &g)
	{
		int n = g.size();
		T time;
		T price;
		bool exists;
		for(int i=0; i<n ; i++)
		{
			for(int j=0; j<n ; j++)
			{
				g.get(i,j,time,price,exists);
				g.set(i,j,time,price,exists&&(time<=maximum_time));
			}
		}
		return g;
	}
};
#endif // __AP_H
