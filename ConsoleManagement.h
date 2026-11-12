#pragma once
#include <Windows.h>
#include <fstream>
class ConsoleManagement
{
public:
	char opposite(char dir);
	char convertWASDtoULDR(char dir);
	void setCursorPosition(SHORT x, SHORT y);
	COORD getCursorPosition();
};

