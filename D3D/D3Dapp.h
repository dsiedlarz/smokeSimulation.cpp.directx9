#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include "D3DUtil.h"

class D3DApp {
public:
	D3DApp(HINSTANCE hInstance);

	virtual ~D3DApp();

	int Run();

	

	virtual bool Init();
	virtual void  Update(float dt) = 0;
	virtual void Render() = 0;
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	HWND			m_hAppWindow;
	HINSTANCE		m_hAppInstance;
	unsigned int	m_uiClientWidth;
	unsigned int	m_uiClientHeight;
	std::string		m_sApppTitle;
	DWORD			m_WindowStyle;

	IDirect3D9*				m_pDirect3D;
	IDirect3DDevice9*		m_pDevice3D;
	D3DPRESENT_PARAMETERS	m_d3dpp;


protected:

	bool InitWindow();

	bool 
		
		InitDirect3D();

};
