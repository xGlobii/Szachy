#include "King.h"

King::King(const PieceColor _color, const PieceType _pType) : Piece(_color, _pType, true)
{
	value = 90;
}
King::~King() {}

bool King::possibleMove(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	//if (canCastle && (boardPosition.x == 2 || boardPosition.x == 6))
	//{
	//	if (piecePosition.y == boardPosition.y)
	//	{
	//		int maxX = std::max(piecePosition.x, boardPosition.x);
	//		int minX = std::min(piecePosition.x, boardPosition.x);

	//		for (int i = minX + 1; i < maxX; i++)
	//		{
	//			if (board[i][piecePosition.y] != nullptr)
	//				return false;
	//		}
	//		canCastle = false;
	//		return true;
	//	}
	//	else
	//		return false;
	//}
	return withinRange(piecePosition, boardPosition, 1);
}

bool withinRange(sf::Vector2i piecePosition, sf::Vector2i boardPosition, int range)
{
	return abs(piecePosition.x - boardPosition.x) <= range && abs(piecePosition.y - boardPosition.y) <= range;
}


//TODO:
// - Konstruktor kopiuj¹cy, ¿eby móc sprawdziæ czy wie¿¹ mo¿e roszowaæ
// - Roszowanie
// - Szachowanie
// - Matowanie
//