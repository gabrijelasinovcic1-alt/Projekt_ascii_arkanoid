#ifndef BLOCK_H
#define BLOCK_H
#include "konstanta.h"
#include <vector>

class Block{
private:
	int x, y;
	bool active;
public:
	static const int WIDTH = 2;
	Block(int startX, int startY);
	void draw(std::vector<std::vector<char>>& buffer);
	bool isActive() const;
	void hit();
	int getX() const;
	int getY() const;
	int getWidth() const;
};

#endif
