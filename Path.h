#pragma once
#include <fstream>
#include <Windows.h>
#include <string>
#include "LinkedList.h"
#include "Node.h"
class Path
{
public:
	LinkedList list;
	Node* current;
	Path();
	~Path();
	void setCursorPosition(SHORT x, SHORT y);
	void drawAt(int x, int y, char symbol);
	int construct();
	bool isValidPath(LinkedList &list);
	bool tryMove(char dir);
	bool tryInsert(char dir);
	bool tryDelete();
	char opposite(char dir);
	char convertWASDtoULDR(char dir);
};

