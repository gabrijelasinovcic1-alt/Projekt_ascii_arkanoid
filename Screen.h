#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include <string>
#include <windows.h>
class Screen {
private:
	int sirina, visina;
	std::vector<std::vector<char>> buffer;
	HANDLE hConsole;
	COORD coord;
public:
	Screen(int s, int v);

	void clear(); // Očisti buffer
	void draw();  // Iscrtaj buffer u konzolu
	void putText(int row, int col, const std::string& text); // Upisi tekst u buffer
	std::vector<std::vector<char>>& getBuffer(); // Vrati referencu na buffer
};

#endif

