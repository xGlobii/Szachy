#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

class Queen : public Piece
{
public:
	Queen(const Color, const PieceType);
	~Queen();

	//bool move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
	bool possibleMove(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
	bool takes(sf::Vector2i piecePosition, sf::Vector2i boardPosition, std::vector<std::vector<Piece*>>& board) override;
};

#endif