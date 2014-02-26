// trees.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "node.h"
#include "AVL.h"

#include <random>

int _tmain(int argc, _TCHAR* argv[])
{
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(1, 100); // define the range

	int values[40] = { 0 };
	int value;

	for (int i = 0; i < 40; ) {

		value = distr(eng);

		for (int j = 0; j <= i; j++) {
			if (values[j] == value)
				break;
			else if (j == i) {
				values[i++] = value;
				break;
			}
		}
	}

	node* head = nullptr;

	for (int n = 0; n < 40; ++n) {
		if (head == nullptr)
			head = new node(values[n]);
		else
			insert(head, values[n]);
	}


	return 0;
}

