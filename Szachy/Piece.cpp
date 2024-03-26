#include "Piece.h"

#pragma region Piece
Piece::Piece(const int _color, const int _pType)
{
	color = static_cast<Color>(_color);
	pType = static_cast<PieceType>(_pType);


	colorStr = setColor(color);
	typeStr = setPiece(pType);

	texture.loadFromFile("C:\\Users\\Kamil\\Desktop\\Rozne\\Programy\\PK4 Projekt\\Szachy\\images\\" + colorStr + typeStr + ".png");
}

Piece::~Piece() {}

void Piece::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	sf::Sprite piece;

	float scaleX = size / texture.getSize().x;
	float scaleY = size / texture.getSize().y;

	piece.setScale(0.1, 0.1);
	piece.setTexture(texture);
	piece.setPosition(position);
	window.draw(piece);
}

#pragma endregion

#pragma region King
King::King(const int _color, const int _pType) : Piece(_color, _pType)
{
	canCastle = true;
}
King::~King() {}

bool King::move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if (possibleMove(piecePosition, boardPosition))
	{
		board[boardPosition.x][boardPosition.y] = board[piecePosition.x][piecePosition.y];
		board[piecePosition.x][piecePosition.y] = nullptr;
		return true;
	}
	else
		return false;
}

bool King::possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if((abs(piecePosition.x - boardPosition.x) >= 0 && abs(piecePosition.x - boardPosition.x) < 2) && (abs(piecePosition.y - boardPosition.y) >= 0 && abs(piecePosition.y - boardPosition.y) < 2))
	{
		canCastle = false;
		return true;
	}
	else
		return false;
}

void King::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

#pragma endregion

#pragma region Queen
Queen::Queen(const int _color, const int _pType) : Piece(_color, _pType) {}
Queen::~Queen() {}

bool Queen::move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if (possibleMove(piecePosition, boardPosition))
	{
		board[boardPosition.x][boardPosition.y] = board[piecePosition.x][piecePosition.y];
		board[piecePosition.x][piecePosition.y] = nullptr;
		return true;
	}
	else
		return false;
}

void Queen::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

bool Queen::possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if ((((abs(piecePosition.x - boardPosition.x) == 0) && abs(piecePosition.y - boardPosition.y) > 0) || (abs(piecePosition.x - boardPosition.x) > 0 && abs(piecePosition.y - boardPosition.y) == 0)) || abs(piecePosition.x - boardPosition.x) == abs(piecePosition.y - boardPosition.y))
	{
		return true;
	}
	else
		return false;
}

#pragma endregion

#pragma region Rook
Rook::Rook(const int _color, const int _pType) : Piece(_color, _pType)
{
	canCastle = true;
}
Rook::~Rook() {}

bool Rook::move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if (possibleMove(piecePosition, boardPosition))
	{
		board[boardPosition.x][boardPosition.y] = board[piecePosition.x][piecePosition.y];
		board[piecePosition.x][piecePosition.y] = nullptr;
		return true;
	}
	else
		return false;
}

void Rook::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

bool Rook::possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if (((abs(piecePosition.x - boardPosition.x) == 0) && abs(piecePosition.y - boardPosition.y) > 0) || (abs(piecePosition.x - boardPosition.x) > 0 && abs(piecePosition.y - boardPosition.y) == 0))
	{
		canCastle = false;
		return true;
	}
	else
		return false;
}

#pragma endregion

#pragma region Bishop
Bishop::Bishop(const int _color, const int _pType) : Piece(_color, _pType) {}
Bishop::~Bishop() {}

bool Bishop::move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if (possibleMove(piecePosition, boardPosition))
	{
		board[boardPosition.x][boardPosition.y] = board[piecePosition.x][piecePosition.y];
		board[piecePosition.x][piecePosition.y] = nullptr;
		return true;
	}
	else
		return false;
}

void Bishop::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

bool Bishop::possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if (abs(piecePosition.x - boardPosition.x) == abs(piecePosition.y - boardPosition.y))
	{
		return true;
	}
	else
		return false;
}
#pragma endregion

#pragma region Knight
Knight::Knight(const int _color, const int _pType) : Piece(_color, _pType) {}
Knight::~Knight() {}

bool Knight::move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if (possibleMove(piecePosition, boardPosition))
	{
		board[boardPosition.x][boardPosition.y] = board[piecePosition.x][piecePosition.y];
		board[piecePosition.x][piecePosition.y] = nullptr;
		return true;
	}
	else
		return false;
}

void Knight::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

bool Knight::possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if ((abs(piecePosition.x - boardPosition.x) == 2 && abs(piecePosition.y - boardPosition.y) == 1) || (abs(piecePosition.x - boardPosition.x) == 1 && abs(piecePosition.y - boardPosition.y) == 2))
	{
		return true;
	}
	else
		return false;
}

#pragma endregion

#pragma region Pawn
Pawn::Pawn(const int _color, const int _pType) : Piece(_color, _pType)
{
	firstMove = true;
}
Pawn::~Pawn() {}

bool Pawn::move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if (possibleMove(piecePosition, boardPosition))
	{
		board[boardPosition.x][boardPosition.y] = board[piecePosition.x][piecePosition.y];
		board[piecePosition.x][piecePosition.y] = nullptr;
		return true;
	}
	else
		return false;
}

void Pawn::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

bool Pawn::possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if (abs(piecePosition.x - boardPosition.x) == 0)
	{
		if ((firstMove && abs(piecePosition.y - boardPosition.y) == 2) || (abs(piecePosition.y - boardPosition.y) == 1))
		{
			firstMove = false;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

#pragma endregion