#pragma once
#include <iostream>
#include <serial/serial.h>
#include <vector>
#include <string>
#include <mutex>

class SerialBuffer;
class GuiInterface
{
public:
	GuiInterface();
	~GuiInterface();

	void Instance();
protected:
	void SetBackGround();
	void GetPortInfo();
	void DataInput();
private:
	std::shared_ptr<SerialBuffer> Buffers = nullptr;

	serial::Serial MySerial;
	std::vector<serial::PortInfo> PortInfos;
	std::vector<std::string> HexBuffer;
	std::mutex HexBufferMutex;
};