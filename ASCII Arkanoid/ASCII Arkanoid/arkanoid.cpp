#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;

/*----------------- SAKRIVANJE CURSORA -----------------*/
#ifdef _WIN32
#include <windows.h>
void hideCursor() {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
}
#endif

#ifndef _WIN32
void hideCursor() {
	cout << "\033[?25l";
}
#endif

/*----------------- TIPKE -----------------*/
#ifdef _WIN32
#include <conio.h>
char getKey() {
	if (_kbhit()) return _getch();
	return 0;
}
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
char getKey() {
	struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	int oldf = fcntl(STDIN_FILENO, F_GETFL);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	char c = 0;
	if (read(STDIN_FILENO, &c, 1) < 0) c = 0;

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	return c;
}
#endif

/*----------------- SPAVANJE -----------------*/
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
void spavanje(int san) {
#ifdef _WIN32
	Sleep(san);
#else
	usleep(san * 1000);
#endif
}
/*OCISTI*/
void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	cout << "\033[2J\033[H";
#endif
}

/*----------------- SCREEN -----------------*/
const int SIRINA = 40;
const int VISINA = 20;

class Screen {
private:
	int sirina, visina;
	vector<vector<char>> buffer;
public:
	Screen(int s, int v) : sirina(s), visina(v) {
		buffer.resize(visina, vector<char>(sirina, ' '));
	}
	void clear() {
		for (int i = 0; i < visina; i++)
			for (int j = 0; j < sirina; j++)
				buffer[i][j] = ' ';
	}
	void draw() {
		clearScreen();
		for (int i = 0; i < visina; i++) {
			for (int j = 0; j < sirina; j++) cout << buffer[i][j];
			cout << "\n";
		}
	}
	void putText(int row, int col, const string& text) {
		for (int i = 0; i < text.size(); i++)
			if (col + i < sirina) buffer[row][col + i] = text[i];
	}
	vector<vector<char>>& getBuffer() { return buffer; }
};

/*----------------- GAME OBJECT -----------------*/
class GameObject {
public:
	virtual void update() {}
	virtual void draw(vector<vector<char>>& buffer) = 0;
	virtual ~GameObject() {}
};

/*----------------- PADDLE -----------------*/
const int DUZINA_PALICE = 9; // nova duzina palice
const int BRZINA_PALICE = 4;

class Paddle : public GameObject {
private:
	int x, y, duzina;
public:
	Paddle(int StartX, int StartY, int d) : x(StartX), y(StartY), duzina(d) {}
	void moveLeft(int minX = 0) { 
		for (int i = 0;i < BRZINA_PALICE;i++) {
			if (x > minX) x--;
		}

	}
	void moveRight(int maxX = SIRINA) {
		for (int i = 0;i < BRZINA_PALICE;i++) {
			if (x + duzina < maxX) x++;
		}
	}
	void draw(vector<vector<char>>& buffer) override {
		for (int i = 0; i < duzina; i++)
			buffer[y][x + i] = '=';
	}
	int getX() const { return x; }
	int getY() const { return y; }
	int getDuzina() const { return duzina; }
};

/*----------------- BLOCK -----------------*/
class Block {
private:
	int x, y;
	bool active;
public:
	static const int WIDTH = 2;
	Block(int startX, int startY) : x(startX), y(startY), active(true) {}
	void draw(vector<vector<char>>& buffer) {
		if (active) {
			if (x < SIRINA) buffer[y][x] = '#';
			if (x + 1 < SIRINA) buffer[y][x + 1] = '#';
		}
	}
	bool isActive() const { return active; }
	void hit() { active = false; }
	int getX() const { return x; }
	int getY() const { return y; }
	int getWidth() const { return WIDTH; }
};

