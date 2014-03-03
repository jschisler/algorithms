/*
	Sorting is a fundamental task in programming, and no one should be taken 
	off guard when asked to sort a collection of integers or strings. One should 
	be able to produce a number of optimal sorts from memory. This means not 
	only avoiding quadratic sorts, but also choosing appropriately between quick 
	sort, merge sort, radix sort, and others depending on the problem constraints.

	All optimal sorts have worse case run time of order nlog n over unbounded 
	data. Insertion sort is often used for small data sets, but it has quadratic 
	worst case running time. It should be avoided unless the problem is explicitly 
	constrained so that it is efficient. Otherwise in an interview the only 
	quadratic sorting algorithm you should ever give is quick sort. And even with 
	a naive quick sort implementation, you should be able to explain that its 
	basic implementation has order nlog(n) running time in expectation, but can 
	be made to have deterministic order nlog(n) running time with a clever choice 
	of pivots.

	We will implement five sorting algorithms and discuss their comparative 
	merits. These algorithms are insertion sort, heap sort, quick sort, 
	radix sort, and merge sort.
*/
#include <vector>
#include <algorithm>
#include <tuple>

/*
	Insertion sort is an inefficient sort that is practical for small data sets 
	or data sets with only a small perturbation from sorted order.

	Insertion sort works by iteratively inserting each element of an array into 
	its proper position in a sorted sub-array. A standard implementation has an 
	outer loop and an inner loop. The outer loop visits each value of an array 
	from front to back, leaving behind a sorted sub-array. The inner loop 
	searches for the position of the outer loop value in the sorted sub-array 
	by scanning from back to front. During this search a portion of the sorted 
	sub-array is shifted right one position, and the value is inserted in the 
	vacated position.

	As can be seen, insertion sort is an in-place algorithm as it does not 
	require allocation of memory from the heap. Insertion sort is also a stable 
	sort, as the algorithm maintains the relative order of elements. Lastly, as 
	only the indices and a temporary value are required insertion sort requires 
	constant extra space. 
	
	The main drawback is that insertion sort has quadratic worst case behavior. 
	Our next algorithm overcomes this drawback.
*/
void insertion_sort(std::vector<int> *array) { 
	for (unsigned int i = 1; i < array->size(); ++i) { 
		auto val = (*array)[i];
		
		for (int j = i - 1; j >= 0 && (*array)[j] > val; --j) { 
			std::swap((*array)[j], (*array)[j + 1]); 
		} 
	} 
}

//  Why not do it this way and avoid the dereferencing of the array?
//  Also declare the variables once and reuse them.  
//  Why query the size of the array constantly if it never changes?
void insertion_sort2(std::vector<int> &array) {
	int val;
	unsigned int count = array.size();

	for (unsigned int i = 1; i < count; ++i) {
		val = array[i];

		for (int j = i - 1; j >= 0 && array[j] > val; --j) {
			std::swap(array[j], array[j + 1]);
		}
	}
}

/*
	Heap sort uses a priority queue to achieve optimal worst case running time 
	for a sort.

	Heap-sort operates as follows. With a call to make_heap , a max heap is 
	built in-place. Afterward, max values are iteratively removed and stored in 
	their proper location in a partially sorted sub-array at the tail end of the 
	array. Recalling that make_heap is an order nlog n operation, and that 
	pop_heap is a logarithmic time operation called once for each element in the 
	array we can see that heap-sort is an optimal sort.

	Using the heap utilities in stl, a concise implementation of heap sort if 
	available.

	Note that by following make_heap with a call to sort_heap call, we can have 
	a two line solution for heap sort. However, doing obscures insight into the 
	operations of heap sort from the text. 
	
	Since building the heap and removing the max require no external memory, 
	heap sort is an in-place algorithm. Since the heap creation may shuffle 
	the relative locations of elements, heap-sort is not a stable sort. And 
	lastly, from our implementation of the in-place heapify method we can see 
	that heap sort uses constant space.
*/
void heap_sort(std::vector<int> *array) { 
	std::make_heap(array->begin(), array->end()); 
	
	for (size_t offset = array->size(); offset > 0; offset--) { 
		std::pop_heap(array->begin(), array->begin() + offset); 
	} 
}

/*
	Quick sort is a divide and conquer algorithm that, like selection, partitions 
	the data on a properly chosen pivot and recursively operates on the sub-arrays 
	produced. 
	
	With a random pivot selection, quick sort provides expected optimal run time. 
	However the worst case run time is quadratic. This is overcome by choosing 
	a pivot element that is near the median of the array. The implementation of 
	median-of-3 provided in the section on selection would suffice to guarantee 
	optimal worst case behavior. For conciseness we use the linear time 
	nth_element method provided by stl. Doing so will cause an increase in 
	effective run time, but the worst-case asymptotic bound is unaffected.

	As can be seen in our study of selection, finding the median element is an 
	in-place operation. Partitioning and sub-array recursing are also in-place, 
	and hence quick sort is an in-place sort. Further, when nth_element is 
	stable so is quick sort. Lastly, note that quick sort uses only a constant 
	amount of extra space. 
	
	For these reasons quick sort is often the best generic sorting algorithm to 
	use in applications.
*/
void quick_sort(std::vector<int>::iterator begin, std::vector<int>::iterator end) { 
	size_t len = end - begin; 
	
	if (len < 2) 
		return; 
	
	size_t pivot = len / 2; 
	std::nth_element(begin, begin + pivot, end); 
	quick_sort(begin, begin + pivot); 
	quick_sort(begin + pivot, end);
}

