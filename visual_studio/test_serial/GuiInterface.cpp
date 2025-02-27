//std
#include <iostream>
#include <functional>
#include <string>
#include <thread>


//lib
#include <serial/serial.h>

//My
#include "GuiInterface.h"
#include "imgui.h"
#include "MyImGui.h"
#include "ThreadPool.h"
#include "ParsingData.h"
#include "R642.h"
#include "R7.h"
#include "ubpulse.h"
#include "MyTime.h"
#include "ThreadPool.h"


GuiInterface::GuiInterface()
{
	R642s = std::make_shared<R642>();
	R7s = std::make_shared<R7>();
	ubpulses = std::make_shared<ubpulse>();

	this->R642s->GetParsingDatas()->Get_ubpulse_DataEvent() = std::bind(&GuiInterface::GetLastData, this);
}
GuiInterface::~GuiInterface()
{

}



void GuiInterface::Instance(ImGuiIO& io)
{
	SetBackGround(io);
	R642s->Instance();
	R7s->Instance();
	ubpulses->Instance();
}


void EventListener()
{
	while (true)
	{
		if (triggered.load(std::memory_order_acquire))
		{
			std::cout << "트리거 실행시간 : " << MyTime::Time->GetLocalTime() << std::endl;
			triggered.store(false, std::memory_order_release);
		}
		std::this_thread::yield(); // CPU 점유 최소화
	}
}

void GuiInterface::SetBackGround(ImGuiIO& _io)
{
	if (ImGui::Button("Connect", ImVec2{ 150,20 }))
	{
		R642s->Connect();
		//R7s->Connect();
	}
	ImGui::SameLine();
	if (ImGui::Button("DisConnect", ImVec2{ 150,20 }))
	{
		R642s->DisConnect();
		//R7s->DisConnect();
	}
}

void GuiInterface::GetLastData()
{
	int a = ubpulses->GetParsingDatas()->GetPacketStreamDataHighByte();
	int b = ubpulses->GetParsingDatas()->GetPacketStreamDataLowByte();
	std::cout <<"트리거 실행시간 : "<< MyTime::Time->GetLocalTime() << " high : " << a << " Low : " << b << std::endl;
}