#include "Module.h"
#include "DetectHeader.h"
#include "ParsingData.h"
#include "MyImGui.h"
#include "MyDefine.h"

#include <thread>
#include <shared_mutex>
#include <serial/serial.h>
#include "imgui.h"
#include "MyTime.h"
#include <CSV.h>




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
		AllPortInfo.emplace_back(V.description.c_str());
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
			ParsingDatas->GetCSVs()->SetIsCreate();
			return true;
		}
	}
	catch (...)
	{
		std::cout << ComPort << " 포트를 열 수 없습니다." << std::endl;
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
		ParsingDatas->GetCSVs()->SetIsCreate();
	}
}


void Module::DataParsingStart(std::string _Name , std::string Time)
{
	std::lock_guard<std::shared_mutex> lock(HexBufferMutex);
	if (DetectHeaders->FindHeader(HexBuffer, MagicNumber))
	{
		ParsingDatas->DataParsing(HexBuffer,_Name , Time);
	}
}


void Module::DataView()
{
	ImVec2 cursorPos = ImGui::GetCursorPos();
	cursorPos.x += HEADERSPACING; cursorPos.y += 16;
	ImGui::SetCursorPos(cursorPos);
	//ImGui::PushFont(MyImGui::MyImGuis->GetUnderTextFont());
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4667f, 0.4667f, 0.4667f, 1.0f));
	ImGui::Text("Version = %s", ParsingDatas->GetVersion().c_str());
	cursorPos.y += HEADERSPACING; ImGui::SetCursorPos(cursorPos);
	ImGui::Text("TotalPacketLength = %d", ParsingDatas->GetTotalPacketLength());
	cursorPos.y += HEADERSPACING; ImGui::SetCursorPos(cursorPos);
	ImGui::Text("Platform = %d", ParsingDatas->GetPlatform());
	cursorPos.y += HEADERSPACING; ImGui::SetCursorPos(cursorPos);
	ImGui::Text("FrameNumber = %d", ParsingDatas->GetFrameNumber());
	cursorPos.y += HEADERSPACING; ImGui::SetCursorPos(cursorPos);
	ImGui::Text("Time = %d", ParsingDatas->GetTime());
	cursorPos.y += HEADERSPACING; ImGui::SetCursorPos(cursorPos);
	ImGui::Text("NumberofDetectedObjects = %d", ParsingDatas->GetNumberofDetectedObjects());
	cursorPos.y += HEADERSPACING; ImGui::SetCursorPos(cursorPos);
	ImGui::Text("NumberofTLVs = %d", ParsingDatas->GetNumberofTLVs());
	cursorPos.y += HEADERSPACING; ImGui::SetCursorPos(cursorPos);
	ImGui::Text("SubframeNumber = %d", ParsingDatas->GetSubframeNumber());
	cursorPos.y += HEADERSPACING; ImGui::SetCursorPos(cursorPos);
	ImGui::Text("TLVType = %d", ParsingDatas->GetTLVType());
	cursorPos.y += HEADERSPACING; ImGui::SetCursorPos(cursorPos);
	ImGui::Text("TLVLength = %d", ParsingDatas->GetTLVLength());
	ImGui::PopStyleColor();
	//ImGui::PopFont();
}

void Module::PulseDataView()
{
	ImVec2 cursorPos = ImGui::GetCursorPos();
	cursorPos.x += HEADERSPACING; cursorPos.y += 16;
	ImGui::SetCursorPos(cursorPos);
	//ImGui::PushFont(MyImGui::MyImGuis->GetUnderTextFont());
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4667f, 0.4667f, 0.4667f, 1.0f));
	ImGui::Text("PacketUnitData0 = %d", ParsingDatas->GetPacketUnitData0());
	cursorPos.y += HEADERSPACING; ImGui::SetCursorPos(cursorPos);
	ImGui::Text("PacketCount = %d", ParsingDatas->GetPacketCount());
	cursorPos.y += HEADERSPACING; ImGui::SetCursorPos(cursorPos);
	ImGui::Text("PacketUnitData1 = %d", ParsingDatas->GetPacketUnitData1());
	cursorPos.y += HEADERSPACING; ImGui::SetCursorPos(cursorPos);
	ImGui::Text("PacketCyclicData = %d", ParsingDatas->GetPacketCyclicData());
	cursorPos.y += HEADERSPACING; ImGui::SetCursorPos(cursorPos);
	ImGui::Text("PacketStreamDataHighByte = %d", ParsingDatas->GetPacketStreamDataHighByte());
	cursorPos.y += HEADERSPACING; ImGui::SetCursorPos(cursorPos);
	ImGui::Text("PacketStreamDataLowByte = %d", ParsingDatas->GetPacketStreamDataLowByte());
	
	ImGui::PopStyleColor();
	//ImGui::PopFont();
}


