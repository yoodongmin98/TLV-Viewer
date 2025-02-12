#include "SerialBuffer.h"
#include "DetectHeader.h"
#include "ParsingData.h"

#include <serial/serial.h>

SerialBuffer::SerialBuffer()
{
	DetectHeaders = std::make_shared<DetectHeader>();
	ParsingDatas = std::make_shared<ParsingData>();
}

SerialBuffer::~SerialBuffer()
{

}


void SerialBuffer::Instance(std::vector<std::string>& _Buffer)
{
	FindHeader(_Buffer);
	DataParsing(_Buffer);

	
}




void SerialBuffer::FindHeader(std::vector<std::string>& _Buffer)
{
	DetectHeaders->FindHeader(_Buffer);
}


void SerialBuffer::DataParsing(std::vector<std::string>& _Buffer)
{
	ParsingDatas->DataParsing(_Buffer);
}