#include "Rook.h"

Rook::Rook(const Color _color, const PieceType _pType) : Piece(_color, _pType), canCastle(true)
{
	value = 5;
}
Rook::~Rook() {}

bool Rook::possibleMove(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if (piecePosition.x != boardPosition.x && piecePosition.y != boardPosition.y)			//Ruch nie jest po linii prostej
		return false;

	if (piecePosition.x == boardPosition.x)
	{
		int maxY = std::max(piecePosition.y, boardPosition.y);
		int minY = std::min(piecePosition.y, boardPosition.y);

		for (int i = minY + 1; i < maxY; i++)
		{
			if (board[piecePosition.x][i] != nullptr)
				return false;
		}
	}
	else if (piecePosition.y == boardPosition.y)
	{
		int maxX = std::max(piecePosition.x, boardPosition.x);
		int minY = std::min(piecePosition.x, boardPosition.x);

		for (int i = minY + 1; i < maxX; i++)
		{
			if (board[i][piecePosition.y] != nullptr)
				return false;
		}
	}
	return true;
}

bool Rook::takes(sf::Vector2i piecePosition, sf::Vector2i boardPosition, std::vector<std::vector<Piece*>>& board)
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