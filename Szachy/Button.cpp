#include "Button.h"

Button::Button(int width, int height, sf::Vector2f position, sf::Color color, std::function<void()> onClick) : onClick(onClick)
{
	buttonBody.setSize(sf::Vector2f(width, height));
	buttonBody.setPosition(position);
	buttonBody.setFillColor(color);
}

void Button::draw(sf::RenderWindow& window)
{
	window.draw(buttonBody);
	window.draw(text);
}

void Button::setText(std::string text)
{
	font.loadFromFile("..\\rsc\\Roboto-Regular.ttf");

	this->text.setString(text);
	this->text.setCharacterSize(24);
	this->text.setFont(font);
	this->text.setFillColor(sf::Color::Black);

	sf::FloatRect textRect = this->text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	this->text.setPosition(buttonBody.getPosition().x + buttonBody.getSize().x / 2.0f, buttonBody.getPosition().y + buttonBody.getSize().y / 2.0f);
}

void Button::handleMouseClick(sf::Vector2f mousePosition)
{
	sf::FloatRect buttonBounds = buttonBody.getGlobalBounds();
	if (buttonBounds.contains(mousePosition))
		onClick();
}



InputBox::InputBox(sf::Vector2f size, sf::Vector2f position) : outlineColor(sf::Color::Black)
{
	box.setPosition(position);
	box.setSize(size);
	box.setFillColor(sf::Color::White);
	box.setOutlineThickness(2);
	box.setOutlineColor(sf::Color(166, 166, 166));

	font.loadFromFile("..\\rsc\\Roboto-Regular.ttf");
	text.setFont(font);
	text.setPosition(position.x + 5, position.y + 5);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Black);

	//textString = "test";
	//text.setString(textString);

	selected = false;
	correctName = false;
}

void InputBox::handleMouseClick(sf::Event& event) 
{
	if (event.type == sf::Event::MouseButtonPressed) 
	{
		sf::FloatRect bounds = box.getGlobalBounds();
		if (bounds.contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) 
		{
			selected = true;
			box.setOutlineColor(sf::Color::Blue);
		}
		else 
		{
			selected = false;
			box.setOutlineColor(outlineColor);
		}
	}
}

void InputBox::handleTextEntered(sf::Event& event)
{
	if (selected && event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode == '\b')
		{
			if (!textString.empty())
			{
				textString.pop_back();
			}
		}
		else if (event.text.unicode < 128)
		{
			textString += static_cast<char>(event.text.unicode);
		}
		text.setString(textString);
	}

	if (checkInput())
	{
		correctName = true;
	}
	else
	{
		correctName = false;
	}
}

bool InputBox::checkInput()
{
	std::regex nameRegex("[A-Z][a-z]+ [A-Z][a-z]+");
	if (std::regex_match(textString, nameRegex))
	{
		setOutlineColor(sf::Color::Green);
		return true;
	}
	else
	{
		setOutlineColor(sf::Color::Red);
		return false;
	}
}

void InputBox::draw(sf::RenderWindow& window)
{
	window.draw(box);
	window.draw(text);
}

void InputBox::setOutlineColor(sf::Color color) 
{
	outlineColor = color;
	box.setOutlineColor(outlineColor);
}

bool InputBox::getCorrectName()
{
	return correctName;
}