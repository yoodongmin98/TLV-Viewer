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



void GuiInterface::Instance()
{
	SetBackGround();

	R642s->Instance();
	R7s->Instance();
	ubpulses->Instance();
}


void GuiInterface::SetBackGround()
{
	
}

void GuiInterface::GetLastData()
{
	R642s->GetParsingDatas()->CSV_WriteData(R642s->GetModuleName());
	R7s->GetParsingDatas()->CSV_WriteData(R7s->GetModuleName());
}