// tree.cpp: определяет точку входа для консольного приложения.
//
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

class binary_tree;
class binary_node;

template<typename T> ostream& operator<<(ostream& out, std::vector<T> &vector)
{
	int n = vector.size();
	for(int i=0; i<n ; i++) out << vector[i] << " ";
	return out;
}

/// <summary>
/// Класс, описывающий бинарное дерево
/// </summary>
class binary_node
{
	friend binary_tree; // https://msdn.microsoft.com/ru-ru/library/465sdshe.aspx
protected:
	vector<binary_node> children;
	string key;
	string value;

public:

	binary_node(string key = "", string value = ""):
		key(key),
		value(value)
	{
	}

	binary_node(const binary_node &node):
		key(node.key),
		value(node.value)
	{
		children.assign(node.children.begin(),node.children.end());
	}

	binary_node(binary_node &node, string key, string value = ""):
		key(key),
		value(value)
	{
		children.push_back(node);
	}

	~binary_node()
	{
		children.clear();
	}

	friend ostream& operator<<(ostream& out, binary_node &node)
	{
		out << node.key << ":" << node.value << " ( ";
		for(vector<binary_node>::iterator iterator=node.children.begin();
				iterator<node.children.end();
				iterator++)
				out << *iterator << " ";
		out << " ) ";
		return out;
	}
};

/// <summary>
/// Класс, описывающий управление бинарным деревом
/// </summary>
class binary_tree
{
protected:
	binary_node root; // Корень бинарного дерева 
	int maxValue; // Максимальное значение ключа в бинарном дереве

public:
	binary_tree():maxValue(0)
	{
	}

	binary_tree(binary_tree &tree):root(tree.root),maxValue(0)
	{
	}

	~binary_tree()
	{
	}

	/// <summary>
	/// Рекурсивный поиск элемента по ключу
	/// </summary>
	/// <param name="key">Ключ элемента в бинарном дереве</param>
	/// <param name="node">Корень поиска</param>
	/// <returns></returns>
	binary_node& search_node(string key, binary_node& node)
	{
		if (node.key == key)
			return node;
		else 
		{
			for(vector<binary_node>::iterator iterator=node.children.begin();
				iterator<node.children.end();
				iterator++)
			{
				try
				{
					return search_node(key, *iterator);
				}
				catch (...)
				{
					continue;
				}
			}
		}
		throw "Не найдено";
	}

	/// <summary>
	/// Добавление элемента с номером key и значением value
	/// </summary>
	/// <param name="key">Ключ элемента в бинарном дереве</param>
	/// <param name="value">Значение элемента в бинарном дереве</param>
	/// <returns></returns>
	bool insert_value(string key, string value)
	{
		try
		{
			binary_node& node = search_node(key, root);
			node.value = value;
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	/// <summary>
	/// Поиск значения элемента бинарного дерева по ключу
	/// </summary>
	/// <param name="key">Ключ элемента в бинарном дереве</param>
	/// <returns>Возвращает значение элемента по ключу key</returns>
	string search_value(string key)
	{
		binary_node& node = search_node(key, root);
		return node.value;
	}

	/// <summary>
	/// Добавление элемента в бинарное дерево к элементу с ключом key
	/// </summary>
	/// <param name="key">Ключ элемента в бинарном дереве</param>
	/// <param name="value">Значение элемента в бинарном дереве</param>
	/// <returns>Возвращает истину, если добавление успешно</returns>
	bool add(string key, string value)
	{
		try
		{
			binary_node& node = search_node(key, root);
			char buffer[256];
			binary_node child(itoa(++maxValue,buffer,10),value);
			node.children.push_back(child);
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	/// <summary>
	/// Удаление элемента в бинарном дереве
	/// </summary>
	/// <param name="key">Ключ элемента в бинарном дереве</param>
	/// <returns>Возвращает истину, если удаление успешно</returns>
	bool del(string key)
	{
		try
		{
			binary_node& node = search_node(key, root);
			node.children.clear();
			node.key = "deleted";
			node.value = "deleted";
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	/// <summary>
	/// Формирование бинарного дерева из массива строк
	/// </summary>
	/// <param name="inp">Массив строк</param>
	bool load(string inp)
	{
		// inp = "1,2,3;2,4,5;3,6,7;4,8,9";
		vector<string> level1;
		vector<string> level2;
		level1.clear();
		split(inp,';',level1);
		//cout << level1 << std::endl;
		for (vector<string>::iterator iterator1=level1.begin();
			iterator1<level1.end();
			iterator1++)
		{
			level2.clear();
			split(*iterator1,',',level2);
			//cout << level2 << std::endl;
			vector<string>::iterator iterator2=level2.begin();
			try
			{
				binary_node& node = search_node(*iterator2, root);
				if (atoi(iterator2->c_str()) > maxValue) maxValue = atoi(iterator2->c_str());
				for (iterator2++;
					iterator2 < level2.end();
					iterator2++)
				{
					binary_node child(*iterator2);
					node.children.push_back(child);
					if (atoi(iterator2->c_str()) > maxValue) maxValue = atoi(iterator2->c_str());
				}
			}
			catch (...)
			{
				binary_node& node=root;
				node.key = *iterator2;
				if (atoi(iterator2->c_str()) > maxValue) maxValue = atoi(iterator2->c_str());
				for (iterator2++;
					iterator2 < level2.end();
					iterator2++)
				{
					binary_node child(*iterator2);
					node.children.push_back(child);
					if (atoi(iterator2->c_str()) > maxValue) maxValue = atoi(iterator2->c_str());
				}
			}
		}
		return true;
	}

private:
	void split(const string str, char delim, vector<string> &parts)
	{
		string::size_type i = 0;
		string::size_type j = str.find(delim);

		while (j != string::npos) {
			string ss = str.substr(i, j - i);
			parts.push_back(ss);

			i =  ++j;
			j = str.find(delim, j);

			if (j == string::npos) {
				parts.push_back(str.substr(i, str.length()));
			}
		}
	}

	friend ostream& operator<<(ostream& out, binary_tree &tree)
	{
		out << tree.root;
		return out;
	}
};

int main(int argc, char* argv[])
{
	binary_tree tree; // создание бинарного дерева
	cout << tree.load("1,2,3;2,4,5;3,6,7;4,8,9") << std::endl; // загрузка дерева
	cout << tree << std::endl;
	cout << tree.add("1", "value for new Item") << std::endl; // добавление нового элемента в элемент с ключом 1
	cout << tree.del("2") << std::endl;
	cout << tree << std::endl;

	getchar();
	getchar();

	return 0;
}

