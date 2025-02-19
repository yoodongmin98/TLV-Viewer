#include "Module.h"
#include "DetectHeader.h"
#include "ParsingData.h"

#include <thread>
#include <shared_mutex>
#include <serial/serial.h>





Module::Module()
	: serialThread(&Module::DataInput, this)
{
	DetectHeaders = std::make_shared<DetectHeader>();
	ParsingDatas = std::make_shared<ParsingData>();
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
	//Debug
	static int Countsss = 0;
	//

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
			Countsss++;
		}

		//Debug
		std::cout <<"읽은 데이터 개수"<< Countsss << std::endl;
		//
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


bool Module::Connect()
{
	if (!MySerial.isOpen())
	{
		MySerial.setPort(ComPort);
		MySerial.setBaudrate(Baudrate);
		MySerial.setTimeout(timeout);
		MySerial.open();
		return true;
	}
	return false;
}


void Module::DataParsing()
{
	DetectHeaders->FindHeader(HexBuffer);
	ParsingDatas->DataParsing(HexBuffer);
	
}




