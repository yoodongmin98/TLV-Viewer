#include <iostream>

#include "imgui.h"
#include "GuiInterface.h"
#include "SerialBuffer.h"







GuiInterface::GuiInterface()
{
	Buffers = std::make_shared<SerialBuffer>();
}
GuiInterface::~GuiInterface()
{

}



void GuiInterface::Instance()
{
	Buffers->Instance();

	ImGui::Begin("TestFrame");
	ImGui::End();
}