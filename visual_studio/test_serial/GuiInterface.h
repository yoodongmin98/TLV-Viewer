#pragma once
#include <iostream>
#include <serial/serial.h>
#include <vector>
#include <string>
#include <shared_mutex>
#include <atomic>
#include "imgui.h"
#include <OpenXLSX.hpp>

using namespace OpenXLSX;

class ubpulse;
class R7;
class R642;
class GuiInterface
{
public:
	static GuiInterface* GUI;
	GuiInterface();
	~GuiInterface();

	void Instance(ImGuiIO& io);
	void SetClock();
	void SettingOption();
	std::string GetPath()
	{
		return Path;
	}
	std::string GetFileNameFromPath(const std::string& path);
protected:
	void SetBackGround(ImGuiIO& io);

	void GetLastData();
	std::string SaveFileDialog();
	void EventListener();
	void REventListener();

	void RightFrameSetting();
private:

	bool StartButton = false;
	bool SaveButton = false;
	std::shared_ptr<R642> R642s = nullptr;
	std::shared_ptr<R7> R7s = nullptr;
	std::shared_ptr<ubpulse> ubpulses = nullptr;

	


	std::vector<const char*> PortInfos;



	std::string Path;

	//Thread
	std::thread listener;
	std::thread Rlistener;
	std::atomic<bool> triggered = false;
	std::atomic<bool> R7triggered = false;
	std::atomic<bool> running= true ;

	XLDocument UbExcel;
	XLWorksheet UbSheet;
	int Cells = 1;
};