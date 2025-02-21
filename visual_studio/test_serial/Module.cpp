#include "Module.h"
#include "DetectHeader.h"
#include "ParsingData.h"

#include <thread>
#include <shared_mutex>
#include <serial/serial.h>
#include "imgui.h"





Module::Module()
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
	HexBuffer.clear();
}


void Module::SetBaudrate()
{

}

void Module::DataInput()
{
	while (!stop)
	{
		try
		{
			if (MySerial.isOpen() && MySerial.available())
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
		catch (...)
		{
			stop = true;
			MySerial.close();
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


bool Module::Connect()
{
	try
	{
		if (!MySerial.isOpen())
		{
			MySerial.setPort(ComPort);
			MySerial.setBaudrate(Baudrate);
			MySerial.setTimeout(timeout);
			MySerial.open();
			stop = false;
			if (serialThread.joinable()) 
				serialThread.join();  // 또는 detach();
			serialThread = std::thread(&Module::DataInput, this);
			return true;
		}
	}
	catch (...)
	{
		std::cout << "포트를 열 수 없습니다." << std::endl;
	}
	return false;
}


void Module::DisConnect()
{
	if (MySerial.isOpen())
	{
		stop = true;
		if (serialThread.joinable())
			serialThread.join();
		MySerial.close();
	}
}


void Module::DataParsing(std::string _Name)
{
	std::vector<int> MagicNumber = { 2,1,4,3,6,5,8,7 };
	if (DetectHeaders->FindHeader(HexBuffer, MagicNumber))
		ParsingDatas->DataParsing(HexBuffer,_Name);
}


void Module::DataView()
{
	ImGui::Text("Version = %s", ParsingDatas->GetVersion().c_str());
	ImGui::Text("TotalPacketLength = %d", ParsingDatas->GetTotalPacketLength());
	ImGui::Text("Platform = %d", ParsingDatas->GetPlatform());
	ImGui::Text("FrameNumber = %d", ParsingDatas->GetFrameNumber());
	ImGui::Text("Time = %d", ParsingDatas->GetTime());
	ImGui::Text("NumberofDetectedObjects = %d", ParsingDatas->GetNumberofDetectedObjects());
	ImGui::Text("NumberofTLVs = %d", ParsingDatas->GetNumberofTLVs());
	ImGui::Text("SubframeNumber = %d", ParsingDatas->GetSubframeNumber());
	ImGui::Text("TLVType = %d", ParsingDatas->GetTLVType());
	ImGui::Text("TLVLength = %d", ParsingDatas->GetTLVLength());
}


