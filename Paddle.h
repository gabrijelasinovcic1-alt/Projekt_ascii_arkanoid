#ifndef PADDLE_H
#define PADDLE_H

#include "GameObject.h"
#include "konstanta.h"
const int DUZINA_PALICE = 13;
const int BRZINA_PALICE = 5;


class Paddle : public GameObject {
private:
	int x, y, duzina;
public:
	Paddle(int StartX, int StartY, int d);
	void moveLeft(int minX = 0);
	void moveRight(int maxX = SIRINA);
	void draw(std::vector<std::vector<char>>& buffer) override;

	int getX() const;
	int getY() const;
	int getDuzina() const;
};

#endif
