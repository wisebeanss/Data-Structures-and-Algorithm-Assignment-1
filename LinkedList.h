#pragma once
#include "Node.h"
#include <iostream>
#include "ConsoleManagement.h"
class LinkedList
{
private:
	int length;
	ConsoleManagement console;
public:
	Node* head;
	Node* tail;
	Node* current;
	LinkedList();
	~LinkedList();


	void InsertAtHead(char data);
	void InsertAtTail(char data);
	void InsertAfter(char data, Node* node);
	void InsertBefore(char data, Node* node);
	void deleteNode(Node* node);

	void drawAt(int x, int y, char symbol);

	int construct();
	bool isValidPath();
	bool tryMove(char dir);
	bool tryInsert(char dir);
	bool tryDelete();


	int indexOf(Node* target);
	Node* nodeAt(int index);

	int getLength() const;
};

