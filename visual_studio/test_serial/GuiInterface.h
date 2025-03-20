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
	GuiInterface();
	~GuiInterface();

	void Instance(ImGuiIO& io);
protected:
	void SetBackGround(ImGuiIO& io);

	void GetLastData();
	std::string SaveFileDialog();
	void EventListener();
	void REventListener();
private:
	std::shared_ptr<R642> R642s = nullptr;
	std::shared_ptr<R7> R7s = nullptr;
	std::shared_ptr<ubpulse> ubpulses = nullptr;

	std::string Time;
	std::string Path;

	//THread
	std::thread listener;
	std::thread Rlistener;
	std::atomic<bool> triggered = false;
	std::atomic<bool> R7triggered = false;
	std::atomic<bool> running= true ;

	XLDocument UbExcel;
	XLWorksheet UbSheet;
	int Cells = 1;
};