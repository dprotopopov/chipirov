#ifndef __SYMMETRIC_MATRIX_H
#define __SYMMETRIC_MATRIX_H

// класс симметричной матрицы
template<typename T> class symmetric_matrix : public std::vector<std::vector<T>>
{
public:
	symmetric_matrix()
	{
	}

	symmetric_matrix(int n)
	{
		assign_default(n);
	}

	symmetric_matrix(symmetric_matrix &s)
	{
		int n=s.size();
		resize(n);
		for(int i=0;i<n;i++) at(i).assign(s[i].begin(),s[i].end());
	}

	~symmetric_matrix()
	{
	}

	void assign_default(int n)
	{
		resize(n);
		for(int i=0;i<n;i++) at(i).assign((i+1),(T)0);
	}	
};
#endif // __SYMMETRIC_MATRIX_H
