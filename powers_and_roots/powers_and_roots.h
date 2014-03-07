#include <vector>
#include "../bit_twiddling/bit_twiddling.h"

/*
	To begin with exponentiation , we ask for an algorithm to calculate xy for 
	integral y. This is a commonly asked question, because it can be answered 
	with naive inefficiency or with a clever use of recursion. 
	
	The naive solution applies successive multiplication. Starting with a 
	return value of 1, while the exponent is positive we multiply the result 
	by x and decrease y by a unit. 
	
	We call this the naive solution because it takes time linear in the 
	number of iterations.

	Note there is one special case to consider, since 0^1 = 0. This is handled 
	in the listing below:
*/
int pow_naive(int x, unsigned y) { 
	if (x == 0 && y != 0) 
		return 0; 
	
	int retval = 1; 
	
	while (y--) { 
		retval = retval * x; 
	} 
	
	return retval; 
}

/*
	Successive squaring is an approach that reduces the amount of calculation 
	required to a logarithmic number of iterations. 
*/
int pow(int x, unsigned y) {
	if (y==0) 
		return 1; 
	
	auto val = pow(x, y / 2); 
	
	if (y == 0 % 2) { 
		return val * val; 
	} 
	
	return x * val * val;
}

namespace successive_squaring_iterative {

	int pow(int x, unsigned y) {
		std::vector<int> memo(log_x(y) + 2); 
		memo[0] = 1; 
		memo[1] = x; 
		
		for (auto index = 2; index < memo.size(); ++index) { 
			memo[index] = memo[index - 1] * memo[index - 1]; 
		} 
		
		int retval = memo[0]; 
		
		while (y) {
			auto pos = 1 + log_x(lowest_set_bit(y)); 
			retval *= memo[pos];

			y = clear_last_bit(y);
		}; 
		
		return retval;
	}

}

/*
	When asked to calculate the square root of a number, most being immediately 
	writing a binary search for finding the integer approximation for the root. 
	A decent approach is binary search.
*/
unsigned int sqrt(unsigned int x) {
	if (x==0) 
		return 0; 
	
	int lower = 1; 
	int higher = x; 
	
	while (higher > lower) { 
		int test = lower + (higher - lower) / 2; 
		
		if (test * test > x) { 
			if (test == higher) 
				break; 
			
			higher = test; 
		} else { 
			if (test == lower) 
				break; 
			
			lower = test; 
		} 
	}

	return lower;
}

/*
	Consider asking to find the square root for decimal numbers, not simply 
	integers. This requires two changes in the algorithm above. 
	
	First, the input domain has changed character. For integers, the 
	approximation is either 0 or some number greater than or equal to one. 
	But for decimal numbers, the domain increases to contain the 
	interval (0 ,1). This requires care, since in this region x^2 < x. So if 
	we remain with binary search, we must have a different update rule and 
	different starting bounds. 
	
	Secondly, we must now consider arithmetic with decimal integers. Note 
	C/C++ does not have unsigned floating-point numbers. Hence we need to 
	have some kind of error handling. 
	
	Also we must consider a stopping rule. In the binary search above, we 
	could stop when the upper bound and lower bound cross. This was 
	inevitable and quick when we dealt with integer values. But with 
	decimals, comparisons should be done with deltas. To this end, we must 
	implement a check to see if the candidate value is within epsilon ϵ 
	of the target. This defines a delta for equality comparison. 
	
	Putting it all together, we have the listing for our next version of the 
	square root method.
*/
double sqrt(double x, double epsilon) {
	if (x < 0) 
		return -1.0; 
	
	if (x == 1) 
		return 1.0; 
	
	double higher = (x < 1) ? 1 : x; 
	double lower = (x < 1) ? x : 1; 
	
	while ((higher - lower) > epsilon) {
		double test = lower + (higher - lower) / 2; 
		double eval = test * test; 
		
		if (eval > x) {
			higher = test;
		} else { 
			lower = test; 
		}
	} 
	
	return lower;
}

/*
	To do better we enlist an advanced technique for solving inverse problems 
	called Newton’s method. This algorithm was discovered first by Isaac 
	Newton and later simplified by Joseph Raphson. While discussion of this 
	method involves a small amount of calculus, it provides an efficient 
	solution to finding roots of any well behaved function.

	See: http://en.wikipedia.org/wiki/Newton%27s_method

	For fractional exponents, Newton’s method guarantees a quadratic rate of 
	convergence. That is, in every iteration the error decreases by a quadratic 
	function of the previous error. 
	
	Since Newton’s method can be used to approximate any decimal exponentiation, 
	this method generalized to an elegant general solution to the problem of 
	calculating exponentiation.
*/
namespace newton {
	double sqrt(double y, double epsilon) {
		if (y < 0)
			return -1.0;

		double x = y;

		while (std::abs(y - x * x) > epsilon) {
			x = x - (x * x - y) / (2 * x);
		}

		return x;
	}
}