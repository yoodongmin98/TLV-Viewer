#include <iostream>
#include "imgui.h"
#include "GuiInterface.h"
#include "SerialBuffer.h"
#include "MyImGui.h"
#include "ThreadPool.h"

#include <iomanip>    
#include <sstream>
#include <string>
#include <functional>
#include <thread>
#include <shared_mutex>
#include <serial/serial.h>




GuiInterface::GuiInterface()
{
	Buffers = std::make_shared<SerialBuffer>();
	std::thread serialThread(&GuiInterface::DataInput, this);
	serialThread.detach();
}
GuiInterface::~GuiInterface()
{
	stop = true;
}



void GuiInterface::Instance()
{
	SetBackGround();
	GetPortInfo();

	////////////////////////////////////////////Test
	if (!MySerial.isOpen())
	{
		MySerial.setPort("COM119");
		MySerial.setBaudrate(115200);
		MySerial.setBytesize(serial::bytesize_t::eightbits);
		MySerial.open();
	}
	std::cout << HexBuffer.size() << std::endl;
	////////////////////////////////////////////////////



	Buffers->Instance(HexBuffer);
}


void GuiInterface::SetBackGround()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(MyImGui::MyImGuis->GetWindowSize_X(), MyImGui::MyImGuis->GetWindowSize_Y()), ImGuiCond_Always);

	ImGui::Begin("TLV-Viewer", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
	ImGui::End();
}


void GuiInterface::GetPortInfo()
{
	PortInfos = serial::list_ports();
}


void GuiInterface::DataInput()
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