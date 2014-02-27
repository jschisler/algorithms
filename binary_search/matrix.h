#include <vector>
#include <functional>
#include <algorithm>

typedef std::vector<std::vector<int>> matrix; 
typedef std::pair<size_t, size_t> coordinate;

/*
	Diagonal Search of a Matrix

*/
coordinate find(const matrix& m, const int key) {
	coordinate coord = { m.size() - 1, 0 };

	while (coord.first < m.size() && coord.second < m[0].size() && m[coord.first][coord.second] != key) { 
		if (m[coord.first][coord.second] > key) { 
			coord.first--; 
		} else { 
			coord.second++; 
		} 
	} 
	
	return coord;
}

coordinate find(const matrix& m, const int key) {
	std::function<bool(	const coordinate& lower, 
						const coordinate& upper, 
						coordinate* out)> 
		recursive_find = [&key, &m, &recursive_find](
			const coordinate& lower, 
			const coordinate& upper, 
			coordinate* out) { 
		
		if (lower.first > upper.first || lower.second > upper.second || m.size() < std::max(lower.first, upper.first) || m[0].size() < std::max(lower.second, upper.second)) { 
			return false; 
		}
		
		size_t mid_row = lower.first + (upper.first - lower.first) / 2; 
		const auto& row_offset = m[mid_row].begin(); 
		size_t mid_col = std::lower_bound(row_offset + lower.second, row_offset + upper.second, key) - row_offset; 
		
		if (mid_row < m.size() && mid_col < m[mid_row].size() && m[mid_row][mid_col] == key) { 
			*out = { mid_row, mid_col }; 
			return true; 
		} 
		
		return 
			recursive_find(	{ mid_row + 1, lower.second }, 
							{ upper.first, mid_col - 1 }, 
							out) 
							|| 
			recursive_find( { lower.first, mid_col }, 
							{ mid_row - 1, upper.second }, 
							out); 
	};

	coordinate lower = { 0, 0 }; 
	coordinate upper = { m.size(), m[0].size() };
	coordinate out = { 0, 0 }; 
	recursive_find(lower, upper, &out); 
	
	return out;
}

