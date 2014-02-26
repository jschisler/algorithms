#include <vector>
#include <set>
#include <algorithm>
#include <functional>
#include <istream>

/*
	Initialization of a binary heap from a vector involves a logical representation 
	of a binary tree and reordering of the elements so that they satisfy the heap 
	property. We begin by describing the binary tree structure used to represent 
	the heap. 
	
	The binary tree structure comes from corresponding the indexes of entries in 
	the vector to level order positions of nodes in a complete binary tree. Index 
	0 of the vector corresponds to the root of the binary tree. For a node at 
	position i in the vector, the left child is at position 2 * i + 1, and the 
	right child is at 2 * i + 2. It will be useful to have a method that computes 
	these indexes.

	Notice with this scheme the next available leaf node is the rightmost element 
	past the end of the heap. The parent of a node at index i is located 
	at (i - 1) ∕ 2. Integer division achieves rounding down.	
*/
size_t lchild_index(const size_t index) { 
	return index * 2 + 1; 
} 

size_t rchild_index(size_t index) {
	return index * 2 + 2;
}

size_t parent_index(const size_t index) { 
	return (index != 0) ? (index - 1) / 2 : 0; 
}

/*
	The heapify operation starts with the single element of the vector at index 0. 
	This single element sub - array trivially satisfies the heap property. The 
	operation next iteratively adds elements from the vector in such a way that 
	the heap property is maintained. Concretely, new elements are first added 
	as leaves.Afterward, parent and child pairs are repeatedly swapped if in 
	violation of the heap property.When the algorithm terminates, the maximum 
	value of the heap is at the first index and the heap property holds.
*/
void heapify(std::vector<int>* array) { 
	for (auto itr = array->begin(); itr != array->end(); ++itr) { 
		auto index = itr - array->begin(); 
		auto parent = parent_index(index); 
		
		while (index != parent && (*array)[parent] < (*array)[index]) { 
			std::swap((*array)[parent], (*array)[index]); 
			index = parent; 
			parent = parent_index(index); 
		} 
	} 
}

/*
	Insertion of a new value into a binary heap begins with adding the value 
	as the next available leaf node in the binary tree.Afterward, the 
	parent-child relationships are updated to satisfy the heap property as was 
	done in heapify. Insertion requires the vector to be dynamically resized, 
	and hence is not an in-place operation.
*/
void insert(std::vector<int>* heap, int value) { 
	heap->push_back(value); 
	auto index = heap->size() - 1; 
	auto parent = parent_index(index); 
	
	while (index != 0 && (*heap)[index] > (*heap)[parent]) { 
		std::swap((*heap)[index], (*heap)[parent]); 
		index = parent; 
		parent = parent_index(index); 
	} 
}

/*
	In a binary heap, the find_max operation requires only dereferencing the 
	first index of the vector. Correctness of this action is guaranteed by the 
	heap property.
*/
int find_max(const std::vector<int>& heap) { 
	return heap[0]; 
}

/*
	Removal of the maximum heap value must accomplish two objectives aside from 
	replacing the value at the initial position. First, it must promote the 
	second minimum value to the first position. Doing so causes the tree to 
	pivot, and may have a cascading affect on the structure of the heap. 
	Secondly, the tree structure and parent-child relationships must be maintained.

	Both requirements are satisfied by the following sequence. First replace 
	the root value of the heap with the value of the last leaf node, and then 
	resize the array to the smaller size. This maintains the tree structure and 
	removes the maximum element from the array. We are required to then fix up 
	the heap property. In this operation, the heap property is maintained by 
	iteratively swapping parent-child pairs if in violation of the heap property.
*/
void remove_max(std::vector<int>* heap) { 
	std::swap(*heap->begin(), * (heap->end() - 1)); 
	heap->resize(heap->size() - 1); 
	
	size_t index = 0; 
	
	while (index < heap->size()) { 
		auto child = lchild_index(index); 
		
		if (child >= heap->size())
			break;
		
		auto right = rchild_index(index);
		
		if (right < heap->size() && (*heap)[right] > (*heap)[child]) 
			child = right; 
		
		if ((*heap)[index] >= (*heap)[child]) 
			break;
		
		std::swap((*heap)[index], (*heap)[child]); 
		index = child; 
	} 
}

