#pragma once
#include <iostream>




class DetectHeader
{
public:
	DetectHeader();
	~DetectHeader();
	bool FindHeader();
protected:
	void FindMagicNumber(); 
	void ParsingData();
private:

};