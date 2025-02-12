#include "DetectHeader.h"
#include <algorithm>


DetectHeader::DetectHeader()
{

}

DetectHeader::~DetectHeader()
{

}


bool DetectHeader::FindHeader(std::vector<std::string>& _Buffer)
{
	std::vector<std::string> pattern = { "02" , "01" , "04" , "03" , "06" , "05" , "08" , "07" };

	//Sliding Window Algorithm
	size_t dataSize = _Buffer.size();
	size_t patternSize = pattern.size();

	if (dataSize < patternSize) return false;

	auto it = search(_Buffer.begin(), _Buffer.end(), pattern.begin(), pattern.end());

	if (it != _Buffer.end()) 
	{
		//¹öÆÛ ÀÌ ^^¤Ó
		_Buffer.erase(_Buffer.begin(), it + pattern.size());
		return true;
	}
	return false;
}




void DetectHeader::FindMagicNumber()
{
	
}

