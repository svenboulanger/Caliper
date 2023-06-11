// Caliper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "point.h"
#include <set>
using namespace std;

int main()
{
	set<int> set;
	std::set<int>::iterator it1, it2, it3;
	it1 = set.insert(1).first;
	it2 = set.insert(2).first;
	it3 = set.insert(3).first;

	cout << *it1 << ' ' << *it2 << ' ' << *it3 << std::endl;
	set.erase(2);

	it2++;
	cout << *it2 << endl;

	cout << "done";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
