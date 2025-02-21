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
//Debug
static int erasemax = 0;
static int eraseCount = 0;
//Debug
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
			//Debug
			erasemax = i;
			std::cout << "한번에 삭제한 프레임 개수 : " << i << std::endl;
			std::cout << "몇번지웠니 : " << eraseCount << std::endl;
			eraseCount++;
			//
			return true;
		}
	}
	return false;
}
