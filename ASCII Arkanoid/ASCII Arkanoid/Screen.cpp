#include "Screen.h"
#include "konstanta.h"
#include <iostream>

using namespace std;

Screen::Screen(int s, int v) : sirina(s), visina(v) {
	buffer.resize(visina, vector<char>(sirina, ' '));
}

void Screen::clear() {
	for (int i = 0; i < visina; i++)
		for (int j = 0; j < sirina; j++)
			buffer[i][j] = ' ';
}

void Screen::draw() {
	clearScreen();
	for (int i = 0; i < visina; i++) {
		for (int j = 0; j < sirina; j++)
			cout << buffer[i][j];
		cout << "\n";
	}
}

void Screen::putText(int row, int col, const string& text) {
	for (int i = 0; i < text.size(); i++)
		if (col + i < sirina)
			buffer[row][col + i] = text[i];
}

vector<vector<char>>& Screen::getBuffer() {
	return buffer;
}
