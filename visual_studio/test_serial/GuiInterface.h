#pragma once
#include <iostream>


class SerialBuffer;
class GuiInterface
{
public:
	GuiInterface();
	~GuiInterface();

	void Instance();
protected:
private:
	std::shared_ptr<SerialBuffer> Buffers = nullptr;
};