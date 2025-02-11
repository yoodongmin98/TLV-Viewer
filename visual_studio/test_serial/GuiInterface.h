#pragma once
#include <iostream>
#include <serial/serial.h>
#include <vector>

class SerialBuffer;
class GuiInterface
{
public:
	GuiInterface();
	~GuiInterface();

	void Instance();
protected:
	void GetPortInfo();
private:
	std::shared_ptr<SerialBuffer> Buffers = nullptr;

	serial::Serial MySerial;

	std::vector<serial::PortInfo> PortInfos;
};