// https://en.wikipedia.org/wiki/Maximum_flow_problem
// ��������� ������� �����������, �������� � ����������� ���������
// ������ ������������� ����������� ������ �� �������� 
// ������� � �������� �� ���������� ��������������� �����.

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

#include "square_matrix.h"
#include "weighted_graph.h"
#include "mfp.h"

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

template<typename T> ostream& operator<<(ostream& out, std::vector<T> &vector)
{
	int n = vector.size();
	for(int i=0; i<n ; i++) out << vector[i] << " ";
	return out;
}


int main(int argc, char* argv[])
{
	char * input_file_name = NULL;
	char * output_file_name = NULL;

	// ��������� ��������� � ������� Windows
	// ������� setlocale() ����� ��� ���������, ������ �������� - ��� ��������� ������, � ����� ������ LC_TYPE - ����� ��������, ������ �������� � �������� ������. 
	// ������ ������� ��������� ����� ������ "Russian", ��� ��������� ������ ������� �������, ����� ����� �������� ����� ����� �� ��� � � ��.
	setlocale(LC_ALL, "");

	for(int i=1; i<argc; i++)
	{
		if(strcmp(argv[i],"-help")==0) 
		{
			std::cout << "������ � ������������ ������" << std::endl;
			std::cout << "\t��. https://en.wikipedia.org/wiki/Maximum_flow_problem" << std::endl;
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

	std::cout << "������� ������. " << std::endl
		<< "������ ������ �������� ���������� �����" << std::endl
		<< "����������� ������ � ������� i j weight" << std::endl
		<< "�� ��������� ����� ������� Ctrl+Z" << std::endl;

	weighted_graph<double> g;
	maximum_flow_problem<double> mfp;

	std::cin >> g;
	int n = g.size();

	std::cout << "����:" << std::endl;
	std::cout << g << std::endl;

	std::cout << "���������� ����:" << std::endl;
	for(int i=0; i<n ; i++)
	{
		for(int j=0; j<n ; j++)
		{
			if(i!=j)
			{
				try
				{
					std::vector<int> path;
					int length = mfp.find_minimal_path(path,i,j,g); // ����������� ���������� ��� ���������� ����
					std::cout << path << std::endl;
				}
				catch (...)
				{
				}
			}
		}
	}

	std::cout << "������������ ���������� ����� �� �������� ������� � �������� �������:" << std::endl;
	for(int i=0; i<n ; i++)
	{
		for(int j=0; j<n ; j++)
		{
			if(i!=j)
			{
				try
				{
					double maximum_flow = mfp.find_maximum_flow(i,j,g); // ����������� ���������� ��� ���������� ����
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

