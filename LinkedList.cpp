#include "LinkedList.h"

LinkedList::LinkedList() {
	head = nullptr;
	tail = nullptr;
	length = 0;
}

LinkedList::LinkedList(const LinkedList& rhs) {
	head = nullptr;
	tail = nullptr;
	length = 0;
	for (Node* node = rhs.head; node != nullptr; node = node->next) {
		InsertAtTail(node->data);
	}
}
void LinkedList::InsertAtHead(char data) {
	if (head == nullptr) {
		head = new Node(data);
		tail = head;
	}
	else {
		Node* newNode = new Node(data);
		newNode->next = head;
		head->back = newNode;
		head = newNode;
	}
	length++;
}
void LinkedList::InsertAtTail(char data) {
	if (head == nullptr) {
		head = new Node(data);
		tail = head; 
	}
	else {
		Node* newNode = new Node(data);
		tail->next = newNode;
		newNode->back = tail;
		tail = newNode;
	}
	length++;

}
void LinkedList::InsertAfter(char data, Node* node) {
	if (node == nullptr) {
		return;
	}
	Node* newNode = new Node(data);
	newNode->next = node->next;
	newNode->back = node;
	if (node->next != nullptr) {
		node->next->back = newNode;
	}
	else {
		tail = newNode;
	}
	node->next = newNode;
	length++;
}
void LinkedList::InsertBefore(char data, Node* node) {
	if (node == nullptr) return;
	Node* newNode = new Node(data);
	newNode->next = node;
	newNode->back = node->back;
	if (node->back != nullptr) {
		node->back->next = newNode;
	}
	else {
		head = newNode;
	}
	node->back = newNode;
	length++;
}
LinkedList::~LinkedList() {
	Node* node = head;
	while (node != nullptr) {
		Node* next = node->next;
		delete node;
		node = next;
	}
}

void LinkedList::deleteNode(Node* node) {
	if (node == nullptr) return;
	if (node->back != nullptr) {
		node->back->next = node->next;
	}
	else {
		head = node->next;
	}
	if (node->next != nullptr) {
		node->next->back = node->back;
	}
	else {
		tail = node->back;
	}
	delete node;
	length--;
}
int LinkedList::indexOf(Node* target) {
	int index = 0;
	Node* node = head;
	while (node != nullptr) {
		if (node == target) {
			return index;
		}
		else {
			node = node->next;
			index++;
		}

	}
	return -1;
}
Node* LinkedList::nodeAt(int index) {
	Node* node = head;
	for (int i = 0; i < index; i++) {
		if (node != nullptr) {
			node = node->next;
		}
		
	}
	return node;
}

int LinkedList::getLength() const {
	return length;
}
void setCursorPosition(SHORT x, SHORT y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };
	SetConsoleCursorPosition(hConsole, pos);
}
void drawAt(int x, int y, char symbol) {
	setCursorPosition(x, y);
	std::cout << symbol;
}

