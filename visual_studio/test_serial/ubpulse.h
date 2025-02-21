#pragma once
#include <iostream>

#include "Module.h"

class ubpulse : public Module
{
public:
	ubpulse();
	~ubpulse();
	void Instance();

protected:
	void SetBaudrate() override;

	void ubpulseSetting();
	void UI();
private:

};