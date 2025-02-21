#include "R642.h"
#include <vector>
#include "imgui.h"
#include "MyImGui.h"
#include <string>

R642::R642()
{
	ModuleName = "R642";
}
R642::~R642()
{

}


void R642::Instance()
{
	SetPortInfo();
	SetBaudrate();
	R642Setting();
	DataView();
	UI();
	ImGui::PopStyleColor();
	ImGui::End();
}


void R642::SetBaudrate()
{
	
	if(AllPort.size()>=SelectPort)
		ComPort = std::string(AllPort[SelectPort]);
	Baudrate = 1843200;
	MagicNumber = { 2,1,4,3,6,5,8,7 };
}


void R642::R642Setting()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.5f, 0.8f, 0.5f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2((MyImGui::MyImGuis->GetWindowSize_X() / 3), 250), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(495, 531), ImGuiCond_Always);
	ImGui::Begin("B", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
	ImGui::SeparatorText("HRS-R642 [1843200]");
	ImGui::Combo("SelectPort", &SelectPort, AllPort.data(), AllPort.size());
	
}

void R642::UI()
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
	}
	
}

