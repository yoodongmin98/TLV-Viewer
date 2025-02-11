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


void SerialBuffer::Instance()
{
	
}