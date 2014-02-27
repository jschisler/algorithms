#include <vector>

/*
A few details of this implementation require further discussion. The function 
above returns either the position of the entry or the point at which the 
algorithm was searching when it determined the key is not present. We will 
see this definition is useful in the sequel.

The use of pointer arithmetic here works since in C++ vector arrays are 
contiguous, and iterators can be considered as their logical address. The 
midpoint is the average of the first address in the array, lower, and the last 
address in the array, array + len-1 added to the first.

Secondly, if we know that key ≠ *mid we no longer should have mid in the search 
range. So we can set lower and upper one position before or after mid as 
appropriate. Concretely, we can set upper to be mid - 1, as that is the lowest 
index that we have not evaluated.

Last, the calculation in the listing below to find the midpoint should always 
be used in programming interviews. It is the only means to write the expression 
for the midpoint without danger of an operation overflow. The more common way 
to write that line is mid = lower + (upper + lower) ∕ 2. However, it is possible 
for upper + lower to overflow. 

Binary search provides order log n lookup, and is optimal for comparison search. 
However while the screening interview may ask for a quick implementation, its 
specific application to interview problems is sometimes subtle.
*/
size_t find(const std::vector<int>& array, int key) { 
	auto lower = array.begin(); 
	auto upper = array.end() - 1; 
	
	while (lower < upper) { 
		auto mid = lower + (upper - lower) / 2; 
		
		if (key == *mid) 
			return mid - array.begin(); 
		
		if (key < *mid) 
			upper = mid - 1; 
		else 
			lower = mid + 1; 
	} 
	
	return lower - array.begin(); 
}

/*
	An interview question that can be solved with slight modification to binary 
	search is the problem of finding the leftmost occurrence of a target value 
	n in a sorted array. Modifying both the update rules and the exit criteria 
	of the previous implementation of binary search can solve this question.

	While previous implementation of the binary search algorithm provides an 
	incomplete answer, it does provide information on the location of the 
	target. We should immediately see that once we find any index that matches 
	the target value, we could scan linearly until we find the left most 
	element with the target value. While this provides a correct solution, 
	the run worst case time of the algorithm is linear.

	Note that when an index with the target value is found, we can update upper 
	to be that index that we discovered. Doing so would always keep the target 
	solution in range of our search. So we change the line upper = mid - 1 to 
	upper = mid. The update rule for lower need not change . However , we see 
	that if the lower bound is ever equal to the target after an update it must 
	be pointing to the left most entry. Our final solution then is a modified 
	binary search.
*/
size_t find_leftmost(const std::vector<int>& array, int key) { 
	auto lower = array.begin(); 
	auto upper = array.end() - 1; 
	
	while (lower < upper && *lower != key) { 
		auto mid = lower + (upper - lower) / 2; 
		
		if (key <= *mid) 
			upper = mid; 
		else 
			lower = mid + 1; 
	} 
	
	return lower - array.begin(); 
}

/*
	A more complicated question asks the candidate to search for a target value 
	in a sorted array that has been rotated. A rotated array is a sorted array 
	of length n in which every index i has been mapped to index i + j for some 
	integer value j. Concretely, the array 12345 rotated by 2 is the array 45123.

	For simplicity, we assume that there do not exist duplicate values in the 
	given array.

	There are two common solutions to this interview question, one requires a 
	complex modification to the update rule. The other requires a two-step 
	search algorithm.

	Let’s again see what modifications to binary search are required. Given 
	the example above, we immediately see that the problem with our candidate 
	solution is only the update rules. Since we cannot guarantee that array is 
	increasing from lower to mid and from mid to upper, we are not updating 
	properly. What is needed is to consider the location of the pivot. By 
	enumerating cases, we can see that the pivot is either in between lower 
	and mid, mid and upper or no longer being considered in the sub-array that 
	is being searched. Since the array does not contain duplicates, we can 
	determine whether or not the pivot is in the range lower to mid by testing 
	if lower < mid and if the key is bounded by lower and mid. This leads to our 
	first solution.
*/
size_t find_in_rotated(const std::vector < int >& array, int key) {
	auto lower = array.begin();
	auto upper = array.end() - 1; 
	
	while (lower < upper) {
		auto mid = lower + (upper - lower) / 2; 
		
		if (key == *mid) 
		{ 
			upper = mid; 
		} else if (*lower <= * mid) { 
			if (*lower <= key && key < *mid) { 
				upper = mid; 
			} else { 
				lower = mid + 1; 
			} 
		} else if (*mid <= *upper) {
			if (*mid < key && key <= *upper) { 
				lower = mid + 1; 
			} else {
				upper = mid;
			}
		}
	} 
	
	return lower - array.begin();
}

