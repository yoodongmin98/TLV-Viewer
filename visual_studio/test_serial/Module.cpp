#include "Module.h"


#include <thread>
#include <shared_mutex>
#include <serial/serial.h>





Module::Module()
	: serialThread(&Module::DataInput, this)
{

}
Module::~Module()
{
	stop = true;
	if (serialThread.joinable()) 
		serialThread.join();
	if (MySerial.isOpen())
		MySerial.close();
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
	PortInfos = serial::list_ports();
	AllPort.clear();
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
		MySerial.setTimeout(timeout);
		MySerial.open();
	}
}