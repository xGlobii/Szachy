#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <regex>

class Button
{
	sf::RectangleShape buttonBody;
	sf::Text text;
	sf::Font font;

	std::function<void()> onClick;

public:
	Button() = default;
	Button(int width, int height, sf::Vector2f position, sf::Color color, std::function<void()> onClick);

	void draw(sf::RenderWindow& window);
	void setText(std::string text);
	void handleMouseClick(sf::Vector2f mousePosition);
};

class InputBox
{
	sf::RectangleShape box;
	sf::Text text;
	std::string textString;
	sf::Font font;
	sf::Color outlineColor;

	bool selected;
	bool correctName;

public:
	InputBox() = default;
	InputBox(sf::Vector2f size, sf::Vector2f position);
	
	void handleMouseClick(sf::Event& event);
	void handleTextEntered(sf::Event& event);
	void draw(sf::RenderWindow& window);
	void setOutlineColor(sf::Color color);

	bool checkInput();
	bool getCorrectName();
};

#endif