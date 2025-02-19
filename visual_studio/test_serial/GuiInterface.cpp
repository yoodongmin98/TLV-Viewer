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



GuiInterface::GuiInterface()
{
	R642s = std::make_shared<R642>();
}
GuiInterface::~GuiInterface()
{

}



void GuiInterface::Instance()
{
	SetBackGround();
	R642s->Instance();
	

}


void GuiInterface::SetBackGround()
{
	
}







//void GuiInterface::R7Setting()
//{
//	ImGuiStyle& style = ImGui::GetStyle();
//	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);  // 다크 그레이 색상
//	style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  // 빨간색 테두리
//
//	static int SelectPort1 = 0;
//	std::vector<const char*> R7Port;
//	ImGui::SetNextWindowPos(ImVec2(0 , 250), ImGuiCond_Always);
//	ImGui::SetNextWindowSize(ImVec2(495, 531), ImGuiCond_Always);
//	ImGui::Begin("A", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
//	ImGui::SeparatorText("HRS-R7 [1843200]");
//	for (serial::PortInfo& V : PortInfos)
//		R7Port.push_back(V.port.c_str());
//	ImGui::Combo("SelectPort", &SelectPort1, R7Port.data(), R7Port.size());
//
//	ImGui::End();
//}
//
//
//
//
//void GuiInterface::ubpulseSetting()
//{
//	static int SelectPort3 = 0;
//	std::vector<const char*> pulsePort;
//	ImGui::SetNextWindowPos(ImVec2((MyImGui::MyImGuis->GetWindowSize_X() / 3)*2, 250), ImGuiCond_Always);
//	ImGui::SetNextWindowSize(ImVec2(495, 531), ImGuiCond_Always);
//	ImGui::Begin("C" , nullptr , ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
//	ImGui::SeparatorText("ubpulse [115200]");
//	ImGui::Combo("SelectPort", &SelectPort3, AllPort.data(), pulsePort.size());
//
//	ImGui::End();
//}