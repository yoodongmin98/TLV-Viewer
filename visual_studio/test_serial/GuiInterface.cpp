//std
#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include <filesystem>

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
#include "CSV.h"


using namespace OpenXLSX;



GuiInterface::GuiInterface()
{
	R642s = std::make_shared<R642>();
	R7s = std::make_shared<R7>();
	ubpulses = std::make_shared<ubpulse>();


	//CallBack
	listener = std::thread(&GuiInterface::EventListener, this);
	Rlistener = std::thread(&GuiInterface::REventListener, this);

	this->R642s->GetParsingDatas()->Get_ubpulse_DataEvent() = std::bind(&GuiInterface::GetLastData, this);


	//EXCEL
	UbExcel.create("ubpulse.xlsx");
	UbSheet= UbExcel.workbook().sheet("Sheet1");
}
GuiInterface::~GuiInterface()
{
	running.store(false, std::memory_order_release);
	if (listener.joinable()) listener.join();
	if (Rlistener.joinable()) Rlistener.join();
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
		if (triggered.exchange(false, std::memory_order_acq_rel))
		{
			std::string Time = "A" + std::to_string(Cells);
			std::string HighByte = "B" + std::to_string(Cells);
			std::string LowByte = "C" + std::to_string(Cells);
			std::string Results = "D" + std::to_string(Cells);
			UbSheet.cell(Time).value() = MyTime::Time->GetLocalTime();
			UbSheet.cell(HighByte).value() = ubpulses->GetParsingDatas()->GetPacketStreamDataHighByte();
			UbSheet.cell(LowByte).value() = ubpulses->GetParsingDatas()->GetPacketStreamDataLowByte();
			UbSheet.cell(Results).value() = ubpulses->GetParsingDatas()->GetPacketStreamDataHighByte() * 256 + ubpulses->GetParsingDatas()->GetPacketStreamDataLowByte();
			Cells++;
			triggered.store(false, std::memory_order_release);
		}
		std::this_thread::yield();
	}
}


void GuiInterface::REventListener()
{
	while (running.load(std::memory_order_acquire))
	{
		if (R7triggered.exchange(false, std::memory_order_acq_rel))
		{
			R7s->DataParsing("R7",MyTime::Time->GetLocalTime());
			R7triggered.store(false, std::memory_order_release);
		}
		std::this_thread::yield(); 
	}
}

void GuiInterface::SetBackGround(ImGuiIO& _io)
{
	

	SetClock();
	/*ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(495, 250), ImGuiCond_Always);
	ImGui::Begin("DEBUG", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);


	if (ImGui::Button("START", ImVec2{ 150,50 }))
	{
		R642s->Connect();
		R7s->Connect();
		ubpulses->Connect();
	}
	ImGui::SameLine();
	if (ImGui::Button("DisConnect & Save", ImVec2{ 150,50 }))
	{
		
		if (R7s->GetSerial().isOpen())
			R7s->GetParsingDatas()->R7Save();
		if(R642s->GetSerial().isOpen())
			R642s->GetParsingDatas()->GetCSVs()->SaveFile();

		R642s->DisConnect();
		R7s->DisConnect();
		ubpulses->DisConnect();
		
		if (UbExcel.isOpen())
		{
			UbExcel.save();
			UbExcel.close();
		}
	}
	ImGui::End();*/
}

void GuiInterface::GetLastData()
{
	if(ubpulses->GetSerial().isOpen())
		triggered.store(true, std::memory_order_release);
	if (R7s->GetSerial().isOpen())
		R7triggered.store(true, std::memory_order_release);
}


std::string GuiInterface::SaveFileDialog()
{
	OPENFILENAME ofn;
	char szFile[260] = { 0 };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = "txt";

	if (GetSaveFileName(&ofn))
		std::wcout << "파일 저장 경로: " << szFile << std::endl;
	else
		std::wcout << "파일 저장 취소됨" << std::endl;

	std::string SZFILE(szFile);

	return SZFILE;
}



void GuiInterface::SetClock()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(41, 38), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(833, 65), ImGuiCond_Always);
	ImGui::Begin("##input", nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	ImGui::End();
	ImGui::PopStyleColor();
}