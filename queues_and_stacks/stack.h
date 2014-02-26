#pragma once

#include <list>
#include <queue>

namespace one {
	class stack
	{
	public:
		stack();
		~stack();

		void push(int data) { list.push_front(data); }
		int top() { return list.front(); }
		void pop() { list.pop_front(); }

	private:
		std::list<int> list;
	};
}

namespace two {
	class stack { 
	
	public: 
		void push(int data) { 
			std::queue<int>* enqueue = queue1_.empty() ? &queue1_ : &queue2_;
			std::queue<int>* dequeue = queue1_.empty() ? &queue2_ : &queue1_;
			
			enqueue->push(data); 
			
			while (!dequeue->empty()) { 
				enqueue->push(dequeue->front()); dequeue->pop(); 
			} 
		} 
		
		int top() const { 
			if (!queue1_.empty()) 
				return queue1_.front(); 
			
			return queue2_.front(); 
		} 
		
		void pop() { 
			if (!queue1_.empty()) { 
				queue1_.pop(); 
				return; 
			} 
			
			queue2_.pop(); 
		} 
	
	private: 
		std::queue<int> queue1_; 
		std::queue<int> queue2_; 
	};
}