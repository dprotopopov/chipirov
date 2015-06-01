// http://en.wikipedia.org/wiki/Vertex_cover
// Написать и исследовать программу,
// осуществляющую рандомизированным спуском по дереву ветвлений поиск 
// локально оптимального минимального покрывающего подмножество вершин.

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

ostream& operator<<(ostream& out, std::vector<bool> &vector)
{
	int n = vector.size();
	for(int i=0; i<n ; i++) out << vector[i];
	return out;
}

#include "symmetric_matrix.h"
#include "disordered_graph.h"
#include "vcp.h"

enum t_ask_mode {
	NOASK = 0,
	ASK = 1
};

enum t_trace_mode {
	NOTRACE = 0,
	TRACE = 1
};

enum t_selftest_mode {
	NOSELFTEST = 0,
	SELFTEST = 1
};

t_ask_mode ask_mode = ASK;
t_trace_mode trace_mode = NOTRACE;
t_selftest_mode selftest_mode = NOSELFTEST;
int test_n=10;

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
			std::cout << "Задача о вершинном покрытии" << std::endl;
			std::cout << "\tСм. http://en.wikipedia.org/wiki/Vertex_cover" << std::endl;
		}
		else if(strcmp(argv[i],"-selftest")==0) selftest_mode = SELFTEST;
		else if(strcmp(argv[i],"-noselftest")==0) selftest_mode = NOSELFTEST;
		else if(strcmp(argv[i],"-ask")==0) ask_mode = ASK;
		else if(strcmp(argv[i],"-noask")==0) ask_mode = NOASK;
		else if(strcmp(argv[i],"-trace")==0) trace_mode = TRACE;
		else if(strcmp(argv[i],"-notrace")==0) trace_mode = NOTRACE;
		else if(strcmp(argv[i],"-input")==0) input_file_name = argv[++i];
		else if(strcmp(argv[i],"-output")==0) output_file_name = argv[++i];
	}

	if(input_file_name!=NULL) freopen(input_file_name,"r",stdin);
	if(output_file_name!=NULL) freopen(output_file_name,"w",stdout);

	if(selftest_mode == NOSELFTEST && ask_mode == ASK) 
		std::cout << "Вводите строки. " << std::endl
		<< "Первая строка содержит количество узлов" << std::endl
		<< "Последующие строки в формате i j" << std::endl
		<< "По окончание ввода введите Ctrl+Z" << std::endl;

	disordered_graph g;
	vertex_cover_problem vcp(trace_mode == TRACE);
	std::vector<std::vector<bool>> solutions;

	if(selftest_mode == NOSELFTEST) std::cin >> g;
	else {
		g.assign_default(test_n);
		for(int i=0;i<test_n;i++) g.set(i,(i+1)%test_n,true);
	}

	std::cout << "Граф:" << std::endl;
	std::cout << g << std::endl;

	vcp.find(solutions,g);

	assert(solutions.size()>0);

	std::cout << "Минимальные покрытия:" << std::endl;
	for(std::vector<std::vector<bool>>::iterator iterator=solutions.begin();
		iterator<solutions.end();
		iterator++)
	{
		std::cout << *iterator << std::endl;
	}

	std::vector<int> count;
	for(std::vector<std::vector<bool>>::iterator iterator=solutions.begin();
		iterator<solutions.end();
		iterator++)
	{
		count.push_back(std::count (iterator->begin(), iterator->end(), true));
	}

	// http://stackoverflow.com/questions/8340569/stdvector-and-stdmin-behavior
	int min = *std::min_element(count.begin(), count.end());
	int max = *std::max_element(count.begin(), count.end());

	assert(min==max);

	std::cout << "Размер минимального покрытия:" << count[0] << std::endl;

	if(selftest_mode == SELFTEST)
	{
		int i=test_n/3;
		int j=test_n%3;
		int test_count=i+((j>0)?1:0);
		assert(count[0]==test_count);
	}

	getchar();
	getchar();

	return 0;
}

