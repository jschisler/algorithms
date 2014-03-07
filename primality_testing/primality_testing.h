#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>
#include <random>
/*
	A straightforward implementation of primality testing tests whether or not 
	any value less than the parameter n divides evenly. For large values, it 
	is inefficient to attempt to test every smaller value. A better 
	implementation of primality testing will first take steps to reduce the 
	number of tests made. 
	
	For instance, it is clear that if there is a value that factors into the 
	parameter, than there must be value must be less than or equal to the 
	square root of the parameter. So we need no check every value, just those 
	between 2 and . And in that range, aside from two no other even value need 
	be tested. With these observations we have our first primality test.
*/

bool isprime_naive(unsigned n) { 
	if (n == 0 || n == 1) return false; 
	
	auto sqroot = sqrt(n); 
	if (2 <= sqroot && n % 2 == 0) 
		return false; 
	
	for (int index = 3; index <= sqroot; index += 2) { 
		if (n == 0 % index) 
			return false; 
	} 
	
	return true; 
}

/*
	The inefficiency of the naive algorithm is due to the redundant checking of 
	factors. We already noted that once 2 has been ruled out as a factor it is 
	clear that no even value can be a factor. Similarly once 3 has been ruled 
	out, then all multiples of three have been ruled out. What we are 
	discovering is called the sieve of Eratosthenes. That is once we know all 
	the primes less than a certain value, the next prime is simply the next 
	value that is not a multiple of any of these.

	As we already noted we need not check values above √n. Implementing the 
	sieve, we need to be concerned with tracking known non-factors above √n. 
	In the following listing, we use a bit vector to track discovered primes.

	See: http://en.wikipedia.org/wiki/Sieve_of_Eratosthenes

	Note that we can start at the square of the current value since every small 
	factor of that value has already been memoized by smaller factors. The sieve 
	trades space for time, by allowing a fast addition operation to replace an 
	expensive division operation. But since we have memoized the sieve, the data 
	calculated above can be reused.
*/
bool isprime(unsigned n) { 
	if (n < 2) 
		return false; 
	
	std::vector<bool> known_prime((unsigned int)sqrt(n), true); 
	
	for (size_t val = 2; val <= known_prime.size(); ++val) { 
		if (known_prime[val]) { 
			if ((n % val) == 0) 
				return false; 
			
			for (auto multiple = val * val; multiple < known_prime.size(); multiple += val) { 
				known_prime[multiple] = false; 
			} 
		} 
	} 
	
	return true; 
}

namespace uses_heap {
	/*
		An attempt to reduce the amount of space used can be made. For instance, a 
		heap provides an efficient data structure to hold the largest evaluated 
		possible factor of n. Once we find that the top of the heap misses a value, 
		we know that value is a prime.
	*/
	bool isprime(unsigned n) {
		if (n == 2) return true; 
	
		if (n < 2 || (n % 2) == 0) 
			return false; 
	
		auto sqroot = sqrt(n); 
		typedef std::pair<unsigned, unsigned> sieve_entry; 
		std::vector<sieve_entry> primes; 
	
		primes.push_back({ 4, 2 }); 
	
		unsigned current = primes.front().second; 
	
		while (current <= sqroot) {
			auto front = primes.front().first; 
			if ((front - 2) == current) { 
				unsigned prime = front - 1; 
			
				if ((n % prime)==0) 
					return false; 
			
				primes.push_back({ prime * prime, prime }); 
				std::make_heap(primes.begin(), primes.end(), std::greater<sieve_entry>()); 
			} 
		
			current = front; 
			std::pop_heap(primes.begin(), primes.end(), std::greater<sieve_entry>()); 
			primes.back().first += primes.back().second; 
			std::push_heap(primes.begin(), primes.end(), std::greater<sieve_entry>());
		} 
	
		return true;
	}
}

/*
	See: http://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test

	Note that the below algorithm can make only one kind of error. When n is 
	composite, it is unlikely that a false positive will be reported. 
	However when n is prime, the Miller-Rabin test is always correct. We can 
	reduce the error at the cost of increased run time by running the test 
	multiple times and reporting a value is composite if ever any run reports 
	it composite. 
	
	In conclusion we mention that the AKS (Agrawal, Kayal, Saxena) primality 
	test is a strongly polynomial deterministic algorithm discovered in 2002. 
	While a great achievement, discussion of that algorithm is much more 
	technical than that required for the Miller-Rabin test above, will need 
	to wait until another edition.

	See:  http://en.wikipedia.org/wiki/AKS_primality_test
*/
/*
namespace miller_rabin {
	bool isprime(unsigned n) {
		if (n == 2 || n == 3) 
			return true; 
		
		if (n < 2 || (n % 2) == 0) 
			return false; 
		
		auto s = 0; 
		auto d = n - 1; 
		
		while ((d % 2) == 0) { 
			s++; 
			d /= 2; 
		} 
		
		std::uniform_int_distribution<unsigned> dist(2, n-2); 
		
		for (auto i = 0; i < 3; ++i) { 
			auto a = dist(rng);  //  doesn't compile because of this
			auto x = ((unsigned)pow(a, d)) % n; 
			
			if (x == 1 || x == (n-1)) 
				continue; 
			
			for (auto j = 1; j < s; ++j) { 
				x = (x * x) % n; 
				
				if (x == 1) 
					return false; 
				
				if (x == (n - 1)) 
					break; 
			} 
			
			if (x == (n - 1)) 
				continue; 
			
			return false; 
		} 
		return true;
	}
}
*/