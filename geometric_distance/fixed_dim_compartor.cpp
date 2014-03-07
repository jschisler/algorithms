#include "stdafx.h"
#include "fixed_dim_compartor.h"
#include "geometric_distance.h"
#include "node.h"

#include <functional>
#include <vector>
#include <algorithm>
#include <stack>

node* initialize(std::vector < vector > points) {
	std::function<node*(size_t, std::vector<vector>::iterator, std::vector<vector>::iterator)> build_kd_tree = 
		[&build_kd_tree](size_t dim, std::vector<vector>::iterator begin, std::vector<vector>::iterator end) { 
			if (begin == end) 
				return (node*) nullptr; 
		
			auto mid = begin + (end - begin) / 2; 
			nth_element(begin, mid, end, fixed_dim_compartor(dim)); 
			auto root = new node(*mid); 
			dim = (dim + 1) % 2;

			root->left = build_kd_tree(dim, begin, mid); 
			root->right = build_kd_tree(dim, mid + 1, end); 
		
			return root; 
		}; 
	
	return build_kd_tree(0, points.begin(), points.end());
}

vector find(node* kd_tree, const vector& point) {
	int dim = 0; 
	std::stack<vector> path; 
	
	while (kd_tree != nullptr) { 
		path.push(kd_tree->point); 
		kd_tree = fixed_dim_compartor(dim)(kd_tree->point, point) ? kd_tree = kd_tree->right : kd_tree = kd_tree->left; 
		dim = (dim + 1) % 2; 
	} 
	
	vector neighbor = { 0, 0 };
	double min = std::numeric_limits<double>::max(); 
	/*
	while (!path.empty()) {
		if (path.top() == point) { //  doesn't compile
			neighbor = path.top(); break;

		}; 
		
		auto dist = distance(path.top(), point); //  doesn't compile
		
		if (dist <= min) { 
			min = dist; 
			neighbor = path.top(); 
		} 
		
		path.pop();
	} 
	*/
	return neighbor;
}