#include "Timer.h"

Timer::Timer()
{
	time = 600;
	actualTime = 600;
}

Timer::Timer(int _time) : time(_time), actualTime(_time) {}

void Timer::startTimer(std::atomic<bool>& stopFlag, std::atomic<bool>& pauseFlag) 
{
	auto lastUpdateTime = std::chrono::high_resolution_clock::now();

	while (!stopFlag) 
	{
		if (!pauseFlag && actualTime > 0) 
		{
			auto now = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsedTime = now - lastUpdateTime;

			if (elapsedTime.count() >= 1.0) 
			{
				{
					std::lock_guard<std::mutex> lock(mtx);
					actualTime -= static_cast<int>(elapsedTime.count());
					if (actualTime < 0) actualTime = 0;
				}
				lastUpdateTime = now;
			}
		}
		else 
		{
			lastUpdateTime = std::chrono::high_resolution_clock::now();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}

void Timer::updateTimer(sf::RenderWindow& window, std::atomic<bool>& stopFlag, sf::Vector2f position)
{
	sf::RectangleShape timePlace;
	sf::Text timeText;
	sf::Font font;

	timePlace.setSize(sf::Vector2f(100, 50));
	timePlace.setFillColor(sf::Color::White);
	timePlace.setPosition(sf::Vector2f(position));

	font.loadFromFile("..\\rsc\\Roboto-Regular.ttf");

	timeText.setFont(font);
	timeText.setCharacterSize(24);
	timeText.setFillColor(sf::Color::Black);

	while (!stopFlag)
	{
		timeText.setString(actualTime / 60 + ":" + actualTime % 60);
		sf::FloatRect textRect = timeText.getLocalBounds();
		timeText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		timeText.setPosition(timeText.getPosition().x + timePlace.getSize().x / 2.0f, timePlace.getPosition().y + timePlace.getSize().y / 2.0f);

		window.draw(timeText);
	}
}

int Timer::getActualTime()
{
	return actualTime;
}

void Timer::setTime(const int time)
{
	actualTime = time;
}