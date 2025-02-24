#pragma once
#include <iostream>

#include "Module.h"
#include "functional"


class ubpulse : public Module
{
public:
	ubpulse();
	~ubpulse();
	void Instance();

	std::function<void()>& Get_ubpulse_DataEvent()
	{
		return DataCallbackFunction;
	}

protected:
	void SetBaudrate() override;

	void ubpulseSetting();
	void UI();

	void CallbackTrigger()
	{
		if (DataCallbackFunction)
			DataCallbackFunction();
	}
private:

	std::function<void()> DataCallbackFunction;
};