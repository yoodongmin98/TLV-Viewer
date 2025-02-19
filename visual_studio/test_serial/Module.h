#pragma once
#include <iostream>
#include <serial/serial.h>
#include <shared_mutex>
#include <atomic>
#include <vector>
#include <string>


class Module
{
public:
	Module();
	~Module();
	virtual void SetBaudrate() = 0;

protected:
	unsigned int  Baudrate = 0;
	std::string ComPort;
	serial::Serial MySerial;
	std::vector<const char*> AllPort;
	std::vector<int> HexBuffer;

	int SelectPort = 0;
	void SetPortInfo();
	void Connect();
private:
	void DataInput();
	std::shared_mutex HexBufferMutex;
	std::atomic<bool> stop = false;
};