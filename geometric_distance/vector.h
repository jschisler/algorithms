#pragma once

#include <cmath>

struct vector {
	double x;
	double y;

	vector operator *(const double& scalar) const {
		return{ x * scalar, y * scalar };
	}

	vector operator +(const vector& rhs) const {
		return{ x + rhs.x, y + rhs.y };
	}

	double dot(const vector& rhs) const {
		return (x * rhs.x) + (y * rhs.y);
	}

	double length() const {
		return sqrt(x * x + y * y);
	}
};

