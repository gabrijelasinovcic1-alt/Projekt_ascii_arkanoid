#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*Za koristenje tipki s tipkovnice*/
/*Za windows*/
#ifdef _WIN32
#include <conio.h>
char getKey() {
	if (_kbhit()) return _getch();
	return 0;
}
/*Za linux*/
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

/* Stednja CPU-a */
/*Windows*/
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void spavanje(int san) {
#ifdef _WIN32
	Sleep(san);  
/*Za linux*/
#else
	usleep(san * 1000);
#endif
}

enum class GameState {
	Lobby,
	Rules,
	Exit,
	Play
};

/*Buffer kako bi konzola i prozor smooth radili*/
const int SIRINA = 40;
const int VISINA = 10;
char prozor[VISINA][SIRINA];

void clearBuffer() {
	for (int i = 0; i < VISINA; i++)
		for (int j = 0; j < SIRINA; j++)
			prozor[i][j] = ' ';
}

void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	cout << "\033[2J\033[H";
#endif
}
void clearBufferAndScreen() {
	clearBuffer();
	clearScreen();
}

void drawBuffer() {
	for (int i = 0; i < VISINA; i++) {
		for (int j = 0; j < SIRINA; j++)
			cout << prozor[i][j];
		cout << '\n';
	}
}

/*LOBBY*/
GameState lobbyScreen() {
		clearBufferAndScreen();
		int rowOffset = 2, colOffset = 5;
		vector<string> opcije = { 
			"===== ARKANOID ASCII GAME =====\n", 
			"          1 [Start]\n", 
			"          2 [Rules]\n", 
			"          3 [Exit]\n" };
		for (int i = 0; i < opcije.size(); i++) {
			for (size_t j = 0; j < opcije[i].size(); j++) {
				prozor[rowOffset + i][colOffset + j] = opcije[i][j];
			}
		}
		
		drawBuffer();
		while (1) {

			char tipkaL = getKey();
			if (tipkaL == '1') {
				return GameState::Play;

			}
			else if (tipkaL == '2') {
				return GameState::Rules;

			}
			else if (tipkaL == '3') {
				return GameState::Exit;

			}
			spavanje(50);
		}
		
	}
	
/*RULES*/
GameState rulesScreen() {
	clearBufferAndScreen();
	int rowOffset = 2, colOffset = 5;
	vector <string> pravila = {
		" ============= RULES =============\n",
		" 1. Move paddle left A and right D\n",
		" 2. You have 5 lives\n",
		" 3. If you miss a ball, -1 life\n",
		" 4. Goal is to break all bricks\n",
		" 5. Have fun playing! :)\n\n",
		" Press X to exit screen.\n"
	};
	for (int i = 0; i < pravila.size(); i++) {
		for (size_t j = 0; j < pravila[i].size(); j++) {
			prozor[rowOffset + i][colOffset + j] = pravila[i][j];
		}
	}
	drawBuffer();
	while (1) {

	
	char tipkaR = getKey();
	if (tipkaR== 'x' || tipkaR == 'X') {
		return GameState::Lobby;
	}
	spavanje(50);
	}
	
}
int main() {

	GameState stanje = GameState::Lobby;

	while (stanje != GameState::Exit) {
		if (stanje == GameState::Lobby) {
			stanje = lobbyScreen();
		}
		else if (stanje == GameState::Rules) {
			stanje = rulesScreen();
		}
		else if (stanje == GameState::Play) {
			clearBuffer();
			drawBuffer();
			char tipkaP = getKey();
			if (tipkaP == 'x' || tipkaP == 'X') {
				stanje= GameState::Lobby;
			}
			spavanje(50);
		}
	}
	return 0;
}