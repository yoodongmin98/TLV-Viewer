#include "R642.h"
#include <vector>
#include "imgui.h"
#include "MyImGui.h"
#include <string>
#include "MyTime.h"
#include <functional>
#include "ThreadPool.h"
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
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(324 , 146), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(270, 310), ImGuiCond_Always);

	ImGui::Begin("B", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);


	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.6f, 0.90196f, 1.0f));
	ImGui::Text("HRS-R642 [1843200]");
	ImGui::PopStyleColor(1);


	ImGui::Combo("##input", &SelectPort, AllPort.data(), AllPort.size());
	
}

void R642::UI()
{
	if (MySerial.isOpen())
	{
		std::function<void()> funcs = std::bind(&R642::DataParsing, this, ModuleName ,"0");
		ThreadPool::TP->AddWork(funcs);
	}
}