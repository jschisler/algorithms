#pragma once

#include "vector.h"

class fixed_dim_compartor
{
public:
	fixed_dim_compartor(unsigned dim) : dim_(dim) {}

	bool operator() (const vector& a, const vector& b) {
		if (dim_ == 0)
			return a.x < b.x;

		return a.y < b.y;
	}

private:
	const unsigned dim_;
};

