#include "ConsoleManagement.h"

// char functions
char ConsoleManagement::opposite(char dir) {
	switch (dir) {
	case 'U': return 'D';
	case 'D': return 'U';
	case 'L': return 'R';
	case 'R': return 'L';
	}
	return '\0';
}
char ConsoleManagement::convertWASDtoULDR(char dir) {
	switch (dir) {
	case 'W': return 'U';
	case 'A': return 'L';
	case 'S': return 'D';
	case 'D': return 'R';
	}
	return '\0';
}

void ConsoleManagement::setCursorPosition(SHORT x, SHORT y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };
	SetConsoleCursorPosition(hConsole, pos);
}
COORD ConsoleManagement::getCursorPosition() {
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