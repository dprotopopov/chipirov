#ifndef __SQUARE_MATRIX_H
#define __SQUARE_MATRIX_H

// класс квадратной матрицы
template<typename T> class square_matrix : public std::vector<std::vector<T>>
{
public:
	square_matrix()
	{
	}

	square_matrix(int n)
	{
		assign_default(n);
	}

	square_matrix(square_matrix &s)
	{
		int n=s.size();
		resize(n);
		for(int i=0;i<n;i++) at(i).assign(s[i].begin(),s[i].end());
	}

	~square_matrix()
	{
	}

	void assign_default(int n)
	{
		resize(n);
		for(int i=0;i<n;i++) at(i).assign(n,(T)0);
	}	
};
#endif // __SQUARE_MATRIX_H
