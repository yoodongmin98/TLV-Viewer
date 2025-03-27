#pragma once
#include <iostream>
#include <string>
#include <chrono>



class MyTime
{
public:
	MyTime();
	~MyTime();
	static MyTime* Time;
	std::string GetLocalTime();
	std::string GetLocalDay();
	int GetInterval();

protected:
private:
	__time64_t now2;
	tm tm_2;

	std::chrono::steady_clock::time_point lastTriggerTime;
};
