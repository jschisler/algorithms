#include <queue>
#include <set>
#include <stack>
#include <complex>
#include <functional>

struct node {
	int value; 
	node* left = nullptr; 
	node* right = nullptr; 
	
	explicit node(int value) : value(value) {};
};

node* insert(node* root, int val) {
	if (root == nullptr) 
		return new node(val);
	
	if (val < root->value) 
		root->left = insert(root->left, val);
	else 
		root->right = insert(root->right, val);
	
	return root;
}

node* find(node* root, int value) { 
	if (root == nullptr || root->value == value) 
		return root;
	
	if (value < root->value) 
		return find(root->left, value);
	
	return find(root->right, value); 
}

node* find_parent(node* root, node* target) { 
	if (root == target) 
		return nullptr; 
	
	while (root->left != target && root->right != target) { 
		if (target->value < root->value) 
			root = root->left;
		else 
			root = root->right; 
	} 
	
	return root; 
}

node* find_successor(node* root, node* target) {
	auto successor = target->right; 
	
	if (successor != nullptr) {
		while (successor->left != nullptr) {
			successor = successor->left;

		} return successor;
	} 
	
	do { 
		if (successor != nullptr) 
			target = successor; 
		
		successor = find_parent(root, target);
	} while (successor != nullptr && successor->right == target); 
	
	return successor;
}

node* remove(node* root, node* target) {
	if (target->left != nullptr && target->right != nullptr) { 
		node* next = find_successor(root, target); 
		int data = next->value; 
		
		remove(root, next); 
		target->value = data; 
		
		return root; 
	} 
	
	if (target->left != nullptr || target->right != nullptr) {
		node* temp = target->left ? target->left : target->right;

		target->value = temp->value; 
		target->left = temp->left; 
		target->right = temp->right; 
		delete temp; 
		
		return root;
	} 
	
	if (root == target) { 
		delete root; 
		return nullptr; 
	} 
	
	node* parent = find_parent(root, target); 
	
	if (parent->left == target) 
		parent->left = nullptr;
	else 
		parent->right = nullptr;
	
	delete target; 
	return root;
}

size_t size(node* root) { 
	if (root == nullptr) return 0; 
	
	size_t count = 0; 
	std::queue<node*> q; 
	
	q.push(root); 
	
	while (!q.empty()) { 
		auto front = q.front(); 
		q.pop(); 
		++count; 
		
		if (front->left != nullptr)
			q.push(front->left); 
		
		if (front->right != nullptr) 
			q.push(front->right);
	} 
	
	return count;
}

size_t depth(node* root) { 
	if (root == nullptr) 
		return 0; 
	
	return 1 + std::max(depth(root->left), depth(root->right));
}

size_t depth_iterative(node* root) {
	if (root == nullptr) 
		return 0; 
	
	size_t maxdepth = 0; 
	std::set<node*> visited; 
	std::stack<node*> s; 
	
	s.push(root); 
	
	while (!s.empty()) {
		node* top = s.top(); 
		
		if (top->left != nullptr && visited.end() == visited.find(top->left)) 
			s.push(top->left);
		else if (top->right != nullptr && visited.end() == visited.find(top->right))
			s.push(top->right);
		else { 
			visited.insert(top); 
			maxdepth = std::max(maxdepth, s.size()); 
			s.pop(); 
		}
	} 
	
	return maxdepth;
}

bool balanced_naive(node* root) { 
	if (root == nullptr) return true; 
	
	if (!balanced_naive(root->left) || !balanced_naive(root->right))
		return false;
	
	size_t left_depth = depth(root->left); 
	size_t right_depth = depth(root->right); 
	
	return std::abs<int>(right_depth - left_depth) <= 1;
}

bool balanced(node* root) { 
	std::function<bool(node*, size_t*)> balanced_depth = [&balanced_depth](node* root, size_t* depth) { 
		if (root == nullptr) { 
			*depth = 0; 
			return true; 
		} 
		
		size_t left; 
		size_t right; 
		
		if (!balanced_depth(root->left, &left)) 
			return false;
		
		if (!balanced_depth(root->right, &right))
			return false;
		
		*depth = 1 + std::max(left, right); 
		
		if (std::abs<int>(left - right) > 1)
			return false;
		
		return true;
	}; 
	
	size_t depth; 
	
	return balanced_depth(root, &depth);
}

bool find_path(node* root, node* target, std::vector<node*>* path) { 
	while (root != nullptr && target != nullptr && (path->empty() || path->back() != target)) { 
		path->push_back(root); 
		
		if (target->value < root->value) 
			root = root->left; 
		else 
			root = root->right; 
	}

	return !target || path->back() == target;
}

node* lca(node* root, node* x, node* y) { 
	std::vector<node*> x_path; 
	std::vector<node*> y_path; 
	
	find_path(root, x, &x_path); 
	find_path(root, y, &y_path); 
	
	node* lca = nullptr; 
	auto x_path_iterator = x_path.begin(); 
	auto y_path_iterator = y_path.begin(); 
	
	while (x_path_iterator != x_path.end() && y_path_iterator != y_path.end() && * x_path_iterator == * y_path_iterator) { 
		lca = * x_path_iterator; 
		x_path_iterator++; 
		y_path_iterator++; 
	} 
	
	return lca; 
}

node* lca_recursive(node* root, node* x, node* y) {
	if (root == x || root == y) return root; 
	
	if ((x->value < root->value && y->value >= root->value) || (y->value < root->value && x->value >= root->value))
		return root;
	
	if (x->value < root->value) 
		return lca_recursive(root->left, x, y);
	
	return lca_recursive(root->right, x, y);
}

node* lca_iterative(node* root, node* x, node* y) { 
	if (x != nullptr && y != nullptr && x->value > y->value) 
		std::swap(x, y);
	
	while (root != nullptr && root != x && root != y && (y->value < root->value || x->value >= root->value)) { 
		if (y->value < root->value) 
			root = root->left; 
		else
			root = root->right; 
	}

	return root;
}

void inorder(node* root, std::function < void(node*)> visit) { 
	if (root == nullptr) 
		return; 
	
	inorder(root->left, visit); 
	visit(root); 
	inorder(root->right, visit);
}

void postorder(node* root, std::function < void(node*)> visit) {
	if (root == nullptr) 
		return; 
	
	postorder(root->left, visit); 
	postorder(root->right, visit); 
	visit(root);
}

void preorder(node* root, std::function < void(node*)> visit) { 
	if (root == nullptr) 
		return; 
	
	visit(root); 
	preorder(root->left, visit); 
	preorder(root->right, visit);
}

node* reconstruct_tree(const std::vector < int >& inorder, const std::vector < int >& preorder) {
	auto root = new node(preorder[0]); 
	size_t pos = std::find(inorder.begin(), inorder.end(), root->value) - inorder.begin(); 
	
	if (pos != 0) { 
		std::vector<int> left_inorder(inorder.begin(), inorder.begin() + pos);
		std::vector<int> left_preorder(preorder.begin() + 1, preorder.begin() + 1 + left_inorder.size());
		root->left = reconstruct_tree(left_inorder, left_preorder);
	} 
	
	if (pos != (inorder.size() - 1)) { 
		std::vector<int> right_inorder(inorder.begin() + pos + 1, inorder.end());
		std::vector<int> right_preorder(preorder.begin() + (preorder.size() - right_inorder.size()), preorder.end());
		
		root->right = reconstruct_tree(right_inorder, right_preorder);
	}
	
	return root;
}