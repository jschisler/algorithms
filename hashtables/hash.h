/*
	A hash table stores elements in an array indexed by their hash value. Each 
	entry in the array is called a bucket. Each element is associated with a key 
	that is used to uniquely identify the bucket that stores the element. The hash 
	function maps the element’s key to the bucket in which to store the element. 
	The goal of hash functions is to provide constant time retrieval of stored 
	elements while minimizing insertion time.
*/

#include <string>

/*
	This function is deterministic and extremely efficient, but its range is 
	limited to only 26 values and hence many strings will hash to the same 
	value. Further, the corpus of English words it is not uniformly distributed 
	across these 26 values. An examination of the words in the Gutenberg 
	library shows that 17% of the words start with t, nearly 12% of words 
	start with the letter a but only 0.05% of words start with z.
*/
size_t hash(const std::string& str) { 
	if (str.empty()) return 0; 

	return str[0] - 'a'; 
}

/*
	While this function has a larger range, it is still small and rather sparse. 
	Also it no longer guarantees constant time operation. The hash value of an 
	extremely long word takes much more time to compute than the hash value of 
	a short word. To overcome these drawbacks, let’s consider taking each four 
	character set from the word and cast them as an integer. This significantly 
	increases the range. We can sum these sets to distribute the words over the 
	range. And to guarantee constant time operation we bound the number of 
	characters used.  This shown in hash3() below.
*/
size_t hash2(const std::string& str) { 
	size_t hash_value = 0; 
	
	for (auto c : str) { 
		hash_value += c - 'a'; 
	} 
	
	return hash_value; 
}

/*
	This is better because it maintains a constant time evaluation and we are 
	increasing the range. 
	
	It is common that complexity increases as we aim to smooth the distribution 
	or increase the range. But with greater complexity comes longer run time, 
	and an unfortunate choice of hash functions can decrease the promise of 
	constant time lookup.
*/
size_t hash3(const std::string& str) { 
	int count = 16; 
	size_t hash_value = 0; 
	const char * cstr = str.c_str(); 
	
	while (cstr && * cstr && --count) { 
		hash_value += (* cstr++ -'a') << (count % 4); 
	} 
	
	return hash_value; 
}

