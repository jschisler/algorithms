#pragma once

#include <list>
#include <stack>

#include "../trees/node.h"

namespace one {

	class queue
	{
	public:
		queue();
		~queue();

		void push(int data) { list.push_back(data); }
		int front()			{ return list.front(); }
		void pop()			{ list.pop_front(); }
	private:
		std::list<int> list;
	};

}

namespace two {

	class queue {
	public: 
		void push(int data) { 
			instack_.push(data); 
		} 
		
		int front() const { 
			transfer(); 
			return outstack_.top(); 
		}

		void pop() { 
			transfer(); 
			return outstack_.pop(); 
		} 
	
	private: 
		void transfer() const { 
			if (outstack_.empty()) { 
				while (!instack_.empty()) { 
					outstack_.push(instack_.top()); 
					instack_.pop(); 
				} 
			} 
		} 
		
		mutable std::stack<int> instack_; 
		mutable std::stack<int> outstack_;
	};	
}

/*
	Another name for this enumeration is breadth-first traversal.
*/
void level_traversal(node* root, std::ostream& out) { 
	std::queue<const node*> queue; 
	queue.push(root); 
	
	while (!queue.empty()) { 
		const node* node = queue.front(); 
		queue.pop(); 
		
		if (node->left != nullptr)
			queue.push(node->left); 
		
		if (node->right != nullptr) 
			queue.push(node->right); 

		out << node->value;
	}
}

/* 
	Level Order Traversal with Separation Tokens

	A common modification to the question is to separate the levels with a token,
	such as a newline. In order to accomplish this, we can seed the queue with a 
	level end token such as the nullptr. When we encounter this token again we 
	know that the current level has been fully enumerated. If the token is then 
	enqueued again, it will be placed at the end of the level which was just 
	enqueued. With this trick, we can complete the solution using nullptr as 
	our level separation token.
*/
void level_traversal_with_tokens(node* root, std::ostream& out) {
	static const node* LEVEL_TOKEN = nullptr; 
	std::queue<const node*> queue; 
	
	queue.push(root); 
	queue.push(LEVEL_TOKEN); 
	
	while (!queue.empty()) {
		const node* node = queue.front(); 
		queue.pop(); 
		
		if (node == LEVEL_TOKEN) {
			if (!queue.empty()) { 
				out << std::endl; 
				queue.push(LEVEL_TOKEN); 
			} 
			
			continue; 
		}
		
		if (node->left != nullptr)
			queue.push(node->left); 
		
		if (node->right != nullptr) 
			queue.push(node->right); 
		
		out << node->value;
	}
}
