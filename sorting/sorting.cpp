// sorting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sorting.h"
// tuple example
#include <iostream>     // std::cout
#include <tuple>        // std::tuple, std::get, std::tie, std::ignore

int _tmain(int argc, _TCHAR* argv[])
{
	std::tuple<int, char> foo(10, 'x');
	auto bar = std::make_tuple("test", 3.1, 14, 'y');

	std::get<2>(bar) = 100;                                    // access element

	int myint; char mychar;

	std::tie(myint, mychar) = foo;                            // unpack elements
	std::tie(std::ignore, std::ignore, myint, mychar) = bar;  // unpack (with ignore)

	mychar = std::get<3>(bar);

	std::get<0>(foo) = std::get<2>(bar);
	std::get<1>(foo) = mychar;

	std::cout << "foo contains: ";
	std::cout << std::get<0>(foo) << ' ';
	std::cout << std::get<1>(foo) << '\n';

	//  Sorting test
	int a[] = { 12, 10, 43, 23, -78, 45, 123, 56, 98, 41, 90, 24 };
	int num;

	num = sizeof(a) / sizeof(int);

	int b[12];

	mergesort(a, b, 0, num - 1);

	for (int i = 0; i<num; i++)
		cout << a[i] << " ";
	cout << endl;

	return 0;
}

