#pragma once
#include <iostream>
#include <vector>
#include <shared_mutex>
#include <string>
#include <deque>



class DetectHeader;
class CSV;
class ParsingData
{
public:
	ParsingData();
	~ParsingData();


	void DataParsing(std::vector<int>& _Buffer, std::string& _Name);

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

protected:

	std::string TransVersion(std::vector<int>& _Buffer);
	int ParseLittleEndian(std::vector<int>& _Buffer, const int _bytesize = 4);

	void TLV_HeaderParsing(std::vector<int>& _Buffer, std::string& _Name);
	bool TLV_TypeParsing(std::vector<int>& _Buffer);

private:
	int BufferIndex = 8;

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

	std::shared_mutex DataMutex;


	std::vector<int> TLV_Datas;

	std::shared_ptr<CSV> CSVs = nullptr;
	std::shared_ptr<DetectHeader> DetectHeaders = nullptr;
};