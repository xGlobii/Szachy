#include "Pawn.h"

Pawn::Pawn(const Color _color, const PieceType _pType) : Piece(_color, _pType), firstMove(true)
{
	value = 1;
}
Pawn::~Pawn() {}

bool Pawn::possibleMove(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	int sign = (color == Color::White ? 1 : -1);
	if (piecePosition.x == boardPosition.x)
	{
		if ((((firstMove && (piecePosition.y - boardPosition.y == 2 * sign)) || (piecePosition.y - boardPosition.y == 1 * sign))))
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

bool Pawn::takes(sf::Vector2i piecePosition, sf::Vector2i boardPosition, std::vector<std::vector<Piece*>>& board)
{
	if (canTake(piecePosition, boardPosition))
	{
		board[boardPosition.x][boardPosition.y] = nullptr;
		board[boardPosition.x][boardPosition.y] = board[piecePosition.x][piecePosition.y];
		board[piecePosition.x][piecePosition.y] = nullptr;
		return true;
	}
	else
		return false;
}

bool Pawn::canTake(sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	int sign = (color == Color::White ? 1 : -1);
	return ((abs(piecePosition.x - boardPosition.x) == 1 && (piecePosition.y - boardPosition.y == 1 * sign)));
}