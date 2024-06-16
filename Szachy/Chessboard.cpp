#include "Chessboard.h"

Chessboard::Chessboard() : whiteTimer(600), blackTimer(600)
{
	board.resize(8, std::vector<Piece*>(8, nullptr));

	whitePauseFlag = false;
	blackPauseFlag = true;
	stopFlag = false;
	whiteTurn = true;

	isCastle = false;
	isCheck = false;
	takes = false;
	checkmate = false;

	movesWithoutTake = 0;
	tileSize = 100;
	whitePoints = 0;
	blackPoints = 0;

	offset = sf::Vector2f(150, 150);

	startTimer = true;
}

Chessboard::~Chessboard()
{
	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board.size(); j++)
		{
			delete board[i][j];
		}
	}
}

void Chessboard::draw(sf::RenderWindow& window)
{
	sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
	sf::RectangleShape chessboard(sf::Vector2f(tileSize * 8 + 80, tileSize * 8 + 80));
	chessboard.setFillColor(sf::Color(112, 59, 19));
	chessboard.setPosition(offset.x - 40, offset.y - 40);

	window.draw(chessboard);


	sf::Color black(184, 135, 98);
	sf::Color white(237, 214, 176);

	for (int y = 0; y < 8; y++)
	{
		tile.setPosition(sf::Vector2f(0, (tileSize * y) + offset.y));
		for (int x = 0; x < 8; x++)
		{
			tile.setPosition(sf::Vector2f((tileSize * x) + offset.x, tile.getPosition().y));
			if ((x + y) % 2 == 1)
				tile.setFillColor(black);
			else
				tile.setFillColor(white);

			window.draw(tile);
		}
	}

	sf::Font font;
	font.loadFromFile("..\\rsc\\Roboto-Regular.ttf");

	sf::Text coordText;
	coordText.setFont(font);
	coordText.setCharacterSize(20);
	coordText.setFillColor(sf::Color::White);

	std::string letters = "abcdefgh";
	std::string numbers = "12345678";

	for (int x = 0; x < 8; x++)
	{
		coordText.setString(letters.substr(x, 1));
		coordText.setPosition(sf::Vector2f((x * tileSize) + offset.x + tileSize / 2 - 5, offset.y - 25));
		window.draw(coordText);
	}

	for (int y = 0; y < 8; y++)
	{
		coordText.setString(numbers.substr(y, 1));
		coordText.setPosition(sf::Vector2f(offset.x - 25, (7 - y) * tileSize + offset.y + tileSize / 2 - 10));
		window.draw(coordText);
	}

	for (int y = 0; y < board.size(); y++)
	{
		for (int x = 0; x < board.size(); x++)
		{
			if (board[x][y] != nullptr)
				board[x][y]->draw(window, 100, sf::Vector2f((x * tileSize) + offset.x, (y * tileSize) + offset.y));
		}
	}
}

void Chessboard::handleMouseClick(sf::RenderWindow& window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	if (mousePosition.x >= (0 + offset.x) && mousePosition.x <= (800 + offset.x) && mousePosition.y >= (0 + offset.y) && mousePosition.y <= (800 + offset.y))		//Check if player clicked on chessboard
	{
		sf::Vector2i boardPosition((mousePosition.x - offset.x) / 100, (mousePosition.y - offset.y) / 100);		//Getting position on board
		if (selectedPiecePosition != sf::Vector2i(-1, -1))									//We have selected piece position
		{
			if ((board[boardPosition.x][boardPosition.y] != nullptr) &&
				((whiteTurn && board[boardPosition.x][boardPosition.y]->getColor() == PieceColor::Black) ||
					(!whiteTurn && board[boardPosition.x][boardPosition.y]->getColor() == PieceColor::White)))							//Taking piece
			{
				pieceTakes(boardPosition, window);
			}
			else if (board[boardPosition.x][boardPosition.y] == nullptr)
			{
				pieceMoves(boardPosition, window);
			}
			selectedPiecePosition = sf::Vector2i(-1, -1);
		}
		else if (board[boardPosition.x][boardPosition.y] != nullptr)
		{
			if (startTimer)
			{
				startTimer = false;
				startTimers();
			}

			if ((whiteTurn && board[boardPosition.x][boardPosition.y]->getColor() == PieceColor::White) ||
				(!whiteTurn && board[boardPosition.x][boardPosition.y]->getColor() == PieceColor::Black))
			{
				selectedPiecePosition = boardPosition;
			}
		}
	}
}

