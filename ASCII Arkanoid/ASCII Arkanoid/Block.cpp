#include "Block.h"
#include "konstanta.h"

Block::Block(int startX, int startY)
	: x(startX), y(startY), active(true) {}

void Block::draw(std::vector<std::vector<char>>& buffer) {
	if (!active) return;
	if (x < SIRINA) buffer[y][x] = '#';
	if (x + 1 < SIRINA) buffer[y][x + 1] = '#';
}

bool Block::isActive() const { return active; }
void Block::hit() { active = false; }
int Block::getX() const { return x; }
int Block::getY() const { return y; }
int Block::getWidth() const { return WIDTH; }
