#include "Paddle.h"
#include "konstanta.h"
Paddle::Paddle(int StartX, int StartY, int d)
	: x(StartX), y(StartY), duzina(d) {}

void Paddle::moveLeft(int minX) {
	for (int i = 0; i < BRZINA_PALICE; i++)
		if (x > minX) x--;
}

void Paddle::moveRight(int maxX) {
	for (int i = 0; i < BRZINA_PALICE; i++)
		if (x + duzina < maxX) x++;
}

void Paddle::draw(std::vector<std::vector<char>>& buffer) {
	for (int i = 0; i < duzina; i++)
		buffer[y][x + i] = '=';
}

int Paddle::getX() const { return x; }
int Paddle::getY() const { return y; }
int Paddle::getDuzina() const { return duzina; }
