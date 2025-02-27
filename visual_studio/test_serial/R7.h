#pragma once
#include <iostream>
#include "Module.h"
#include <functional>


class R7 : public Module
{
public:
	R7();
	~R7();
	void Instance();

	
protected:
	void SetBaudrate() override;

	void R7Setting();
	void UI();
private:
	
};