#include <iostream>
#include "R7.h"
#include <imgui.h>
#include "MyImGui.h"


R7::R7()
{

}


R7::~R7()
{

}

void R7::Instance()
{
	SetPortInfo();
	SetBaudrate();
	R7Setting();
	UI();
}


void R7::SetBaudrate()
{
	if (AllPort.size() >= SelectPort)
		ComPort = std::string(AllPort[SelectPort]);
	Baudrate = 1843200;
}


void R7::R7Setting()
{
	ImGui::SetNextWindowPos(ImVec2(0, 250), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(495, 531), ImGuiCond_Always);
	ImGui::Begin("A", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
	ImGui::SeparatorText("HRS-R7 [1843200]");
	ImGui::Combo("SelectPort", &SelectPort, AllPort.data(), AllPort.size());
}


void R7::UI()
{
	if (ImGui::Button("Connect", ImVec2{ 150,20 }))
	{
		Connect();
	}
	if (MySerial.isOpen())
	{
		DataParsing();
	}
	ImGui::End();
}

