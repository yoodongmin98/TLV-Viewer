#pragma once
#include <iostream>
#include <vector>



class DetectHeader
{
public:
	DetectHeader();
	~DetectHeader();
	bool FindHeader(std::vector<std::string>& _Buffer);
protected:
	void FindMagicNumber(); 
	void ParsingData();
private:
	
};