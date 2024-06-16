#include "Game.h"

Game::Game() : window(sf::VideoMode(1920, 1000), "Chess by Globii.", sf::Style::Titlebar), input1(sf::Vector2f(300, 150), sf::Vector2f(1200, 150)),
input2(sf::Vector2f(300, 150), sf::Vector2f(1200, 350))
{
	window.setFramerateLimit(60);
	tileSize = 100.f;
	backgroundTexture.loadFromFile("..\\rsc\\images\\background.jpg");
	background.setTexture(backgroundTexture);
	loaded = false;

	{
		startButton = Button(300, 100, sf::Vector2f(150, 150), sf::Color::Blue, std::bind(&Game::startGame, this));
		startButton.setText("Start");

		exitBtn = Button(300, 100, sf::Vector2f(150, 800), sf::Color::Blue, std::bind(&Game::exitGame, this));
		exitBtn.setText("Exit");

		decrementWhiteTimer = Button(100, 100, sf::Vector2f(150, 600), sf::Color::Blue, std::bind(&Game::decrementWhite, this));
		decrementWhiteTimer.setText("-");

		incrementWhiteTimer = Button(100, 100, sf::Vector2f(500, 600), sf::Color::Blue, std::bind(&Game::incrementWhite, this));
		incrementWhiteTimer.setText("+");

		decrementBlackTimer = Button(100, 100, sf::Vector2f(700, 600), sf::Color::Blue, std::bind(&Game::decrementBlack, this));
		decrementBlackTimer.setText("-");

		incrementBlackTimer = Button(100, 100, sf::Vector2f(1050, 600), sf::Color::Blue, std::bind(&Game::incrementBlack, this));
		incrementBlackTimer.setText("+");

		loadButton = Button(200, 100, sf::Vector2f(150, 400), sf::Color::Blue, std::bind(&Game::load, this));
		loadButton.setText("Load from file");

		saveButton = Button(200, 100, sf::Vector2f(1000, 150), sf::Color::Blue, std::bind(&Game::save, this));
		saveButton.setText("Save game");

		gameExitButton = Button(200, 100, sf::Vector2f(1000, 275), sf::Color::Blue, std::bind(&Game::exitGame, this));
		gameExitButton.setText("Exit game");
	}

	//{
	//	input1 = InputBox(sf::Vector2f(300, 150), sf::Vector2f(1200, 150));
	//	input2 = InputBox(sf::Vector2f(300, 150), sf::Vector2f(1200, 350));
	//}

	whiteTimer = 600;
	blackTimer = 600;

	isGameStarted = false;
}

void Game::handleEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			if (!board.getTimerStatus())
				board.stopTimers();
			window.close();
		}
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			if (isGameStarted)
			{
				board.handleMouseClick(window);
				saveButton.handleMouseClick(sf::Vector2f(sf::Mouse::getPosition(window)));
				gameExitButton.handleMouseClick(sf::Vector2f(sf::Mouse::getPosition(window)));
			}
			else
			{
				startButton.handleMouseClick(sf::Vector2f(sf::Mouse::getPosition(window)));
				exitBtn.handleMouseClick(sf::Vector2f(sf::Mouse::getPosition(window)));
				loadButton.handleMouseClick(sf::Vector2f(sf::Mouse::getPosition(window)));
				incrementWhiteTimer.handleMouseClick(sf::Vector2f(sf::Mouse::getPosition(window)));
				decrementWhiteTimer.handleMouseClick(sf::Vector2f(sf::Mouse::getPosition(window)));
				incrementBlackTimer.handleMouseClick(sf::Vector2f(sf::Mouse::getPosition(window)));
				decrementBlackTimer.handleMouseClick(sf::Vector2f(sf::Mouse::getPosition(window)));
				input1.handleMouseClick(event);
				input2.handleMouseClick(event);
			}
		}
		if (event.type == sf::Event::TextEntered) {
			if (!isGameStarted) {
				input1.handleTextEntered(event);
				input2.handleTextEntered(event);
			}
		}
	}
}

