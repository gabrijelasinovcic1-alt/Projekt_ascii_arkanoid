#ifndef GAME_H
#define GAME_H

#include "Screen.h"
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include "konstanta.h"
#include <vector>

enum class GameState { Lobby, Rules, Exit, Play };

class Game {
private:
	GameState stanje;
	Screen slika;
	Paddle palica;
	Ball lopta;
	std::vector<Block> blokovi;
	int lives;

	GameState lobbyScreen();
	GameState rulesScreen();
	GameState playScreen();
	void resetGame();

public:
	Game();
	void igraj();
};

#endif
