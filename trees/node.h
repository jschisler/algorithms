#include <queue>
#include <set>
#include <stack>
#include <complex>
#include <functional>

/*
	A binary search tree is a binary tree that maintains a sort order between 
	the parent and children nodes.The order invariant is whenever a node has a 
	left or right child, 
	node->value > left->value and node->value < = right->value.
	A binary search tree is balanced when for every node, the depth of the 
	sub-trees from the left child and right child differ by at most one. Most 
	of the complexity bounds discussed below hold only for balanced binary 
	search trees.
*/
struct node {
	int value; 
	node* left = nullptr; 
	node* right = nullptr; 
	
	explicit node(int value) : value(value) {};
};

/*
	Insertion in a binary search tree is a logarithmic time operation. This is 
	because to maintain the order invariant, an insertion requires search to 
	identify which node in a tree will become the parent of the inserted node. 
	After a parent node is identified by a find operation, only a constant 
	number of operations are needed to complete insertion.
*/
node* insert(node* root, int val) {
	if (root == nullptr) 
		return new node(val);
	
	if (val < root->value) 
		root->left = insert(root->left, val);
	else 
		root->right = insert(root->right, val);
	
	return root;
}

/*
	The utility of a binary search tree is derived from the logarithmic time search 
	guarantee of the data structure. This guarantee is only for balanced binary 
	search trees. Search is the motivation for use of a binary tree, and as such 
	is the fundamental operation on binary search trees.

	Note that the logarithmic time for search is guaranteed because when we branch 
	left or right, we are decreasing the search space by a constant fraction. Each 
	branch cuts the search space in half. We can only divide the search space by 
	two a logarithmic number of times before we are left with a single element.
*/
node* find(node* root, int value) { 
	if (root == nullptr || root->value == value) 
		return root;
	
	if (value < root->value) 
		return find(root->left, value);
	
	return find(root->right, value); 
}

/*
	The find parent operation is used to find the successor of a node in a binary 
	search tree. Every node in binary search tree has a data member with a comparison 
	operation. Suppose the data in a binary tree is sorted in increasing order. 
	The successor of a target node is a node in a binary search binary tree whose 
	data would immediately follow the target node’s data in a sorted list of all 
	the data members associated with the binary tree. The successor plays a role 
	in removal from a binary search tree in that it can replace the target node 
	and maintain the order invariant.
*/
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

/*
	Every node in binary search tree has a data member with 
	a comparison operation. Suppose the data in a binary tree is sorted in 
	increasing order. The successor of a target node is a node in a binary 
	search binary tree whose data would immediately follow the target node’s 
	data in a sorted list of all the data members associated with the binary 
	tree. The successor plays a role in removal from a binary search tree in 
	that it can replace the target node and maintain the order invariant.
*/
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

/*
	Removal from a binary search tree has a number of cases that need to be 
	considered to maintain the order invariant. Removing a leaf node is simple, 
	since the node has no children it can be deallocated and the parent updated. 
	If a node has a single child, then that child can replace the node in the 
	binary search tree and the node deallocated. In the final case when a node 
	has two children, we want to replace the node with the value of its 
	successor. Doing so would maintain the order invariant at the node’s location 
	in a binary search tree. We then need to recursively remove the successor from 
	the tree to avoid having a duplicate node. 
	
	All together, this provides the following algorithm:
*/
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

/*
	The size of a binary tree is its number of nodes. Notice that from any node, 
	the size of the sub-tree rooted at that node is one more than the sum of the 
	size of the sub-trees rooted at it’s children. We can easily calculate the 
	size of a binary tree recursively.
*/
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

/*
	The depth of a binary tree is the length of the longest simple path from 
	the root to a leaf node. The depth from a node is one more than the depth 
	of that node’s deepest sub-tree, so there is an immediate recursive solution.
*/
size_t depth(node* root) { 
	if (root == nullptr) 
		return 0; 
	
	return 1 + std::max(depth(root->left), depth(root->right));
}

/*
	Let’s consider an iterative solution. We will again traverse the tree, but 
	this time repeatedly to the leaf nodes. In order to determine if we are 
	branching left or right in a depth first enumeration, we keep track of the 
	list of visited nodes in a set. At any point in the algorithm below, the 
	stack contains the path from the root to the node on top of the stack, so 
	the current depth is equal to the size of the stack. The max depth of any 
	leaf is equal to the depth of the binary search tree.
*/
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

