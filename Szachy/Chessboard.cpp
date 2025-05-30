#include "Chessboard.h"

Chessboard::Chessboard() : whiteTurn(true), stopTimers(false), startTimer(true), checking(board), promoting(false)
{
	board.resize(8, std::vector<Piece*>(8, nullptr));

	movesWithoutTake = 0;
	tileSize = 100;
	whitePoints = 0;
	blackPoints = 0;

	checkmate = false;

	offset = sf::Vector2f(150, 150);
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

	if (promoting) 
	{
		handlePromotionClick(mousePosition);
		if (checking.isCheckmate(whiteTurn ? PieceColor::White : PieceColor::Black, promotionPosition))
		{
			if (whiteTurn)
				whiteTurn = false;
			else
				whiteTurn = true;
			std::cout << whiteTurn << std::endl;
			record.isCheckmate = true;
			stopTimers = true;
			std::string col = whiteTurn == true ? "White wins" : "Black wins";
			endGameType = "Checkmate\n";
			return;
		}
		if (checking.isKingInCheck(whiteTurn ? PieceColor::White : PieceColor::Black))
		{
			record.isCheck = checking.isKingInCheck(whiteTurn ? PieceColor::White : PieceColor::Black);
		}
		record.isPromoting = true;

		record.notation = generateAlgebricNotation(promotionPositionFrom, promotionPosition, board[promotionPosition.x][promotionPosition.y]->getPiece());
		moveHistory.push_back(record.notation);

		return;
	}
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
				startTimer = false;

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
		record.isCastle = true;
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

			if (!checking.isKingInCheck(whiteTurn ? PieceColor::White : PieceColor::Black))
			{
				movesWithoutTake++;

				if (board[to.x][to.y]->getPiece() == PieceType::Pawn && (to.y == 0 || to.y == 7)) {
					promoting = true;
					promotionPosition = to;
					promotionPositionFrom = selectedPiecePosition;
					return;
				}

				checking.updatePositionHistory(whiteTurn);
				if (checking.isDraw(whiteTurn ? PieceColor::Black : PieceColor::White, whiteTurn, movesWithoutTake))
					stopTimers = true;

				if (checking.isCheckmate(whiteTurn ? PieceColor::Black : PieceColor::White, to))
				{
					record.isCheckmate = true;
					stopTimers = true;
					std::string col = whiteTurn == true ? "White wins" : "Black wins";
					endGameType = "Checkmate\n" + col;
					return;
				}

				if (!checkmate)
					record.isCheck = checking.isKingInCheck(whiteTurn ? PieceColor::Black : PieceColor::White);
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

		if (!checking.isKingInCheck(whiteTurn ? PieceColor::White : PieceColor::Black))
		{
			if (whiteTurn)
				whitePoints += points;
			else
				blackPoints += points;

			record.isTaking = true;

			if (board[to.x][to.y]->getPiece() == PieceType::Pawn && (to.y == 0 || to.y == 7)) {
				promoting = true;
				promotionPosition = to;
				promotionPositionFrom = selectedPiecePosition;
				return;
			}

			if (checking.isCheckmate(whiteTurn ? PieceColor::Black : PieceColor::White, to))
			{
				record.isCheckmate;
				stopTimers = true;
				std::string col = whiteTurn == true ? "White wins" : "Black wins";
				endGameType = "Checkmate\n" + col;
				return;
			}

			checking.updatePositionHistory(whiteTurn);
			if (checking.isDraw(whiteTurn ? PieceColor::Black : PieceColor::White, whiteTurn, movesWithoutTake))
				stopTimers = true;

			if (!checkmate)
				record.isCheck = checking.isKingInCheck(whiteTurn ? PieceColor::Black : PieceColor::White);
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
		}
	}
	selectedPiecePosition = sf::Vector2i(-1, -1);
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
		bool kingInCheck = checking.isKingInCheck(whiteTurn ? PieceColor::White : PieceColor::Black);
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

	if (record.isCastle)
	{
		if (to.x == 2)
			notation = "O-O-O";
		else if (to.x == 6)
			notation = "O-O";
	}
	else if (record.isPromoting)
	{
		notation += columns[from.x];
		notation += std::to_string(8 - to.y);
		notation += "=";
		notation += pieceTypeToSymbol(pt);
		if (record.isCheck)
			notation += "+";
		else if (record.isCheckmate)
			notation += "#";
	}
	else
	{
		notation += pieceTypeToSymbol(pt);
		notation += columns[from.x];
		notation += std::to_string(8 - from.y);
		if (!record.isTaking)
			notation += "-";
		else
			notation += "x";
		notation += columns[to.x];
		notation += std::to_string(8 - to.y);

		if (record.isCheck)
			notation += "+";
		if (record.isCheckmate)
			notation += "#";
	}
	record.isCheck = false;
	record.isTaking = false;
	record.isCastle = false;
	record.isCheckmate = false;
	record.isPromoting = false;
	from = sf::Vector2i(-1, -1);
	to = sf::Vector2i(-1, -1);

	return notation;
}

