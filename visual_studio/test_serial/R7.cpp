#include <iostream>
#include "R7.h"
#include <imgui.h>
#include "MyImGui.h"
#include "MyTime.h"
#include "ThreadPool.h"

R7::R7()
{
	ModuleName = "R7";
}


R7::~R7()
{

}

void R7::Instance()
{
	SetPortInfo();
	SetBaudrate();
	R7Setting();
	DataView();
	UI();
	ImGui::PopStyleColor();
	ImGui::End();
}


void R7::SetBaudrate()
{
	if (AllPort.size() >= SelectPort)
		ComPort = std::string(AllPort[SelectPort]);
	Baudrate = 1843200;
	MagicNumber = { 2,1,4,3,6,5,8,7 };
}


void R7::R7Setting()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(41, 146), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(270, 310), ImGuiCond_Always);

	ImGui::Begin("A", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);


	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.6f, 0.90196f, 1.0f)); 
	//ImGui::PushFont(MyImGui::MyImGuis->GetLargeBoldFont());
	ImGui::Text("HRS-R7 [1843200]");
	ImGui::PopStyleColor();
	//ImGui::PopFont();



	ImGui::Combo("##input", &SelectPort, AllPort.data(), AllPort.size());
}


void R7::UI()
{
	
}

