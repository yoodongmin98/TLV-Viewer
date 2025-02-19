#pragma once
#include <iostream>
#include <vector>
#include <shared_mutex>
#include <string>


class CSV;
class ParsingData
{
public:
	ParsingData();
	~ParsingData();

	
	void DataParsing(std::vector<int>& _Buffer);
protected:
	std::string TransVersion(std::vector<int>& _Buffer);
	int ParseLittleEndian(std::vector<int>& _Buffer , const int _bytesize=4);
	void TLV_HeaderParsing(std::vector<int>& _Buffer);
	void TLV_TypeParsing(std::vector<int>& _Buffer);
	void DataView();

	
private:
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

	std::shared_mutex DataMutex;

	
	std::vector<std::vector<int>> TLV_Datas;

	std::shared_ptr<CSV> CSVs = nullptr;
};