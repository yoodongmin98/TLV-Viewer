#pragma once
#include <iostream>
#include <vector>
#include <shared_mutex>


class ParsingData
{
public:
	ParsingData();
	~ParsingData();

	
	void DataParsing(std::vector<int>& _Buffer);
protected:
	int ParseLittleEndian(std::vector<int>& _Buffer , const int _bytesize);
	void DataView();
private:
	std::shared_mutex DataMutex;

	int BufferIndex = 0;
	int Version = 0;
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