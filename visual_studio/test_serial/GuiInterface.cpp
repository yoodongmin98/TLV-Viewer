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

	this->ubpulses->Get_ubpulse_DataEvent() = std::bind(&GuiInterface::GetLastData, this);
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


void GuiInterface::SetBackGround(ImGuiIO& _io)
{
}

void GuiInterface::GetLastData()
{
	std::ostringstream oss;
	oss << "[ " << MyTime::Time->GetLocalDay() << " " << MyTime::Time->GetLocalTime() << " ]";
	std::string Time = oss.str();

	std::function<void()> boundFunction = std::bind(&ParsingData::CSV_WriteData,
		R642s->GetParsingDatas(), 
		R642s->GetModuleName(),    
		Time);
	std::function<void()> boundFunctions = std::bind(&ParsingData::CSV_WriteData,
		R7s->GetParsingDatas(),
		R7s->GetModuleName(),
		Time);
	ThreadPool::TP->AddWork(boundFunction);
	ThreadPool::TP->AddWork(boundFunctions);
	//R642s->GetParsingDatas()->CSV_WriteData(R642s->GetModuleName(), Time);
	//R7s->GetParsingDatas()->CSV_WriteData(R7s->GetModuleName(), Time);
	
	
}