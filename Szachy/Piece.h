#ifndef PIECE_H
#define PIECE_H

#include <SFML/Graphics.hpp>
#include <iostream>



enum class PieceColor { Black, White };

enum class PieceType { King, Queen, Rook, Bishop, Knight, Pawn };

class Piece
{
protected:
	PieceColor color;
	PieceType pType;
	sf::Vector2f position;
	sf::Texture texture;
	sf::Sprite sprite;
	int value;
	bool specialMove;

private:
	std::string getStringType(const PieceType pType);
	std::string getStringColor(const PieceColor color);

public:
	Piece(const PieceColor, const PieceType, const bool specialMove);
	virtual ~Piece();


	bool move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition);
	void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
	virtual bool possibleMove(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition) = 0;
	virtual bool takes(sf::Vector2i piecePosition, sf::Vector2i boardPosition, std::vector<std::vector<Piece*>>& board);
	int getValue();

	PieceColor getColor();
	PieceType getPiece();

	bool getSpecialMoveStatus();
	void setSpecialMove(bool specialMove);

	sf::Sprite getSprite();

};

#endif