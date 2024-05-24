#include <SFML/Graphics.hpp>

#include "Chessboard.h"

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

	sf::RenderWindow window(sf::VideoMode(800, 800), "Chess by Globii.", sf::Style::Default);
	window.setFramerateLimit(60);

	float tileSize = 100.f;

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