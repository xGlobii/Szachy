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
	std::string getColor(const Color color)
	{
		return color == Color::Black ? "Black" : "White";
	}

	std::string getPiece(const PieceType type)
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

	virtual void move() const = 0;
	virtual void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
};

class King : public Piece
{
public:
	King(const int, const int);
	~King();

	void move() const override;
	void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
};

class Queen : public Piece
{
public:
	Queen(const int, const int);
	~Queen();

	void move() const override;
	void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
};

class Rook : public Piece
{
public:
	Rook(const int, const int);
	~Rook();

	void move() const override;
	void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
};

class Bishop : public Piece
{
public:
	Bishop(const int, const int);
	~Bishop();

	void move() const override;
	void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
};

class Knight : public Piece
{
public:
	Knight(const int, const int);
	~Knight();

	void move() const override;
	void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
};

class Pawn : public Piece
{
public:
	Pawn(const int, const int);
	~Pawn();

	void move() const override;
	void draw(sf::RenderWindow& window, const float size, sf::Vector2f position);
};

#endif