#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <thread>
#include <ctime>

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

	bool lock;
	bool saved;

	std::atomic<bool> whitePauseFlag;
	std::atomic<bool> blackPauseFlag;
	std::atomic<bool> stopFlag;

	int whiteClockValue;
	int blackClockValue;


	Timer blackTimer;
	Timer whiteTimer;

	std::thread whiteClockThread;
	std::thread blackClockThread;

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

	std::vector<std::string> gameHistory;

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

	void startTimers();
	void stopTimers();
	void setTimers(sf::Vector2f position, sf::RenderWindow& window, const PieceColor pc);
	void updateTurn();

	void drawMatchHistory();
};

#endif