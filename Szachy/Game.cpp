#include "Game.h"

Game::Game() : window(sf::VideoMode::getDesktopMode(), "Chess by Globii.", sf::Style::None), input1(sf::Vector2f(300, 50), sf::Vector2f(330, 275)), input2(sf::Vector2f(300, 50), sf::Vector2f(1290, 275)),
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
		startButton = Button(300, 100, sf::Vector2f(840, 50), sf::Color(100, 100, 100), std::bind(&Game::startGame, this));
		startButton.setText("Start");

		loadButton = Button(300, 100, sf::Vector2f(840, 400), sf::Color(100, 100, 100), std::bind(&Game::load, this));
		loadButton.setText("Load from file");

		exitBtn = Button(300, 100, sf::Vector2f(840, 850), sf::Color(100, 100, 100), std::bind(&Game::exitGame, this));
		exitBtn.setText("Exit");

		decrementWhiteTimer = Button(100, 100, sf::Vector2f(280, 600), sf::Color(100, 100, 100), std::bind(&Game::decrementWhite, this));
		decrementWhiteTimer.setText("-");

		incrementWhiteTimer = Button(100, 100, sf::Vector2f(580, 600), sf::Color(100, 100, 100), std::bind(&Game::incrementWhite, this));
		incrementWhiteTimer.setText("+");

		decrementBlackTimer = Button(100, 100, sf::Vector2f(1240, 600), sf::Color(100, 100, 100), std::bind(&Game::decrementBlack, this));
		decrementBlackTimer.setText("-");

		incrementBlackTimer = Button(100, 100, sf::Vector2f(1540, 600), sf::Color(100, 100, 100), std::bind(&Game::incrementBlack, this));
		incrementBlackTimer.setText("+");

		saveButton = Button(250, 100, sf::Vector2f(1100, 900), sf::Color(100, 100, 100), std::bind(&Game::save, this));
		saveButton.setText("Save game");

		gameExitButton = Button(250, 100, sf::Vector2f(1450, 900), sf::Color(100, 100, 100), std::bind(&Game::exitGame, this));
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
		drawTimer(sf::Vector2f(430, 600), window, PieceColor::White);
		drawTimer(sf::Vector2f(1390, 600), window, PieceColor::Black);
		drawName(sf::Vector2f(370, 200), "White Player");
		drawName(sf::Vector2f(1330, 200), "Black Player");
		input1.draw(window);
		input2.draw(window);
	}
	else if (isGameStarted)
	{
		board.draw(window);
		drawPointsPlace(board.getPlayersPoints(PieceColor::White), sf::Vector2f(725, 1000));
		drawPointsPlace(board.getPlayersPoints(PieceColor::Black), sf::Vector2f(725, 50));
		drawInGameTimers(sf::Vector2f(850, 1000), PieceColor::White);
		drawInGameTimers(sf::Vector2f(850, 50), PieceColor::Black);
		drawInGameNickname(sf::Vector2f(150, 1000), input1.getName());
		drawInGameNickname(sf::Vector2f(150, 50), input2.getName());
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
			slManager.setSaveName();
		}
	}
}

