#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <d3d11.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <string>
#include <serial/serial.h>
#include <fstream>
#include <thread>


// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



class GuiInterface;
class ThreadPool;
class MyImGui
{
public:

    static MyImGui* MyImGuis;
    MyImGui();
    ~MyImGui();
    void Instance();

    void LogFlash(std::string _PortName, std::string _Content);

    std::shared_ptr<ThreadPool> GetThreadPool()
    {
        return ThreadPools;
    }
    const int GetWindowSize_X()
    {
        return width;
    }
    const int GetWindowSize_Y()
    {
        return height;
    }
    void SetLogBoxBool()
    {
        LogFileSet = !LogFileSet;
    }
    void SetLogBoxFileClose()
    {
        if (logFile.is_open())
            logFile.close();
    }
    HWND& GetWindowHandle()
    {
        return hwnd;
    }
    RECT& GetRECT()
    {
        return rect;
    }
    ImFont* GetLargeBoldFont()
    {
        return myFontLarge;
    }
    ImFont* GetSmallFont()
    {
        return myFontSmall;
    }
    ImFont* GetMediumFont()
    {
        return myFontMedium;
    }
    ImFont* GetComboFont()
    {
        return myFontCombo;
    }
    ImFont* GetUnderTextFont()
    {
        return myFontUnderText;
    }
    ImFont* GetTriggerFont()
    {
        return Trigger;
    }
    ImFont* GetTriggerNumberFont()
    {
        return TriggerNumber;
    }

protected:
    void LogFileOpen();
    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();

    void RenderLoop(ImGuiIO& io);
private:
    HWND hwnd;
    RECT rect;
    bool LogFileSet = false;

    int width = 0;
    int height = 0;

    ImFont* myFontLarge;
    ImFont* myFontMedium;
    ImFont* myFontSmall;
    ImFont* myFontCombo;
    ImFont* myFontUnderText;
    ImFont* Trigger;
    ImFont* TriggerNumber;
    
    std::shared_ptr<ThreadPool> ThreadPools = nullptr;
    std::shared_ptr<GuiInterface> Interfaces = nullptr;

    std::ofstream logFile;
    std::string LogFileName;
};