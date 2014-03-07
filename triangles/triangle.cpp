#include "stdafx.h"
#include "triangle.h"
#include "../geometric_distance/vector.h"

double area(const triangle& t) { 
	vector ab = t.b - t.a; 
	vector ac = t.c - t.a; 
	double a = 0.5 *(ab.x * ac.y - ab.y * ac.x); 
	
	return std::abs(a); 
}

double epsilon;

bool point_in_triangle(const triangle& t, const point& d) { 
	double triange_area = area(t); 
	triangle abd = triangle(d, t.a, t.b); 
	triangle adc = triangle(d, t.a, t.c); 
	triangle dbc = triangle(d, t.b, t.c); 
	double sum = area(abd) + area(adc) + area(dbc); 
	
	return std::abs(sum - triange_area) < epsilon; 
}
