#include "Game.h"
#include "konstanta.h"
#include <string>

using namespace std;


Game::Game()
	: stanje(GameState::Lobby), slika(SIRINA, VISINA),
	palica(SIRINA / 2 - 4, VISINA - 1, 9),
	lopta(SIRINA / 2, VISINA - 3),
	lives(10) {}

void Game::igraj() {
	while (stanje != GameState::Exit) {
		if (stanje == GameState::Lobby) stanje = lobbyScreen();
		else if (stanje == GameState::Rules) stanje = rulesScreen();
		else if (stanje == GameState::Play) stanje = playScreen();
		else stanje = GameState::Exit;
	}
}

GameState Game::lobbyScreen() {
	slika.clear();
	vector<string> opcije = {
		"===== ARKANOID ASCII GAME =====\n",
		"          1 [Start]\n",
		"          2 [Rules]\n",
		"          3 [Exit]\n"
	};
	for (int i = 0; i < opcije.size(); i++) slika.putText(2 + i, 5, opcije[i]);
	slika.draw();

	while (1) {
		char k = getKey();
		if (k == '1') return GameState::Play;
		if (k == '2') return GameState::Rules;
		if (k == '3') return GameState::Exit;
		spavanje(30);
	}
}

GameState Game::rulesScreen() {
	slika.clear();
	vector<string> pravila = {
		" ============= RULES =============\n",
		" 1. Move paddle left A and right D\n",
		" 2. Goal is to break all bricks\n",
		" 3. Press X to return to menu\n"
	};
	for (int i = 0; i < pravila.size(); i++) slika.putText(2 + i, 5, pravila[i]);
	slika.draw();

	while (1) {
		char k = getKey();
		if (k == 'x' || k == 'X') return GameState::Lobby;
		spavanje(30);
	}
}

GameState Game::playScreen() {
	resetGame();
	bool loptaStart = false;

	while (1) {
		char k = getKey();
		if (k == 'x' || k == 'X') return GameState::Lobby;

		if (k == 'a' || k == 'A') {
			palica.moveLeft();
			if (!loptaStart) {
				lopta.start();
				loptaStart = true;
				spavanje(20);
			}
		}
		if (k == 'd' || k == 'D') {
			palica.moveRight();
			if (!loptaStart) {
				lopta.start();
				loptaStart = true;
				spavanje(20);
			}
		}

		lopta.attachToPaddle(palica.getX(), palica.getY(), palica.getDuzina());

		if (lopta.jelAktivna()) {
			lopta.move();
			lopta.checkPaddleCollision(palica.getX(), palica.getY(), palica.getDuzina());
			lopta.checkBlockCollision(blokovi);

			if (lopta.getY() >= VISINA) {
				palica = Paddle(SIRINA / 2 - palica.getDuzina() / 2, VISINA - 2, palica.getDuzina());
				lopta = Ball(SIRINA / 2, VISINA - 2);
				lives--;
				loptaStart = false;
			}
		}

		slika.clear();
		for (auto &b : blokovi) b.draw(slika.getBuffer());
		palica.draw(slika.getBuffer());
		lopta.draw(slika.getBuffer());

		int aktivniBlokovi = 0;
		for (auto &b : blokovi) if (b.isActive()) aktivniBlokovi++;

		string status_z = "  Lives: " + to_string(lives) + "              Blocks left: " + to_string(aktivniBlokovi);
		slika.putText(0, 0, status_z);

		slika.draw();
		spavanje(20);
	}
}

void Game::resetGame() {
	palica = Paddle(SIRINA / 2 - DUZINA_PALICE / 2, VISINA - 1, DUZINA_PALICE);
	lopta = Ball(SIRINA / 2, VISINA - 2);
	blokovi.clear();
	for (int j = 2; j < 6; j++)
		for (int i = 2; i < SIRINA; i += Block::WIDTH)
			blokovi.push_back(Block(i, j));
}
