#pragma once
#include <iostream>
#include <serial/serial.h>
#include <vector>
#include <string>
#include <shared_mutex>
#include <atomic>



class ubpulse;
class R7;
class R642;
class GuiInterface
{
public:
	GuiInterface();
	~GuiInterface();

	void Instance();
protected:
	void SetBackGround();


private:
	std::shared_ptr<R642> R642s = nullptr;
	std::shared_ptr<R7> R7s = nullptr;
	std::shared_ptr<ubpulse> ubpulses = nullptr;

};