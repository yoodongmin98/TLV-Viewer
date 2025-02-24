#include <iostream>
#include "R7.h"
#include <imgui.h>
#include "MyImGui.h"


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
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.4f, 0.6f, 0.9f, 1.0f));
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
	ImGui::SameLine();
	if (ImGui::Button("DisConnect", ImVec2{ 150,20 }))
	{
		DisConnect();
	}
	if (MySerial.isOpen())
	{
		DataParsing(ModuleName);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.3f, 0.4f, 0.5f));
		ImGui::BeginChild("Data Processing Overview", ImVec2(0, 0), true);
		ImGui::Text("Recorded Data Count = %d", 1);
		ImGui::EndChild();
		ImGui::PopStyleColor();
	}
}

