#pragma once
#include <iostream>
#include <vector>
#include <shared_mutex>
#include <string>
#include <deque>
#include <functional>


class DetectHeader;
class CSV;
class ParsingData
{
public:
	ParsingData();
	~ParsingData();


	void DataParsing(std::vector<int>& _Buffer, std::string& _Name, std::string Time);

	const std::string GetVersion()
	{
		return Version;
	}
	const int GetTotalPacketLength()
	{
		return TotalPacketLength;
	}
	const int GetPlatform()
	{
		return Platform;
	}
	const int GetFrameNumber()
	{
		return FrameNumber;
	}
	const int GetTime()
	{
		return Time;
	}
	const int GetNumberofDetectedObjects()
	{
		return NumberofDetectedObjects;
	}
	const int GetNumberofTLVs()
	{
		return NumberofTLVs;
	}
	const int GetSubframeNumber()
	{
		return SubframeNumber;
	}
	const int GetTLVType()
	{
		return TLVType;
	}
	const int GetTLVLength()
	{
		return TLVLength;
	}

	const int GetPacketUnitData0()
	{
		return PacketUnitData0;
	}
	const int GetPacketCount()
	{
		return PacketCount;
	}
	const int GetPacketUnitData1()
	{
		return PacketUnitData1;
	}
	const int GetPacketCyclicData()
	{
		return PacketCyclicData;
	}
	const int GetPacketStreamDataHighByte()
	{
		return PacketStreamDataHighByte;
	}
	const int GetPacketStreamDataLowByte()
	{
		return PacketStreamDataLowByte;
	}
	const int GetTriggerInterval()
	{
		return TriggerInterval;
	}
	const int GetTriggerCount()
	{
		return TriggerCount;
	}


	const void TriggerDataReset()
	{
		TriggerCount = 0;
		TriggerInterval = 0;
		R7Vector.clear();
		R642Vector.clear();
	}









	std::vector<int> GetTLVBuffer()
	{
		return TLV_Datas;
	}
		
	std::function<void()>& Get_ubpulse_DataEvent()
	{
		return DataCallbackFunction;
	}

	std::shared_ptr<CSV>& GetCSVs()
	{
		return CSVs;
	}
	void RSave(bool Standard = 0);
protected:

	std::string TransVersion(std::vector<int>& _Buffer);
	int ParseLittleEndian(std::vector<int>& _Buffer, const int _bytesize = 4);

	void TLV_HeaderParsing(std::vector<int>& _Buffer, std::string& _Name, std::string Time);
	bool TLV_TypeParsing(std::vector<int>& _Buffer);

	void ubpulse_HeaderParsing(std::vector<int>& _Buffer);

private:
	int BufferIndex = 8;

	//TLV
	std::string Version;
	int TotalPacketLength = 0;
	int Platform = 0;
	int FrameNumber = 0;
	int Time = 0;
	int NumberofDetectedObjects = 0;
	int NumberofTLVs = 0;
	int SubframeNumber = 0;

	int TLVType = 0;
	int TLVLength = 0;

	//ubpulse
	int PacketUnitData0 = 0;
	int PacketCount = 0;
	int PacketUnitData1 = 0;
	int PacketCyclicData = 0;
	int PacketStreamDataHighByte = 0;
	int PacketStreamDataLowByte = 0;

	int TriggerInterval = 0;
	int TriggerCount = 0;

	std::shared_mutex DataMutex;
	std::vector<int> TLV_Datas;

	std::shared_ptr<CSV> CSVs = nullptr;
	std::shared_ptr<DetectHeader> DetectHeaders = nullptr;


	std::vector<std::tuple<std::vector<int>,std::string ,std::string>> R7Vector;
	std::vector<std::tuple<std::vector<int>, std::string, std::string>> R642Vector;
	
	void CallbackTrigger()
	{
		if (DataCallbackFunction)
			DataCallbackFunction();
	}
	std::function<void()> DataCallbackFunction;
};