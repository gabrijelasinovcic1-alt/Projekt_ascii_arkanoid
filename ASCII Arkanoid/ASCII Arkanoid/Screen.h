#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include <string>
using namespace std;


class Screen {
private:
	int sirina, visina;
	vector<vector<char>> buffer;

public:
	Screen(int s, int v);
	void clear();
	void draw();
	void putText(int row, int col, const string& text);
	vector<vector<char>>& getBuffer();
};

#endif