void Chessboard::pieceMoves(sf::Vector2i to, sf::RenderWindow& window)
{
	bool tempSpecial = board[selectedPiecePosition.x][selectedPiecePosition.y]->getSpecialMoveStatus();
	if (board[selectedPiecePosition.x][selectedPiecePosition.y]->getPiece() == PieceType::King &&
		abs(to.x - selectedPiecePosition.x) == 2)
	{
		canCastle(to);
		isCastle = true;
		record.notation = generateAlgebricNotation(selectedPiecePosition, to, PieceType::King);
		moveHistory.push_back(record.notation);
	}
	else
	{

		if (board[selectedPiecePosition.x][selectedPiecePosition.y]->possibleMove(board, selectedPiecePosition, to))
		{
			PieceType type = board[selectedPiecePosition.x][selectedPiecePosition.y]->getPiece();
			board[to.x][to.y] = board[selectedPiecePosition.x][selectedPiecePosition.y];
			board[selectedPiecePosition.x][selectedPiecePosition.y] = nullptr;

			if (!isKingInCheck(whiteTurn ? PieceColor::White : PieceColor::Black))
			{
				if (whiteTurn)
				{
					whitePauseFlag = true;
					blackPauseFlag = false;
				}
				else
				{
					whitePauseFlag = false;
					blackPauseFlag = true;
				}
				movesWithoutTake++;

				if (board[to.x][to.y]->getPiece() == PieceType::Pawn && (to.y == 0 || to.y == 7))
				{
					promotePawn(to, window);
				}

				if (isCheckmate(whiteTurn ? PieceColor::Black : PieceColor::White, to))
				{
					checkmate = true;
					stopTimers();
				}

				if (!checkmate)
					isCheck = isKingInCheck(whiteTurn ? PieceColor::Black : PieceColor::White);
				record.notation = generateAlgebricNotation(selectedPiecePosition, to, type);
				moveHistory.push_back(record.notation);

				whiteTurn = !whiteTurn;

			}
			else
			{
				board[selectedPiecePosition.x][selectedPiecePosition.y] = std::move(board[to.x][to.y]);
				board[selectedPiecePosition.x][selectedPiecePosition.y]->setSpecialMove(tempSpecial);
				board[to.x][to.y] = nullptr;
			}
		}



		selectedPiecePosition = sf::Vector2i(-1, -1);
	}
}

void Chessboard::pieceTakes(sf::Vector2i to, sf::RenderWindow& window)
{
	bool tempSpecial = board[selectedPiecePosition.x][selectedPiecePosition.y]->getSpecialMoveStatus();
	if (board[selectedPiecePosition.x][selectedPiecePosition.y]->takes(selectedPiecePosition, to, board))
	{
		int points = board[to.x][to.y]->getValue();
		Piece* tempPiece = board[to.x][to.y];
		PieceType type = board[selectedPiecePosition.x][selectedPiecePosition.y]->getPiece();
		board[to.x][to.y] = board[selectedPiecePosition.x][selectedPiecePosition.y];
		board[selectedPiecePosition.x][selectedPiecePosition.y] = nullptr;

		if (!isKingInCheck(whiteTurn ? PieceColor::White : PieceColor::Black))
		{
			if (whiteTurn)
			{
				whitePoints += points;
				whitePauseFlag = true;
				blackPauseFlag = false;
			}
			else
			{
				blackPoints += points;
				whitePauseFlag = false;
				blackPauseFlag = true;
			}

			takes = true;

			if (isCheckmate(whiteTurn ? PieceColor::Black : PieceColor::White, to))
			{
				checkmate = true;
				stopTimers();
			}

			if (!checkmate)
				isCheck = isKingInCheck(whiteTurn ? PieceColor::Black : PieceColor::White);
			record.notation = generateAlgebricNotation(selectedPiecePosition, to, type);
			moveHistory.push_back(record.notation);

			whiteTurn = !whiteTurn;

			movesWithoutTake = 0;
		}
		else
		{
			board[selectedPiecePosition.x][selectedPiecePosition.y] = std::move(board[to.x][to.y]);
			board[to.x][to.y] = tempPiece;
			board[selectedPiecePosition.x][selectedPiecePosition.y]->setSpecialMove(tempSpecial);
			//delete tempPiece;
		}
	}
	selectedPiecePosition = sf::Vector2i(-1, -1);
}

