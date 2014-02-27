#include <vector>
#include <algorithm>
#include <functional>

int select(std::vector<int>* array, size_t k) { 
	sort(array->begin(), array->begin() + array->size()); 
	return array->at(k);
}

int median(std::vector<int>* array) { 
	return select(array, (array->size() - 1) / 2); 
}

/*
	In binary search, we were able to cut the search space in half by knowing 
	that every element with an index lower than the probe’s was smaller than 
	the probe, and every element with an index higher than the probe was 
	larger than the probe. Finding a good pivot element will allow similar 
	narrowing of the search space in selection.

	This is achieved by pivoting an array on an element that is close to the 
	median. Given a value and an array , the pivot operation leaves the array 
	in a state where every element of the array smaller than the pivot precedes 
	it, and every element in the array at least as larger as the pivot succeeds it.

	There are many methods of implementing a partition on an array. One could 
	use a list and push back larger values and push front smaller values. In 
	the sequel we implement an in place pivot which avoids memory allocation.

	The following listing partitions the sub-array defined by the offset and length 
	on the value provided. The algorithm used initializes the array with the pivot 
	element in the first position. It then enumerates the array from right to left. 
	When an entry with value less than the pivot is detected, the tail of the array 
	is shifted to the right of the pivot. That smaller value is then stored in the 
	empty location created by the shift.

	The return value is an index into the array. Every element of the array which 
	is less than or equal than the pivot has a lower index than the return value. 
	Every element that is greater than the pivot value has an index no less than 
	the return value.
*/
size_t partition(std::vector<int>* array, size_t pos, size_t offset, size_t length) { 
	if (length <= 1) 
		return offset; 
	
	int val = (*array)[pos]; 
	std::swap((*array)[offset], (*array)[pos]); 
	auto left = offset; 
	auto right = offset + length - 1; 
	
	while (left < right) { 
		if (val < (*array)[right]) { 
			--right; 
		} else { 
			std::swap((*array)[left], (*array)[left + 1]); 
			
			if (right != left + 1) { 
				std::swap((*array)[left], (*array)[right]); 
			} 
			
			++left; 
		} 
	} 
	
	return left; 
}

size_t pivot_index(const std::vector < int >& array, size_t offset, size_t size);

int select_with_pivot(std::vector<int>* array, size_t k) {
	std::function < int(std::vector < int >*, size_t, size_t) > selection = [k, &selection](std::vector<int>* array, size_t offset, size_t length) { 
		auto index = pivot_index(*array, offset, length); 
		size_t pos = partition(array, index, offset, length);

		if (pos == k) { 
			return array->at(k); 
		} 
		
		if (pos > k) {
			return selection(array, offset, pos - offset);
		} 
		
		return selection(array, pos + 1, length - (pos + 1 - offset)); 
	};
	
	return selection(array, 0, array->size());
}

int median_of_3(const std::vector<int>& array, size_t offset, size_t length) { 
	auto begin = array.begin() + offset; 
	std::vector<int> sub(begin, begin + length); 
	std::sort(sub.begin(), sub.end()); 
	
	return sub[(length - 1) / 2]; 
}

size_t pivot_index(const std::vector<int>& array, size_t offset, size_t length) {
	auto find_index = [&array, offset](const int val) { 
		size_t index = offset; 
		
		while (val != array[index]) { 
			++index; 
		} 
		
		return index; 
	}; 
	
	if (length <= 3) { 
		return find_index(median_of_3(array, offset, length)); 
	} 
	
	size_t num = length / 3 + ((length % 3) ? 1 : 0); 
	std::vector<int> medians(num); 
	
	for (size_t step = 0; step < num; ++step) {
		const static size_t three = 3; 
		size_t index = step * 3; medians[step] =
			median_of_3(array, offset + index, std::min(three, length - index));
	} 
	
	return find_index(median(&medians));
}