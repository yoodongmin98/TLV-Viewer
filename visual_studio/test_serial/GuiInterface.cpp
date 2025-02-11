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
	GetPortInfo();
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(1475, 775), ImGuiCond_Always);
	ImGui::Begin("TLV-Viewer", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
	ImGui::End();

}

void GuiInterface::GetPortInfo()
{
	PortInfos = serial::list_ports();
}