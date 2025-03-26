#include "DetectHeader.h"
#include <algorithm>
#include <shared_mutex>
#include <string_view>

#include "MyTime.h"

DetectHeader::DetectHeader()
{

}

DetectHeader::~DetectHeader()
{

}


bool DetectHeader::FindHeader(std::vector<int>& _Buffer , std::vector<int> pattern)
{
	std::lock_guard<std::shared_mutex> lock(HeaderMutex);

	if (_Buffer.size() < 1500)
		return false;

	for (auto i = 0; i < _Buffer.size() - pattern.size(); ++i)
	{
		if (std::equal(_Buffer.begin() + i, _Buffer.begin() + i + pattern.size(), pattern.begin()))
		{
			_Buffer.erase(_Buffer.begin(), _Buffer.begin() + i);
			return true;
		}
	}
	return false;
}
