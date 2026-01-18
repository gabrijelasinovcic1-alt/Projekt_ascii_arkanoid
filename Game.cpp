#include "Game.h"
#include "konstanta.h"
#include <string>
#include "Screen.h"
using namespace std;


Game::Game()
	: stanje(GameState::Lobby), slika(SIRINA, VISINA),
	palica(SIRINA / 2 - 4, VISINA - 1, 9),
	lopta(SIRINA / 2, VISINA - 3),
	lives() {}

void Game::igraj() {
	while (stanje != GameState::Exit) {
		if (stanje == GameState::Lobby) stanje = lobbyScreen();
		else if (stanje == GameState::Rules) stanje = rulesScreen();
		else if (stanje == GameState::Play) stanje = playScreen();
		else if (stanje == GameState::Win) stanje = winScreen();
		else if (stanje == GameState::Lose) stanje = loseScreen();
		else stanje = GameState::Exit;
	}
}

GameState Game::lobbyScreen() {
	slika.clear();
	vector<string> opcije = {
		"===== ARKANOID ASCII GAME =====",
		" ",
		"          1 [Start]",
		" ",
		"          2 [Rules]",
		" ",
		"          3 [Exit]"
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
		" ============= RULES =============",
		" ",
		" 1. Move paddle left A and right D.",
		" ",
		" 2. Catch ball with paddle.",
		" ",
		" 3. If you miss a ball = -1 life.",
		" ",
		" 4. Goal is to break all bricks.",
		" ",
		" 5. Press X to return to menu."
	};
	for (int i = 0; i < pravila.size(); i++) slika.putText(2+ i, 5, pravila[i]);
	slika.draw();

	while (1) {
		char k = getKey();
		if (k == 'x' || k == 'X') return GameState::Lobby;
		spavanje(30);
	}
}

int frameCounter = 0;
const int moveEveryNFrames = 1;

GameState Game::playScreen() {
	resetGame();
	bool loptaStart = false;
	int frameCounter = 0;

	while (1) {
	
		char k = getKey();
		if (k == 'x' || k == 'X') return GameState::Lobby;

		if (k == 'a' || k == 'A') {
			palica.moveLeft();
			if (!loptaStart) {
				lopta.start();
				loptaStart = true;
				
			}
		}
		if (k == 'd' || k == 'D') {
			palica.moveRight();
			if (!loptaStart) {
				lopta.start();
				loptaStart = true;
			
			}
		}

		lopta.attachToPaddle(palica.getX(), palica.getY(), palica.getDuzina());

		frameCounter++;
		if (frameCounter >= moveEveryNFrames) {
			frameCounter = 0;
			if (lopta.jelAktivna()) {
				lopta.move();
				lopta.checkPaddleCollision(palica.getX(), palica.getY(), palica.getDuzina());
				lopta.checkBlockCollision(blokovi);

				if (lopta.getY() >= VISINA) {
					palica = Paddle(SIRINA / 2 - palica.getDuzina() / 2, VISINA - 2, palica.getDuzina());
					lopta = Ball(SIRINA / 2, VISINA - 2);
					lives--;

					if (lives <= 0) {
						return GameState::Lose;
					}
					loptaStart = false;
					sound();
				}
			}
		}

		slika.clear();
		for (auto &b : blokovi) b.draw(slika.getBuffer());
		palica.draw(slika.getBuffer());
		lopta.draw(slika.getBuffer());

		int aktivniBlokovi = 0;
		for (auto &b : blokovi) if (b.isActive()) aktivniBlokovi++;
		if (aktivniBlokovi == 0) {
			return GameState::Win;
		}

		string status_z = "  Lives: " + to_string(lives) + "              Blocks left: " + to_string(aktivniBlokovi);
		slika.putText(0, 0, status_z);

		slika.draw();
		spavanje(30);
	}
}

GameState Game::winScreen(){
	sound();
	slika.clear();
	slika.putText(2, 4, " CONGRATS, YOU WON!");
	slika.putText(4, 4, "1 [Play Again]");
	slika.putText(6, 4, "3 [Exit]");

	slika.draw();

	while (1) {
		char k = getKey();
		if (k == '1') {
			return GameState::Play;
		}
		if (k == '3') {
			return GameState::Lobby;
		}
		
		spavanje(30);
		
	}
}

GameState Game::loseScreen() {
	sound();
	slika.clear();
	slika.putText(2, 4, "   GAME OVER!");
	slika.putText(4, 6, " 1 [Play Again]");
	slika.putText(6, 6, " 3 [Exit]");

	slika.draw();

	while (1) {
		char k = getKey();
		if (k == '1') {
			return GameState::Play;
		}
		if (k == '3') {
			return GameState::Lobby;
		}
		
		spavanje(30);
		
	}
}
void Game::resetGame() {
	lives = 10;
	palica = Paddle(SIRINA / 2 - DUZINA_PALICE / 2, VISINA - 1, DUZINA_PALICE);
	lopta = Ball(SIRINA / 2, VISINA - 2);
	blokovi.clear();
	for (int j = 2; j < 6; j++)
		for (int i = 2; i < SIRINA; i += Block::WIDTH)
			blokovi.push_back(Block(i, j));
}
