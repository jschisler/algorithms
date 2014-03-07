#include "../geometric_distance/node.h"

struct triangle { 
	point a; 
	point b; 
	point c; 
	
	triangle(const point& a, const point& b, const point& c) : a(a), b(b), c(c) {} 
};