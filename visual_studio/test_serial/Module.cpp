#include "Module.h"


#include <thread>
#include <shared_mutex>
#include <serial/serial.h>





Module::Module()
{
	std::thread serialThread(&Module::DataInput, this);
	serialThread.detach();
}
Module::~Module()
{
	stop = true;
}


void Module::SetBaudrate()
{

}

void Module::DataInput()
{
	while (!stop)
	{
		if (MySerial.available())
		{
			std::string hexs = MySerial.read();
			{
				std::lock_guard<std::shared_mutex> lock(HexBufferMutex);
				for (unsigned char c : hexs)
					HexBuffer.push_back(static_cast<int>(c));
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(0));
		}
	}
}

void Module::SetPortInfo()
{
	std::vector<serial::PortInfo> PortInfos = serial::list_ports();
	for (serial::PortInfo& V : PortInfos)
	{
		AllPort.emplace_back(V.port.c_str());
	}
}

void Module::Connect()
{
	if (!MySerial.isOpen())
	{
		MySerial.setPort(ComPort);
		MySerial.setBaudrate(Baudrate);
		MySerial.open();
	}
}