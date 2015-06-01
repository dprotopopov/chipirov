#ifndef __BF_H
#define __BF_H

// алгоритм полного перебора
class brute_force
{
	std::vector<std::vector<int>> permutations;
	std::vector<std::vector<int>>::iterator iterator;

	// генератор перестановок
	std::vector<std::vector<int>> generate_permutations(int n)
	{
		if(n==1) {
			std::vector<std::vector<int>> pn;
			std::vector<int> zero;
			zero.push_back(0);
			pn.push_back(zero);
			return pn;
		}
		std::vector<std::vector<int>> ps= generate_permutations(n-1);
		std::vector<std::vector<int>> pn;
		for(std::vector<std::vector<int>>::iterator iterator=ps.begin();
			iterator<ps.end();
			iterator++)
		{
			std::vector<int>& p=*iterator;
			p.push_back(n-1);
			pn.push_back(p);
			for(int i=n-1;i-->0;)
			{
				int t=p[i];
				p[i]=p[i+1];
				p[i+1]=t;
				pn.push_back(p);
			}
		}
		return pn;
	}

public:
	bool get_first(std::vector<int> &assignment, int n)
	{
		permutations = generate_permutations(n);
		iterator=permutations.begin();
		assignment=*iterator;
		return iterator++<permutations.end();
	}

	bool get_next(std::vector<int> &assignment)
	{
		if(iterator<permutations.end())
		{
			assignment=*iterator++;
			return true;
		}
		return false;
	}
};

#endif // __BF_H
