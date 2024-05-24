#include "King.h"

King::King(const Color _color, const PieceType _pType) : Piece(_color, _pType), canCastle(true)
{
	value = 90;
}
King::~King() {}

bool King::possibleMove(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	return withinRange(piecePosition, boardPosition, 1);
}

bool King::takes(sf::Vector2i piecePosition, sf::Vector2i boardPosition, std::vector<std::vector<Piece*>>& board)
{
	if (possibleMove(board, piecePosition, boardPosition))
	{
		board[boardPosition.x][boardPosition.y] = nullptr;
		board[boardPosition.x][boardPosition.y] = board[piecePosition.x][piecePosition.y];
		board[piecePosition.x][piecePosition.y] = nullptr;
		return true;
	}
	else
		return false;
}

bool withinRange(sf::Vector2i piecePosition, sf::Vector2i boardPosition, int range)
{
	return abs(piecePosition.x - boardPosition.x) <= range && abs(piecePosition.y - boardPosition.y) <= range;
}