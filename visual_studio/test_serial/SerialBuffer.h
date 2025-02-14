#pragma
#include <iostream>
#include <serial/serial.h>
#include <vector>
#include <string>

class DetectHeader;
class ParsingData;
class SerialBuffer
{
public:
	SerialBuffer();
	~SerialBuffer();

	void Instance(std::vector<int>& _Buffer);
protected:
	void FindHeader(std::vector<int>& _Buffer);
	void DataParsing(std::vector<int>& _Buffer);
private:
	std::shared_ptr<DetectHeader> DetectHeaders = nullptr;
	std::shared_ptr<ParsingData> ParsingDatas = nullptr;

};