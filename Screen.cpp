#include "screen.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

Screen::Screen(int s, int v) : sirina(s), visina(v) {
	buffer.resize(visina, std::vector<char>(sirina, ' '));
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	coord = { 0,0 };
}

void Screen::clear() {
	for (int i = 0; i < visina; i++)
		for (int j = 0; j < sirina; j++)
			buffer[i][j] = ' ';
}

void Screen::draw() {
#ifdef _WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD topLeft = { 0,0 };
	SetConsoleCursorPosition(hOut, topLeft); // vrati cursor na vrh
#endif

	for (int y = 0;y < visina;y++) {
		for (int x = 0;x < sirina;x++)
			std::cout << (buffer[y][x]);
		std::cout << "\n";
	}
}

void Screen::putText(int row, int col, const std::string& text) {
	for (size_t i = 0; i < text.size(); i++)
		if (col + i < sirina) buffer[row][col + i] = text[i];
}

std::vector<std::vector<char>>& Screen::getBuffer() {
	return buffer;
}
