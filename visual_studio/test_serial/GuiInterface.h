#pragma once
#include <iostream>
#include <serial/serial.h>
#include <vector>
#include <string>
#include <shared_mutex>
#include <atomic>

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


};