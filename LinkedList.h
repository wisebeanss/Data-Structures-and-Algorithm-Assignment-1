#pragma once
#include "Node.h"
#include <iostream>
#include <Windows.h>
class LinkedList
{
private:
	int length;
public:
	Node* head;
	Node* tail;
	
	LinkedList();
	~LinkedList();
	LinkedList(const LinkedList& rhs);

	void InsertAtHead(char data);
	void InsertAtTail(char data);
	void InsertAfter(char data, Node* node);
	void InsertBefore(char data, Node* node);
	void deleteNode(Node* node);

	int indexOf(Node* target);
	Node* nodeAt(int index);

	int getLength() const;
};