void Game::render()
{
	window.clear(sf::Color(102, 62, 15));
	window.draw(background);
	if (!isGameStarted)
	{
		startButton.draw(window);
		exitBtn.draw(window);
		decrementWhiteTimer.draw(window);
		incrementWhiteTimer.draw(window);
		decrementBlackTimer.draw(window);
		incrementBlackTimer.draw(window);
		loadButton.draw(window);
		drawTimer(sf::Vector2f(200, 600), window, PieceColor::White);
		drawTimer(sf::Vector2f(800, 600), window, PieceColor::Black);

		input1.draw(window);
		input2.draw(window);
	}
	else
	{
		board.draw(window);
		board.drawPointsPlace(window, board.getPlayersPoints(PieceColor::White), sf::Vector2f(100, 1000));
		board.drawPointsPlace(window, board.getPlayersPoints(PieceColor::Black), sf::Vector2f(100, 100));
		board.setTimers(sf::Vector2f(250, 1000), window, PieceColor::White);
		board.setTimers(sf::Vector2f(250, 100), window, PieceColor::Black);
		saveButton.draw(window);
		gameExitButton.draw(window);
		board.drawLastMoves(window);
	}
	window.display();
}

void Game::startGame()
{
	if (loaded)
	{
		isGameStarted = true;
	}
	if (!loaded)
	{
		if (input1.getCorrectName() && input2.getCorrectName())
		{
			isGameStarted = true;
			board.initializeBoard();
			board.setWhiteTimerValue(whiteTimer);
			board.setBlackTimerValue(blackTimer);
		}
	}
}

void Game::run()
{
	while (window.isOpen())
	{
		handleEvents();
		//checkIfEnd();
		render();
	}
}

void Game::exitGame()
{
	if (!board.getTimerStatus())
		board.stopTimers();
	window.close();
}


void Game::incrementWhite()
{
	whiteTimer += 15;
}

void Game::decrementWhite()
{
	if (whiteTimer - 15 > 0)
		whiteTimer -= 15;
}

void Game::incrementBlack()
{
	blackTimer += 15;
}

void Game::decrementBlack()
{
	if (blackTimer - 15 > 0)
		blackTimer -= 15;
}

void Game::load()
{
	slManager.loadFromFile(board);
	loaded = true;
	startGame();
}

void Game::save()
{
	slManager.saveToFile(board);
}

void Game::drawTimer(sf::Vector2f position, sf::RenderWindow& window, const PieceColor pc)
{
	sf::RectangleShape timePlace;
	sf::Text timeText;
	sf::Font font;

	timePlace.setSize(sf::Vector2f(100, 50));
	timePlace.setFillColor(sf::Color::White);
	timePlace.setPosition(sf::Vector2f(position));

	font.loadFromFile("..\\rsc\\Roboto-Regular.ttf");

	timeText.setFont(font);
	timeText.setCharacterSize(24);
	timeText.setFillColor(sf::Color::Black);
	if (pc == PieceColor::White)
		timeText.setString(std::to_string(whiteTimer / 60) + ":" + std::to_string(whiteTimer % 60));
	else
		timeText.setString(std::to_string(blackTimer / 60) + ":" + std::to_string(blackTimer % 60));

	sf::FloatRect textRect = timeText.getLocalBounds();
	timeText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	timeText.setPosition(timePlace.getPosition().x + timePlace.getSize().x / 2.0f, timePlace.getPosition().y + timePlace.getSize().y / 2.0f);

	window.draw(timePlace);
	window.draw(timeText);
}

void Game::checkIfEnd()
{
	if (!stop)
	{

		if (isGameStarted)
		{
			if (board.getMovesWithoutTakes() >= 50)
			{
				if (!board.getTimerStatus())
				{
					stop = true;
					board.stopTimers();
					std::cout << "Tie" << std::endl;
				}
			}
		}
	}
}