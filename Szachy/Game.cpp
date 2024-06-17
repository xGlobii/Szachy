#include "Game.h"

Game::Game() : window(sf::VideoMode::getDesktopMode(), "Chess by Globii.", sf::Style::Titlebar), input1(sf::Vector2f(300, 150), sf::Vector2f(1200, 150)), input2(sf::Vector2f(300, 150), sf::Vector2f(1200, 350)),
whiteTimer(600), blackTimer(600)
{
	window.setFramerateLimit(60);
	tileSize = 100.f;
	backgroundTexture.loadFromFile("..\\rsc\\images\\background.jpg");
	background.setTexture(backgroundTexture);
	loaded = false;

	whitePauseFlag = false;
	blackPauseFlag = true;

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

		saveButton = Button(1500, 100, sf::Vector2f(1000, 150), sf::Color::Blue, std::bind(&Game::save, this));
		saveButton.setText("Save game");

		gameExitButton = Button(1500, 100, sf::Vector2f(1000, 275), sf::Color::Blue, std::bind(&Game::exitGame, this));
		gameExitButton.setText("Exit game");
	}

	lock = false;
	saved = false;

	isGameStarted = false;
}

void Game::handleEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			if (!board.getTimerStatus() || board.getGameStatus() && !stopFlag)
				stopTimers();
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
	if (!isGameStarted && board.getTimerStatus())
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
	else if (isGameStarted)
	{
		board.draw(window);
		board.drawPointsPlace(window, board.getPlayersPoints(PieceColor::White), sf::Vector2f(100, 1000));
		board.drawPointsPlace(window, board.getPlayersPoints(PieceColor::Black), sf::Vector2f(100, 100));
		setTimers(sf::Vector2f(250, 1000), window, PieceColor::White);
		setTimers(sf::Vector2f(250, 100), window, PieceColor::Black);
		saveButton.draw(window);
		gameExitButton.draw(window);
		board.drawLastMoves(window);
		if (board.getPromoting())
		{
			board.drawPromotionIcons(window, board.getPromotionPosition());
		}
	}

	if (isGameStarted && board.getGameStatus())
		drawMatchHistory();

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
		}
	}
}

void Game::run()
{
	while (window.isOpen())
	{
		handleEvents();
		if (!stopFlag)
			checkIfEnd();
		render();
		if (!board.getTimerStatus() && !lock)
		{
			lock = true;
			slManager.loadGameHistory(gameHistory);
			startTimers();
		}
		if (board.getGameStatus() && !stopFlag)
			stopFlag = true;

		if (whiteTimer.getActualTime() <= 0 || blackTimer.getActualTime() <= 0)
		{
			board.setGameStatus(true);
			stopTimers();
		}

		updateTurn();
	}
}

#pragma region Buttons

void Game::exitGame()
{
	if (!board.getTimerStatus() || board.getGameStatus() && !stopFlag)
		stopTimers();
	window.close();
}

void Game::incrementWhite()
{
	whiteTimer.setTime(whiteTimer.getActualTime() + 15);
}

void Game::decrementWhite()
{
	if (whiteTimer.getActualTime() - 15 > 0)
		whiteTimer.setTime(whiteTimer.getActualTime() - 15);
}

void Game::incrementBlack()
{
	blackTimer.setTime(blackTimer.getActualTime() + 15);
}

void Game::decrementBlack()
{
	if (blackTimer.getActualTime() - 15 > 0)
		blackTimer.setTime(blackTimer.getActualTime() - 15);
}

void Game::load()
{
	slManager.loadFromFile(board, whiteTimer, blackTimer, input1, input2);
	loaded = true;
	startGame();
}

void Game::save()
{
	slManager.saveToFile(board, whiteTimer, blackTimer, input1, input2);
}

#pragma endregion

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
		timeText.setString(std::to_string(whiteTimer.getActualTime() / 60) + ":" + std::to_string(whiteTimer.getActualTime() % 60));
	else
		timeText.setString(std::to_string(blackTimer.getActualTime() / 60) + ":" + std::to_string(blackTimer.getActualTime() % 60));

	sf::FloatRect textRect = timeText.getLocalBounds();
	timeText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	timeText.setPosition(timePlace.getPosition().x + timePlace.getSize().x / 2.0f, timePlace.getPosition().y + timePlace.getSize().y / 2.0f);

	window.draw(timePlace);
	window.draw(timeText);
}

