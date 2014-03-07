#include <vector>

/* 
	See: http://en.wikipedia.org/wiki/Fibonacci_number

	Naive recursive solution (never use!)

	When it terminates, this code will correctly produce the nth Fibonacci 
	number, but its running time is an exponential function of n.

	The problem is that in the recursive calculation for F(n-1) we need 
	to calculate F(n-2). But after this calculation we are asking the 
	function to again calculate F(n-2). So we are doing the same work 
	twice. This can be alleviated with memoization, or keeping track of 
	intermediary calculations to use again later. With this idea we can 
	write a linear version of our program (fib_memoization below).
 */
unsigned fib_naive(unsigned n) { 
	if (n == 0) 
		return 0; 
	
	if (n == 1) 
		return 1; 
	
	return fib_naive(n - 1) + fib_naive(n - 2);
}

/*
	Since the 44th Fibonacci number (701408733) is larger than max-int on a 
	32-bit machine there isn’t worry about a dynamic data structure growing too 
	large. Memoization could have been used in a recursive implementation as 
	well, but it would have been wise to pass the memo vector by reference. To 
	see that it has linear run time, note that the number of intermediate 
	calculations is the same as the sequence number we are asked to compute.
*/
unsigned fib_memoization(unsigned n) { 
	std::vector<unsigned> memo(n + 1); 
	
	for (size_t i = 0; i <= n; ++i) { 
		if (i == 0) 
			memo[i] = 0; 
		else if (i == 1) 
			memo[i] = 1; 
		else 
			memo[i] = memo[i-1] + memo[i-2]; 
	} 
	
	return memo[n]; 
}

/*
	Now we focus on minor improvements. For those with an eye to reducing memory 
	usage, we can see that only the last two entries of the memo need to be 
	kept. That is for Fibonacci, full memorization is unnecessary. We need memo 
	only the last two trailing values of the sequence. Implementing this idea 
	we have the following listing:
*/
unsigned fib_iterative(unsigned n) {
	if (n==0) 
		return 0;

	if (n==1) 
		return 1; 
	
	unsigned cur = 0; 
	unsigned trailing = 1; 
	
	for (size_t i = 0; i < n; ++i) { 
		unsigned temp = cur; 
		
		cur = cur + trailing; 
		trailing = temp; 
	} 
	
	return cur;
}

/*
	We’ve created an iterative program and abandoned the naive recursive call. 
	But there is another optimization that will take us back to recursion. 
	Tail recursion is an optimization in which the recursion can be unrolled. 
	In order to make use of it here, we will need to make use of helper 
	function. The helper function will hold the current and trailing values of 
	the sequence in the parameter list. It is defined inline. 
	
	To implement this version, we call the tail recursive helper function with 
	the properly initialized arguments.
*/
unsigned fib_tail_recursion(unsigned n, unsigned fib0, unsigned fib1) { 
	if (n==1) 
		return fib1; 
	
	return fib_tail_recursion(n - 1, fib1, fib0 + fib1); 
} 

unsigned fib(unsigned n) {
	if (n == 0)
		return 0;

	return fib_tail_recursion(n, 0, 1);
}

/*
	We leave the Fibonacci sequence with a trick bound to impress anyone asking 
	this question. Unless you’re familiar with recurrence relations, it is a 
	shock to many that we can do better than linear. In fact Fibonacci n can be 
	computed directly in logarithmic time. The reason for this is that there is 
	a closed form formula for the nth Fibonacci number, used to derive the 
	recurrence relation below.

	See: http://en.wikipedia.org/wiki/Golden_ratio
*/
unsigned fib_direct(unsigned n) {
	double positive_golden_ratio = 1 + sqrt(5.); 
	double negative_golden_ratio = 1 - sqrt(5.); 
	double pos_pow = pow(positive_golden_ratio, n); 
	double neg_pow = pow(negative_golden_ratio, n); 
	double numerator = pos_pow - neg_pow; 
	double denominator = pow(2., n) * sqrt(5.); 
	
	return (unsigned)(numerator / denominator); 
}