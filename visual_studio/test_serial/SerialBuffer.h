#pragma
#include <iostream>
#include <serial/serial.h>
#include <vector>


class DetectHeader;
class ParsingData;
class SerialBuffer
{
public:
	SerialBuffer();
	~SerialBuffer();

	void Instance();
protected:
	void GetPortInfo();
private:
	std::shared_ptr<DetectHeader> DetectHeaders = nullptr;
	std::shared_ptr<ParsingData> ParsingDatas = nullptr;

	serial::Serial MySerial;

	std::vector<serial::PortInfo> PortInfos;
};