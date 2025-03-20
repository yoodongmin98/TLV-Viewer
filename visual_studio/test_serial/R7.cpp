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
	ImGui::PushFont(MyImGui::MyImGuis->GetLargeBoldFont());
	ImVec2 cursorPos = ImGui::GetCursorPos();
	cursorPos.x += 17;
	cursorPos.y += 18;
	ImGui::SetCursorPos(cursorPos);
	ImGui::Text("HRS-R7 [1843200]");
	ImGui::PopStyleColor();
	ImGui::PopFont();


	cursorPos.x -= 5;
	cursorPos.y += 34;
	ImGui::SetCursorPos(cursorPos);
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 new_bg_color = ImVec4(0.941f, 0.941f, 0.941f, 1.0f); 
	ImVec4 text_color = ImVec4(0.467, 0.467f, 0.467f, 1.0f); 
	ImGui::PushStyleColor(ImGuiCol_Text, text_color);
	ImGui::PushStyleColor(ImGuiCol_PopupBg, new_bg_color);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, new_bg_color);
	ImGui::PushFont(MyImGui::MyImGuis->MyImGuis->GetComboFont());
	ImGui::PushItemWidth(236.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20.0f, 7.0f));
	ImGui::PushStyleColor(ImGuiCol_Button, new_bg_color);
	ImGui::Combo("##input", &SelectPort, AllPort.data(), AllPort.size());
	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(4);
	ImGui::PopItemWidth();
	ImGui::PopFont();
}


void R7::UI()
{
	
}

