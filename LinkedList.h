#pragma once
#include "Node.h"
#include <iostream>
class LinkedList
{
private:
public:
	Node* head;
	Node* tail;
	LinkedList();
	~LinkedList();
	void InsertAtHead(char data);
	void InsertAtTail(char data);
	void InsertAfter(char data, Node* node);
	void InsertBefore(char data, Node* node);
	void deleteValue(char data);
	void display();
};

