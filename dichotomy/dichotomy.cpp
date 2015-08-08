// https://ru.wikipedia.org/wiki/Дихотомия

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

typedef bool (check_func)(std::vector<double> x); // Профиль проверочной функции
typedef double (value_func)(std::vector<double> x); // Профиль искомой функции

/////////////////////////////////////////////////////////
// Набор проверочных функций
bool f1(std::vector<double> x)
{
	static const double _c[] = {1, 1};
	static const double b = 16;
	std::vector<double> c(_c, _c + sizeof(_c) / sizeof(_c[0]) );
	assert(x.size()==c.size());
	double y=0;
	for(unsigned i=0;i<x.size()&&i<c.size();i++) y+=(x[i]-c[i])*(x[i]-c[i]);
	return y<b;
}
bool f2(std::vector<double> x)
{
	static const double _c[] = {2, 2};
	static const double b = 16;
	std::vector<double> c(_c, _c + sizeof(_c) / sizeof(_c[0]) );
	assert(x.size()==c.size());
	double y=0;
	for(unsigned i=0;i<x.size()&&i<c.size();i++) y+=(x[i]-c[i])*(x[i]-c[i]);
	return y<b;
}
// ...

/////////////////////////////////////////////////////////
// Искомая функция
double w(std::vector<double> x)
{
	static const double _c[] = {2, 3};
	std::vector<double> c(_c, _c + sizeof(_c) / sizeof(_c[0]) );
	assert(x.size()==c.size());
	double y=0;
	for(unsigned i=0;i<x.size()&&i<c.size();i++) y+=(x[i]-c[i])*(x[i]-c[i]);
	return y;
}

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

/////////////////////////////////////////////////////////
// Дефолтные значения
static const unsigned _n = 2;
static const unsigned _m[] = {3, 3};
static const double _a[] = {0, 0};
static const double _b[] = {100, 100};
static check_func *_f[]  = {&f1,&f2};
static const double _e=1e-15;


/////////////////////////////////////////////////////////
// Вычисление растояния между двумя векторами координат
double delta(std::vector<double> x,std::vector<double> y)
{
	double diff=0;
	unsigned i=0;
	for(;i<x.size() && i<y.size();i++) diff=max(diff,abs(x[i]-y[i]));
	for(;i<x.size();i++) diff=max(diff,abs(x[i]));
	for(;i<y.size();i++) diff=max(diff,abs(y[i]));
	return diff;
}

/////////////////////////////////////////////////////////
// Вычисление вектора индексов координат решётки по номеру узла
// index - номер узла решётки
std::vector<unsigned> vector_of(unsigned long index, std::vector<unsigned> m)
{
	std::vector<unsigned> vector;
	for(unsigned i=0;i<m.size();i++)
	{
		vector.push_back(index%(1ul+m[i]));
		index/=1ul+m[i];
	}
	return vector;
}

/////////////////////////////////////////////////////////
// Преобразование вектора индексов координат решётки
// в вектор координат точки
// vector - вектор индексов координат решётки
// m - число сегментов по каждому из измерений
// a - вектор минимальных координат точек
// b - вектор максимальных координат точек
std::vector<double> point_of(std::vector<unsigned> vector,
							 std::vector<unsigned> m,
							 std::vector<double> a,
							 std::vector<double> b)
{
	std::vector<double> point=a;
	for(unsigned i=0;i<m.size();i++) point[i]+=(b[i]-a[i])*vector[i]/m[i];
	return point;
}

/////////////////////////////////////////////////////////
// Проверка принадлежности точки области, заданной ограничениями
// x - координаты точки
// f - набор проверочных функций
bool check(std::vector<double> x, std::vector<check_func *> f)
{
	bool b=true;
	for(unsigned i=0;b&&i<f.size();i++)	b=(*f[i])(x);
	return b;
}

/////////////////////////////////////////////////////////
// Вычисление числа узлов решётки
// m - число сегментов по каждому из измерений
unsigned long total_of(std::vector<unsigned> m)
{
	unsigned long index=1;
	for(unsigned i=0;i<m.size();i++) index*=1ul+m[i];
	return index;
}

// Алгоритм
double find(std::vector<double> &x,
			value_func *w,
			std::vector<check_func *> f,
			std::vector<double> a,
			std::vector<double> b,
			std::vector<unsigned> m,
			double e)
{
	for(unsigned i=0;i<m.size();i++) assert(m[i]>2);

	unsigned long total=total_of(m);

	while(true)
	{
		// Находим первую точку в области, заданной ограничениями
		unsigned long index=0;
		while(index<total)
		{
			x = point_of(vector_of(index++, m), m, a, b);
			if(check(x,f)) break;
		}
		double y=(*w)(x);

		while(index<total)
		{
			std::vector<double> x1 = point_of(vector_of(index++, m), m, a, b);
			if(!check(x1,f)) continue;
			double y1 = (*w)(x1);
			if(y1>y) continue;
			x=x1;
			y=y1;
		}

		if(trace_mode==TRACE) for(unsigned i=0;i<x.size();i++) std::cout << x[i] << " "; 
		if(trace_mode==TRACE) std::cout << "-> " << y << std::endl; 

		if(delta(a,b)<e) return y;

		for(unsigned i=0;i<std::min(a.size(),b.size());i++) {
			double aa = a[i];
			double bb = b[i];
			double xx = x[i];
			a[i]=std::max(aa,xx-(bb-aa)/m[i]);
			b[i]=std::min(bb,xx+(bb-aa)/m[i]);
		}
	}
}

