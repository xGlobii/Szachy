#include "Piece.h"

std::string Piece::getStringColor(const PieceColor color)
{
	return (color == PieceColor::Black ? "Black" : "White");
}

Piece::Piece(PieceColor color, PieceType pType, bool specialMove) : color(color), pType(pType), specialMove(specialMove)
{
	std::string colorStr = (getStringColor(color));
	std::string typeStr = (getStringType(pType));

	texture.loadFromFile("..\\rsc\\images\\" + colorStr + typeStr + ".png");
}

Piece::~Piece() {}

void Piece::draw(sf::RenderWindow& window, const float size, sf::Vector2f position)
{
	float scaleX = size / texture.getSize().x;
	float scaleY = size / texture.getSize().y;

	sprite.setScale(0.1, 0.1);
	sprite.setTexture(texture);
	sprite.setPosition(position);
	window.draw(sprite);
}

bool Piece::move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if (possibleMove(board, piecePosition, boardPosition))
	{
		board[boardPosition.x][boardPosition.y] = std::move(board[piecePosition.x][piecePosition.y]);
		board[piecePosition.x][piecePosition.y] = nullptr;
		return true;
	}
	else
		return false;
}

std::string Piece::getStringType(const PieceType pType)
{
	switch (pType)
	{
	case PieceType::King:
		return "King";
		break;
	case PieceType::Queen:
		return "Queen";
		break;
	case PieceType::Rook:
		return "Rook";
		break;
	case PieceType::Bishop:
		return "Bishop";
		break;
	case PieceType::Knight:
		return "Knight";
		break;
	case PieceType::Pawn:
		return "Pawn";
		break;
	}
}

bool Piece::takes(sf::Vector2i piecePosition, sf::Vector2i boardPosition, std::vector<std::vector<Piece*>>& board)
{
	if (possibleMove(board, piecePosition, boardPosition))
		return true;
	else
		return false;
}

int Piece::getValue()
{
	return value;
}

PieceColor Piece::getColor()
{
	return color;
}

PieceType Piece::getPiece()
{
	return pType;
}

bool Piece::getSpecialMoveStatus()
{
	return specialMove;
}

void Piece::setSpecialMove(bool _specialMove)
{
	specialMove = _specialMove;
}

sf::Sprite Piece::getSprite() 
{
	return sprite;
}
