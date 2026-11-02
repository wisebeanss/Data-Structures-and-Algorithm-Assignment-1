#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include "LinkedList.h"


char opposite(char dir) {
	switch (dir) {
		case 'U': return 'D';
		case 'D': return 'U';
		case 'L': return 'R';
		case 'R': return 'L';
	}
	return '\0';
}
char convertWASDtoULDR(char dir) {
	switch (dir) {
	case 'W': return 'U';
	case 'A': return 'L';
	case 'S': return 'D';
	case 'D': return 'R';
	}
	return '\0';
}


void setCursorPosition(SHORT x, SHORT y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };
	SetConsoleCursorPosition(hConsole, pos);
}
COORD getCursorPosition(HANDLE hConsoleOutput) {
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi)) {
		return cbsi.dwCursorPosition;
	}
	else {
		COORD invalid = { 0, 0 };
		return invalid;
	}
}
void drawAt(int x, int y, char symbol) {
	setCursorPosition(x, y);
	std::cout << symbol;
}

bool isValidPath(LinkedList list) {
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

void constructMap(LinkedList &list, Node*& current) {
	int mouseX = 0;
	int mouseY = 0;
	int maxY = 0;
	for(Node* n = list.head; n != nullptr; n = n->next) {
		int dx = 0; int dy = 0;
		char connector = '-';		
		drawAt(mouseX, mouseY, (current == n)? 'o' : 'x');
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
	//Makes sure the path does not get cutoff
	setCursorPosition(0, maxY + 2);
}
bool tryMove(Node*& current, char dir) {
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
bool tryInsert(LinkedList& list, Node* current, char dir) {
	int idx = list.indexOf(current);
	LinkedList copyList(list);
	copyList.InsertAfter(dir, copyList.nodeAt(idx));
	if (!isValidPath(copyList)) {
		return false;
	}
	list.InsertAfter(dir, current);
	return true;
}
bool tryDelete(LinkedList& list, Node* current) {
	if (current == nullptr) {
		return false;
	}
	int idx = list.indexOf(current);
	LinkedList copyList(list);
	copyList.deleteNode(copyList.nodeAt(idx));
	if (!isValidPath(copyList)) {
		return false;
	}
	list.deleteNode(current);
	return true;
}
int main(void) {
	char c;
	LinkedList list;
	
	std::ifstream myfile("path.txt");
	if (myfile.is_open()) {
		while (myfile.get(c)) {
			list.InsertAtTail(c);
			myfile.get(c);
		}
		list.InsertAtTail('N');
		myfile.close();
	}
	else {
		std::cout << "Unable to open file\n";
	}
	char input;
	
	
	Node* current = list.head;
	bool insertSuccessful = true;
	bool deleteSuccessful = true;
	std::string message;
	do {
		//Clears the screen, moves cursor to the top-left corner
		std::cout << "\033[2J\033[H";
		constructMap(list, current);
		//Put Messaging System here
		std::cout << "Edit Mode: WASD to Move, + to add, - to del, q to quit: ";
		COORD mousePos = getCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE));
		std::cout << "\n" << message;
		message = " ";
		//Moves cursor back to edit message
		setCursorPosition(mousePos.X, mousePos.Y);
		std::cin >> input;
		switch (input) {
		case 'W':
		case 'A':
		case 'S':
		case 'D':
			if (!tryMove(current, convertWASDtoULDR(input))) {
				message = "Invalid Direction";
			}
			break;
		case '+':
			if (current == list.tail) {
				char insertInput;
				std::cout << "Last Node Detected! Enter Direction of tail node (WASD): ";
				std::cin >> insertInput;
				char dir = convertWASDtoULDR(insertInput);
				if (dir == '\0') {
					message = "Invalid Direction";
				}

				insertSuccessful = tryInsert(list, current->back, convertWASDtoULDR(insertInput));
			}
			else {
				insertSuccessful = tryInsert(list, current, current->data);
			}
			if (!insertSuccessful) {
				message = "Invalid Insertion";
				break;
			}
			
			break;
		case '-':
			if (list.getLength() <= 2) {
				message = "2 Nodes detected. Cannot delete";
			}
			else {
				Node* next = current->next;
				Node* prev = current->back;

				if (tryDelete(list, current)) {
					current = (next != nullptr) ? next : prev;
				}
				else {
					message = "Invalid Deletion";
				}
			}
			break;
		case 'q':
			break;
		default:
			message = "Invalid Input Error";
		}
		
		
	} while (input != 'q');
  
	return 0;
}
