#include "Pawn.h"

Pawn::Pawn(const PieceColor _color, const PieceType _pType) : Piece(_color, _pType, true)
{
	value = 1;
}
Pawn::~Pawn() {}

bool Pawn::possibleMove(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	int sign = (color == PieceColor::White ? 1 : -1);
	if (piecePosition.x == boardPosition.x)
	{
		if ((((specialMove && (piecePosition.y - boardPosition.y == 2 * sign)) || (piecePosition.y - boardPosition.y == 1 * sign))))
		{
			specialMove = false;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool Pawn::takes(sf::Vector2i piecePosition, sf::Vector2i boardPosition, std::vector<std::vector<Piece*>>& board)
{
	if (canTake(piecePosition, boardPosition))
		return true;
	else
		return false;
}

bool Pawn::canTake(sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	int sign = (color == PieceColor::White ? 1 : -1);
	return ((abs(piecePosition.x - boardPosition.x) == 1 && (piecePosition.y - boardPosition.y == 1 * sign)));
}