void Chessboard::promotePawn(sf::Vector2i position, sf::RenderWindow& window)
{
	board[position.x][position.y] = new Queen(whiteTurn ? PieceColor::White : PieceColor::Black, PieceType::Queen);
	drawPromotionIcons(window, position);
}

void Chessboard::drawPromotionIcons(sf::RenderWindow& window, sf::Vector2i position)
{
	sf::RectangleShape box(sf::Vector2f(100, 400));
	//box.setPosition(position.x * 100 + 150, position.y * 100 + 150);
	box.setPosition(sf::Vector2f(0, 0));
	//box.setSize(sf::Vector2f(100, 400));
	box.setFillColor(sf::Color::Red);
	window.draw(box);
}

int Chessboard::getPlayersPoints(PieceColor pc)
{
	if (pc == PieceColor::White)
		return whitePoints;
	else
		return blackPoints;
}

void Chessboard::setPlayerPoints(PieceColor pc, int value)
{
	if (pc == PieceColor::White)
		whitePoints = value;
	else
		blackPoints = value;
}

sf::Vector2i Chessboard::findKing(const PieceColor pc)
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (board[x][y] != nullptr && board[x][y]->getPiece() == PieceType::King && board[x][y]->getColor() == pc)				//Searching for king
				return sf::Vector2i(x, y);																							//Returning position of king
		}
	}
	return sf::Vector2i(-1, -1);																									//King not found
}

bool Chessboard::isKingInCheck(const PieceColor kingColor)
{
	sf::Vector2i kingPosition = findKing(kingColor);
	if (kingPosition == sf::Vector2i(-1, -1))				//King not found
		return false;

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (board[x][y] != nullptr && board[x][y]->getColor() != kingColor)				//Searching for 'enemy' pieces on board
			{
				if (board[x][y]->takes(sf::Vector2i(x, y), kingPosition, board))		//Checking if king is in direct attack
					return true;
			}
		}
	}

	return false;
}

void Chessboard::setTimers(sf::Vector2f position, sf::RenderWindow& window, const PieceColor pc)
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

void Chessboard::startTimers()
{
	if (whiteTurn)
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
}

void Chessboard::stopTimers()
{
	stopFlag = true;
	whiteClockThread.join();
	blackClockThread.join();
}

void Chessboard::drawPointsPlace(sf::RenderWindow& window, const int points, sf::Vector2f position)
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

std::vector<std::vector<Piece*>> Chessboard::getBoard()
{
	return board;
}

void Chessboard::setBoard(std::vector<std::vector<Piece*>>& board)
{
	this->board = board;
}

void Chessboard::initializeBoard()
{
	for (int i = 0; i < board.size(); i++)
	{
		board[i][1] = new Pawn(PieceColor::Black, PieceType::Pawn);
		board[i][6] = new Pawn(PieceColor::White, PieceType::Pawn);
	}

	//Black
	board[0][0] = new Rook(PieceColor::Black, PieceType::Rook);
	board[1][0] = new Knight(PieceColor::Black, PieceType::Knight);
	board[2][0] = new Bishop(PieceColor::Black, PieceType::Bishop);
	board[3][0] = new Queen(PieceColor::Black, PieceType::Queen);
	board[4][0] = new King(PieceColor::Black, PieceType::King);
	board[5][0] = new Bishop(PieceColor::Black, PieceType::Bishop);
	board[6][0] = new Knight(PieceColor::Black, PieceType::Knight);
	board[7][0] = new Rook(PieceColor::Black, PieceType::Rook);

	//White
	board[0][7] = new Rook(PieceColor::White, PieceType::Rook);
	board[1][7] = new Knight(PieceColor::White, PieceType::Knight);
	board[2][7] = new Bishop(PieceColor::White, PieceType::Bishop);
	board[3][7] = new Queen(PieceColor::White, PieceType::Queen);
	board[4][7] = new King(PieceColor::White, PieceType::King);
	board[5][7] = new Bishop(PieceColor::White, PieceType::Bishop);
	board[6][7] = new Knight(PieceColor::White, PieceType::Knight);
	board[7][7] = new Rook(PieceColor::White, PieceType::Rook);
}

bool Chessboard::getTurn()
{
	return whiteTurn;
}

void Chessboard::setTurn(bool whiteTurn)
{
	this->whiteTurn = whiteTurn;
}

int Chessboard::getWhiteTimerValue()
{
	return whiteTimer.getActualTime();
}

int Chessboard::getBlackTimerValue()
{
	return blackTimer.getActualTime();
}

