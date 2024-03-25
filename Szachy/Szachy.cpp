#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Piece.h"

class Chessboard
{
private:
	std::vector<std::vector<Piece*>> board;
	sf::Vector2i selectedPiecePosition = sf::Vector2i(-1, -1);

public:
	Chessboard()
	{
		board.resize(8, std::vector<Piece*>(8, nullptr));

		for (int i = 0; i < board.size(); i++)
		{
			board[i][1] = new Pawn(-1, 1);
			board[i][6] = new Pawn(1, 1);
		}

		//Black
		board[0][0] = new Rook(-1, 4);
		board[1][0] = new Knight(-1, 2);
		board[2][0] = new Bishop(-1, 3);
		board[3][0] = new Queen(-1, 5);
		board[4][0] = new King(-1, 6);
		board[5][0] = new Bishop(-1, 3);
		board[6][0] = new Knight(-1, 2);
		board[7][0] = new Rook(-1, 4);

		//White
		board[0][7] = new Rook(1, 4);
		board[1][7] = new Knight(1, 2);
		board[2][7] = new Bishop(1, 3);
		board[3][7] = new Queen(1, 5);
		board[4][7] = new King(1, 6);
		board[5][7] = new Bishop(1, 3);
		board[6][7] = new Knight(1, 2);
		board[7][7] = new Rook(1, 4);
	}

	~Chessboard()
	{
		for (int i = 0; i < board.size(); i++)
		{
			for (int j = 0; j < board.size(); j++)
			{
				delete board[i][j];
			}
		}
	}

public:
	void draw(sf::RenderWindow& window)
	{
		for (int i = 0; i < board.size(); i++)
		{
			for (int j = 0; j < board.size(); j++)
			{
				if (board[i][j] != nullptr)
				{
					board[i][j]->draw(window, 100, sf::Vector2f(i * 100, j * 100));
				}
			}
		}
	}

	void handleMouseClick(sf::RenderWindow& window)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
		int col = mousePosition.x / 100;
		int row = mousePosition.y / 100;

		if (board[col][row] != nullptr) 
		{
			selectedPiecePosition = sf::Vector2i(col, row);
			std::cout << "Clicked on piece at position: " << col << ", " << row << std::endl;
		}
		else 
		{
			if (selectedPiecePosition != sf::Vector2i(-1, -1)) 
			{
				board[col][row] = board[selectedPiecePosition.x][selectedPiecePosition.y];
				board[selectedPiecePosition.x][selectedPiecePosition.y] = nullptr;
				selectedPiecePosition = sf::Vector2i(-1, -1);
			}
		}
	}
};

void drawBoard(sf::RenderWindow& window, float tileSize)
{
	sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));

	sf::Color black(184, 135, 98);
	sf::Color white(237, 214, 176);

	int board[8][8] =
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

	for (int i = 0; i < 8; i++)
	{
		tile.setPosition(sf::Vector2f(0, tileSize * i));
		for (int j = 0; j < 8; j++)
		{
			tile.setPosition(sf::Vector2f(tileSize * j, tile.getPosition().y));
			if (board[i][j] % 2 == 0)
				tile.setFillColor(black);
			else
				tile.setFillColor(white);

			window.draw(tile);
		}
	}
}

int main()
{
	Chessboard board;

	sf::RenderWindow window(sf::VideoMode(800, 800), "Chess", sf::Style::Default);
	window.setFramerateLimit(60);

	float tileSize = 100.f;
	int index;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				board.handleMouseClick(window);
			}
		}

		//Update


		//Draw
		window.clear(sf::Color(102, 62, 15));
		drawBoard(window, tileSize);
		board.draw(window);
		window.display();
	}

	return 0;
}