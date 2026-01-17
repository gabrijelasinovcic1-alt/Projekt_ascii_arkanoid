#ifndef BALL_H
#define BALL_H

#include <vector>
#include "Block.h"
#include "konstanta.h"

class Ball{
private:
	float x, y, dx, dy;
	bool aktivna;
public:
	Ball(int startX, int startY);

	void start();
	bool jelAktivna() const;
	void attachToPaddle(int paddleX, int paddleY, int paddleLen);
	void move();
	void checkPaddleCollision(int paddleX, int paddleY, int paddleLength);
	void checkBlockCollision(std::vector<Block>& blokovi);
	void draw(std::vector<std::vector<char>>& buffer);

	int getX() const;
	int getY() const;
	void bounceUp();
};

#endif
