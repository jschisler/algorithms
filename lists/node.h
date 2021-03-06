﻿#include <algorithm>
#include <stack>

//  This implementation is just like a stack
struct node {
	node* next;
	int data;

	explicit node(int data) : node(nullptr, data) {}
	node(node* head, int data) : next(head), data(data) {}
};

//  Returns the head of the list.
//  The old name was insert() but another name for this is push(), since it becomes the head of the list, pushing the rest to below it
node* push(node* head, int data) { return new node(head, data); }

//  Returns the head of the list.
//  This should be called insert() and overload the previous one
node* insert_sorted(node* head, int data) {
	if (head == nullptr || data <= head->data) return new node(head, data);

	auto current = head;

	while (current->next != nullptr && current->next->data < data)
		current = current->next;
	
	current->next = (current->next == nullptr) ? new node(data): new node(current->next, data);

	return head;
}

//  Returns the head of the list.
node* insert_sorted_test(node* head, int data) {
	if (head == nullptr || data <= head->data) return new node(head, data);

	auto current = head;

	while (current->next != nullptr && data > current->next->data)
		current = current->next;

	current->next = (current->next == nullptr) ? new node(data) : new node(current->next, data);

	return head;
}

node* find(node* head, int value) {
	while (head && head->data != value) {
		head = head->next;
	}

	return head;
}

node* remove(node* head, node* target) {

	if (target == head) {
		head = head->next;
		delete target;
		return head;
	}

	auto temp = head;

	while (temp->next != nullptr && temp->next != target) {
		temp = temp->next;
	}

	if (target == temp->next) {
		temp->next = temp->next->next;
		delete target;
	}

	return head;
}

//  This function inserts a new node before the current head node
node* circular_insert(node* head, int data) {
	auto insertion = new node(data); 
	
	//  If the list is empty, this becomes the head and needs to refer to itself
	if (nullptr == head) { 
		insertion->next = insertion; 
		return insertion; 
	} 
	
	//  If this list has 1 item in it, we put the new node after the current head
	//  and fix up the links
	if (head == head->next) {
		head->next = insertion; 
		insertion->next = head;
		return insertion;
	} 

	//  This is if there are multiple items in the list
	//  First, we swap the head value with the value in the new node
	//  This basically makes the new node's value the head without moving pointers, simplifying the whole process
	std::swap(head->data, insertion->data); 

	//  Set insertion->next to point to the same node as head->next
	insertion->next = head->next;

	//  Change head->next to point to the new insertion node
	head->next = insertion; 

	//  head hasn't changed.  Return it.
	return head;
}

node* circular_find(node* head, int value) { 
	auto current = head; 
	
	while (current != nullptr && current->data != value) {
		current = current->next; 
		
		if (current == head) 
			return nullptr;
	} 
	
	return current; 
}

node* circular_remove(node* head, node* target) { 
	if (head == head->next) { 
		delete head; 
		return nullptr; 
	} 
	
	auto next = target->next;
	target->data = next->data; 
	target->next = next->next; 
	delete next;

	return target; 
}

//  Wasteful version 
node* mid_by_counting(node* head) {
	if (head == nullptr) return head;

	auto trailing = head;
	size_t size = 0;

	while (head != nullptr) {
		++size;
		head = head->next;
	}

	size_t mid = size / 2;

	while (mid--) {
		trailing = trailing->next;
	}

	return trailing;
}

//  Optimized version
node* mid(node* head) {
	auto trailing = head;

	while (head != nullptr) {
		head = head->next;

		if (head != nullptr) {
			head = head->next;
			trailing = trailing->next;
		}
	}

	return trailing;
}

node* kth_from_end(node* head, size_t k) { 
	auto trailing = head; 
	
	while (k-- && head != nullptr) {
		head = head->next; 
	} 
	
	while (head != nullptr && head->next != nullptr) {
		head = head->next; 
		trailing = trailing->next; 
	} 
	
	return trailing; 
}

node* remove_values(node* head, int value) {
	while (head != nullptr && head->data == value) { 
		auto temp = head; 
		head = head->next; 
		delete temp; 
	} 
	
	auto target = head; 
	
	while (target != nullptr) { 
		while (target->next != nullptr && target->next->data == value) { 
			node* temp = target->next; 
			target->next = target->next->next; 
			delete temp; 
		} 
		target = target->next;
	} 
	
	return head;
}

node* reverse(node* head) {
	node* prev = nullptr;

	while (head != nullptr) {
		node* temp = head->next;
		head->next = prev;
		prev = head;
		head = temp;
	}

	return prev;
}

bool equals(node* x, node* y) { 
	while (x && y) { 
		if (x->data != y->data) return false; 
		
		x = x->next; 
		y = y->next; 
	}
	
	if (x == nullptr && y == nullptr) return true; 
	
	return false;
}

node* merge(node* h1, node* h2) { 
	if (h1 == nullptr || h2 == nullptr) return h1 ? h1 : h2; 
	
	auto head = h1; 
	
	while (h1->next != nullptr) h1 = h1->next; 

	h1->next = h2; 
	
	return head;
}

bool is_palindrome(node* head) {
	auto temp = head;
	std::stack <node* > s;

	while (temp != nullptr) {
		s.push(temp); 
		temp = temp->next; 
	} 
	
	while (!s.empty()) { 
		if (head->data != s.top()->data) return false;
		
		s.pop(); 
		head = head->next; 
	} 
	return true;
}

bool detect_loop(node* head) {
	if (head == nullptr) return false; 
	
	auto trailing = head; 
	auto leading = head; 
	
	while (leading != nullptr) {
		leading = leading->next; 
		
		if (trailing == leading) return true;
		
		trailing = trailing->next;
		
		if (leading != nullptr) leading = leading->next;
	} 
	
	return false;
}