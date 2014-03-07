#include "stdafx.h"
#include "polygon.h"
#include "../geometric_distance/geometric_distance.h"

#include <algorithm>

point center(const polygon& poly) {
	double x = 0.0; 
	double y = 0.0; 
	
	for (auto& p : poly) {
		x += p.x / poly.size(); 
		y += p.y / poly.size();
	} 
	
	return { x, y };
}

/*  //  This is giving me irritation
double sine_of_angle(point a, point b, point c) {
	auto bottom = unit_vector(b - a);
	auto top = unit_vector(c - b);
	return top.x * bottom.y - top.y * bottom.x;
}

polygon convex_hull(const polygon& poly) {
	polygon out(poly); 
	point initial = poly[0];

	for (auto& p : poly) { 
		if (p.y < initial.y || (p.y == initial.y && p.x < initial.x)) { 
			initial = p; 
		} 
	} 
	
	std::sort(out.begin(), out.end(), [&initial](const point& lhs, const point& rhs) {
		double epsilon = 100;

		if (lhs == initial)
			return true;

		if (rhs == initial)
			return false;

		vector horizon = { 1, 0 };
		auto ulhs = unit_vector(lhs - initial);
		auto urhs = unit_vector(rhs - initial);
		auto diff = horizon.dot(ulhs) - horizon.dot(urhs);

		if (diff > epsilon)
			return true;
		else if (diff < -1 * epsilon)
			return false;

		return lhs.length() < rhs.length();
	});

	for (int index=2; index < out.size(); ++index) {
		while (out.size() > 3 &&
			0 < sine_of_angle(out[index - 2],
			out[index - 1],
			out[index])) {
			out.erase(out.begin() + index - 1);
			index = std::max(2, index - 1);
		}
	}
	return out;
}
*/