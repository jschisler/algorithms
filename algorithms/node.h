#include <algorithm>

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

node* circular_insert(node* head, int data) {
	auto insertion = new node(data); 
	
	if (nullptr == head) { 
		insertion->next = insertion; 
		return insertion; 
	} 
	
	if (head == head->next) {
		head->next = insertion; 
		insertion->next = head;
		return insertion;
	} 
	
	std::swap(head->data, insertion->data); 
	insertion->next = head->next; 
	head->next = insertion; 
	return head;
}

