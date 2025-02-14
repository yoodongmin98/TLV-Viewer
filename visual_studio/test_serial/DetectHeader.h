#pragma once
#include <iostream>
#include <vector>
#include <shared_mutex>


class DetectHeader
{
public:
	DetectHeader();
	~DetectHeader();
	bool FindHeader(std::vector<int>& _Buffer);

protected:
	void FindMagicNumber(); 

private:
	std::shared_mutex HeaderMutex;
};