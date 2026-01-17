#include "konstanta.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif
using namespace std;

const int SIRINA = 40;
const int VISINA = 20;
/* CURSOR */
#ifdef _WIN32
void hideCursor() {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
}
#else
void hideCursor() {
	cout << "\033[?25l";
}
#endif

/* TIPKE */
#ifdef _WIN32
char getKey() {
	if (_kbhit()) return _getch();
	return 0;
}
#else
char getKey() {
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	int oldf = fcntl(STDIN_FILENO, F_GETFL);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	char c = 0;
	if (read(STDIN_FILENO, &c, 1) < 0) c = 0;

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	return c;
}
#endif
/*ZVUK*/
void sound() {
#ifdef _WIN32
	Beep(900, 70);
#else
	std::cout << "\a";
#endif
}

/* SPAVANJE */
void spavanje(int san) {
#ifdef _WIN32
	Sleep(san);
#else
	usleep(san * 1000);
#endif
}

/* OČISTI */
void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	cout << "\033[2J\033[H";
#endif
}
