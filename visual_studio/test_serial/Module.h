#pragma once
#include <iostream>
#include <serial/serial.h>
#include <shared_mutex>
#include <atomic>
#include <vector>
#include <string>
#include <thread>
#include <deque>


class ParsingData;
class DetectHeader;
class Module
{
public:
	Module();
	~Module();
	virtual void SetBaudrate() = 0;


	std::shared_ptr<ParsingData>& GetParsingDatas()
	{
		return ParsingDatas;
	}

	std::string& GetModuleName()
	{
		return ModuleName;
	}
	serial::Serial& GetSerial()
	{
		return MySerial;
	}
	bool Connect();
	void DisConnect();
	void DataParsing(std::string _Name , std::string Time);
protected:
	//Serial
	serial::Serial MySerial;
	std::string ComPort;
	unsigned int  Baudrate = 0;
	serial::Timeout timeout = serial::Timeout::simpleTimeout(100);
	std::vector<serial::PortInfo> PortInfos;

	//Data Buffer
	std::vector<const char*> AllPort;
	std::vector<int> HexBuffer;


	//Module Name
	std::string ModuleName;
	std::vector<int> MagicNumber;

	int SelectPort = 0;

	void SetPortInfo();
	void DataView();
	void PulseDataView();
private:
	//Thread
	void DataInput();
	std::thread serialThread;
	std::shared_mutex HexBufferMutex;
	std::atomic<bool> stop = false;
	
	//Data Parsing
	std::shared_ptr<DetectHeader> DetectHeaders = nullptr;
	std::shared_ptr<ParsingData> ParsingDatas = nullptr;

	//�ϴ� ���������� ��Ʈ ���õ� ������(�� ����Ǿ��ִ���)
	//�����ϴ°� ������

	//������Ʈ : Ʈ���� ����޾Ҵ���?

};