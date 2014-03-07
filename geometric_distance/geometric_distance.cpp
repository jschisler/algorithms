// geometric_distance.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "vector.h"
#include "line.h"
#include "geometric_distance.h"

double epsilon;

bool parallel(const line& s, const line& t) {
	return std::abs(s.u.dot(t.u) - 1) < epsilon;
}

bool operator == (const line& p, const line& q) {
	if (!parallel(p, q))
		return false;

	auto t = 0.0;

	if (0 != q.u.x)
		t = (p.a.x - q.a.x) / q.u.x;
	else
		t = (p.a.y - q.a.y) / q.u.y;

	vector a = { q.a.x + t * q.u.x, q.a.y + t * q.u.y };

	if (std::abs(a.x - p.a.x) < epsilon && std::abs(a.y - p.a.y) < epsilon) {
		return true;
	}

	return false;
}

double distance(const vector& a, const vector& b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double distance(const vector& p, const line& l) {
	vector a_minus_p = l.a + (p * -1);
	vector n = l.u * a_minus_p.dot(l.u);
	vector perp = a_minus_p + (n * -1);

	return perp.length();
}

vector intersection(const line& p, const line& q){
	auto det = p.u.x * q.u.y - p.u.y * q.u.x;
	auto t = (q.u.x*(p.a.y - q.a.y) - q.u.y*(p.a.x - q.a.x)) / det;
	return{ p.a.x + t * p.u.x, p.a.y + t * p.u.y };
}

vector unit_vector(const vector& v) {
	if (v.x == 0 && v.y == 0)
		return v;

	auto length = v.length();

	return{ v.x / length, v.y / length };
}
