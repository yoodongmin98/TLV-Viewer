#include <iostream>
#include "imgui.h"
#include "GuiInterface.h"
#include "SerialBuffer.h"
#include "MyImGui.h"
#include "ThreadPool.h"

#include <iomanip>    
#include <sstream>
#include <string>
#include <functional>




GuiInterface::GuiInterface()
{
	Buffers = std::make_shared<SerialBuffer>();
    std::function<void()> Functions = std::bind(&GuiInterface::DataInput, this);
    MyImGui::MyImGuis->GetThreadPool()->AddWork(Functions);
}
GuiInterface::~GuiInterface()
{
	 
}



void GuiInterface::Instance()
{
    SetBackGround();
	GetPortInfo();
	
    ////////////////////////////////////////////Test
    if (!MySerial.isOpen())
    {
        MySerial.setPort("COM120");
        MySerial.setBaudrate(1843200);
        MySerial.setBytesize(serial::bytesize_t::eightbits);
        MySerial.setTimeout(serial::Timeout::simpleTimeout(10));
        MySerial.open();
    }
    std::cout << HexBuffer.size() << std::endl;
  ////////////////////////////////////////////////////
   


    Buffers->Instance();
}


void GuiInterface::SetBackGround()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(MyImGui::MyImGuis->GetWindowSize_X(), MyImGui::MyImGuis->GetWindowSize_Y()), ImGuiCond_Always);
    
    ImGui::Begin("TLV-Viewer", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
    ImGui::End();
}


void GuiInterface::GetPortInfo()
{
	PortInfos = serial::list_ports();
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Always);
    ImGui::Begin("Select Port",nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
   
    ImGui::End();
}


void GuiInterface::DataInput()
{
    while (true)
    {
        if (MySerial.available())
        {
            std::lock_guard<std::mutex> lock(HexBufferMutex);
            std::string hexs = MySerial.read();

            std::stringstream hexStream;
            hexStream.str("");
            for (unsigned char c : hexs)
                hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c) << " ";
            HexBuffer.push_back(hexStream.str());
        }
    }
}

//typedef struct TLV_Header_Struct 
//{
//    /*! @brief   Output buffer magic word (sync word). It is initialized to  {0x0102,0x0304,0x0506,0x0708} */
//    uint32_t magicWord1;
//    uint32_t magicWord2;
//
//    /*! brief   Version: : MajorNum * 2^24 + MinorNum * 2^16 + BugfixNum * 2^8 + BuildNum   */
//    uint32_t version;
//
//    /*! @brief   Total packet length including header in Bytes */
//    uint32_t totalPacketLen;
//
//    /*! @brief   platform type */
//    uint32_t platform;
//
//    /*! @brief   Frame number */
//    uint32_t frameNumber;
//
//    /*! @brief   Time in CPU cycles when the message was created. For XWR16xx/XWR18xx: DSP CPU cycles, for XWR14xx: R4F CPU cycles */
//    uint32_t timeCpuCycles;
//
//    /*! @brief   Number of detected objects */
//    uint32_t numDetectedObj;
//
//    /*! @brief   Number of TLVs */
//    uint32_t numTLVs;
//
//    /*! @brief   For Advanced Frame config, this is the sub-frame number in the range
//    * 0 to (number of subframes - 1). For frame config (not advanced), this is always
//    * set to 0. */
//    uint32_t subFrameNumber;
//} TLV_Header_t;