/*
	Let’s consider a second solution by addressing the problem of an unknown 
	pivot another way. If we knew the location of the pivot j, then instead of 
	special casing the ranges of lower to mid and mid to upper, we can instead 
	modify the binary search update rule to use modulo arithmetic. Concretely, 
	if n is the length of the array, we could set lower = j, 
	upper = j + n - 1(mod n) = j - 1(mod n) , and 
	mid = lower + (upper -lower) ∕ 2 (mod n). Our previous binary search is 
	then correct. This is implemented below.
*/
size_t find_with_pivot_offset(const std::vector<int>& array, size_t pivot, int key) { 
	auto begin = array.begin(); 
	auto length = array.size(); 
	auto lower = array.begin(); 
	auto upper = array.end() - 1; 
	
	while (lower < upper) { 
		auto mid = lower + (upper - lower) / 2; 
		auto mid_val = begin + (mid - begin + pivot) % length; 
		
		if (key <= * mid_val) 
			upper = mid; 
		else 
			lower = mid + 1; 
	} 
	
	return (lower - begin + pivot) % length; 
}

/*
	How can we find j? In linear time we can search for the index of the 
	minimum value. However, an optimal method can be built upon binary search 
	using update rules similar to the first solution of this problem. That is 
	we update the lower and upper bounds of a binary search to insure that the 
	pivot is always between them. When this can no longer be satisfied, then 
	the lower bound must be the pivot value.
*/
size_t find_pivot(const std::vector < int >& array) { 
	auto lower = array.begin(); 
	auto upper = array.end() - 1; 
	
	while (lower < upper) { 
		auto mid = lower + (upper - lower) / 2; 
		
		if (*lower <= *mid) { 
			if (*mid < *upper) 
				break; 
			
			lower = mid + 1; 
		} else { 
			upper = mid; 
		} 
	} 
	return lower - array.begin(); 
}

/*
	An interesting mathematical programming problem asks the candidate to find 
	the fixed-point in a function.

	Concretely, suppose that a monotonically increasing integral function is 
	defined on the integers [0, 100] taking values in [0, 100]. It is 
	represented by an array A, where f(x) = A[x]. Since the function is 
	bounded and increasing, there must be a value y such that f(y) = y. 
	Such a y is called a fixed-point.

	We can find the fixed-point with binary search with a process similar to 
	what was used to find the pivot in a rotated array. The idea is to always 
	update the bounds so that the fixed-point is in the range being searched. 
	To do this, we update so that lower ≤ A[lower] and upper ≥ A[upper]. 
	
	Below is an implementation of this solution.
*/
size_t fixed_point(const std::vector<int>& array) {
	auto lower = array.begin(); 
	auto upper = array.end() - 1;

	while (lower < upper) { 
		auto mid = lower + (upper - lower) / 2; 
		
		if (*mid <= (mid - array.begin())) { 
			upper = mid; 
		} else { 
			lower = mid + 1; 
		} 
	} 
	
	return lower - array.begin();
}

/*
	Suppose we are asked to find one value in a complete sequence of integers 
	that appears twice. A complete sequence of length n contains all the values 
	from 1 to n. We can solve this problem with binary search.

	The idea is to update the bounds so that we consider the half that is larger 
	than the number of integers it could hold if there were no duplicates. The 
	reason this works is similar to above. For if there is a duplicate value in 
	the array, and the array has all the elements in the sequence, then the length 
	of the array must be larger than the difference of its last and first elements.

	Note the similarity with finding the fixed-point of a function. The expected 
	middle value is what would be present if there were no duplicates. By 
	calculating it above and comparing against the actual mid, we are converting 
	the bounds so this and the fixed-point problem have the same problem.
*/
size_t find_duplicates(const std::vector<int>& array) {
	auto lower = array.begin(); 
	auto upper = array.end() - 1; 
	
	while (lower < upper) {
		auto mid = lower + (upper - lower) / 2; 
		auto mid_val = *lower + (*upper - *lower) / 2; 
		
		if (mid_val < *mid) { 
			upper = mid; 
		} else { 
			lower = mid + 1; 
		}
	}
	
	return lower - array.begin();
}