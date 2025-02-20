#include <iostream>
#include "imgui.h"
#include "GuiInterface.h"

#include "MyImGui.h"
#include "ThreadPool.h"

#include <iomanip>    
#include <sstream>
#include <string>
#include <functional>
#include <thread>
#include <shared_mutex>
#include <serial/serial.h>
#include "R642.h"
#include "R7.h"


GuiInterface::GuiInterface()
{
	R642s = std::make_shared<R642>();
	R7s = std::make_shared<R7>();
}
GuiInterface::~GuiInterface()
{

}



void GuiInterface::Instance()
{
	SetBackGround();
	R642s->Instance();
	R7s->Instance();
	

}


void GuiInterface::SetBackGround()
{
	
}