std::string Chessboard::pieceTypeToSymbol(PieceType pt)
{
	switch (pt)
	{
	case PieceType::King: return "K"; break;
	case PieceType::Queen: return "Q"; break;
	case PieceType::Rook: return "R"; break;
	case PieceType::Bishop: return "B"; break;
	case PieceType::Knight: return "N"; break;
	case PieceType::Pawn: return ""; break;
	}
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
	rectangle.setPosition(1100, 100);
	rectangle.setOutlineColor(sf::Color::Black);
	rectangle.setOutlineThickness(3);

	window.draw(rectangle);

	int lineHeight = 15;
	int startIdx;

	if (moveHistory.size() % 2 == 0)
	{
		startIdx = (moveHistory.size() > 48) ? moveHistory.size() - 48 : 0;
	}
	else
	{
		startIdx = (moveHistory.size() > 48) ? moveHistory.size() - 48 + 1 : 0;
	}

	for (int i = startIdx, pos = 0; i < moveHistory.size(); i += 2, pos += 2)
	{
		text.setString(moveHistory[i]);
		text.setPosition(1250, 120 + pos * lineHeight);
		window.draw(text);


		if (i + 1 < moveHistory.size())
		{
			text.setString(moveHistory[i + 1]);
			text.setPosition(1450, 120 + pos * lineHeight);
			window.draw(text);
		}
	}
}

bool Chessboard::getGameStatus()
{
	return stopTimers;
}

std::string Chessboard::getEndGameType()
{
	return checking.getEndgametype();
}

void Chessboard::setGameStatus(bool status)
{
	stopTimers = status;
}

std::vector<std::string> Chessboard::getMoves()
{
	return moveHistory;
}

void Chessboard::setMoves(std::vector<std::string> moves)
{
	moveHistory = moves;
}

std::vector<std::string> Chessboard::getMovesHistory() 
{
	return movesHistory;
}

void Chessboard::promotePawn(sf::Vector2i position, sf::RenderWindow& window) {
	promotionPosition = position;
	promoting = true;
}

void Chessboard::drawPromotionIcons(sf::RenderWindow& window, sf::Vector2i position) {
	sf::RectangleShape box(sf::Vector2f(100, 400));
	int offs = whiteTurn ? 0 : -300;
	box.setPosition(position.x * 100 + offset.x, position.y * 100 + offset.y + offs);
	box.setFillColor(sf::Color(100, 100, 100, 200));
	window.draw(box);

	std::vector<sf::Texture> textures(4);
	if (whiteTurn)
	{
		textures[0].loadFromFile("..\\rsc\\images\\WhiteQueen.png");
		textures[1].loadFromFile("..\\rsc\\images\\WhiteRook.png");
		textures[2].loadFromFile("..\\rsc\\images\\WhiteBishop.png");
		textures[3].loadFromFile("..\\rsc\\images\\WhiteKnight.png");
	}
	else
	{
		textures[0].loadFromFile("..\\rsc\\images\\BlackQueen.png");
		textures[1].loadFromFile("..\\rsc\\images\\BlackRook.png");
		textures[2].loadFromFile("..\\rsc\\images\\BlackBishop.png");
		textures[3].loadFromFile("..\\rsc\\images\\BlackKnight.png");
	}

	std::vector<sf::Sprite> icons(4);
	for (int i = 0; i < 4; i++) {
		icons[i].setScale(0.1, 0.1);
		icons[i].setTexture(textures[i]);
		icons[i].setPosition(position.x * 100 + offset.x, position.y * 100 + offset.y + i * 100 + offs);
		window.draw(icons[i]);
	}
}

void Chessboard::handlePromotionClick(sf::Vector2i mousePosition) {
	int offs = whiteTurn ? 0 : -300;
	int x = promotionPosition.x * 100 + offset.x;
	int y = promotionPosition.y * 100 + offset.y + offs;
	int index = (mousePosition.y - y) / 100;
	if (mousePosition.x >= x && mousePosition.x <= x + 100 && index >= 0 && index < 4) {
		delete board[promotionPosition.x][promotionPosition.y];
		switch (index) {
		case 0:
			board[promotionPosition.x][promotionPosition.y] = new Queen(whiteTurn ? PieceColor::White : PieceColor::Black, PieceType::Queen);
			break;
		case 1:
			board[promotionPosition.x][promotionPosition.y] = new Rook(whiteTurn ? PieceColor::White : PieceColor::Black, PieceType::Rook);
			break;
		case 2:
			board[promotionPosition.x][promotionPosition.y] = new Bishop(whiteTurn ? PieceColor::White : PieceColor::Black, PieceType::Bishop);
			break;
		case 3:
			board[promotionPosition.x][promotionPosition.y] = new Knight(whiteTurn ? PieceColor::White : PieceColor::Black, PieceType::Knight);
			break;
		}
		promoting = false;
		if (whiteTurn)
			whiteTurn = false;
		else
			whiteTurn = true;
	}
}

bool Chessboard::getPromoting()
{
	return promoting;
}

sf::Vector2i Chessboard::getPromotionPosition()
{
	return promotionPosition;
}