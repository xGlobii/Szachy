#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>
#include <mutex>
#include <SFML/Graphics.hpp>

class Timer
{
	long time;
	long actualTime;
	std::mutex mtx;

public:
	Timer();
	Timer(int);

	int getActualTime();

	void startTimer(std::atomic<bool>& stopFlag, std::atomic<bool>& pauseFlag);
	void updateTimer(sf::RenderWindow& window, std::atomic<bool>& stopFlag, sf::Vector2f position);
	void setTime(const int time);
};

#endif