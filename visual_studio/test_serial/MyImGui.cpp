#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>

#include <functional>
#include <iostream>
#include <string>


#include "MyImGui.h"
#include "MyTime.h"
#include "ThreadPool.h"
#include "MyDefine.h"
#include "GuiInterface.h"




MyImGui* MyImGui::MyImGuis = nullptr;
std::atomic<bool> g_Running = true;  // ������ ���� �÷���
std::atomic<bool> g_IsMoving = false;  // â �̵� ���� �÷���


MyImGui::MyImGui()
	: ThreadPools(std::make_shared<ThreadPool>(MYThreadCount))
{
	MyImGuis = this;
	Interfaces = std::make_shared<GuiInterface>();
}
MyImGui::~MyImGui()
{
	if (logFile.is_open())
	{
		logFile.close();
	}
}


void MyImGui::Instance()
{
	//�����������ε� �̰� �� ���Ѻ����ҵ�
	//SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
	WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
	::RegisterClassExW(&wc);
	hwnd = ::CreateWindowW(wc.lpszClassName, L"TLV - Viewer", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 100, 100, 1211, 540, nullptr, nullptr, wc.hInstance, nullptr);



	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
	}

	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;


	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);


	std::thread renderThread(std::bind(&MyImGui::RenderLoop, this, io));
	MSG msg;
	
	while (true)
	{
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				g_Running = false;  
				renderThread.join(); 
				return;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	g_Running = false;
	renderThread.join();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}


void MyImGui::RenderLoop(ImGuiIO& io)
{
	ImVec4 clear_color = ImVec4(0.631f, 0.761f, 0.957f, 1.00f);
	// Main loop
	myFontSmall = io.Fonts->AddFontFromFileTTF("PretendardVariable.ttf", 14.0f);
	myFontLarge = io.Fonts->AddFontFromFileTTF("Pretendard-SemiBold.otf", 25.0f);
	myFontCombo = io.Fonts->AddFontFromFileTTF("Pretendard-Medium.otf", 17.0f);
	myFontUnderText = io.Fonts->AddFontFromFileTTF("Pretendard-Medium.otf", 14.0f);
	while (g_Running)
	{

		if (GetClientRect(hwnd, &rect))
		{
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (g_IsMoving)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}
		g_SwapChainOccluded = false;

		if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
			g_ResizeWidth = g_ResizeHeight = 0;
			CreateRenderTarget();
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		Interfaces->Instance(io);
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		//setting
		ImGui::GetStyle().WindowBorderSize = 0.0f;
		ImGui::GetStyle().WindowRounding = 15.0f;
		ImGui::GetStyle().FrameRounding = 15.0f;





		////////////////////////////////////////////////////////////////////////////////////////////////////////
		ImGui::Render();
		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		HRESULT hr = g_pSwapChain->Present(1, 0);
		g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
	}

	g_Running = false;

	// ImGui ���� ���ҽ� ����
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool MyImGui::CreateDeviceD3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED)
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}


void MyImGui::CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
			return 0;
		g_ResizeWidth = (UINT)LOWORD(lParam);
		g_ResizeHeight = (UINT)HIWORD(lParam);
		return 0;
	}
	case WM_MOVE:
	{
		g_IsMoving = false; // â�� �̵��ϸ� ������ ������ �ٽ� ����
		break;
	}
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		g_Running = false;
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}


void MyImGui::CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
}


void MyImGui::CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}


void MyImGui::LogFileOpen()
{
	//�α����� ����
	if (!logFile.is_open())
		logFile.open(LogFileName, std::ios::app);
}

void MyImGui::LogFlash(std::string _PortName, std::string _Content)
{
	logFile << "\r [" << MyTime::Time->GetLocalDay() << MyTime::Time->GetLocalTime() << "]" << _PortName + _Content << std::flush;
}