#include "R642.h"
#include <vector>
#include "imgui.h"
#include "MyImGui.h"


R642::R642()
{
	
}
R642::~R642()
{

}


void R642::Instance()
{
	SetPortInfo();
	SetBaudrate();
	R642Setting();
}


void R642::SetBaudrate()
{
	ComPort = AllPort[SelectPort];
	Baudrate = 1843200;
}


void R642::R642Setting()
{
	ImGui::SetNextWindowPos(ImVec2((MyImGui::MyImGuis->GetWindowSize_X() / 3), 250), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(495, 531), ImGuiCond_Always);
	ImGui::Begin("B", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
	ImGui::SeparatorText("HRS-R642 [1843200]");
	ImGui::Combo("SelectPort", &SelectPort, AllPort.data(), AllPort.size()); //포트번호가 왜깨질까/
	if (ImGui::Button("TestConnect"))
	{
		Connect();
	}
	ImGui::End();
}