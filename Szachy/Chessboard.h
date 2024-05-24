#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "Piece.h"
#include "King.h"
#include "Queen.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "Pawn.h"

class Chessboard
{
private:
	std::vector<std::vector<Piece*>> board;
	sf::Vector2i selectedPiecePosition = sf::Vector2i(-1, -1);

	int tileSize = 100;
	int chessboard[8][8] =
	{
		{1,0,1,0,1,0,1,0},
		{0,1,0,1,0,1,0,1},
		{1,0,1,0,1,0,1,0},
		{0,1,0,1,0,1,0,1},
		{1,0,1,0,1,0,1,0},
		{0,1,0,1,0,1,0,1},
		{1,0,1,0,1,0,1,0},
		{0,1,0,1,0,1,0,1}
	};

	bool whiteTurn = true;

public:
	Chessboard();
	~Chessboard();

public:
	void draw(sf::RenderWindow& window);
	void handleMouseClick(sf::RenderWindow& window);
};

#endif