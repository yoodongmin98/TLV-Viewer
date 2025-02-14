#include "ParsingData.h"

#include "imgui.h"



//
//������ ��� ���� :
//
//Sync Pattern(8 bytes) : �������� ������ ��Ÿ���� ������ �����Դϴ�.
//Version(4 bytes) : �������� ������ ��Ÿ���ϴ�.
//Total Packet Length(4 bytes) : ������ ��ü�� ���̸� ��Ÿ���ϴ�.
//Platform(4 bytes) : �ϵ���� �÷��� ������ �����մϴ�.
//Frame Number(4 bytes) : �������� ������ ��Ÿ���ϴ�.
//Time(4 bytes) : ������ ���� ������ CPU ����Ŭ ������ ��Ÿ���ϴ�.
//Number of Detected Objects(4 bytes) : ������ ��ü�� ���� ��Ÿ���ϴ�.
//Number of TLVs(4 bytes) : �����ӿ� ���Ե� TLV ����� ���� ��Ÿ���ϴ�.
//Subframe Number(4 bytes) : ���������� ��ȣ�� ��Ÿ����, ��� ���������� ��尡 Ȱ��ȭ���� ���� ��� 0�Դϴ�.
//TLV ��� ���� :
//
//Type(4 bytes) : TLV�� ������ ������ ��Ÿ���ϴ�.
//Length(4 bytes) : TLV ����� ������ ��ü TLV ����� ���̸� ��Ÿ���ϴ�.



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