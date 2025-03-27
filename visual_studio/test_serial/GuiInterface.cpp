//std
#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include <filesystem>

//lib
#include <serial/serial.h>
#include <OpenXLSX.hpp>

//My
#include "GuiInterface.h"
#include "imgui.h"
#include "MyImGui.h"
#include "ThreadPool.h"
#include "ParsingData.h"
#include "R642.h"
#include "R7.h"
#include "ubpulse.h"
#include "MyTime.h"
#include "ThreadPool.h"
#include "CSV.h"


using namespace OpenXLSX;



GuiInterface::GuiInterface()
{
	R642s = std::make_shared<R642>();
	R7s = std::make_shared<R7>();
	ubpulses = std::make_shared<ubpulse>();


	//CallBack
	listener = std::thread(&GuiInterface::EventListener, this);
	Rlistener = std::thread(&GuiInterface::REventListener, this);

	this->R642s->GetParsingDatas()->Get_ubpulse_DataEvent() = std::bind(&GuiInterface::GetLastData, this);


	//EXCEL
	UbExcel.create("ubpulse.xlsx");
	UbSheet= UbExcel.workbook().sheet("Sheet1");
}
GuiInterface::~GuiInterface()
{
	running.store(false, std::memory_order_release);
	if (listener.joinable()) listener.join();
	if (Rlistener.joinable()) Rlistener.join();
}



void GuiInterface::Instance(ImGuiIO& io)
{
	SetBackGround(io);
	R642s->Instance();
	R7s->Instance();
	ubpulses->Instance();
}


void GuiInterface::EventListener()
{
	while (running.load(std::memory_order_acquire))
	{
		if (triggered.exchange(false, std::memory_order_acq_rel))
		{
			std::string Time = "A" + std::to_string(Cells);
			std::string HighByte = "B" + std::to_string(Cells);
			std::string LowByte = "C" + std::to_string(Cells);
			std::string Results = "D" + std::to_string(Cells);
			UbSheet.cell(Time).value() = MyTime::Time->GetLocalTime();
			UbSheet.cell(HighByte).value() = ubpulses->GetParsingDatas()->GetPacketStreamDataHighByte();
			UbSheet.cell(LowByte).value() = ubpulses->GetParsingDatas()->GetPacketStreamDataLowByte();
			UbSheet.cell(Results).value() = ubpulses->GetParsingDatas()->GetPacketStreamDataHighByte() * 256 + ubpulses->GetParsingDatas()->GetPacketStreamDataLowByte();
			Cells++;
			triggered.store(false, std::memory_order_release);
		}
		std::this_thread::yield();
	}
}

void GuiInterface::REventListener()
{
	while (running.load(std::memory_order_acquire))
	{
		if (R7triggered.exchange(false, std::memory_order_acq_rel))
		{
			R7s->DataParsingStart("R7",MyTime::Time->GetLocalTime());
			R7triggered.store(false, std::memory_order_release);
		}
		std::this_thread::yield(); 
	}
}

void GuiInterface::SetBackGround(ImGuiIO& _io)
{
	SetClock();
	SettingOption();
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(495, 250), ImGuiCond_Always);
	ImGui::Begin("DEBUG", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);


	if (ImGui::Button("START", ImVec2{ 150,50 }))
	{
		R642s->Connect();
		R7s->Connect();
		ubpulses->Connect();
	}
	ImGui::SameLine();
	if (ImGui::Button("DisConnect & Save", ImVec2{ 150,50 }))
	{
		
		if (R642s->GetSerial().isOpen())
		{
			R642s->GetParsingDatas()->RSave(true);
			R642s->GetParsingDatas()->TriggerDataReset();
			R642s->SetDataThreadStopFlag();
		}
		if (R7s->GetSerial().isOpen())
		{
			R7s->GetParsingDatas()->RSave();
			R7s->SetDataThreadStopFlag();
		}
		if (ubpulses->GetSerial().isOpen())
		{
			ubpulses->SetDataThreadStopFlag();
		}
	

		R642s->DisConnect();
		R7s->DisConnect();
		ubpulses->DisConnect();
		
		if (UbExcel.isOpen())
		{
			UbExcel.save();
			UbExcel.close();
		}
	}
	ImGui::End();
}

void GuiInterface::GetLastData()
{
	if(ubpulses->GetSerial().isOpen())
		triggered.store(true, std::memory_order_release);
	if (R7s->GetSerial().isOpen())
		R7triggered.store(true, std::memory_order_release);
}


std::string GuiInterface::SaveFileDialog()
{
	OPENFILENAME ofn;
	char szFile[260] = { 0 };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = "txt";

	if (GetSaveFileName(&ofn))
		std::wcout << "파일 저장 경로: " << szFile << std::endl;
	else
		std::wcout << "파일 저장 취소됨" << std::endl;

	std::string SZFILE(szFile);

	return SZFILE;
}



void GuiInterface::SetClock()
{
	ImVec4 text_color = ImVec4(0.467, 0.467f, 0.467f, 1.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, text_color);

	ImGui::SetNextWindowPos(ImVec2(41, 38), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(833, 65), ImGuiCond_Always);


	ImGui::Begin("##input", nullptr,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);


	ImGui::PushFont(MyImGui::MyImGuis->GetLargeBoldFont());
	ImVec2 cursorPos = ImGui::GetCursorPos();
	cursorPos.x += 280;
	cursorPos.y += 13;
	ImGui::SetCursorPos(cursorPos);
	ImGui::Text(MyTime::Time->GetLocalDay().c_str());
	ImGui::SameLine();
	ImGui::Text(MyTime::Time->GetLocalTime().c_str());

	ImGui::PopFont();
	ImGui::PopStyleColor(2);
	ImGui::End();
}

