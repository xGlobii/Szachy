#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "Piece.h"

class Chessboard
{
private:
	std::vector<std::vector<Piece*>> board;
	sf::Vector2i selectedPiecePosition = sf::Vector2i(-1, -1);

	bool whiteTurn = true;

public:
	Chessboard();
	~Chessboard();

public:
	void draw(sf::RenderWindow& window);
	void handleMouseClick(sf::RenderWindow& window);
};

#endif