void Chessboard::setWhiteTimerValue(const int time)
{
	whiteTimer.setTime(time);
}

void Chessboard::setBlackTimerValue(const int time)
{
	blackTimer.setTime(time);
}

bool Chessboard::getTimerStatus()
{
	return startTimer;
}

bool Chessboard::canCastle(sf::Vector2i boardPosition)
{
	int rookX;

	if (selectedPiecePosition.y != boardPosition.y)
		return false;

	if (boardPosition.x == 2)
		rookX = 0;
	else if (boardPosition.x == 6)
		rookX = 7;
	else
		return false;

	if (!board[selectedPiecePosition.x][selectedPiecePosition.y]->getSpecialMoveStatus() ||
		!board[rookX][selectedPiecePosition.y]->getSpecialMoveStatus())
		return false;

	int step = (boardPosition.x == 2) ? -1 : 1;
	for (int i = selectedPiecePosition.x + step; i != rookX; i += step)
	{
		if (board[i][selectedPiecePosition.y] != nullptr)
			return false;
	}

	for (int i = selectedPiecePosition.x + step; i != boardPosition.x + step; i += step)
	{
		std::swap(board[selectedPiecePosition.x][selectedPiecePosition.y], board[i][selectedPiecePosition.y]);
		bool kingInCheck = isKingInCheck(whiteTurn ? PieceColor::White : PieceColor::Black);
		std::swap(board[selectedPiecePosition.x][selectedPiecePosition.y], board[i][selectedPiecePosition.y]);

		if (kingInCheck)
			return false;
	}

	std::swap(board[selectedPiecePosition.x][selectedPiecePosition.y], board[boardPosition.x][boardPosition.y]);
	if (boardPosition.x == 2)
		std::swap(board[rookX][selectedPiecePosition.y], board[boardPosition.x + 1][selectedPiecePosition.y]);
	else if (boardPosition.x == 6)
		std::swap(board[rookX][selectedPiecePosition.y], board[boardPosition.x - 1][selectedPiecePosition.y]);

	board[boardPosition.x][boardPosition.y]->setSpecialMove(false);
	if (boardPosition.x == 2)
		board[boardPosition.x + 1][selectedPiecePosition.y]->setSpecialMove(false);
	else if (boardPosition.x == 6)
		board[boardPosition.x - 1][selectedPiecePosition.y]->setSpecialMove(false);

	if (whiteTurn)
	{
		whitePauseFlag = true;
		blackPauseFlag = false;
	}
	else
	{
		whitePauseFlag = false;
		blackPauseFlag = true;
	}
	whiteTurn = !whiteTurn;

	return true;
}

int Chessboard::getMovesWithoutTakes()
{
	return movesWithoutTake;
}

void Chessboard::setMovesWithoutTakes(int moves)
{
	movesWithoutTake = moves;
}

std::string Chessboard::generateAlgebricNotation(sf::Vector2i from, sf::Vector2i to, PieceType pt)
{
	std::string columns = "abcdefgh";
	std::string notation = "";

	if (isCastle)
	{
		if (to.x == 2)
			notation = "O-O-O";
		else if (to.x == 6)
			notation = "O-O";
	}
	else
	{
		notation += pieceTypeToSymbol(pt);
		notation += columns[from.x];
		notation += std::to_string(8 - from.y);
		if (!takes)
			notation += "-";
		else
			notation += "x";
		notation += columns[to.x];
		notation += std::to_string(8 - to.y);

		if (isCheck)
			notation += "+";
		if (checkmate)
			notation += "#";
	}

	this->isCheck = false;
	this->takes = false;
	this->isCastle = false;
	this->checkmate = false;
	from = sf::Vector2i(-1, -1);
	to = sf::Vector2i(-1, -1);

	return notation;
}

std::string Chessboard::pieceTypeToSymbol(PieceType pt)
{
	switch (pt)
	{
	case PieceType::King:
		return "K";
		break;
	case PieceType::Queen:
		return "Q";
		break;
	case PieceType::Rook:
		return "R";
		break;
	case PieceType::Bishop:
		return "B";
		break;
	case PieceType::Knight:
		return "N";
		break;
	case PieceType::Pawn:
		return "";
		break;
	default:
		break;
	}
}