/*
	In radix sort values are first bucketed by the value of some digit prior 
	to merging. By bucketing over more than two sets, radix sort is a 
	generalization of quick sort. For input from a bounded set, radix sort 
	achieves linear time worst case behavior. However this run time falls back 
	to quadratic when the input is from an unbounded domain.

	Radix sort can be implemented on any radix; for instance binary digits or 
	decimal digits. Below we implement radix sort on hexadecimal digit 
	representations of integers.

	Radix sort is only optimal for fixed length data, such as 32-bit 
	integers. In order to bucket the input, radix sort requires external 
	memory and is not in-place. Radix sort is a stable sort, as the relative 
	order of values is maintained through each iteration of the algorithm.
*/
void radix_sort(std::vector<int> *array) { 
	std::vector<std::vector<int>> buckets(0xf + 1); 
	
	for (size_t byte = 0; byte < sizeof(int); ++byte) { 
		unsigned int offset = byte * 4; 
		unsigned int mask = 0xf << offset; 
		
		for (auto& val : *array) { 
			auto bucket = (mask & val) >> offset; 
			buckets[bucket].push_back(val); 
		} 
		
		size_t index = 0; 
		
		for (auto& bucket : buckets) { 
			for (auto& val : bucket) { 
				array->at(index++) = val; 
			} 
			
			bucket.clear(); 
		} 
	} 
}

/*
	Merge sort is another divide-and-conquer algorithm for sorting. The idea 
	springs from the observation that if one starts with two sorted sub-arrays,
	it is simple to combine them into a single sorted array. This can be done 
	by first comparing the smallest elements of each array for the minimum. That 
	element is removed from its current array and added to the result , and the 
	process iterates. Since the original arrays are sorted, this involves only a 
	linear scan. So all that is left is how to divide. But as with quick sort we 
	can always divide input into smaller halves until we are left with individual 
	units from which we can begin to combine into sorted output.

	In-place merge is a difficult algorithm to implement; however, it is provided 
	by stl as the function inplace_merge. Leveraging this, we have a parsimonious 
	implementation of in-place merge sort in C++.

	As the merge is in-place, this implementation requires only memory allocation 
	on the stack. However the recursive calls may require up to logarithmic extra 
	space. Merge sort can be implemented as a stable sort. 
	
	While worst case behavior is asymptotically equivalent to that of quick sort, 
	it has been argued that in-place quick sort is faster in practice. There are 
	many possible reasons for this, including optimization of the inner loop of 
	quick sort and lower stack space requirements. However, merge sort is the 
	obvious choice when sorting over large data.
*/
void merge_sort(std::vector<int>::iterator begin, std::vector<int>::iterator end) { 
	size_t len = end - begin; 
	
	if (1 >= len) 
		return; 
	
	size_t prefix_len = len / 2; 
	merge_sort(begin, begin + prefix_len); 
	merge_sort(begin + prefix_len, end); 
	std::inplace_merge(begin, begin + prefix_len, end);
}

/*
	A favorite interview question at Google asks to sort 4GB of data using only 
	100MB of memory. While the specific numerical limits may change, the key insight 
	is to use merge sort to accomplish sorting over large data. 
	
	This is an exercise in partitioning and merging. First each contiguous 
	sub-array of the smaller bound of data should be sorted with quick sort. 
	Doing so is fast, in place, and requires only a constant amount of extra 
	memory. Afterward, each of these sub-arrays are merged as with merge sort. 
	A sample implementation of the merge is provided below.
*/
void merge_vectors(const std::vector<std::vector<int>>& in, std::vector<int> * out) {
	typedef std::vector<int> Array; 
	typedef Array::const_iterator Iterator; 
	typedef std::tuple<Iterator, Iterator> Element; 
	std::vector<Element> heap; 
	
	for (unsigned int index = 0; index < in.size(); ++index) { 
		if (!in[index].empty()) { 
			Element tuple(in[index].begin(), in[index].end()); 
			heap.push_back(tuple); 
		} 
	} 
	
	auto comparator = [](const Element& lhs, const Element& rhs) { 
		return *std::get<0>(lhs) < *std::get<0>(rhs); 
	}; 
	
	std::make_heap(heap.begin(), heap.end(), comparator); 
	
	while (!heap.empty()) { 
		auto tuple = heap.front(); 
		std::pop_heap(heap.begin(), heap.end(), comparator); 
		out->push_back(*std::get<0>(tuple)); 
		
		if (std::get<1>(tuple) != ++std::get<0>(tuple)) { 
			heap.push_back(tuple); 
			std::push_heap(heap.begin(), heap.end(), comparator); 
		} 
	}
}