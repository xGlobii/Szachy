#include "Knight.h"

Knight::Knight(const PieceColor _color, const PieceType _pType) : Piece(_color, _pType, false)
{
	value = 3;
}
Knight::~Knight() {}

bool Knight::possibleMove(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition)
{
	return ((abs(piecePosition.x - boardPosition.x) == 2 && abs(piecePosition.y - boardPosition.y) == 1) || (abs(piecePosition.x - boardPosition.x) == 1 && abs(piecePosition.y - boardPosition.y) == 2));
}