#ifndef PIECE_H
#define PIECE_H

#include <SFML/Graphics.hpp>
#include <iostream>

enum class Color { Black, White };

enum class PieceType { King, Queen, Rook, Bishop, Knight, Pawn };

class Piece
{
protected:
	Color color;
	PieceType pType;
	sf::Vector2f position;
	sf::Texture texture;
	int value;

private:
	std::string getStringType(const PieceType pType);
	std::string getStringColor(const Color color);

public:
	Piece(const Color, const PieceType);
	virtual ~Piece();


	bool move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition);
	void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
	virtual bool possibleMove(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition) = 0;
	virtual bool takes(sf::Vector2i piecePosition, sf::Vector2i boardPosition, std::vector<std::vector<Piece*>>& board) = 0;

	Color getColor()
	{
		return color;
	}

	PieceType getPiece()
	{
		return pType;
	}
};

#endif