#include "ubpulse.h"


#include "imgui.h"
#include "MyImGui.h"
#include "MyTime.h"


ubpulse::ubpulse()
{
	ModuleName = "ubpulse";
}


ubpulse::~ubpulse()
{

}


void ubpulse::Instance()
{
	SetPortInfo();
	SetBaudrate();
	ubpulseSetting();
	PulseDataView();
	UI();
	ImGui::PopStyleColor();
	ImGui::End();
}


void ubpulse::SetBaudrate()
{
	if (AllPort.size() >= SelectPort)
		ComPort = std::string(AllPort[SelectPort]);
	Baudrate = 115200;
	MagicNumber = { 255,254 };
}


void ubpulse::ubpulseSetting()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(607,146), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(270, 310), ImGuiCond_Always);
	ImGui::Begin("C", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);


	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.6f, 0.90196f, 1.0f));
	ImGui::Text("ubpulse [115200]");
	ImGui::PopStyleColor(1);


	ImGui::Combo("##input", &SelectPort, AllPort.data(), AllPort.size());
}


void ubpulse::UI()
{
	if (MySerial.isOpen())
	{
		DataParsing(ModuleName,"0");
	}
}


