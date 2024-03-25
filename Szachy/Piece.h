#ifndef PAWN_H
#define PAWN_F

#include <SFML/Graphics.hpp>
#include <iostream>

enum class Color
{
	Black = -1,
	White = 1
};

enum class PieceType
{
	King = 6,
	Queen = 5,
	Rook = 4,
	Bishop = 3,
	Knight = 2,
	Pawn = 1
};

class Piece
{
protected:
	Color color;
	PieceType pType;
	sf::Vector2f position;
	sf::Texture texture;
	std::string colorStr;
	std::string typeStr;

private:
	std::string setColor(const Color color)
	{
		return color == Color::Black ? "Black" : "White";
	}

	std::string setPiece(const PieceType type)
	{
		switch (type)
		{
		case PieceType::King:
			return "King";
			break;
		case PieceType::Queen:
			return "Queen";
			break;
		case PieceType::Rook:
			return "Rook";
			break;
		case PieceType::Bishop:
			return "Bishop";
			break;
		case PieceType::Knight:
			return "Knight";
			break;
		case PieceType::Pawn:
			return "Pawn";
			break;
		}
	}


public:
	Piece(const int, const int);
	virtual ~Piece();

	virtual bool move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition) = 0;
	virtual void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
	virtual bool possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition) = 0;

	std::string getColor()
	{
		return color == Color::Black ? "Black" : "White";
	}
};

class King : public Piece
{
public:
	King(const int, const int);
	~King();

	bool canCastle;

	bool move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
	void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
	bool possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
};

class Queen : public Piece
{
public:
	Queen(const int, const int);
	~Queen();

	bool move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
	void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
	bool possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
};

class Rook : public Piece
{
public:
	Rook(const int, const int);
	~Rook();

	bool canCastle;

	bool move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
	void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
	bool possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
};

class Bishop : public Piece
{
public:
	Bishop(const int, const int);
	~Bishop();

	bool move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
	void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
	bool possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
};

class Knight : public Piece
{
public:
	Knight(const int, const int);
	~Knight();

	bool move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
	void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
	bool possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
};

class Pawn : public Piece
{
public:
	Pawn(const int, const int);
	~Pawn();

	bool firstMove;

	bool move(std::vector<std::vector<Piece*>>& board, sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
	void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
	bool possibleMove(sf::Vector2i piecePosition, sf::Vector2i boardPosition) override;
};

#endif