#pragma once
#include <iostream>
#include <vector>
#include <shared_mutex>
#include <string>

class ParsingData
{
public:
	ParsingData();
	~ParsingData();

	
	void DataParsing(std::vector<int>& _Buffer);
protected:
	int ParseLittleEndian(std::vector<int>& _Buffer , const int _bytesize);
	void DataView();

	//Test
	std::string TransVersion(std::vector<int>& _Buffer);
private:
	std::shared_mutex DataMutex;

	int BufferIndex = 0;
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
};