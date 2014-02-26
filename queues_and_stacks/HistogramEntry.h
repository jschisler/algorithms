#include <vector>
#include <stack>
#include <algorithm>

struct HistogramEntry {
	int x; int y;
}; 

typedef std::vector<HistogramEntry> Histogram;

/*
	A naive solution starts by enumerating all pairs of histogram entries α and β. 
	It calculates the minimum height between α and β by iteratively checking each 
	of those values. The area is then computed and a running max is tracked. 
	However, this solution is cubic! 
	
	Using a stack, we can find a solution that solves the problem in a single pass. 
	The stack maintains a collection of all ‘open’ rectangles. The solution takes 
	it in turn to inspect each entry of the histogram from left to right. When 
	the height of histogram entries is increasing, an open rectangle is added to 
	the stack. It is specified by its x and y coordinates. But the y coordinate 
	must be the earliest rectangle preceding the entry that is at least as tall. 
	We will see that the y coordinate can be determined by closing open rectangles 
	before pushing the new entry onto the stack.
	
	When the height of histogram entry decreases during enumeration, all open entries 
	with height larger than the current value must be closed. The area of an open 
	entry is its height multiplied by width. The width is simply the current x 
	location minus the starting location. After the last entry has been visited, 
	a final pass must close all open entries.
*/
size_t largest_area(const Histogram& histogram) {
	size_t max_area = 0; 
	int current_pos = 0; 
	std::stack<HistogramEntry> open_entries; 
	
	for (auto& entry : histogram) {
		current_pos = entry.x; 
		int last_closed_pos = entry.x; 
		
		while (!open_entries.empty() && open_entries.top().y >= entry.y) {
			HistogramEntry top = open_entries.top(); 
			open_entries.pop(); 
			last_closed_pos = top.x; 
			size_t area = top.y * ((current_pos - 1) - (top.x - 1)); 
			max_area = std::max(max_area, area);
		} 
		
		open_entries.push({ last_closed_pos, entry.y });
	} 
	
	++current_pos; 
	
	while (!open_entries.empty()) { 
		HistogramEntry top = open_entries.top(); 
		open_entries.pop(); 
		size_t area = top.y * ((current_pos - 1) - (top.x - 1)); 
		max_area = std::max(max_area, area); 
	} 
	
	return max_area;
}