/*
	A binary tree is balanced if for every node the difference between the depth of 
	its left and right sub-trees is at most one. To determine whether or not a 
	binary tree is balanced we must evaluate whether or not every sub-tree is 
	balanced. Doing so naively would require multiple calculations of depth.
*/
bool balanced_naive(node* root) { 
	if (root == nullptr) return true; 
	
	if (!balanced_naive(root->left) || !balanced_naive(root->right))
		return false;
	
	size_t left_depth = depth(root->left); 
	size_t right_depth = depth(root->right); 
	
	return std::abs<int>(right_depth - left_depth) <= 1;
}

/*
	A linear solution would require modifying the recursion to return the depth 
	of the sub-tree as well as if it is balanced or not. The run time gains 
	come from reusing the partial calculations made in determining if the left 
	and right sub-trees are balanced.
*/
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

/*
	The ancestry of a node in a binary tree is the path from the root to the 
	node. A mathematical property of a tree is that there is exactly one 
	simple path between any two nodes in a tree. The calculation of ancestry 
	in a binary search tree is extremely efficient. We need only store the 
	search path used in the find operation.
*/
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

/*
	The lowest common ancestor of two nodes in a binary tree is the last common 
	node in the paths from the root to each node. This node can be found by 
	using our previous implementation of find_path. Having calculated the path 
	from the root to a node, we can compare the two paths until they diverge.
*/
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

/*
	If we were to consider calculating the paths in tandem, a node would be in 
	both paths only if both parameters were left children or right children of
	the target node. This observation leads to a recursive solution.
*/
node* lca_recursive(node* root, node* x, node* y) {
	if (root == x || root == y) return root; 
	
	if ((x->value < root->value && y->value >= root->value) || (y->value < root->value && x->value >= root->value))
		return root;
	
	if (x->value < root->value) 
		return lca_recursive(root->left, x, y);
	
	return lca_recursive(root->right, x, y);
}

/*
	An iterative solution to this problem is easier than iterative solutions to 
	calculating size or paths. Since determining the least common ancestor does 
	not require keeping state, we have no need of an ancillary queue or stack.
*/
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

/*
	An in-order traversal guarantees that the data members of a binary search 
	tree will be visited in their sort order. This is accomplished by first 
	recursing left, visiting the current node, and then recursing right.
*/
void inorder(node* root, std::function < void(node*)> visit) { 
	if (root == nullptr) 
		return; 
	
	inorder(root->left, visit); 
	visit(root); 
	inorder(root->right, visit);
}

/*
	A post-order traversal evaluates a current node’s data value only after 
	recursively visiting both children.
*/
void postorder(node* root, std::function < void(node*)> visit) {
	if (root == nullptr) 
		return; 
	
	postorder(root->left, visit); 
	postorder(root->right, visit); 
	visit(root);
}

/*
	A pre-order traversal is the opposite of a post-order traversal. A pre-order 
	traversal evaluates a current nodes data value prior to recursively visiting 
	either children.
*/
void preorder(node* root, std::function < void(node*)> visit) { 
	if (root == nullptr) 
		return; 
	
	visit(root); 
	preorder(root->left, visit); 
	preorder(root->right, visit);
}

/*
	We show how to reconstruct a binary tree given any two distinct recursive 
	traversals. The key to solving this problem is recognizing the root from 
	one traversal, and then dividing the traversals into left and right sub-trees 
	using the other.
	
	In the following, we provide an algorithm for tree reconstruction 
	from an in-order traversal and a pre-order traversal. In a pre-order traversal, 
	the first element is always the root of the next sub-tree. The ambiguity is 
	whether the next value is a left or right child of the root. In the in-order 
	traversal, given an index in the traversal, all values to the left are in the 
	left sub-tree and all values to the right are in the right sub-tree. 
	
	In the solution below, we first extract the root from the pre-order traversal. 
	We then find the index of this value in the in-order traversal. If the value 
	does not appear at the beginning of the in-order traversal there is left 
	sub-tree. If it does not appear at the end, there is a right sub-tree. In either 
	case we extract from each traversal the subsequences corresponding to a sub-tree, 
	and recurse.
*/
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