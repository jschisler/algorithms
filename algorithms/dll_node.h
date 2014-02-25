////////////////////////////////////////////////////////
//  Doubly linked list
struct dll_node {
	dll_node* prev;
	dll_node* next;
	int data;

	explicit dll_node(int data) : dll_node(this, this, data) {}
	dll_node(dll_node* prev, dll_node* next, int data) : prev(prev), next(next), data(data) {}
};

dll_node* insert(dll_node* head, int data) {
	if (head == nullptr)
		return new dll_node(data);

	auto insertion = new dll_node(head->prev, head, data);
	insertion->prev->next = insertion;
	insertion->next->prev = insertion;

	return insertion;
}

dll_node* find(dll_node* head, int value) {
	auto current = head;

	while (current != nullptr && current->data != value) {
		current = current->next;

		if (current == head) return nullptr;
	}

	return current;
}

dll_node* remove(dll_node* head, dll_node* node) {
	if (head->next == head) {
		delete node;
		return nullptr;
	}

	node->prev->next = node->next;
	node->next->prev = node->prev;

	if (head == node) {
		head = node->next;
	}

	delete node;
	return head;
}

dll_node* reverse(dll_node* head) {
	if (head == nullptr) return nullptr;

	if (head->next == head->prev) {
		return head;
	}

	auto tail = head;

	do {
		auto temp = tail->next;
		tail->next = tail->prev;
		tail->prev = temp;
		tail = temp;
	} while (tail != head);

	return head->prev;
}

bool is_palindrome(dll_node* head) {
	if (head == nullptr || head == head->next) return true;

	auto tail = head->prev;

	do {
		if (head->data != tail->data) return false;

		head = head->next;
		tail = tail->prev;
	} while (head != tail && head != tail->next);

	return true;
}