void Game::checkIfEnd()
{
	if (isGameStarted)
	{
		if (board.getMovesWithoutTakes() >= 100)
		{
			if (!board.getTimerStatus())
			{
				stopFlag = true;
				stopTimers();
				std::cout << "Tie" << std::endl;
			}
		}
	}
}

void Game::startTimers()
{
	if (board.getTurn())
	{
		whitePauseFlag = false;
		blackPauseFlag = true;
	}
	else
	{
		whitePauseFlag = true;
		blackPauseFlag = false;
	}

	whiteClockThread = std::thread(&Timer::startTimer, &whiteTimer, std::ref(stopFlag), std::ref(whitePauseFlag));
	blackClockThread = std::thread(&Timer::startTimer, &blackTimer, std::ref(stopFlag), std::ref(blackPauseFlag));

	whiteClockThread.detach();
	blackClockThread.detach();
}

void Game::stopTimers()
{
	stopFlag = true;
}

void Game::setTimers(sf::Vector2f position, sf::RenderWindow& window, const PieceColor pc)
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
		timeText.setString(std::to_string(whiteTimer.getActualTime() / 60) + ":" + std::to_string(whiteTimer.getActualTime() % 60));
	else
		timeText.setString(std::to_string(blackTimer.getActualTime() / 60) + ":" + std::to_string(blackTimer.getActualTime() % 60));

	sf::FloatRect textRect = timeText.getLocalBounds();
	timeText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	timeText.setPosition(timePlace.getPosition().x + timePlace.getSize().x / 2.0f, timePlace.getPosition().y + timePlace.getSize().y / 2.0f);

	window.draw(timePlace);
	window.draw(timeText);
}

void Game::updateTurn()
{
	if (board.getTurn())
	{
		whitePauseFlag = false;
		blackPauseFlag = true;
	}
	else
	{
		whitePauseFlag = true;
		blackPauseFlag = false;
	}
}

void Game::drawMatchHistory()
{
	sf::RectangleShape box;
	sf::Text text;
	sf::Font font;
	sf::Text players;

	std::time_t now = std::time(nullptr);
	std::tm local_time;

	char date_buffer[11];

	localtime_s(&local_time, &now);
	std::strftime(date_buffer, sizeof(date_buffer), "%d-%m-%Y", &local_time);

	box.setPosition(200, 200);
	box.setSize(sf::Vector2f(1200, 700));
	box.setFillColor(sf::Color(40, 40, 40, 200));

	sf::FloatRect textBounds = text.getLocalBounds();
	float textX = box.getPosition().x + (box.getSize().x - textBounds.width) / 2.f;
	float textY = box.getPosition().y + 10.f;

	font.loadFromFile("..\\rsc\\Roboto-Regular.ttf");
	text.setCharacterSize(24);
	text.setFont(font);
	text.setFillColor(sf::Color::White);

	text.setPosition(textX, textY);

	std::string end;
	std::string result;

	if (board.getEndGameType() == "")
		end = "Time out";
	else
		end = board.getEndGameType();

	text.setString(end);


	players.setFont(font);
	players.setCharacterSize(24);
	players.setFillColor(sf::Color::White);
	if (board.getEndGameType()[0] == 'D')
	{
		result = " 0.5 - 0.5 ";
		players.setString(input1.getName() + result + input2.getName() + " - " + date_buffer);
	}
	else if (board.getEndGameType()[0] == 'C')
	{
		result = (board.getTurn() ? " 1 - 0 " : " 0 - 1 ");
		players.setString(input1.getName() + result + input2.getName() + " - " + date_buffer);
	}
	else if (end[0] == 'T')
	{
		result = (whiteTimer.getActualTime() > 0 ? " 1-0 " : " 0-1 ");
		players.setString(input1.getName() + result + input2.getName() + " - " + date_buffer);
	}


	players.setPosition(text.getPosition().x, text.getPosition().y + 45);

	if (!saved)
	{
		saved = true;
		gameHistory.push_back(input1.getName() + result + input2.getName() + " - " + date_buffer);
		slManager.saveGameHistory(gameHistory);
	}

	window.draw(box);
	window.draw(text);
	for (int i = 0; i < gameHistory.size(); i++)
	{

		players.setString(gameHistory[i]);
		players.setPosition(text.getPosition().x, text.getPosition().y + 45 + i * 15);

		window.draw(players);
	}
}