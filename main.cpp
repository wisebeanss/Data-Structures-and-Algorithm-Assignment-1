#include <iostream>
#include <fstream>
#include <Windows.h>
#include "LinkedList.h"
using namespace std;

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
//char convertULDRtoWASD(char dir) {
//	switch (dir) {
//	case 'U': return 'W';
//	case 'L': return 'A';
//	case 'D': return 'S';
//	case 'R': return 'D';
//	}
//}
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
void setCursorPosition(SHORT x, SHORT y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };
	SetConsoleCursorPosition(hConsole, pos);
}
void drawAt(int x, int y, char symbol) {
	setCursorPosition(x, y);
	cout << symbol;
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
		cout << "Unable to open file\n";
	}
	char input;
	char insertInput;
	
	Node* current = list.head;
	bool moved = true;
	do {
		constructMap(list, current);
		cout << "Edit Mode: WASD to Move, + to add, - to del, q to quit: ";
		cin >> input;
		switch (input) {
		case 'W':
		case 'A':
		case 'S':
		case 'D':
			moved = tryMove(current, convertWASDtoULDR(input));
			break;
		case '+':
			if (current == list.tail) {
				cout << "Last Node Detected! Enter Direction of tail node (WASD): ";
				cin >> insertInput;			
				if (insertInput != 'W' && insertInput != 'A' && insertInput != 'S' && insertInput != 'D') {
					cout << "\n Invalid Direction";
					break;
				}
				list.InsertBefore(convertWASDtoULDR(insertInput), list.tail);
				list.display();
			}
			else {
				list.InsertAfter(current->data, current);
			}
			break;
		case '-':
			if (list.getLength() == 2) {
				cout << "2 Nodes detected. Cannot delete";
			}
			else if (current == list.head) {
				current = list.head->next;
				list.deleteValue(list.head->data);
			}
			else if (current == list.tail) {
				current = list.tail->back;
				list.deleteValue('N');
				current->data = 'N';
			}
			else {
				list.deleteValue(current->data);
			}
			break;
		case 'q':
			break;
		default:
			moved = false;
			cout << "Invalid Input Error";
		}
		if (!moved) {
			cout << "Invalid Direction";
		}
		//Clears the screen, moves cursor to the top-left corner
		cout << "\033[2J\033[H" << endl;
	} while (input != 'q');
  
	return 0;
}
