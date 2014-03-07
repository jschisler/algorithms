#pragma once

#include "vector.h"

struct node {
	vector point;
	node* left = nullptr;
	node* right = nullptr;

	explicit node(vector point) : point(point) {}
};