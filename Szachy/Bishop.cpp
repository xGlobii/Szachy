#include "Bishop.h"

Bishop::Bishop(const PieceColor _color, const PieceType _pType) : Piece(_color, _pType, false)
{
	value = 3;
}
Bishop::~Bishop() {}

bool Bishop::possibleMove(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	// SprawdŸ, czy ruch jest przek¹tny
	if (abs(piecePosition.x - boardPosition.x) != abs(piecePosition.y - boardPosition.y))
		return false;

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

	return true;
}


//bool Bishop::takes(sf::Vector2i piecePosition, sf::Vector2i boardPosition, std::vector<std::vector<Piece*>>& board)
//{
//	if (possibleMove(board, piecePosition, boardPosition))
//	{
//		board[boardPosition.x][boardPosition.y] = nullptr;
//		board[boardPosition.x][boardPosition.y] = board[piecePosition.x][piecePosition.y];
//		board[piecePosition.x][piecePosition.y] = nullptr;
//		return true;
//	}
//	else
//		return false;
//}