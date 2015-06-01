// https://en.wikipedia.org/wiki/Assignment_problem
// �������� � ����������� ��������� ������� ������ � ����������� ������ ���������,
// ����� ������� �������� ����������� ��������� ������ ��� �������,
// ��� ����� ���������� ����� �� ��������� � � �������������� ������� ����������.

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

template<typename T> ostream& operator<<(ostream& out, std::vector<T> &vector)
{
	int n = vector.size();
	for(int i=0; i<n ; i++) out << vector[i] << " ";
	return out;
}

#include "square_matrix.h"
#include "graph.h"
#include "bf.h"
#include "ap.h"

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
			std::cout << "������ � �����������" << std::endl;
			std::cout << "\t��. https://en.wikipedia.org/wiki/Assignment_problem" << std::endl;
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

	std::cout << "������� ������������ �����: " << std::endl;
	double maximum_time; 
	std::string buffer = "";
	std::getline(std::cin, buffer);
	std::istringstream ss(buffer);
	ss>>maximum_time;

	std::cout << "������� ������. " << std::endl
		<< "������ ������ �������� ���������� �����" << std::endl
		<< "����������� ������ � ������� i j time price" << std::endl
		<< "�� ��������� ����� ������� Ctrl+Z" << std::endl;

	graph<double> g;
	assignment_problem<double> ap;

	std::cin >> g;
	int n = g.size();

	std::cout << "����:" << std::endl;
	std::cout << g << std::endl;

	std::cout << "����� ����������:" << std::endl;
	try
	{
		std::vector<int> assignment;
		double time;
		double price;
		ap.remove_gt_maximum_time(maximum_time,g); // �������� ��������� ���������� ������� ������ ���������
		std::cout << g << std::endl;
		ap.find_optimal_assignment(assignment,time,price,g); // ����������� ���������� ��� ���������� ����������
		std::cout << "����������:" << assignment << std::endl;
		std::cout << "����:" << price << std::endl;
		std::cout << "�����:" << time << std::endl;
	}
	catch (...)
	{
		std::cout << "�� �������" << std::endl;
	}

	getchar();
	getchar();

	return 0;
}

