#include "Ball.h"
#include <cstdlib>
#include <cmath>
#include "konstanta.h"

Ball::Ball(int startX, int startY)
	: x(startX), y(startY), dx(1), dy(-1), aktivna(false) {}

void Ball::start() {
	aktivna = true;
	dy = -1;
	dx = (rand() % 2 + 1) * ((rand() % 2) ? 1 : -1);
}

bool Ball::jelAktivna() const { return aktivna; }

void Ball::attachToPaddle(int paddleX, int paddleY, int paddleLen) {
	if (!aktivna) {
		x = paddleX + paddleLen / 2;
		y = paddleY - 1;
	}
}

void Ball::move() {
	if (!aktivna) return;
	int newX = x + dx;
	int newY = y + dy;

	if (newX <= 0 || newX >= SIRINA - 1) dx = -dx;
	if (newY <= 0) dy = -dy;

	x += dx;
	y += dy;
}

void Ball::checkPaddleCollision(int paddleX, int paddleY, int paddleLength) {
	if (!aktivna) return;
	if (y == paddleY - 1 && x >= paddleX && x < paddleX + paddleLength) {
		dy = -abs(dy);
		dx = (rand() % 2 + 1) * ((rand() % 2) ? 1 : -1);
		y = paddleY - 1;
	}
}

void Ball::checkBlockCollision(std::vector<Block>& blokovi) {
	if (!aktivna) return;
	for (auto &b : blokovi) {
		if (!b.isActive()) continue;
		if (y == b.getY() && x >= b.getX() && x < b.getX() + b.getWidth()) {
			b.hit();
			
			dy = -dy;
			dx = (rand() % 2 + 1) * ((rand() % 2) ? 1 : -1);
			break;
		}
	}
}

void Ball::draw(std::vector<std::vector<char>>& buffer) {
	if (y >= 0 && y < VISINA && x >= 0 && x < SIRINA)
		buffer[y][x] = 'O';
}

int Ball::getX() const { return x; }
int Ball::getY() const { return y; }
void Ball::bounceUp() { dy = -dy; }
