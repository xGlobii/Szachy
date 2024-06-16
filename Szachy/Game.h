#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <future>

#include "Chessboard.h"
#include "Button.h"
#include "SaveLoadManager.h"

class Game
{
	float tileSize;

	sf::RenderWindow window;

	sf::Sprite background;
	sf::Texture backgroundTexture;

	bool isGameStarted;
	bool loaded;
	bool stop = false;

	int whiteTimer;
	int blackTimer;

	Chessboard board;
	SaveLoadManager slManager;

	Button startButton;
	Button exitBtn;
	Button decrementWhiteTimer;
	Button incrementWhiteTimer;
	Button decrementBlackTimer;
	Button incrementBlackTimer;
	Button loadButton;

	Button saveButton;
	Button gameExitButton;

	InputBox input1;
	InputBox input2;

public:
	Game();

	void handleEvents();
	void render();

	void run();
	void startGame();

	void exitGame();

	void incrementWhite();
	void decrementWhite();
	void incrementBlack();
	void decrementBlack();
	void load();
	void save();
	void drawTimer(sf::Vector2f position, sf::RenderWindow& window, const PieceColor pc);
	void checkIfEnd();
};

#endif