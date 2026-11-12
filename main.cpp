#include <iostream>
#include "Path.h"

COORD getCursorPosition() {
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
int main(void) {
	char c;
	Path path;
	
	std::ifstream myfile("path.txt");
	if (myfile.is_open()) {
		while (myfile.get(c)) {
			path.list.InsertAtTail(c);
			myfile.get(c);
		}
		path.list.InsertAtTail('N');
		myfile.close();
	}
	else {
		std::cout << "Unable to open file\n";
	}
	char input;
	
	path.current = path.list.head;
	bool insertSuccessful = true;
	std::string message;
	do {
		//Clears the screen, moves cursor to the top-left corner
		std::cout << "\033[2J\033[H";
		int bottom = path.construct();
		//Put Messaging System here
		path.setCursorPosition(0, bottom + 2);
		std::cout << "Edit Mode: WASD to Move, + to add, - to del, q to quit: ";
		COORD mousePos = getCursorPosition();
		std::cout << "\n" << message;
		message = " ";
		//Moves cursor back to edit message
		path.setCursorPosition(mousePos.X, mousePos.Y);
		std::cin >> input;
		switch (input) {
		case 'W':
		case 'A':
		case 'S':
		case 'D':
			if (!path.tryMove(path.convertWASDtoULDR(input))) {
				message = "Invalid Direction";
			}
			break;
		case '+':
			if (path.current == path.list.tail) {
				char insertInput;
				std::cout << "Last Node Detected! Enter Direction of tail node (WASD): ";
				std::cin >> insertInput;
				char dir = path.convertWASDtoULDR(insertInput);
				if (dir == '\0') {
					message = "Invalid Direction";
				}

				if (path.tryInsert(path.convertWASDtoULDR(insertInput))) {
					path.current = path.current->back;
					insertSuccessful = true;
				}
				
			}
			else {
				insertSuccessful = path.tryInsert(path.current->data);
			}
			if (!insertSuccessful) {
				message = "Invalid Insertion";
				break;
			}
			
			break;
		case '-':
			if (path.list.getLength() <= 2) {
				message = "2 Nodes detected. Cannot delete";
			}
			else {
				Node* next = path.current->next;
				Node* prev = path.current->back;
				bool canDelete = false;
				if (path.current == path.list.tail) {
					canDelete = path.tryDelete();
				}
				else {
					canDelete = path.tryDelete();
					path.current = (next != nullptr) ? next : prev;
				}
				if (!canDelete) {
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
