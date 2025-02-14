#include "DetectHeader.h"
#include <algorithm>
#include <shared_mutex>
#include <string_view>
DetectHeader::DetectHeader()
{

}

DetectHeader::~DetectHeader()
{

}


bool DetectHeader::FindHeader(std::vector<int>& _Buffer)
{
	std::vector<int> pattern = { 2,1,4,3,6,5,8,7 };
	size_t patternsize = pattern.size();

	std::lock_guard<std::shared_mutex> lock(HeaderMutex);

	if (_Buffer.size() < 2000)
		return false;


	for (auto i = 0; i < _Buffer.size() - pattern.size(); ++i)
	{
		if (std::equal(_Buffer.begin() + i, _Buffer.begin() + i + patternsize, pattern.begin()))
		{
			_Buffer.erase(_Buffer.begin(), _Buffer.begin() + i + 8);
			return true;
		}
	}
	return false;
}
