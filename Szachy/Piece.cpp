#include "Piece.h"

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

King::King(const int _color, const int _pType) : Piece(_color, _pType)
{
	canCastle = true;
}
King::~King() {}

bool King::move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	return true;
}

bool King::possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	return true;
}

void King::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

Queen::Queen(const int _color, const int _pType) : Piece(_color, _pType) {}
Queen::~Queen() {}

bool Queen::move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	return true;
}

void Queen::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

bool Queen::possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	return true;
}

Rook::Rook(const int _color, const int _pType) : Piece(_color, _pType)
{
	canCastle = true;
}
Rook::~Rook() {}

bool Rook::move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	return true;
}

void Rook::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

bool Rook::possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	return true;
}

Bishop::Bishop(const int _color, const int _pType) : Piece(_color, _pType) {}
Bishop::~Bishop() {}

bool Bishop::move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	return true;
}

void Bishop::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

bool Bishop::possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	return true;
}

Knight::Knight(const int _color, const int _pType) : Piece(_color, _pType) {}
Knight::~Knight() {}

bool Knight::move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	return true;
}

void Knight::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	Piece::draw(window, size, position);
}

bool Knight::possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	return true;
}

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
}