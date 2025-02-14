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

	R7Setting();
	R642Setting();
	ubpulseSetting();
	
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
	/*ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(MyImGui::MyImGuis->GetWindowSize_X(), MyImGui::MyImGuis->GetWindowSize_Y()), ImGuiCond_Always);

	ImGui::Begin("TLV-Viewer", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
	ImGui::End();*/
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

void GuiInterface::R7Setting()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);  // 다크 그레이 색상
	style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  // 빨간색 테두리

	static int SelectPort1 = 0;
	std::vector<const char*> R7Port;
	ImGui::SetNextWindowPos(ImVec2(0 , 250), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(495, 531), ImGuiCond_Always);
	ImGui::Begin("A", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
	ImGui::SeparatorText("HRS-R7 [1843200]");
	for (serial::PortInfo& V : PortInfos)
		R7Port.push_back(V.port.c_str());
	ImGui::Combo("SelectPort", &SelectPort1, R7Port.data(), R7Port.size());

	ImGui::End();
	
}


void GuiInterface::R642Setting()
{
	static int SelectPort2 = 0;
	std::vector<const char*> R642Port;
	ImGui::SetNextWindowPos(ImVec2((MyImGui::MyImGuis->GetWindowSize_X()/3), 250), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(495, 531), ImGuiCond_Always);
	ImGui::Begin("B", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
	ImGui::SeparatorText("HRS-R642 [1843200]");
	for (serial::PortInfo& V : PortInfos)
		R642Port.push_back(V.port.c_str());
	ImGui::Combo("SelectPort", &SelectPort2, R642Port.data(), R642Port.size());
	
	ImGui::End();
}


void GuiInterface::ubpulseSetting()
{
	static int SelectPort3 = 0;
	std::vector<const char*> pulsePort;
	ImGui::SetNextWindowPos(ImVec2((MyImGui::MyImGuis->GetWindowSize_X() / 3)*2, 250), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(495, 531), ImGuiCond_Always);
	ImGui::Begin("C" , nullptr , ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
	ImGui::SeparatorText("ubpulse [115200]");
	for (serial::PortInfo& V : PortInfos)
		pulsePort.push_back(V.port.c_str());
	ImGui::Combo("SelectPort", &SelectPort3, pulsePort.data(), pulsePort.size());

	ImGui::End();
}