bool Chessboard::isCheckmate(PieceColor pc, sf::Vector2i attackerPosition)
{
	if (!isKingInCheck(pc))
		return false;

	sf::Vector2i kingPosition = findKing(pc);

	std::future<bool> f1 = std::async(&Chessboard::checkIfKingCanEscape, this, kingPosition, attackerPosition, pc);
	std::future<bool> f2 = std::async(&Chessboard::checkIfThereMultipleChecks, this, kingPosition, pc);
	std::future<bool> f3 = std::async(&Chessboard::checkIfCanBlockMate, this, kingPosition, attackerPosition, pc);

	if (!f1.get() || f2.get() || !f3.get())
		return false;
	else
		return true;
}

bool Chessboard::checkIfKingCanEscape(sf::Vector2i kingPosition, sf::Vector2i attackerPosition, PieceColor pc)
{
	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			if (dx == 0 && dy == 0)
				continue;

			int newX = kingPosition.x + dx;
			int newY = kingPosition.y + dy;

			if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8)
			{
				if (board[newX][newY] == nullptr ||
					(board[newX][newY]->getColor() != pc && board[newX][newY]->takes(sf::Vector2i(newX, newY), kingPosition, board)))
				{
					Piece* tempPiece = board[newX][newY];
					bool tempSpecial = board[kingPosition.x][kingPosition.y]->getSpecialMoveStatus();

					board[newX][newY] = board[kingPosition.x][kingPosition.y];
					board[kingPosition.x][kingPosition.y] = nullptr;

					if (!isKingInCheck(pc))
					{
						board[kingPosition.x][kingPosition.y] = std::move(board[newX][newY]);
						board[newX][newY] = tempPiece;
						board[kingPosition.x][kingPosition.y]->setSpecialMove(tempSpecial);
						return false;
					}

					board[kingPosition.x][kingPosition.y] = std::move(board[newX][newY]);
					board[newX][newY] = tempPiece;
					board[kingPosition.x][kingPosition.y]->setSpecialMove(tempSpecial);
				}
			}

		}
	}

	return true;
}

bool Chessboard::checkIfThereMultipleChecks(sf::Vector2i kingPosition, PieceColor pc)
{
	std::vector<sf::Vector2i> attackers;
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (board[x][y] != nullptr && board[x][y]->getColor() != pc && board[x][y]->takes(sf::Vector2i(x, y), kingPosition, board))
				attackers.push_back(sf::Vector2i(x, y));
		}
	}
	
	if (attackers.size() > 1)
		return true;
	else
		return false;
}

bool Chessboard::checkIfCanBlockMate(sf::Vector2i kingPosition, sf::Vector2i attackerPosition, PieceColor pc)
{
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (board[x][y] != nullptr && board[x][y]->getColor() == pc && board[x][y]->getPiece() != PieceType::King)
			{
				if (board[x][y]->takes(sf::Vector2i(x, y), attackerPosition, board))
					return false;

				int dx = attackerPosition.x - kingPosition.x;
				int dy = attackerPosition.y - kingPosition.y;

				int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
				int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

				for (int i = kingPosition.x + stepX, j = kingPosition.y + stepY;
					(stepX == 0 || i != attackerPosition.x) && (stepY == 0 || j != attackerPosition.y);
					i += stepX, j += stepY)
				{
					if (board[x][y]->possibleMove(board, sf::Vector2i(x, y), sf::Vector2i(i, j)))
						return false;
				}
			}
		}
	}
	return true;
}

void Chessboard::drawLastMoves(sf::RenderWindow& window)
{
	sf::Font font;
	font.loadFromFile("..\\rsc\\Roboto-Regular.ttf");

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Black);

	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(600.f, 750.f));
	rectangle.setFillColor(sf::Color::White);
	rectangle.setPosition(1000.f, 50.f);

	window.draw(rectangle);

	int lineHeight = 15;
	int startIdx;

	if (moveHistory.size() % 2 == 0)
	{
		startIdx = (moveHistory.size() > 30) ? moveHistory.size() - 30 : 0;
	}
	else
	{
		startIdx = (moveHistory.size() > 30) ? moveHistory.size() - 30 + 1 : 0;
	}

	for (int i = startIdx, pos = 0; i < moveHistory.size(); i += 2, pos += 2)
	{
		text.setString(moveHistory[i]);
		text.setPosition(1020.f, 70.f + pos * lineHeight);
		window.draw(text);


		if (i + 1 < moveHistory.size())
		{
			text.setString(moveHistory[i + 1]);
			text.setPosition(1120.f, 70.f + pos * lineHeight);
			window.draw(text);
		}
	}
}