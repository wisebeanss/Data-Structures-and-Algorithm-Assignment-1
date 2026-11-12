#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include "LinkedList.h"


int main(void) {
	char c;
	LinkedList list;
	ConsoleManagement console;
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
	
	list.current = list.head;
	bool insertSuccessful = true;
	std::string message;
	do {
		//Clears the screen, moves cursor to the top-left corner
		std::cout << "\033[2J\033[H";
		int bottom = list.construct();
		//Put Messaging System here
		console.setCursorPosition(0, bottom + 2);
		std::cout << "Edit Mode: WASD to Move, + to add, - to del, q to quit: ";
		COORD mousePos = console.getCursorPosition();
		std::cout << "\n" << message;
		message = " ";
		//Moves cursor back to edit message
		console.setCursorPosition(mousePos.X, mousePos.Y);
		std::cin >> input;
		switch (input) {
		case 'W':
		case 'A':
		case 'S':
		case 'D':
			if (!list.tryMove(console.convertWASDtoULDR(input))) {
				message = "Invalid Direction";
			}
			break;
		case '+':
			if (list.current == list.tail) {
				char insertInput;
				std::cout << "Last Node Detected! Enter Direction of tail node (WASD): ";
				std::cin >> insertInput;
				char dir = console.convertWASDtoULDR(insertInput);
				if (dir == '\0') {
					message = "Invalid Direction";
					break;
				}
				insertSuccessful = list.tryInsert(dir);
				if (insertSuccessful) {
					list.current = list.current->back;
				}
				
			}
			else {
				insertSuccessful = list.tryInsert(list.current->data);
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
				Node* next = list.current->next;
				Node* prev = list.current->back;
				bool canDelete = list.tryDelete();

				if (!canDelete) {
					message = "Invalid Deletion";
					break;
				}			
				if (!(list.current == list.tail)) {
					list.current = (next != nullptr) ? next : prev;
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