int main(int argc, char* argv[])
{
	// http://stackoverflow.com/questions/2236197/what-is-the-easiest-way-to-initialize-a-stdvector-with-hardcoded-elements
	unsigned n=_n;
	std::vector<unsigned> m(_m, _m + sizeof(_m) / sizeof(_m[0]) );
	std::vector<double> a(_a, _a + sizeof(_a) / sizeof(_a[0]) );
	std::vector<double> b(_b, _b + sizeof(_b) / sizeof(_b[0]) );
	std::vector<check_func *> f(_f, _f + sizeof(_f) / sizeof(_f[0]) );
	double e=_e;

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
			std::cout << "Алгоритм деления значений аргумента фукции" << std::endl;
			//			std::cout << "\t-n <размерность пространства>" << std::endl;
			std::cout << "\t-m <число сегментов по каждому из измерений>" << std::endl;
			std::cout << "\t-a <минимальные координаты по каждому из измерений>" << std::endl;
			std::cout << "\t-b <максимальные координаты по каждому из измерений>" << std::endl;
			std::cout << "\t-e <очность вычислений>" << std::endl;
			std::cout << "\t-ask/noask" << std::endl;
			std::cout << "\t-trace/notrace" << std::endl;
			std::cout << "\tСм. https://ru.wikipedia.org/wiki/Дихотомия" << std::endl;
		}
		else if(strcmp(argv[i],"-ask")==0) ask_mode = ASK;
		else if(strcmp(argv[i],"-noask")==0) ask_mode = NOASK;
		else if(strcmp(argv[i],"-trace")==0) trace_mode = TRACE;
		else if(strcmp(argv[i],"-notrace")==0) trace_mode = NOTRACE;
		//		else if(strcmp(argv[i],"-n")==0) n = atoi(argv[++i]);
		else if(strcmp(argv[i],"-e")==0) e = atof(argv[++i]);
		else if(strcmp(argv[i],"-m")==0) {
			std::istringstream ss(argv[++i]);
			m.clear();
			for(unsigned i=0;i<n;i++) m.push_back(atoi(argv[++i]));
		}
		else if(strcmp(argv[i],"-a")==0) {
			a.clear();
			for(unsigned i=0;i<n;i++) a.push_back(atof(argv[++i]));
		}
		else if(strcmp(argv[i],"-b")==0) {
			b.clear();
			for(unsigned i=0;i<n;i++) b.push_back(atof(argv[++i]));
		}
		else if(strcmp(argv[i],"-input")==0) input_file_name = argv[++i];
		else if(strcmp(argv[i],"-output")==0) output_file_name = argv[++i];
	}

	if(input_file_name!=NULL) freopen(input_file_name,"r",stdin);
	if(output_file_name!=NULL) freopen(output_file_name,"w",stdout);

	if(ask_mode == ASK)
	{
		//  std::cout << "Введите размерность пространства:"<< std::endl; std::cin >> n;

		std::cout << "Введите число сегментов по каждому из измерений m[" << n << "]:"<< std::endl;
		m.clear();
		for(unsigned i=0;i<n;i++)
		{
			int x; std::cin >> x;
			m.push_back(x);
		}

		std::cout << "Введите минимальные координаты по каждому из измерений a[" << n << "]:"<< std::endl;
		a.clear();
		for(unsigned i=0;i<n;i++)
		{
			double x; std::cin >> x;
			a.push_back(x);
		}

		std::cout << "Введите максимальные координаты по каждому из измерений b[" << n << "]:"<< std::endl;
		b.clear();
		for(unsigned i=0;i<n;i++)
		{
			double x; std::cin >> x;
			b.push_back(x);
		}

		std::cout << "Введите точность вычислений:"<< std::endl; std::cin >> e;
	}

	std::vector<double> x;
	double y = find(x, &w, f, a, b, m, e);

	std::cout << "Размерность пространства : " << n << std::endl;
	std::cout << "Число сегментов          : "; for(unsigned i=0;i<m.size();i++) std::cout << m[i] << " "; std::cout << std::endl; 
	std::cout << "Минимальные координаты   : "; for(unsigned i=0;i<a.size();i++) std::cout << a[i] << " "; std::cout << std::endl; 
	std::cout << "Максимальные координаты  : "; for(unsigned i=0;i<b.size();i++) std::cout << b[i] << " "; std::cout << std::endl; 
	std::cout << "Точность вычислений      : " << e << std::endl;
	std::cout << "Точка минимума           : "; for(unsigned i=0;i<x.size();i++) std::cout << x[i] << " "; std::cout << std::endl; 
	std::cout << "Минимальное значение     : " << y << std::endl; 
	std::cout << "См. https://ru.wikipedia.org/wiki/Дихотомия" << std::endl;

	getchar();
	getchar();

	return 0;
}

