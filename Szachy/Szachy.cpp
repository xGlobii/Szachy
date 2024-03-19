#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Chess", sf::Style::Default);
	window.setFramerateLimit(60);

	sf::Color black(184, 135, 98);
	sf::Color white(237, 214, 176);

	float tileSize = 100.f;
	sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
	sf::RectangleShape piece(sf::Vector2f(tileSize, tileSize));

	std::vector<sf::Texture> textures;

	//Textures
	sf::Texture texture1;
	sf::Texture texture2;
	sf::Texture texture3;
	sf::Texture texture4;
	sf::Texture texture5;
	sf::Texture texture6;
	sf::Texture texture7;
	sf::Texture texture8;
	sf::Texture texture9;
	sf::Texture texture10;
	sf::Texture texture11;
	sf::Texture texture12;
	texture1.loadFromFile("C:\\Users\\Kamil\\Desktop\\Rozne\\Programy\\PK4 Projekt\\Szachy\\images\\BlackBishop.png");
	texture1.setSmooth(true);
	texture2.loadFromFile("C:\\Users\\Kamil\\Desktop\\Rozne\\Programy\\PK4 Projekt\\Szachy\\images\\BlackKing.png");
	texture2.setSmooth(true);
	texture3.loadFromFile("C:\\Users\\Kamil\\Desktop\\Rozne\\Programy\\PK4 Projekt\\Szachy\\images\\BlackQueen.png");
	texture3.setSmooth(true);
	texture4.loadFromFile("C:\\Users\\Kamil\\Desktop\\Rozne\\Programy\\PK4 Projekt\\Szachy\\images\\BlackRook.png");
	texture4.setSmooth(true);
	texture5.loadFromFile("C:\\Users\\Kamil\\Desktop\\Rozne\\Programy\\PK4 Projekt\\Szachy\\images\\BlackKnight.png");
	texture5.setSmooth(true);
	texture6.loadFromFile("C:\\Users\\Kamil\\Desktop\\Rozne\\Programy\\PK4 Projekt\\Szachy\\images\\BlackPawn.png");
	texture6.setSmooth(true);
	texture7.loadFromFile("C:\\Users\\Kamil\\Desktop\\Rozne\\Programy\\PK4 Projekt\\Szachy\\images\\WhiteBishop.png");
	texture7.setSmooth(true);
	texture8.loadFromFile("C:\\Users\\Kamil\\Desktop\\Rozne\\Programy\\PK4 Projekt\\Szachy\\images\\WhiteKing.png");
	texture8.setSmooth(true);
	texture9.loadFromFile("C:\\Users\\Kamil\\Desktop\\Rozne\\Programy\\PK4 Projekt\\Szachy\\images\\WhiteQueen.png");
	texture9.setSmooth(true);
	texture10.loadFromFile("C:\\Users\\Kamil\\Desktop\\Rozne\\Programy\\PK4 Projekt\\Szachy\\images\\WhiteRook.png");
	texture10.setSmooth(true);
	texture11.loadFromFile("C:\\Users\\Kamil\\Desktop\\Rozne\\Programy\\PK4 Projekt\\Szachy\\images\\WhiteKnight.png");
	texture11.setSmooth(true);
	texture12.loadFromFile("C:\\Users\\Kamil\\Desktop\\Rozne\\Programy\\PK4 Projekt\\Szachy\\images\\WhitePawn.png");
	texture12.setSmooth(true);

	sf::RectangleShape bBishop(sf::Vector2f(tileSize, tileSize));
	sf::RectangleShape bKing(sf::Vector2f(tileSize, tileSize));
	sf::RectangleShape bQueen(sf::Vector2f(tileSize, tileSize));
	sf::RectangleShape bRook(sf::Vector2f(tileSize, tileSize));
	sf::RectangleShape bKnight(sf::Vector2f(tileSize, tileSize));
	sf::RectangleShape bPawn(sf::Vector2f(tileSize, tileSize));
	sf::RectangleShape wBishop(sf::Vector2f(tileSize, tileSize));
	sf::RectangleShape wKing(sf::Vector2f(tileSize, tileSize));
	sf::RectangleShape wQueen(sf::Vector2f(tileSize, tileSize));
	sf::RectangleShape wRook(sf::Vector2f(tileSize, tileSize));
	sf::RectangleShape wKnight(sf::Vector2f(tileSize, tileSize));
	sf::RectangleShape wPawn(sf::Vector2f(tileSize, tileSize));
	bBishop.setTexture(&texture1);
	bKing.setTexture(&texture2);
	bQueen.setTexture(&texture3);
	bRook.setTexture(&texture4);
	bKnight.setTexture(&texture5);
	bPawn.setTexture(&texture6);
	wBishop.setTexture(&texture7);
	wKing.setTexture(&texture8);
	wQueen.setTexture(&texture9);
	wRook.setTexture(&texture10);
	wKnight.setTexture(&texture11);
	wPawn.setTexture(&texture12);

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

	int ch[8][8] =
	{
		{-5,-4,-3,-8,-9,-3,-4,-5},
		{-1,-1,-1,-1,-1,-1,-1,-1},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1},
		{5,4,3,8,9,3,4,5}
	};

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && sf::Keyboard::Escape)
				window.close();
		}

		//Update
		
		//Draw
		window.clear(sf::Color(102, 62, 15));
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

		for (int i = 0; i < 8; i++)
		{
			tile.setPosition(sf::Vector2f(0, tileSize * i));
			for (int j = 0; j < 8; j++)
			{
				tile.setPosition(sf::Vector2f(tileSize * j, tile.getPosition().y));
				if (ch[i][j] == -5)
				{
					bRook.setPosition(sf::Vector2f(100 * j, 100 * i));
					window.draw(bRook);
				}
				else if (ch[i][j] == -4)
				{
					bKnight.setPosition(sf::Vector2f(100 * j, 100 * i));
					window.draw(bKnight);
				}
				else if (ch[i][j] == -3)
				{
					bBishop.setPosition(sf::Vector2f(100 * j, 100 * i));
					window.draw(bBishop);
				}
				else if (ch[i][j] == -8)
				{
					bQueen.setPosition(sf::Vector2f(100 * j, 100 * i));
					window.draw(bQueen);
				}
				else if (ch[i][j] == -9)
				{
					bKing.setPosition(sf::Vector2f(100 * j, 100 * i));
					window.draw(bKing);
				}
				else if (ch[i][j] == -1)
				{
					bPawn.setPosition(sf::Vector2f(100 * j, 100 * i));
					window.draw(bPawn);
				}
				else if (ch[i][j] == 5)
				{
					wRook.setPosition(sf::Vector2f(100 * j, 100 * i));
					window.draw(wRook);
				}
				else if (ch[i][j] == 4)
				{
					wKnight.setPosition(sf::Vector2f(100 * j, 100 * i));
					window.draw(wKnight);
				}
				else if (ch[i][j] == 3)
				{
					wBishop.setPosition(sf::Vector2f(100 * j, 100 * i));
					window.draw(wBishop);
				}
				else if (ch[i][j] == 8)
				{
					wQueen.setPosition(sf::Vector2f(100 * j, 100 * i));
					window.draw(wQueen);
				}
				else if (ch[i][j] == 9)
				{
					wKing.setPosition(sf::Vector2f(100 * j, 100 * i));
					window.draw(wKing);
				}
				else if (ch[i][j] == 1)
				{
					wPawn.setPosition(sf::Vector2f(100 * j, 100 * i));
					window.draw(wPawn);
				}
			}
		}
		
		window.display();
	}

	return 0;
}