/*
	When a key is increased in a max heap, the heap property may no longer hold 
	for that node and its parent. Hence, by increasing the value of a key, we 
	may need to decrease its index repeatedly until the array again has the heap 
	property. This is done by our standard practice of iteratively swapping.
*/
void increase_key(std::vector<int>* heap, size_t index) { 
	(*heap)[index]++; 
	
	auto parent = parent_index(index); 
	
	while (index != 0 && (*heap)[index] > (*heap)[parent]) { 
		std::swap((*heap)[index], (*heap)[parent]);
		
		index = parent_index(index);
		parent = parent_index(index);
	}
}

/*
	Solving programming problems relies as much upon choosing the correct data 
	structure as it does the proper algorithm. Consider the problem of 
	enumerating powers of an initial set of non-negative numbers . Concretely, 
	given a set of integers the question is to list the powers of the integers 
	in increasing order without duplicates. For example, given the set of 2,3,4, 
	the first six powers would be 1=2^0, 2=2^1, 3=3^1, 4=4^1=2^2, 8=2^3, 9=3^2 .
*/
void enumerate_powers(const std::set<unsigned>& set, size_t num_powers, std::vector<int> * out) {
	typedef std::pair<unsigned, unsigned> ValueTerm; 
	std::vector<ValueTerm> heap; 
		
	for (auto& value : set) { 
		heap.push_back({ 1, value });
	} 
		
	std::make_heap(heap.begin(), heap.end(), std::greater<ValueTerm>());
		
	int value = 0; 
		
	while (num_powers != 0 && heap.size() != 0) { 
		auto entry = heap.front(); 
		std::pop_heap(heap.begin(), heap.end(), std::greater<ValueTerm>());
			
		if (value != entry.first) { 
			value = entry.first; 
			out->push_back(value); 
			--num_powers; 
		} 
			
		heap.back() = { entry.first * entry.second, entry.second };
		std::push_heap(heap.begin(), heap.end(), std::greater<ValueTerm>());
	}
}

/*
	Consider the problem of determining the top k elements of a stream. Online 
	algorithms with stream inputs usually constrain the problem to a single 
	pass, as either the data is volatile or is too large to fit into 
	main memory.
	
	Let’s begin by reviewing the simple case of finding the maximum element. The 
	maximum element of a vector can be found by a linear scan.
*/
int max(const std::vector<int>& array) { 
	int max_val = array[0]; 
	
	for (auto& element : array) { 
		max_val = std::max(element, max_val); 
	} 
	
	return max_val; 
}

/*
	A little intuition leads us to recall that a single element array is a 
	trivial priority queue. The generalization of this process to finding the 
	top k elements of an n element stream is solved in a single pass with a min 
	heap. To operate within the constraints of memory, we will bound size of the 
	priority queue.

	To operate on a bounded size heap, we only insert when the value of an item is 
	larger than the smallest element in the heap . When it is, we remove the 
	smallest element with a remove_max operation and then insert the new value.

	The run time of this algorithm is an efficient O(log k * n), as each removal 
	of the minimum element and insertion may take time logarithmic in the size of 
	the heap. This is efficient for the online problem.
*/
void find_topk(std::istream& in, size_t k, std::vector<int>* heap) {
	heap->reserve(k); 
	
	int val; 
	
	while (heap->size() < k && in >> val) { 
		heap->push_back(val); 
	} 
	
	std::make_heap(heap->begin(), heap->end(), std::greater<int>()); 
	
	while (!in.eof() && in >> val) { 
		if (val > heap->front()) { 
			std::pop_heap(heap->begin(), heap->end(), std::greater<int>());
			heap->back() = val; 
			std::push_heap(heap->begin(), heap->end(), std::greater<int>()); 
		}
	}
}