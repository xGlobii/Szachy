#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece
{
public:
	King(const PieceColor, const PieceType);
	~King();

	bool possibleMove(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
};

bool withinRange(sf::Vector2i piecePosition, sf::Vector2i boardPosition, int range);

#endif