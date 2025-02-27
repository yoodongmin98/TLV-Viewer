//std
#include <iostream>
#include <functional>
#include <string>
#include <thread>


//lib
#include <serial/serial.h>
#include <OpenXLSX.hpp>

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


using namespace OpenXLSX;



GuiInterface::GuiInterface()
{
	R642s = std::make_shared<R642>();
	R7s = std::make_shared<R7>();
	ubpulses = std::make_shared<ubpulse>();


	//CallBack
	listener = std::thread(&GuiInterface::EventListener, this);

	this->R642s->GetParsingDatas()->Get_ubpulse_DataEvent() = std::bind(&GuiInterface::GetLastData, this);


	//EXCEL
	UbExcel.create("ubpulse.xlsx");
	UbSheet= UbExcel.workbook().sheet("Sheet1");
}
GuiInterface::~GuiInterface()
{
	running.store(false, std::memory_order_release);
	if (listener.joinable()) listener.join();
	if (UbExcel.isOpen())
	{
		UbExcel.save();
		UbExcel.close();
	}
}



void GuiInterface::Instance(ImGuiIO& io)
{
	SetBackGround(io);
	R642s->Instance();
	R7s->Instance();
	ubpulses->Instance();
}


void GuiInterface::EventListener()
{
	while (running.load(std::memory_order_acquire))
	{
		if (triggered.load(std::memory_order_acquire))
		{
			triggered.store(false, std::memory_order_release);

			std::string HighByte = "A" + std::to_string(Cells);
			std::string LowByte = "B" + std::to_string(Cells);
			UbSheet.cell(HighByte).value() = ubpulses->GetParsingDatas()->GetPacketStreamDataHighByte();
			UbSheet.cell(LowByte).value() = ubpulses->GetParsingDatas()->GetPacketStreamDataLowByte();
			Cells++;
		}
		std::this_thread::yield(); // CPU 점유 최소화
	}
}

void GuiInterface::SetBackGround(ImGuiIO& _io)
{
	if (ImGui::Button("Connect", ImVec2{ 150,20 }))
	{
		R642s->Connect();
		R7s->Connect();
	}
	ImGui::SameLine();
	if (ImGui::Button("DisConnect", ImVec2{ 150,20 }))
	{
		R642s->DisConnect();
		R7s->DisConnect();
		ubpulses->DisConnect();
	}
}

void GuiInterface::GetLastData()
{
	triggered.store(true, std::memory_order_release);
}