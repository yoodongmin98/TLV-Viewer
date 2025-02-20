#include "ParsingData.h"

#include "imgui.h"
#include "CSV.h"
#include "DetectHeader.h"
//
//프레임 헤더 구조 :
//
//Sync Pattern(8 bytes) : 프레임의 시작을 나타내는 고정된 패턴입니다.
//Version(4 bytes) : 프로토콜 버전을 나타냅니다.
//Total Packet Length(4 bytes) : 프레임 전체의 길이를 나타냅니다.
//Platform(4 bytes) : 하드웨어 플랫폼 정보를 포함합니다.
//Frame Number(4 bytes) : 프레임의 순번을 나타냅니다.
//Time(4 bytes) : 프레임 생성 시점을 CPU 사이클 단위로 나타냅니다.
//Number of Detected Objects(4 bytes) : 감지된 객체의 수를 나타냅니다.
//Number of TLVs(4 bytes) : 프레임에 포함된 TLV 블록의 수를 나타냅니다.
//Subframe Number(4 bytes) : 서브프레임 번호를 나타내며, 고급 서브프레임 모드가 활성화되지 않은 경우 0입니다.
//TLV 헤더 구조 :
//
//Type(4 bytes) : TLV의 데이터 유형을 나타냅니다.
//Length(4 bytes) : TLV 헤더를 포함한 전체 TLV 블록의 길이를 나타냅니다.



ParsingData::ParsingData()
{
    CSVs = std::make_shared<CSV>();
    DetectHeaders = std::make_shared<DetectHeader>();
}


ParsingData::~ParsingData()
{

}


void ParsingData::DataParsing(std::vector<int>& _Buffer) 
{
    TLV_HeaderParsing(_Buffer);
    DataView();
}


void ParsingData::TLV_HeaderParsing(std::vector<int>& _Buffer)
{
    if (_Buffer.size() > 100)
    {
        Version = TransVersion(_Buffer);
        TotalPacketLength = ParseLittleEndian(_Buffer);
        Platform = ParseLittleEndian(_Buffer);
        FrameNumber = ParseLittleEndian(_Buffer);
        Time = ParseLittleEndian(_Buffer);
        NumberofDetectedObjects = ParseLittleEndian(_Buffer);
        NumberofTLVs = ParseLittleEndian(_Buffer);
        SubframeNumber = ParseLittleEndian(_Buffer);
        TLV_TypeParsing(_Buffer);
        if (TLV_Datas.size() == 2)
            CSVs->WriteFile(TLV_Datas[1]);
        BufferIndex = 8;
    }
}


bool ParsingData::TLV_TypeParsing(std::vector<int>& _Buffer)
{
    std::vector<int> TLVHeader = { 4,0,0,0,0,4,0,0 };
    TLV_Datas.clear();
    TLV_Datas.resize(NumberofTLVs);
    for (auto i = 0; i < NumberofTLVs; ++i)
    {
        if (DetectHeaders->FindHeader(_Buffer, TLVHeader))
        {
            //좀 짜치지만 나중에 바꾸는거로
            BufferIndex = 0;
            TLVType = ParseLittleEndian(_Buffer);
            TLVLength = ParseLittleEndian(_Buffer);
            for (auto k = 0; k < TLVLength / 2; ++k)
            {
                TLV_Datas[i].push_back(ParseLittleEndian(_Buffer, 2));
            }
        }
    }
    return true;
}


void ParsingData::DataView()
{
    ImGui::Text("Version = %s", Version.c_str());
    ImGui::Text("TotalPacketLength = %d", TotalPacketLength);
    ImGui::Text("Platform = %d", Platform);
    ImGui::Text("FrameNumber = %d", FrameNumber);
    ImGui::Text("Time = %d", Time);
    ImGui::Text("NumberofDetectedObjects = %d", NumberofDetectedObjects);
    ImGui::Text("NumberofTLVs = %d", NumberofTLVs);
    ImGui::Text("SubframeNumber = %d", SubframeNumber);
    ImGui::Text("TLVType = %d", TLVType);
    ImGui::Text("TLVLength = %d", TLVLength);
}



int ParsingData::ParseLittleEndian(std::vector<int>& _Buffer , int _bytesize)
{
    int value = 0;
    if (_Buffer.size() < 4 || BufferIndex + 3 >= _Buffer.size()) return 0;

    if (_bytesize == 4)
    {
        std::lock_guard<std::shared_mutex> lock(DataMutex);
        value = (_Buffer[BufferIndex] << 0) | (_Buffer[BufferIndex + 1] << 8) | (_Buffer[BufferIndex + 2] << 16) | (_Buffer[BufferIndex + 3] << 24);
    }
    else if (_bytesize == 2)
    {
        value = (_Buffer[BufferIndex] << 0) | (_Buffer[BufferIndex + 1] << 8);
    }
    
    BufferIndex += _bytesize;
    return value;
}


std::string ParsingData::TransVersion(std::vector<int>& _Buffer)
{
    std::string VersionString;
    VersionString = VersionString + 
    std::to_string(_Buffer[BufferIndex+3]) + "." +
    std::to_string(_Buffer[BufferIndex+2]) + "." +
    std::to_string(_Buffer[BufferIndex+1]) + "." +
    std::to_string(_Buffer[BufferIndex+0]);
    BufferIndex += 4;
    return VersionString;
}