void Game::run()
{
	while (window.isOpen())
	{
		handleEvents();

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

	timePlace.setSize(sf::Vector2f(100, 100));
	timePlace.setFillColor(sf::Color::White);
	timePlace.setPosition(sf::Vector2f(position));

	font.loadFromFile("..\\rsc\\Roboto-Regular.ttf");

	timeText.setFont(font);

	timeText.setCharacterSize(40);
	timeText.setFillColor(sf::Color::White);

	timeText.setString("White Timer");
	timeText.setPosition(380, 525);
	window.draw(timeText);

	timeText.setString("Black Timer");
	timeText.setPosition(1340, 525);
	window.draw(timeText);

	timeText.setCharacterSize(24);
	timeText.setFillColor(sf::Color::Black);

	std::string minutes, seconds;

	if (pc == PieceColor::White)
	{
		minutes = std::to_string(whiteTimer.getActualTime() / 60);
		if (whiteTimer.getActualTime() % 60 < 10)
			seconds = "0" + std::to_string(whiteTimer.getActualTime() % 60);
		else
			seconds = std::to_string(whiteTimer.getActualTime() % 60);
		timeText.setString(minutes + ":" + seconds);
	}
	else
	{
		if (blackTimer.getActualTime() % 60 < 10)
			seconds = "0" + std::to_string(blackTimer.getActualTime() % 60);
		else
			seconds = std::to_string(blackTimer.getActualTime() % 60);
		timeText.setString(std::to_string(blackTimer.getActualTime() / 60) + ":" + seconds);
	}

	sf::FloatRect textRect = timeText.getLocalBounds();
	timeText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	timeText.setPosition(timePlace.getPosition().x + timePlace.getSize().x / 2.0f, timePlace.getPosition().y + timePlace.getSize().y / 2.0f);

	window.draw(timePlace);
	window.draw(timeText);
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

void Game::drawInGameTimers(sf::Vector2f position, const PieceColor pc)
{
	sf::RectangleShape timePlace;
	sf::Text timeText;
	sf::Font font;

	timePlace.setSize(sf::Vector2f(100, 50));
	timePlace.setFillColor(sf::Color::White);
	timePlace.setPosition(sf::Vector2f(position));
	timePlace.setOutlineColor(sf::Color::Black);
	timePlace.setOutlineThickness(4);

	font.loadFromFile("..\\rsc\\Roboto-Regular.ttf");

	timeText.setFont(font);
	timeText.setCharacterSize(24);
	timeText.setFillColor(sf::Color::Black);

	std::string minutes, seconds;

	if (pc == PieceColor::White)
	{
		minutes = std::to_string(whiteTimer.getActualTime() / 60);
		if (whiteTimer.getActualTime() % 60 < 10)
			seconds = "0" + std::to_string(whiteTimer.getActualTime() % 60);
		else
			seconds = std::to_string(whiteTimer.getActualTime() % 60);
		timeText.setString(minutes + ":" + seconds);
	}
	else
	{
		if (blackTimer.getActualTime() % 60 < 10)
			seconds = "0" + std::to_string(blackTimer.getActualTime() % 60);
		else
			seconds = std::to_string(blackTimer.getActualTime() % 60);
		timeText.setString(std::to_string(blackTimer.getActualTime() / 60) + ":" + seconds);
	}

	sf::FloatRect textRect = timeText.getLocalBounds();
	timeText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	timeText.setPosition(timePlace.getPosition().x + timePlace.getSize().x / 2.0f, timePlace.getPosition().y + timePlace.getSize().y / 2.0f);

	window.draw(timePlace);
	window.draw(timeText);
}

void Game::drawInGameNickname(sf::Vector2f position, std::string name)
{
	sf::RectangleShape box;
	sf::Text text;
	sf::Font font;

	box.setSize(sf::Vector2f(300, 50));
	box.setFillColor(sf::Color::White);
	box.setPosition(sf::Vector2f(position));
	box.setOutlineColor(sf::Color::Black);
	box.setOutlineThickness(4);

	font.loadFromFile("..\\rsc\\Roboto-Regular.ttf");

	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Black);
	
	text.setString(name);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(box.getPosition().x + box.getSize().x / 2.0f, box.getPosition().y + box.getSize().y / 2.0f);

	window.draw(box);
	window.draw(text);
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
	box.setSize(sf::Vector2f(1520, 680));
	box.setFillColor(sf::Color(40, 40, 40, 200));

	sf::FloatRect textBounds = text.getLocalBounds();
	float textX = 760;
	float textY = box.getPosition().y + 20;

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

	if (board.getEndGameType()[0] != 'D')
		text.setString(end + "\n" + (board.getTurn() == 1 ? "White wins" : "Black wins"));


	players.setFont(font);
	players.setCharacterSize(24);
	players.setFillColor(sf::Color::White);
	if (board.getEndGameType()[0] == 'D')
	{
		result = " 0.5 - 0.5 ";
		players.setString("\n" + input1.getName() + result + input2.getName() + " - " + date_buffer);
	}
	else if (board.getEndGameType()[0] == 'C')
	{
		result = (board.getTurn() ? " 1 - 0 " : " 0 - 1 ");
		players.setString("\n" + input1.getName() + result + input2.getName() + " - " + date_buffer);
	}
	else if (end[0] == 'T')
	{
		result = (whiteTimer.getActualTime() > 0 ? " 1-0 " : " 0-1 ");
		players.setString("\n" + input1.getName() + result + input2.getName() + " - " + date_buffer);
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
	int startIdx = (gameHistory.size() > 24) ? gameHistory.size() - 22 : 0;
	for (int i = startIdx, pos = 0; i < gameHistory.size(); i++, pos++)
	{

		players.setString(gameHistory[i]);
		players.setPosition(text.getPosition().x, text.getPosition().y + 60 + pos * 25);

		window.draw(players);
	}
}

void Game::drawPointsPlace(const int points, sf::Vector2f position)
{
	sf::RectangleShape pointsPlace;
	sf::Text pointsText;
	sf::Font font;

	pointsPlace.setSize(sf::Vector2f(100, 50));
	pointsPlace.setFillColor(sf::Color::White);
	pointsPlace.setPosition(sf::Vector2f(position));

	font.loadFromFile("..\\rsc\\Roboto-Regular.ttf");

	pointsText.setFont(font);
	pointsText.setCharacterSize(24);
	pointsText.setFillColor(sf::Color::Black);
	pointsText.setString(std::to_string(points));


	sf::FloatRect textRect = pointsText.getLocalBounds();
	pointsText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	pointsText.setPosition(pointsPlace.getPosition().x + pointsPlace.getSize().x / 2.0f, pointsPlace.getPosition().y + pointsPlace.getSize().y / 2.0f);

	window.draw(pointsPlace);
	window.draw(pointsText);
}

void Game::drawName(sf::Vector2f position, std::string text)
{
	sf::Text nameText;
	sf::Font font;

	font.loadFromFile("..\\rsc\\Roboto-Regular.ttf");

	nameText.setFont(font);

	nameText.setCharacterSize(40);
	nameText.setFillColor(sf::Color::White);

	nameText.setString(text);
	nameText.setPosition(position);

	window.draw(nameText);
}