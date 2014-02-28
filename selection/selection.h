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

typedef std::vector<int> Array;
typedef std::vector<Array> Arrays;

typedef std::tuple<Array&, size_t, size_t> ArrayBounds; 
typedef std::vector<ArrayBounds> ArrayBoundsList;

void initialize_data(Arrays* arrays, ArrayBoundsList* data, size_t* total_size) { 
	for (auto& array : *arrays) { 
		data->push_back(ArrayBounds(array, 0, array.size())); 
		*total_size += array.size(); 
	};
}

/*
	In our solution we must distinguish between the position of an element in 
	an array and its index. The reason for this is that if we find that a value 
	is in the first position of n lists, it is also in the nth overall 
	position. However the sum of the indices is 0. We distinguish between 0 
	based indexes and 1 based position counter by variable name.

	We shall also see that we must pivot over a value that may not be in the 
	array. In order to properly update the search bounds, we must be able to 
	know whether or not the pivot operation encountered the pivot value. 
	Pivot information and an array of pivot information are stored in the 
	following data types. In the pair, the first entry is the position of the 
	pivot and the second is whether or not the pivot value was encountered 
	in partitioning.
*/

typedef std::pair<size_t, bool> PivotInfo; 
typedef std::vector<PivotInfo> Pivots;

void partition(const size_t index, const int value, ArrayBoundsList* data, Pivots* pivots, size_t* next_pos);
void update_search_bounds(const size_t target_pos, const size_t current_pos, const size_t data_index, ArrayBoundsList* data, Pivots* pivots);

/*
	With the pre- amble concluded, the following listing is the main loop of 
	a solution for the median of large data. Notice that it takes each array 
	in turn, and determines if the median is a member of that list.

	It is determined if the median is a member of the current list by doing a 
	binary search over the known bounds of the median, and finding the 
	location of that element’s value within lists not yet searched.
*/
int median(Arrays* arrays) {
	ArrayBoundsList data; 
	size_t total_size = 0; 
	
	initialize_data(arrays, &data, &total_size); 
	auto median_pos = (total_size - 1) / 2 + 1; 
	size_t pos_offset = 0; 
	
	for (size_t data_index = 0; data_index < data.size(); ++data_index) {
		auto& tuple = data[data_index]; 
		auto& array = std::get<0>(tuple); 
		auto& offset = std::get<1>(tuple); 
		auto& length = std::get<2>(tuple); 
		auto begin = array.begin() + offset; 
		
		while (length != 0) {
			auto mid_index = offset + length / 2; 
			auto mid = array.begin() + mid_index; 
			
			std::nth_element(begin, mid, begin + length); 
			
			auto value = *(mid); 
			auto mid_pos = pos_offset + mid_index + 1; 
			
			Pivots pivots; 
			
			pivots.push_back({ mid_index, true }); 
			partition(data_index, value, &data, &pivots, &mid_pos);
			
			if (mid_pos == median_pos) 
				return value; 
			
			update_search_bounds(median_pos, mid_pos, data_index, &data, &pivots);
		} 
		
		pos_offset += offset;
	} 
	
	auto& tuple = data[data.size() - 1]; 
	auto& array = std::get<0>(tuple); 
	auto& offset = std::get<1>(tuple); 
	
	return array[offset];
}

/*
	This method pivots all other lists on the value being considered for the 
	median in the current list. It uses the stl partition method to first 
	separate values above and below the pivot value. It then checks the 
	minimum value of that partition to see if the pivot is a member of the 
	upper partition.
*/
void partition(const size_t index, const int value, ArrayBoundsList* data, Pivots* pivots, size_t* next_pos) {
	for (size_t offset = 1; offset + index < data->size(); ++offset) {
		auto data_index = index + offset; 
		auto& tuple = data->at(data_index);
		auto& array = std::get<0>(tuple); 
		auto begin = array.begin() + std::get<1>(tuple); 
		auto end = begin + std::get<2>(tuple); 
		auto pos = std::partition(begin, end, [value](int rhs) { 
			return rhs < value; 
		});
		bool found_value = false; 
		
		if (pos != end) { 
			nth_element(pos, pos, end); 
			
			if (value == *pos) { 
				found_value = true; 
			} 
		} 
		
		pivots->push_back({ pos - array.begin(), found_value }); 
		*next_pos += std::get<1>(tuple) + pivots->back().first + (pivots->back().second ? 1 : 0);
	}
}

/*
	This updates the search bounds for every list. The update is based on whether 
	the over-all median is less than or greater than the considered value. It is 
	during this update that the binary search is affected on the current list.
*/
void update_search_bounds(const size_t target_pos, const size_t current_pos, const size_t data_index, ArrayBoundsList* data, Pivots* pivots) {
	std::function<void(size_t, size_t)> update = nullptr;

	if (current_pos > target_pos) { 
		update = [data, pivots](size_t index, size_t offset) { 
			auto& tuple = data->at(index + offset); 
			std::get<2>(tuple) = pivots->at(offset).first - std::get<1>(tuple); 
		}; 
	} else { 
		update = [data, pivots](size_t index, size_t offset) { 
			auto& tuple = data->at(index + offset); 
			std::get<2>(tuple) -= pivots->at(offset).first - std::get<1>(tuple); 
			std::get<1>(tuple) = pivots->at(offset).first; 
			
			if (offset == 0) { 
				std::get<2>(tuple) -= 1; 
				std::get<1>(tuple) += 1; 
			} 
		}; 
	} 
	
	for (size_t offset = 0; offset + data_index < data->size(); ++offset) { 
		update(data_index, offset); 
	}
}