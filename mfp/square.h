#ifndef __SQUARE_H
#define __SQUARE_H

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
#endif // __SQUARE_H
