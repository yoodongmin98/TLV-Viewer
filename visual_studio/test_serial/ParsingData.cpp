#include "ParsingData.h"

#include "imgui.h"



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

}


ParsingData::~ParsingData()
{

}


void ParsingData::DataParsing(std::vector<int>& _Buffer) 
{
    Version = ParseLittleEndian(_Buffer , 4);
    TotalPacketLength = ParseLittleEndian(_Buffer, 4);
    Platform = ParseLittleEndian(_Buffer, 4);
    FrameNumber = ParseLittleEndian(_Buffer, 4);
    Time = ParseLittleEndian(_Buffer, 4);
    NumberofDetectedObjects = ParseLittleEndian(_Buffer, 4);
    NumberofTLVs = ParseLittleEndian(_Buffer, 4);
    SubframeNumber = ParseLittleEndian(_Buffer, 4);

    TLVType = ParseLittleEndian(_Buffer, 4);
    TLVLength = ParseLittleEndian(_Buffer, 4);
    BufferIndex = 0;
    DataView();
}


void ParsingData::DataView()
{
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Always);
    ImGui::Begin("Select Port", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

    ImGui::Text("Version = %d", Version);
    ImGui::Text("TotalPacketLength = %d", TotalPacketLength);
    ImGui::Text("Platform = %d", Platform);
    ImGui::Text("FrameNumber = %d", FrameNumber);
    ImGui::Text("Time = %d", Time);
    ImGui::Text("NumberofDetectedObjects = %d", NumberofDetectedObjects);
    ImGui::Text("NumberofTLVs = %d", NumberofTLVs);
    ImGui::Text("SubframeNumber = %d", SubframeNumber);
    ImGui::Text("TLVType = %d", TLVType);
    ImGui::Text("TLVLength = %d", TLVLength);
    ImGui::End();
}


int ParsingData::ParseLittleEndian(std::vector<int>& _Buffer , const int _bytesize)
{
    int value = 0;
    if (_Buffer.size() < 4 || BufferIndex + 3 >= _Buffer.size()) return 0;

    {
        std::lock_guard<std::shared_mutex> lock(DataMutex);
        value = (_Buffer[BufferIndex] << 0) | (_Buffer[BufferIndex + 1] << 8) |  (_Buffer[BufferIndex + 2] << 16) |  (_Buffer[BufferIndex + 3] << 24);
    }
    
    BufferIndex += _bytesize;
    return value;
}