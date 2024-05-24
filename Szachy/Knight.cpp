#include "Knight.h"

Knight::Knight(const Color _color, const PieceType _pType) : Piece(_color, _pType)
{
	value = 3;
}
Knight::~Knight() {}

bool Knight::possibleMove(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	return ((abs(piecePosition.x - boardPosition.x) == 2 && abs(piecePosition.y - boardPosition.y) == 1) || (abs(piecePosition.x - boardPosition.x) == 1 && abs(piecePosition.y - boardPosition.y) == 2));
}

bool Knight::takes(sf::Vector2i piecePosition, sf::Vector2i boardPosition, std::vector<std::vector<Piece*>>& board)
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