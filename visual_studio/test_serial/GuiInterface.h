#pragma once
#include <iostream>
#include <serial/serial.h>
#include <vector>
#include <string>
#include <shared_mutex>
#include <atomic>

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

	void R7Setting();
	void R642Setting();
	void ubpulseSetting();
private:
	std::shared_ptr<SerialBuffer> Buffers = nullptr;

	serial::Serial MySerial;
	std::vector<serial::PortInfo> PortInfos;

	std::vector<int> HexBuffer;
	std::shared_mutex HexBufferMutex;
	std::atomic<bool> stop = false;
};