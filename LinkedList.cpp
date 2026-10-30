#include "LinkedList.h"

LinkedList::LinkedList() {
	head = nullptr;
	tail = nullptr;
}
void LinkedList::InsertAtHead(char data) {
	if (head == nullptr) {
		head = new Node(data);
		tail = head;

	}
	else {
		Node* newNode = new Node(data);
		newNode->next = head;
		head = newNode;
	}
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

}
void LinkedList::InsertAfter(char data, Node* node) {
	if (node == nullptr) return;
	Node* newNode = new Node(data);
	newNode->next = node->next;
	newNode->back = node;
	node->next = newNode;
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
}
LinkedList::~LinkedList() {
	Node* node = head;
	while (node != nullptr) {
		Node* next = node->next;
		delete node;
		node = next;
	}
}


void LinkedList::deleteValue(char data) {
	Node* node = head;
	Node* prev = nullptr;
	while (node != nullptr && node->data != data) {
		prev = node;
		node = node->next;
	}
	if (node == nullptr) { return; }
	if (prev == nullptr) { head = node->next; }
	else {
		prev->next = node->next;
		node->back = prev;
	}
	delete node;

}
void LinkedList::display() {
	for (Node* node = head; node != nullptr; node = node->next) {
		std::cout << node->data << " ";
	}
}