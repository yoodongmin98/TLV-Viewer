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
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.6f, 0.5f, 0.9f, 0.5f));
	ImGui::SetNextWindowPos(ImVec2((MyImGui::MyImGuis->GetWindowSize_X() / 3) * 2, 250), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(495, 531), ImGuiCond_Always);
	ImGui::Begin("C", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
	ImGui::SeparatorText("ubpulse [115200]");
	ImGui::Combo("SelectPort", &SelectPort, AllPort.data(), AllPort.size());
}


void ubpulse::UI()
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
		CallbackTrigger();
		//std::cout << "trigger호출된 시간 : " << MyTime::Time->GetLocalTime() << std::endl;
	}
}


