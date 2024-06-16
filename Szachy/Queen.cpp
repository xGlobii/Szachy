#include "Queen.h"

Queen::Queen(const PieceColor _color, const PieceType _pType) : Piece(_color, _pType, false)
{
	value = 9;
}
Queen::~Queen() {}

bool Queen::possibleMove(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	if ((piecePosition.x != boardPosition.x) && (piecePosition.y != boardPosition.y) && (abs(piecePosition.x - boardPosition.x) != abs(piecePosition.y - boardPosition.y)))
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
		int minX = std::min(piecePosition.x, boardPosition.x);

		for (int i = minX + 1; i < maxX; i++)
		{
			if (board[i][piecePosition.y] != nullptr)
				return false;
		}
	}
	else if (abs(piecePosition.x - boardPosition.x) == abs(piecePosition.y - boardPosition.y))
	{
		int dx = (boardPosition.x - piecePosition.x) > 0 ? 1 : -1; // Kierunek ruchu w poziomie
		int dy = (boardPosition.y - piecePosition.y) > 0 ? 1 : -1; // Kierunek ruchu w pionie

		int x = piecePosition.x + dx;
		int y = piecePosition.y + dy;

		while (x != boardPosition.x && y != boardPosition.y)
		{
			if (board[x][y] != nullptr)
				return false; // Na drodze goñca znajduje siê inna figura

			x += dx;
			y += dy;
		}
	}
	return true;
}