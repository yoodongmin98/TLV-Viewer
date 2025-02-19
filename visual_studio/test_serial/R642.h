#pragma once
#include <iostream>
#include "Module.h"



class R642 : public Module
{
public:
	R642();
	~R642();

	void Instance();
protected:
	void SetBaudrate() override;

	void R642Setting();
	void UI();
	
private:

};