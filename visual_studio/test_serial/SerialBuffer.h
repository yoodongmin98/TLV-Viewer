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

	void Instance(std::vector<std::string>& _Buffer);
protected:
	void FindHeader(std::vector<std::string>& _Buffer);
	void DataParsing(std::vector<std::string>& _Buffer);
private:
	std::shared_ptr<DetectHeader> DetectHeaders = nullptr;
	std::shared_ptr<ParsingData> ParsingDatas = nullptr;

};