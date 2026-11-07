#include "Path.h"

// Constructors & Destructors
Path::Path() {
	current = list.head;
}
Path::~Path() {
	current = nullptr;
}

// char functions
char Path::opposite(char dir) {
	switch (dir) {
	case 'U': return 'D';
	case 'D': return 'U';
	case 'L': return 'R';
	case 'R': return 'L';
	}
	return '\0';
}
char Path::convertWASDtoULDR(char dir) {
	switch (dir) {
	case 'W': return 'U';
	case 'A': return 'L';
	case 'S': return 'D';
	case 'D': return 'R';
	}
	return '\0';
}

// Cursor Functions
COORD Path::getCursorPosition() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsole, &cbsi)) {
		return cbsi.dwCursorPosition;
	}
	else {
		COORD invalid = { 0, 0 };
		return invalid;
	}
}
void Path::setCursorPosition(SHORT x, SHORT y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };
	SetConsoleCursorPosition(hConsole, pos);
}


void Path::drawAt(int x, int y, char symbol) {
	setCursorPosition(x, y);
	std::cout << symbol;
}
int Path::construct() {
	int mouseX = 0;
	int mouseY = 0;
	int maxY = 0;
	for (Node* n = list.head; n != nullptr; n = n->next) {
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
bool Path::isValidPath(LinkedList& list) {
	int mouseX = 0; int mouseY = 0; int index = 0;
	int length = list.getLength();
	int* mouseXseen = new int[length + 1];
	int* mouseYseen = new int[length + 1];
	bool invalid = false;
	//Record Start cell
	mouseXseen[index] = mouseX;
	mouseYseen[index] = mouseY;
	index++;
	for (Node* n = list.head; n != nullptr; n = n->next) {
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

bool Path::tryMove(char dir) {
	if (current->next != nullptr && current->data == dir) {
		current = current->next;
		return true;
	}
	if (current->back != nullptr && current->back->data == opposite(dir)) {
		current = current->back;
		return true;
	}
	return false;
}


bool Path::tryInsert(char dir) {
	if (current == nullptr) {
		return false;
	}
	Node* target = (current == list.tail) ? current->back : current;
	if (target == nullptr) {
		return false;
	}
	int idx = list.indexOf(target);
	LinkedList copyList(list);
	copyList.InsertAfter(dir, copyList.nodeAt(idx));
	if (!isValidPath(copyList)) {
		return false;
	}
	list.InsertAfter(dir, target);
	return true;
}
bool Path::tryDelete() {
	if (current == nullptr) {
		return false;
	}
	Node* target = (current == list.tail) ? current->back : current;
	if (target == nullptr) {
		return false;
	}
	int idx = list.indexOf(target);
	LinkedList copyList(list);
	copyList.deleteNode(copyList.nodeAt(idx));
	if (!isValidPath(copyList)) {
		return false;
	}
	if (target == current) {
		current = (target->next != nullptr) ? target->next : target->back;
	}
	list.deleteNode(target);
	return true;
}
