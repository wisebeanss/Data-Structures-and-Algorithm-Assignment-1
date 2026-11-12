#include "LinkedList.h"

LinkedList::LinkedList() {
	head = nullptr;
	tail = nullptr;
	current = nullptr;
	length = 0;
}

//LinkedList::LinkedList(const LinkedList& rhs) {
//	head = nullptr;
//	tail = nullptr;
//	length = 0;
//	for (Node* node = rhs.head; node != nullptr; node = node->next) {
//		InsertAtTail(node->data);
//	}
//}
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
void LinkedList::drawAt(int x, int y, char symbol) {
	console.setCursorPosition(x, y);
	std::cout << symbol;
}
int LinkedList::construct() {
	int mouseX = 0;
	int mouseY = 0;
	int maxY = 0;
	for (Node* n = head; n != nullptr; n = n->next) {
		int dx = 0; int dy = 0;
		char connector = '-';
		drawAt(mouseX, mouseY, (current == n) ? 'o' : 'x');

		switch (n->data) {
		case 'U':
			dy = -1; connector = '|';
			break;
		case 'L':
			dx = -1; connector = '-';
			break;
		case 'D':
			dy = 1; connector = '|';
			break;
		case 'R':
			dx = 1; connector = '-';
			break;
		case 'N':
			break;
		}
		if (dx != 0 || dy != 0) {
			drawAt(mouseX + dx, mouseY + dy, connector);
		}
		mouseX += dx * 2;
		mouseY += dy * 2;
		if (mouseY > maxY) {
			maxY = mouseY;
		}
	}
	return maxY;
}
bool LinkedList::isValidPath() {
	int mouseX = 0; int mouseY = 0; int index = 0;
	int length = getLength();
	int* mouseXseen = new int[length + 1];
	int* mouseYseen = new int[length + 1];
	bool invalid = false;
	//Record Start cell
	mouseXseen[index] = mouseX;
	mouseYseen[index] = mouseY;
	index++;
	for (Node* n = head; n != nullptr; n = n->next) {
		int dx = 0; int dy = 0;
		switch (n->data) {
		case 'U':
			dy = -2;
			break;
		case 'L':
			dx = -2;
			break;
		case 'D':
			dy = 2;
			break;
		case 'R':
			dx = 2;
			break;
		default:
			continue;
		}
		mouseX += dx;
		mouseY += dy;

		if (mouseX < 0 || mouseY < 0) {
			invalid = true;
		}
		for (int i = 0; i < index; i++) {
			if (mouseXseen[i] == mouseX && mouseYseen[i] == mouseY) {
				invalid = true;
			}
		}
		if (index > length) {
			invalid = true;
		}
		if (invalid) {
			delete[] mouseXseen;
			delete[] mouseYseen;
			return false;
		}
		mouseXseen[index] = mouseX;
		mouseYseen[index] = mouseY;
		index++;

	}
	delete[] mouseXseen;
	delete[] mouseYseen;
	return true;
}
bool LinkedList::tryMove(char dir) {
	if (current == nullptr) {
		return false;
	}
	if (current->next != nullptr && current->data == dir) {
		current = current->next;
		return true;
	}
	if (current->back != nullptr && current->back->data == console.opposite(dir)) {
		current = current->back;
		return true;
	}
	return false;
}
bool LinkedList::tryInsert(char dir) {
	if (current == nullptr) {
		return false;
	}
	Node* target = (current == tail) ? current->back : current;
	if (target == nullptr) {
		return false;
	}
	int idx = indexOf(target);
	LinkedList copyList;
	for (Node* n = head; n != nullptr; n = n->next) {
		copyList.InsertAtTail(n->data);
	}
	copyList.InsertAfter(dir, copyList.nodeAt(idx));
	if (!copyList.isValidPath()) {
		return false;
	}
	InsertAfter(dir, target);
	return true;
}
bool LinkedList::tryDelete() {
	if (current == nullptr) {
		return false;
	}
	Node* target = (current == tail) ? current->back : current;
	if (target == nullptr) {
		return false;
	}
	int idx = indexOf(target);
	LinkedList copyList;
	for (Node* n = head; n != nullptr; n = n->next) {
		copyList.InsertAtTail(n->data);
	}
	copyList.deleteNode(copyList.nodeAt(idx));
	if (!copyList.isValidPath()) {
		return false;
	}
	if (target == current) {
		current = (target->next != nullptr) ? target->next : target->back;
	}
	deleteNode(target);
	return true;
}
