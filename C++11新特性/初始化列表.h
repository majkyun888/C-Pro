#pragma once
#include <vector>
#include <string>
#include <list>
#include <map>
using namespace std;

vector<double> v = { 1, 2, 3.456, 99.99 };
list<pair<string, string>> languages = {
	{"Nygaard", "Simula"},
	{"Richards", "BCPL"},
	{"Ritchie", "C"}
};

map<vector<string>, vector<int>> years = {
	{
		{"Maurice","Vincent", "Wilkes"},
		{1913, 1945, 1951, 1967, 2000}
	},
	{
		{"Martin", "Ritchards"},
			  {1982, 2003, 2007}
	},
	{
		{"David", "John", "Wheeler"},
			  {1927, 1947, 1951, 2004}
	}
};
void f(std::initializer_list<int> li)
{
	for (auto ite = li.begin(); ite != li.end(); ite++)
		cout << *ite << "\t";
	cout << endl;
}