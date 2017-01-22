#include "D3Dapp.h"
#include <string>

namespace
{

	D3DApp* g_pd3dApp;
}

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	return g_pd3dApp->MsgProc(hwnd, msg, wParam, lParam);
}


D3DApp::D3DApp(HINSTANCE hInstance) {
	m_hAppInstance = hInstance;
	m_hAppWindow = NULL;
	m_uiClientWidth = 600;
	m_uiClientHeight = 600;
	m_sApppTitle = "Test";
	m_WindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;




}

D3DApp::~D3DApp()
{

}

int D3DApp::Run()
{

	
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update(0.01f);
			Render();
		}
	}
	return static_cast<int>(msg.wParam);
}

bool D3DApp::Init()
{
	if (!InitWindow()) {
		return false;
	}

	if (!InitDirect3D())
	{
		return false;
	}

	return true;
}

bool D3DApp::InitWindow()
{

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = m_hAppInstance;
	wcex.lpfnWndProc = MainWindowProc;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);	
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "D3DAPPWNDCLASS";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, "Failed to register window class", NULL, NULL);
		return false;
	}
		
	RECT r = { 0, 0, m_uiClientWidth, m_uiClientHeight };
	AdjustWindowRect(&r, m_WindowStyle, false);
	int width = r.right - r.left;
	int height = r.bottom - r.top;

	m_hAppWindow = CreateWindow("D3DAPPWNDCLASS", m_sApppTitle.c_str(), m_WindowStyle,
								GetSystemMetrics(SM_CXSCREEN) /2 - width/2,
								GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2,
								width, height, NULL, NULL, m_hAppInstance, NULL);

	if (!m_hAppWindow)
	{
		MessageBox(NULL, "Failed to create window", NULL, NULL);
		return false;
	}

	ShowWindow(m_hAppWindow, SW_SHOW);

	return true;
}






bool D3DApp::InitDirect3D()
{

	m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!m_pDirect3D)
	{
		MessageBox(NULL, "Failed to create Direct 3d com object", NULL, NULL);
		return false;
	}
	
	D3DCAPS9 m_d3dDevCaps;
	m_pDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_d3dDevCaps);

	int vp;
	if (m_d3dDevCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	ZeroMemory(&m_d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	m_d3dpp.BackBufferWidth = m_uiClientWidth;
	m_d3dpp.BackBufferHeight = m_uiClientHeight;
	m_d3dpp.Windowed = true;
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_d3dpp.MultiSampleQuality = 0;
	m_d3dpp.hDeviceWindow = m_hAppWindow;
	m_d3dpp.Flags = 0;
	m_d3dpp.EnableAutoDepthStencil = true;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	HRESULT res = m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hAppWindow,
		vp, &m_d3dpp, &m_pDevice3D);
	
	

	if (!m_pDevice3D)
	{
		std::string message = "Failed device creation ";
		MessageBox(NULL, message.c_str(), NULL, NULL);
		return false;
	}

	D3DVIEWPORT9 viewport;

	ZeroMemory(&viewport, sizeof(D3DVIEWPORT9));

	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = m_uiClientWidth;
	viewport.Height = m_uiClientHeight;
	viewport.MinZ = 0;
	viewport.MaxZ = 1;

	m_pDevice3D->SetViewport(&viewport);

	return true;
}
 



LRESULT D3DApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	extern int key_w;
	extern int key_a;
	extern int key_s;
	extern int key_d;
	extern int key_space;
	extern int key_left;
	extern int key_right;
	extern int key_up;
	extern int key_down;
	switch (msg)
	{


	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;


		break;
	case WM_KEYDOWN:
	{

		int key = HIWORD(lParam) & 0x1ff;
		//debug_log("%x", key);
		if (key == 0x11) key_w = 1;
		if (key == 0x1e) key_a = 1;
		if (key == 0x1f) key_s = 1;
		if (key == 0x20) key_d = 1;
		if (key == 0x39) key_space = 1;
		if (key == 0x14b) key_left = 1;
		if (key == 0x14d) key_right = 1;
		if (key == 0x148) key_up = 1;
		if (key == 0x150) key_down = 1;


	}
	break;

	case WM_KEYUP:
	{
		int key = HIWORD(lParam) & 0x1ff;
		if (key == 0x11) key_w = 0;
		if (key == 0x1e) key_a = 0;
		if (key == 0x1f) key_s = 0;
		if (key == 0x20) key_d = 0;
		if (key == 0x39) key_space = 0;
		if (key == 0x14b) key_left = 0;
		if (key == 0x14d) key_right = 0;
		if (key == 0x148) key_up = 0;
		if (key == 0x150) key_down = 0;

	}

	break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
