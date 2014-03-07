#pragma once

#include "../geometric_distance/vector.h"

typedef vector point;

struct node {
	vector point;
	node* left = nullptr;
	node* right = nullptr;

	explicit node(vector point) : point(point) {}
};