/*----------------- BALL -----------------*/
class Ball {
private:
	int x, y, dx, dy;
	bool aktivna = false;
public:
	Ball(int startX, int startY) : x(startX), y(startY), dx(1), dy(-1) {}
	void start() {
		aktivna = true;
		dy = -1;
		dx = (rand() % 2 + 1) * ((rand() % 2) ? 1 : -1);
	}
	bool jelAktivna() const { return aktivna; }
	void attachToPaddle(int paddleX, int paddleY, int paddleLen) {
		if (!aktivna) {
			x = paddleX + paddleLen / 2;
			y = paddleY - 1;
		}
	}
	void move() {
		if (!aktivna) return;
		int newX = x + dx;
		int newY = y + dy;

		// odbijanje od zidova
		if (newX <= 0 || newX >= SIRINA - 1) dx = -dx;
		if (newY <= 0) dy = -dy;

		// sada pomakni loptu
		x += dx;
		y += dy;
	}
	void checkPaddleCollision(int paddleX, int paddleY, int paddleLength) {
		if (!aktivna) return;
		if (y == paddleY - 1 && x >= paddleX && x < paddleX + paddleLength) {
			dy = -abs(dy);
			dx = (rand() % 2 + 1) * ((rand() % 2) ? 1 : -1);
			y = paddleY - 1;
		}
	}
	void checkBlockCollision(vector<Block>& blokovi) {
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
	void draw(vector<vector<char>>& buffer) {
		if (y >= 0 && y < VISINA && x >= 0 && x < SIRINA)
			buffer[y][x] = 'O';
	}
	int getX() const { return x; }
	int getY() const { return y; }
	void bounceUp() { dy = -dy; }
};

/*----------------- GAME -----------------*/
enum class GameState { Lobby, Rules, Exit, Play };

class Game {
private:
	GameState stanje;
	Screen slika;
	Paddle palica;
	Ball lopta;
	vector<Block> blokovi;
	int lives=10;
public:
	Game() : stanje(GameState::Lobby), slika(SIRINA, VISINA),
		palica(SIRINA / 2 - 4, VISINA - 1, 9),
		lopta(SIRINA / 2, VISINA - 3) {}

	void igraj() {
		while (stanje != GameState::Exit) {
			if (stanje == GameState::Lobby) stanje = lobbyScreen();
			else if (stanje == GameState::Rules) stanje = rulesScreen();
			else if (stanje == GameState::Play) stanje = playScreen();
			else stanje = GameState::Exit;
		}
	}
private:
	GameState lobbyScreen() {
		slika.clear();
		vector<string> opcije = {
			"===== ARKANOID ASCII GAME =====\n",
			"          1 [Start]\n",
			"          2 [Rules]\n",
			"          3 [Exit]\n"
		};
		for (int i = 0;i < opcije.size();i++) slika.putText(2 + i, 5, opcije[i]);
		slika.draw();
		while (1) {
			char k = getKey();
			if (k == '1') return GameState::Play;
			if (k == '2') return GameState::Rules;
			if (k == '3') return GameState::Exit;
			spavanje(30);
		}
	}
	GameState rulesScreen() {
		slika.clear();
		vector<string> pravila = {
			" ============= RULES =============\n",
			" 1. Move paddle left A and right D\n",
			" 2. Goal is to break all bricks\n",
			" 3. Press X to return to menu\n"
		};
		for (int i = 0;i < pravila.size();i++) slika.putText(2 + i, 5, pravila[i]);
		slika.draw();
		while (1) {
			char k = getKey();
			if (k == 'x' || k == 'X') return GameState::Lobby;
			spavanje(30);
		}
	}
	GameState playScreen() {
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
							// ne dira se 'blokovi' -> prethodno uništeni ostaju
						}
					}

					slika.clear();
					for (auto &b : blokovi) b.draw(slika.getBuffer());
					palica.draw(slika.getBuffer());
					lopta.draw(slika.getBuffer());

					int aktivniBlokovi = 0;
					for (auto &b : blokovi) {
						if (b.isActive()) {
							aktivniBlokovi++;
						}
					}
					string status_z = "  Lives: " + to_string(lives) + "              Blocks left: " + to_string(aktivniBlokovi);
					slika.putText(0, 0, status_z);
					slika.draw();

					spavanje(20);
				}
			}

			void resetGame() {
				palica = Paddle(SIRINA / 2 - DUZINA_PALICE / 2, VISINA - 1, DUZINA_PALICE);
				lopta = Ball(SIRINA / 2, VISINA - 2);
				blokovi.clear();
				for (int j = 2; j < 6; j++)
					for (int i = 2; i < SIRINA; i += Block::WIDTH)
						blokovi.push_back(Block(i, j));
			}

		};

		int main() {
			hideCursor();
			Game igra;
			igra.igraj();
			return 0;
		}