void GuiInterface::SettingOption()
{
	ImGui::SetNextWindowPos(ImVec2(915, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(280, 501), ImGuiCond_Always);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	ImGui::Begin("##inputs", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	RightFrameSetting();

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::End();
}

//ImGuiCol_Text,
//ImGuiCol_TextDisabled,
//ImGuiCol_WindowBg,              // Background of normal windows
//ImGuiCol_ChildBg,               // Background of child windows
//ImGuiCol_PopupBg,               // Background of popups, menus, tooltips windows
//ImGuiCol_Border,
//ImGuiCol_BorderShadow,
//ImGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
//ImGuiCol_FrameBgHovered,
//ImGuiCol_FrameBgActive,
//ImGuiCol_TitleBg,               // Title bar
//ImGuiCol_TitleBgActive,         // Title bar when focused
//ImGuiCol_TitleBgCollapsed,      // Title bar when collapsed
//ImGuiCol_MenuBarBg,
//ImGuiCol_ScrollbarBg,
//ImGuiCol_ScrollbarGrab,
//ImGuiCol_ScrollbarGrabHovered,
//ImGuiCol_ScrollbarGrabActive,
//ImGuiCol_CheckMark,             // Checkbox tick and RadioButton circle
//ImGuiCol_SliderGrab,
//ImGuiCol_SliderGrabActive,
//ImGuiCol_Button,
//ImGuiCol_ButtonHovered,
//ImGuiCol_ButtonActive,
//ImGuiCol_Header,                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
//ImGuiCol_HeaderHovered,
//ImGuiCol_HeaderActive,
//ImGuiCol_Separator,
//ImGuiCol_SeparatorHovered,
//ImGuiCol_SeparatorActive,
//ImGuiCol_ResizeGrip,            // Resize grip in lower-right and lower-left corners of windows.
//ImGuiCol_ResizeGripHovered,
//ImGuiCol_ResizeGripActive,
//ImGuiCol_TabHovered,            // Tab background, when hovered
//ImGuiCol_Tab,                   // Tab background, when tab-bar is focused & tab is unselected
//ImGuiCol_TabSelected,           // Tab background, when tab-bar is focused & tab is selected
//ImGuiCol_TabSelectedOverline,   // Tab horizontal overline, when tab-bar is focused & tab is selected
//ImGuiCol_TabDimmed,             // Tab background, when tab-bar is unfocused & tab is unselected
//ImGuiCol_TabDimmedSelected,     // Tab background, when tab-bar is unfocused & tab is selected
//ImGuiCol_TabDimmedSelectedOverline,//..horizontal overline, when tab-bar is unfocused & tab is selected
//ImGuiCol_PlotLines,
//ImGuiCol_PlotLinesHovered,
//ImGuiCol_PlotHistogram,
//ImGuiCol_PlotHistogramHovered,
//ImGuiCol_TableHeaderBg,         // Table header background
//ImGuiCol_TableBorderStrong,     // Table outer and header borders (prefer using Alpha=1.0 here)
//ImGuiCol_TableBorderLight,      // Table inner borders (prefer using Alpha=1.0 here)
//ImGuiCol_TableRowBg,            // Table row background (even rows)
//ImGuiCol_TableRowBgAlt,         // Table row background (odd rows)
//ImGuiCol_TextLink,              // Hyperlink color
//ImGuiCol_TextSelectedBg,
//ImGuiCol_DragDropTarget,        // Rectangle highlighting a drop target
//ImGuiCol_NavHighlight,          // Gamepad/keyboard: current highlighted item
//ImGuiCol_NavWindowingHighlight, // Highlight window when using CTRL+TAB
//ImGuiCol_NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
//ImGuiCol_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
//ImGuiCol_COUNT,
void GuiInterface::RightFrameSetting()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.839f, 0.839f, 0.839f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));





	ImGui::SetCursorPos(ImVec2(15, 38)); 
	ImGui::BeginChild("##1", ImVec2{ 250,70 }, true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	ImGui::SetCursorPos(ImVec2(20, 11));
	ImGui::PushFont(MyImGui::MyImGuis->GetTriggerFont());
	ImGui::Text("Trigger Interval"); 
	ImGui::SetCursorPos(ImVec2(140, 11));
	ImGui::Text("TriggerCount");
	ImGui::PopFont();


	ImGui::PushFont(MyImGui::MyImGuis->GetTriggerNumberFont());
	ImGui::SetCursorPos(ImVec2(25, 29));
	ImGui::Text("%d", this->R642s->GetParsingDatas()->GetTriggerInterval()); ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(145, 29));
	ImGui::Text("%d", this->R642s->GetParsingDatas()->GetTriggerCount());
	ImGui::PopFont();

	ImGui::PushFont(MyImGui::MyImGuis->GetMediumFont());
	ImGui::SetCursorPos(ImVec2(80, 41));
	ImGui::Text("m/s"); ImGui::SameLine();
	ImGui::PopFont();


	ImGui::EndChild();





	ImGui::SetCursorPos(ImVec2(15, 121));
	ImGui::BeginChild("##2", ImVec2{ 250,200 }, true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	PortInfos.clear();
	/*PortInfos = R642s->GetAllPortInfo();
	for (const char* V : PortInfos)
	{
		ImGui::Text(V);
	}*/
	ImGui::EndChild();


	ImGui::SetCursorPos(ImVec2(15, 334));
	ImGui::BeginChild("##3", ImVec2{ 250,65 }, true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	ImGui::EndChild();





	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar(2);
}