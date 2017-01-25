//PROJEKT ZALICZENIOWY MiSS
// POGOCKI JAN, DAWID SIEDLARZ
// AGH, SEMESTR ZIMOWY 2016

#include <Windows.h>
#include <atlimage.h>
#include "D3Dapp.h"


// System and DirectX includes.
#include <d3d9.h>
#include <d3dx9.h>
#include <strsafe.h>
#include <math.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

// local includes
#include "Vector3.h"
#include "ICol3d.h"
#include <iostream>


#pragma warning(disable: 4995)          // don't warn about deprecated functions
#define _CRT_SECURE_NO_DEPRECATE		// ditto

CRITICAL_SECTION	debug_CS;

//config 
int const size = 140;
int const sizeX = size;
int const sizeY = size;
int const sizeZ = size;

int const generatorX = sizeX /2;
int const generatorY = sizeX * 0.9;
int const generatorZ = sizeZ*0.9;
int const generatorInk = 10;
int const generatorHeat = 10;


//DIRECTX struktura odzworowuj¹ca punkt w przestrzeci wraz z jego kolorem.
struct VertexPositionColor
{
	VertexPositionColor() {}
	VertexPositionColor(float _x, float _y, float _z, D3DCOLOR c)
	{
		x = _x; y = _y; z = _z; color = c;
	}
	float x, y, z;
	D3DCOLOR color;
	static const DWORD FVF;
};

//Klasa Klasa zajmuj¹ca siê renderowaniem widkomu za pomoc¹ DIRECTX3D
class TestApp : public D3DApp
{
public:
	TestApp(HINSTANCE);
	~TestApp();

	bool Init() override;
	void Update(float dt) override;
	void Render() override;
};

//obs³uga "kamery"
int	key_left = 0;
int key_right = 0;
int key_up = 0;
int key_down = 0;
int key_w = 0;
int key_a = 0;//zatrzymanie update'u
int key_s = 0;
int key_d = 0;
int key_space = 0;


//Konfiguracja DIRECTX3D
const DWORD VertexPositionColor::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX0;
IDirect3DVertexBuffer9* VB;
IDirect3DIndexBuffer9* IB;
D3DXMATRIX World;
int const vbSize = sizeX * sizeY *sizeZ;
VertexPositionColor  verts[8 * vbSize];
WORD indices[36 * vbSize];
int currentCube = 0;
VOID* pVerts;
VOID* pIndices;
float rotY;
float rotX;
int frame_counter = 1;
D3DXMATRIX Rx, Ry;


int	m_w;
int	m_h;
int	m_d;



void    debug_log(const char* text, ...);
int has_focus = 1;
bool    g_resize = false;

TestApp::TestApp(HINSTANCE hInstance) :D3DApp(hInstance)
{

}

TestApp::~TestApp()
{

}

//rysowanie szescianiu za pomoc¹ trójtk¹tów, u¿ycie bufora indeksów i krawêdzi
void drawCube(float xx, float yy, float zz, D3DCOLOR color) {

	yy = yy * -1;

	int curr = currentCube;
	int vertInd = 8 * currentCube;
	verts[vertInd] = VertexPositionColor(xx, yy + 1, zz, color);
	verts[vertInd + 1] = VertexPositionColor(xx + 1, yy + 1, zz, color);
	verts[vertInd + 2] = VertexPositionColor(xx, yy, zz, color);
	verts[vertInd + 3] = VertexPositionColor(xx + 1, yy, zz, color);

	verts[vertInd + 4] = VertexPositionColor(xx, yy + 1, zz + 1, color);
	verts[vertInd + 5] = VertexPositionColor(xx + 1, yy + 1, zz + 1, color);
	verts[vertInd + 6] = VertexPositionColor(xx, yy, zz + 1, color);
	verts[vertInd + 7] = VertexPositionColor(xx + 1, yy, zz + 1, color);


	int indiciesInd = 36 * currentCube;
	indices[indiciesInd + 0] = 0 + vertInd;
	indices[indiciesInd + 1] = 1 + vertInd;
	indices[indiciesInd + 2] = 2 + vertInd;
	indices[indiciesInd + 3] = 2 + vertInd;
	indices[indiciesInd + 4] = 1 + vertInd;
	indices[indiciesInd + 5] = 3 + vertInd;
	indices[indiciesInd + 6] = 5 + vertInd;
	indices[indiciesInd + 7] = 4 + vertInd;
	indices[indiciesInd + 8] = 7 + vertInd;
	indices[indiciesInd + 9] = 7 + vertInd;
	indices[indiciesInd + 10] = 4 + vertInd;
	indices[indiciesInd + 11] = 6 + vertInd;
	indices[indiciesInd + 12] = 1 + vertInd;
	indices[indiciesInd + 13] = 5 + vertInd;
	indices[indiciesInd + 14] = 3 + vertInd;
	indices[indiciesInd + 15] = 3 + vertInd;
	indices[indiciesInd + 16] = 5 + vertInd;
	indices[indiciesInd + 17] = 7 + vertInd;
	indices[indiciesInd + 18] = 4 + vertInd;
	indices[indiciesInd + 19] = 0 + vertInd;
	indices[indiciesInd + 20] = 6 + vertInd;
	indices[indiciesInd + 21] = 6 + vertInd;
	indices[indiciesInd + 22] = 0 + vertInd;
	indices[indiciesInd + 23] = 2 + vertInd;
	indices[indiciesInd + 24] = 4 + vertInd;
	indices[indiciesInd + 25] = 5 + vertInd;
	indices[indiciesInd + 26] = 0 + vertInd;
	indices[indiciesInd + 27] = 0 + vertInd;
	indices[indiciesInd + 28] = 5 + vertInd;
	indices[indiciesInd + 29] = 1 + vertInd;
	indices[indiciesInd + 30] = 2 + vertInd;
	indices[indiciesInd + 31] = 3 + vertInd;
	indices[indiciesInd + 32] = 6 + vertInd;
	indices[indiciesInd + 33] = 6 + vertInd;
	indices[indiciesInd + 34] = 3 + vertInd;
	indices[indiciesInd + 35] = 7 + vertInd;
	if (currentCube >= vbSize) {
		int d = currentCube;
	}
	else {

		currentCube++;
	}
}

//Inicjalizacja okna
bool TestApp::Init()
{
	if (!D3DApp::Init()) {
		return false;
	}
	currentCube = 0;

	m_pDevice3D->CreateVertexBuffer(16 * sizeof(VertexPositionColor)*vbSize, 0, VertexPositionColor::FVF, D3DPOOL_MANAGED, &VB, NULL);

	m_pDevice3D->CreateIndexBuffer(36 * sizeof(WORD)*vbSize, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &IB, NULL);

	D3DXMATRIX view;
	D3DXMATRIX proj;

	D3DXVECTOR3 position = D3DXVECTOR3(2 * sizeX, -sizeY / 8, 2 * sizeZ);
	D3DXVECTOR3 target = D3DXVECTOR3(sizeX / 2, -sizeY / 2, sizeZ / 2);
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&view, &position, &target, &up);
	m_pDevice3D->SetTransform(D3DTS_VIEW, &view);

	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4, static_cast<float>(m_uiClientWidth) / m_uiClientHeight, 1.0f, 1000.0f);

	m_pDevice3D->SetTransform(D3DTS_PROJECTION, &proj);

	m_pDevice3D->SetRenderState(D3DRS_LIGHTING, false);
	m_pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice3D->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	m_pDevice3D->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	m_pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, false);

	m_pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	return true;
}

//Zape³nienie buforów po aktualizacji macierzy stanów
void updateBuffers() {

	VB->Lock(0, sizeof(verts), (void**)&pVerts, 0);
	memcpy(pVerts, verts, sizeof(verts));
	VB->Unlock();


	IB->Lock(0, sizeof(indices), (void**)&pIndices, 0);
	memcpy(pIndices, indices, sizeof(indices));
	IB->Unlock();
}


//Renderowanie sceny
void TestApp::Render()
{
	m_pDevice3D->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, d3dColors::black, 1.0f, 0);

	m_pDevice3D->BeginScene();

	m_pDevice3D->SetTransform(D3DTS_WORLD, &World);

	m_pDevice3D->SetStreamSource(0, VB, 0, sizeof(VertexPositionColor));
	m_pDevice3D->SetIndices(IB);
	m_pDevice3D->SetFVF(VertexPositionColor::FVF);

	int first_prim = 0;

	//wysy³anie paczkami, pozwala wyœwietliæ wiêcej elementów
	while (currentCube > 0 && currentCube < vbSize)
	{
		int batch_size = 2000;
		int chunk;
		if (currentCube <= batch_size)
			chunk = currentCube;
		else
			chunk = batch_size;

		int curr = currentCube;

		int s = sizeof(indices);

		int fp8 = 8 * first_prim;
		int cb136 = 36 * (batch_size);
		int b8 = 8 * batch_size;

		m_pDevice3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 8 * first_prim, 0, 36 * (chunk), 0, 36 * chunk);


		first_prim += chunk;
		currentCube -= chunk;
	}


	m_pDevice3D->EndScene();

	m_pDevice3D->Present(0, 0, 0, 0);
	currentCube = 0;


	//Zapis wyrenderowanej sceny do pliku
	RECT rect = { 0 };

	GetWindowRect(m_hAppWindow, &rect);
	ATL::CImage* image_ = new CImage();
	image_->Create(rect.right - rect.left, rect.bottom - rect.top, 32);

	HDC device_context_handle = image_->GetDC();
	PrintWindow(m_hAppWindow, device_context_handle, PW_CLIENTONLY);
	std::string path;
	char sb[100];  // You had better have room for what you are sprintf()ing!
	sprintf_s(sb, "C:\\Users\\Dawid\\Pictures\\smoke\\frame%d.jpg", frame_counter);
	path = sb;

	image_->Save(_T(path.c_str()), Gdiplus::ImageFormatBMP);
	frame_counter++;
	image_->ReleaseDC();

	delete image_;
}





inline void swap(float*&p0, float *&p1)
{
	float *t = p0;
	p0 = p1;
	p1 = t;
}



class CFluid
{

public:

	CFluid();
	~CFluid();
	void	Init(int w, int h, int d);

	void ReverseAdvection(float *p_in, float *p_out, float scale);
	void ReverseSignedAdvection(float *p_in, float *p_out, float scale);
	void ForwardAdvection(float *p_in, float *p_out, float scale);
	void PartialForwardAdvection(float *p_in, float *p_out, float scale, float partial);
	void BFECCForwardAdvection(float *p_in, float*p_out, float scale);
	void PressureAcceleration(float force);
	void Diffusion(float *p_in, float *p_out, float scale);
	void VelocityFriction(float a, float b, float c);
	void QuadraticDecay(float *p_in, float *p_out, float a, float b, float c);
	void ClampVelocity(float max_v);
	void Render(float x, float y, float w, float h);
	void Update(float dt);
	//void MouseInput();
	void Reset();
	void ZeroEdge(float *p_in);
	void PushEdge(float *p_in);
	void CopyEdge(float *p_in, float *p_out);
	void CopyField(float *p_in, float *p_out);
	void AddFields(float *p_a, float *p_b, float *p_out);
	void SubFields(float *p_a, float *p_b, float *p_out);
	void AddField(float *p_a, float f, float *p_out);
	void MulField(float *p_a, float f, float *p_out);
	void MulFields(float *p_a, float *p_b, float *p_out);
	void SetField(float *p_a, float f);
	void ZeroField(float *p_field);
	void AddValue(float *p_in, float x, float y, float z, float v);
	float GetValue(float *p_in, float x, float y, float z);
	void SetValue(float *p_in, float x, float y, float z, float v);
	void EdgeVelocities();
	void ForceFrom(float *p_from, float *p_to, float f);
	void ApplyForce(float *p_to, float f);
	void EdgeForce(float *p_to, float left, float right, float top, float bot);
	void RemoveNegative(float *p_out);
	void VorticityConfinement(float scale);
	void Collide(float x, float y, float z, float &x1, float &y1, float &z1);

	bool	m_field_test;

	float m_dt;

	float m_screen_x, m_screen_y, m_screen_w, m_screen_h;

	//int Cell(int x, int y) { return x + m_w *y; }

	int Cell(int x, int y, int z) {

		if (x < 0) {
			x = 0;
		}

		if (y < 0) {
			y = 0;
		}

		if (z < 0) {
			z = 0;
		}


		if (x >= m_w) {
			x = m_w - 1;
		}

		if (y >= m_h) {
			y = m_h - 1;
		}

		if (z >= m_d) {
			z = m_d - 1;
		}
		return x + m_w *y + m_w *m_h*z;
	}

	// Wektory prêdkoœæi dla poszczególnych kierunków przed i po operacji
	float*	mp_xv0;
	float*	mp_xv1;
	float*	mp_xv2;
	float*	mp_yv0;
	float*	mp_yv1;
	float*	mp_yv2;
	float*	mp_zv0;
	float*	mp_zv1;
	float*	mp_zv2;
	// wketory ciœnienia
	float*		mp_p0;
	float*		mp_p1;
	// wketory wype³nienia
	float*		mp_ink0;
	float*		mp_ink1;
	//wartosci temperatury
	float*		mp_heat0;
	float*		mp_heat1;

	// tymczasowe tablice u¿ywane do obliczania adwekcji
	int*		mp_sources;
	float*		mp_source_fractions;
	float*		mp_fraction;
	float*		mp_BFECC;
	float*		mp_balance;

	int m_init;
	int m_last_x;
	int m_last_y;
	int m_last_z;


	// Fizyczne sta³e u¿ywane w obliczeniach
	float m_velocity_diffusion;

	int	m_diffusion_iterations;

	float m_pressure_diffusion;

	float m_heat_diffusion;

	float m_ink_diffusion;

	float m_velocity_friction_a;
	float m_velocity_friction_b;
	float m_velocity_friction_c;

	float m_vorticity;
	float m_pressure_acc;
	float m_ink_heat;
	float m_heat_force;
	float m_heat_friction_a;
	float m_heat_friction_b;
	float m_heat_friction_c;
	float m_ink_advection;

	float m_velocity_advection;
	float m_pressure_advection;
	float m_heat_advection;


};

CFluid	g_fluid1;




#ifdef		CRITICAL_SECTION_RND
CRITICAL_SECTION	rnd_CS;
#endif

//random

unsigned int	rnd()
{

#ifdef		CRITICAL_SECTION_RND
	EnterCriticalSection(&rnd_CS);
#endif
#ifdef	FAST_RND
	static int rnd_a = 12345678;
	static int rnd_b = 12393455;

	rnd_a = rnd_a ^ 0x10010100;
	rnd_a = (rnd_a << 1) | ((rnd_a >> 31) & 1);
	rnd_a ^= rnd_b;
	rnd_b = rnd_b * 255 + 32769;
	unsigned int return_value = rnd_a;
#else

	static int rnd_a = 12345678;
	static int rnd_b = 12393455;
	static int rnd_c = 45432838;

	rnd_a = rnd_a ^ 0x10010100;
	rnd_a = (rnd_a << 1) | ((rnd_a >> 31) & 1);
	rnd_a ^= rnd_b ^ rnd_c;
	rnd_b = rnd_b * 255 + 32769;
	rnd_c = rnd_a + rnd_b + rnd_c + 1;
	unsigned int return_value = rnd_a;
#endif
#ifdef		CRITICAL_SECTION_RND
	LeaveCriticalSection(&rnd_CS);
#endif
	return return_value;
}


int rnd(unsigned int a)
{
	return rnd() % a;
}

float rndf(float f = 1.0f)
{
	return f*(float)rnd() / 4294967296.0f;
}

float rndf(float f1, float f2)
{
	return f1 + (f2 - f1)*(float)rnd() / 4294967296.0f;
}

inline float  scale_x(float x) { return x; }
inline float  scale_y(float y) { return y; }
inline float  scale_z(float z) { return z; }


void	InitFluids();


CFluid::CFluid()
{

}

CFluid::~CFluid()
{
	if (m_w > 0)
	{
		delete mp_xv0;
		delete mp_yv0;
		delete mp_zv0;
		delete mp_yv1;
		delete mp_zv1;
		delete mp_xv1;
		delete mp_yv2;
		delete mp_xv2;
		delete mp_zv2;
		delete mp_p0;
		delete mp_p1;
		delete mp_ink0;
		delete mp_ink1;
		delete mp_heat0;
		delete mp_heat1;
		delete mp_sources;
		delete mp_source_fractions;
		delete mp_fraction;
		delete mp_BFECC;
		delete mp_balance;
	}
}

void	CFluid::Init(int w, int h, int d)
{
	m_w = w;
	m_h = h;
	m_d = d;
	int size = m_w * m_h * m_d;
	mp_xv0 = new float[size];
	mp_yv0 = new float[size];
	mp_zv0 = new float[size];
	mp_xv1 = new float[size];
	mp_yv1 = new float[size];
	mp_zv1 = new float[size];
	mp_xv2 = new float[size];
	mp_yv2 = new float[size];
	mp_zv2 = new float[size];
	mp_p0 = new float[size];
	mp_p1 = new float[size];
	mp_ink0 = new float[size];
	mp_ink1 = new float[size];
	mp_heat0 = new float[size];
	mp_heat1 = new float[size];
	mp_sources = new int[size];
	mp_source_fractions = new float[size * 8];
	mp_fraction = new float[size];
	mp_BFECC = new float[size];
	mp_balance = new float[size];
	Reset();
}

void CFluid::Reset()
{
	int size = m_w * m_h * m_d;
	for (int i = 0; i<size; i++)
	{
		mp_xv0[i] = 0.0f;
		mp_yv0[i] = 0.0f;
		mp_zv0[i] = 0.0f;

		mp_xv1[i] = 0.0f;
		mp_yv1[i] = 0.0f;
		mp_zv1[i] = 0.0f;

		mp_xv2[i] = 0.0f;
		mp_yv2[i] = 0.0f;
		mp_zv2[i] = 0.0f;
		mp_p0[i] = 0.6f;
		mp_p1[i] = 0.0f;
		mp_heat0[i] = 0.00f;
		mp_heat1[i] = 0.00f;
			mp_ink0[i] = 0.00f;  
	}
}


void CFluid::Diffusion(float *p_in, float *p_out, float scale)
{
	// Build the new pressure values in p1, then swap p0 and p1


	float a = m_dt * scale;
	int cell;

	//#endif
	for (int x = 1; x < m_w - 1; x++)
	{
		for (int y = 1; y < m_h - 1; y++)
		{
			for (int z = 1; z < m_d - 1; z++)
			{
				cell = Cell(x, y, z);
				p_out[cell] = p_in[cell] + a * (p_in[Cell(x, y + 1, z)] + p_in[Cell(x, y - 1, z)] + p_in[Cell(x + 1, y, z)] + p_in[Cell(x - 1, y, z)] + p_in[Cell(x, y, z + 1)] + p_in[Cell(x, y, z - 1)] - 6.0f * p_in[cell]);
			}
		}
	}

}



void CFluid::CopyEdge(float *p_in, float *p_out)
{
	for (int x = 0; x<m_w; x++)
	{
		p_out[x] = p_in[x];
		p_out[m_w * (m_h - 1) + x] = p_in[m_w * (m_h - 1) + x];
	}

	for (int y = 1; y<m_h - 1; y++)
	{
		p_out[y * m_w] = p_in[y * m_w];
		p_out[y * m_w + (m_w - 1)] = p_in[y * m_w + (m_w - 1)];
	}
}

void CFluid::CopyField(float *p_in, float *p_out)
{
	int size = m_w * m_h * m_d;


	memcpy(p_out, p_in, size * sizeof(float));


}

void CFluid::AddFields(float *p_a, float *p_b, float *p_out)
{
	int size = m_w * m_h * m_d;
	for (int x = 0; x<size; x++)
	{
		p_out[x] = p_a[x] + p_b[x];
	}
}

void CFluid::SubFields(float *p_a, float *p_b, float *p_out)
{
	int size = m_w * m_h * m_d;
	for (int x = 0; x<size; x++)
	{
		p_out[x] = p_a[x] - p_b[x];
	}
}

void CFluid::MulFields(float *p_a, float *p_b, float *p_out)
{
	int size = m_w * m_h * m_d;
	for (int x = 0; x<size; x++)
	{
		p_out[x] = p_a[x] * p_b[x];
	}
}

void CFluid::MulField(float *p_a, float f, float *p_out)
{
	int size = m_w * m_h * m_d;
	for (int x = 0; x<size; x++)
	{
		p_out[x] = p_a[x] * f;
	}

}

void CFluid::AddField(float *p_a, float f, float *p_out)
{
	int size = m_w * m_h * m_d;
	for (int x = 0; x<size; x++)
	{
		p_out[x] = p_a[x] + f;
	}

}


void CFluid::RemoveNegative(float *p_out)
{
	int size = m_w * m_h * m_d;
	for (int x = 0; x<size; x++)
	{
		if (p_out[x] < 0) {
			p_out[x] = 0;
		}
	}

}



void CFluid::ZeroField(float *p_field)
{
	int size = m_w * m_h * m_d;
	memset((void*)p_field, 0, size * sizeof(float));
}

void CFluid::SetField(float *p_field, float f)
{
	int size = m_w * m_h * m_d;
	for (int x = 0; x<size; x++)
	{
		p_field[x] = f;
	}
}


void CFluid::ZeroEdge(float *p_in)
{
	for (int x = 0; x<m_w; x++)
	{
		p_in[x] = 0;
		p_in[m_w * (m_h - 1) + x] = 0;
	}

	for (int y = 1; y<m_h - 1; y++)
	{
		p_in[y * m_w] = 0;
		p_in[y * m_w + (m_w - 1)] = 0;
	}
}

void CFluid::PushEdge(float *p_in)
{
	for (int x = 0; x<m_w; x++)
	{
		p_in[x + m_w] += p_in[x];
		p_in[m_w * (m_h - 2) + x] += p_in[m_w * (m_h - 1) + x];
	}

	for (int y = 1; y<m_h - 1; y++)
	{
		p_in[y * m_w + 1] += p_in[y * m_w];
		p_in[y * m_w + (m_w - 1) - 1] += p_in[y * m_w + (m_w - 1)];
	}
}

void CFluid::EdgeForce(float *p_in, float left, float right, float top, float bot)
{
	for (int x = 0; x<m_w; x++)
	{
		p_in[x] += top;
		p_in[m_w * (m_h - 1) + x] += bot;
	}

	for (int y = 0; y<m_h; y++)
	{
		p_in[y * m_w] += left;
		p_in[y * m_w + (m_w - 1)] += right;
	}

}

// any velocities that are facing outwards at the edge cells, face them inwards
void CFluid::EdgeVelocities()
{
	for (int y = 0; y<m_h; y++)
	{
		for (int z = 1; z < m_d - 1; z++)
		{
			int left_cell = Cell(0, y, z);
			if (mp_xv0[left_cell] < 0.0f)
			{
				mp_xv0[left_cell] = -mp_xv0[left_cell];
			}
			int right_cell = Cell(m_w - 1, y, z);
			if (mp_xv0[right_cell] > 0.0f)
			{
				mp_xv0[right_cell] = -mp_xv0[right_cell];
			}
		}
	}

	for (int x = 0; x<m_w; x++)
	{
		for (int z = 1; z < m_d - 1; z++)
		{
			int top_cell = Cell(x, 0, z);
			if (mp_yv0[top_cell] < 0.0f)
			{
				mp_yv0[top_cell] = -mp_yv0[top_cell];
			}
			int bot_cell = Cell(x, m_h - 1, z);
			if (mp_yv0[bot_cell] > 0.0f)
			{
				mp_yv0[bot_cell] = -mp_yv0[bot_cell];
			}
		}
	}

}


// metoda wykrywaj¹ca kolizje. W tej wersji kolizje tylko z scianami
inline void CFluid::Collide(float x, float y, float z, float &x1, float &y1, float &z1)
{
	float left_bound = m_w - 1.0001f;
	float bot_bound = m_h - 1.0001f;
	float back_bound = m_d - 1.0001f;


	if (x1<0) x1 = -x1;
	else if (x1>left_bound) x1 = left_bound - (x1 - left_bound);
	if (y1<0) y1 = -y1;
	else if (y1>bot_bound) y1 = bot_bound - (y1 - bot_bound);
	if (z1<0) z1 = -z1;
	else if (z1>back_bound) z1 = back_bound - (z1 - back_bound);

}

//przspieszenie wywo³ane ciœnieniem
void CFluid::PressureAcceleration(float force)
{

//ró¿nica ciœnien
	float a = m_dt * force;


	for (int x = 0; x < m_w; x++)
	{
		for (int y = 0; y < m_h; y++)
		{
			for (int z = 0; z < m_d; z++)
			{
				int cell = Cell(x, y, z);
				mp_xv1[cell] = mp_xv0[cell];
				mp_yv1[cell] = mp_yv0[cell];
				mp_zv1[cell] = mp_zv0[cell];
			}
		}
	}

	
		for (int x = 0; x < m_w - 1; x++)
		{
			for (int y = 0; y < m_h - 1; y++)
			{
				for (int z = 0; z < m_d - 1; z++)
				{
					int cell = Cell(x, y, z);

					float force_x = mp_p0[cell] - mp_p0[Cell(x + 1, y, z)];
					float force_y = mp_p0[cell] - mp_p0[Cell(x, y + 1, z)];
					float force_z = mp_p0[cell] - mp_p0[Cell(x, y, z + 1)];


					mp_xv1[cell] += a * force_x;
					mp_xv1[Cell(x + 1, y, z)] += a * force_x;

					mp_yv1[cell] += a * force_y;
					mp_yv1[Cell(x, y + 1, z)] += a * force_y;

					mp_zv1[cell] += a * force_z;
					mp_zv1[Cell(x, y, z + 1)] += a * force_z;


				}

			}
		}

		float *t = mp_xv1;
		mp_xv1 = mp_xv0;
		mp_xv0 = t;

		t = mp_yv1;
		mp_yv1 = mp_yv0;
		mp_yv0 = t;

		t = mp_zv1;
		mp_zv1 = mp_zv0;
		mp_zv0 = t;
	

}


//rozk³ad wartosci przy u¿ycie funkcji kwadratowej
void CFluid::QuadraticDecay(float *p_in, float *p_out, float a, float b, float c)
{
	float dt = m_dt;
	int size = m_w * m_h * m_d;
	for (int cell = 0; cell<size; cell++)
	{
		float v = p_in[cell];

		float v2 = v*v;
		v -= dt*(a*v2 + b*v + c);
		if (v <0.0f)
			v = 0.0f;
		if (v >50.0f)
			v = 50.0f;

		p_in[cell] = v;
	}
}


//oblicza si³e odzia³ywuj¹c¹ miedzy dwoma komórkami
void CFluid::ForceFrom(float *p_from, float *p_to, float f)
{
	f *= m_dt;
	int size = m_w * m_h * m_d;
	for (int cell = 0; cell < size; cell++)
	{
		*p_to++ += *p_from++*f;
	}
}
//aplikacja podanej si³y
void CFluid::ApplyForce(float *p_to, float f)
{
	f *= m_dt;
	int size = m_w * m_h * m_d;
	for (int cell = 0; cell < size; cell++)
	{
		*p_to++ += f;
	}
}




// Adwekcja wsteczna, w duchu zasady akcja-reakcja

void CFluid::ReverseAdvection(float *p_in, float*p_out, float scale)
{
	// wsteczna wiêc ujemna wartoœæ
	float a = -m_dt * scale;

	int size = m_w*m_h*m_d;

	CopyField(p_in, p_out);

	ZeroField(mp_fraction);

	for (int x = 0; x < m_w; x++)
	{
		for (int y = 0; y < m_h; y++)
		{
			for (int z = 0; z < m_d; z++)
			{
				int cell = Cell(x, y, z);
				float vx = mp_xv0[cell];
				float vy = mp_yv0[cell];
				float vz = mp_zv0[cell];
				if (vx != 0.0f || vy != 0.0f || vz != 0.0f)
				{

					float step = 1.25;
					if (vx > step) {
						vx = step;
					}
					if (vy > step) {
						vy = step;
					}
					if (vz > step) {
						vz = step;
					}
					if (vz < -1 * step) {
						vz = -1 * step;
					}
					if (vx < -1 * step) {
						vx = -1 * step;
					}
					if (vy < -1 * step) {
						vy = -1 * step;
					}
					float x1 = x + vx * a;
					float y1 = y + vy * a;
					float z1 = z + vz * a;
					Collide(x, y, z, x1, y1, z1);
					if (x1 > m_w) x1 = m_w;
					if (y1 > m_h) y1 = m_h;
					if (z1 > m_d) z1 = m_d;


					int cell1 = Cell((int)x1, (int)y1, (int)z1);

					int ix = (int)x1;
					int iy = (int)y1;
					int iz = (int)z1;

					// get fractional parts
					float fx = x1 - (int)x1;
					float fy = y1 - (int)y1;
					float fz = z1 - (int)z1;

				//odleg³oœæ wierzcho³ka komórki od miejsca w przestrzeni
					float ixyz = (1.0f - fx)*(1.0f - fy)*(1.0f - fz);
					float ixyz1 = (1.0f - fx)*(1.0f - fy)*(fz);
					float ixy1z = (1.0f - fx)*(fy)*(1.0f - fz);
					float ix1yz = (fx)*(1.0f - fy)*(1.0f - fz);

					float ixy1z1 = (1.0f - fx)*(fy)*(fz);
					float ix1yz1 = (fx)*(1.0f - fy)*(fz);
					float ix1y1z = (fx)*(fy)*(1.0f - fz);
					float ix1y1z1 = (fx)*(fy)*(fz);
					
					mp_sources[cell] = cell1;  
					{

						mp_source_fractions[cell * 8 + 0] = ixyz;
						mp_source_fractions[cell * 8 + 1] = ixyz1;
						mp_source_fractions[cell * 8 + 2] = ixy1z;
						mp_source_fractions[cell * 8 + 3] = ix1yz;
						mp_source_fractions[cell * 8 + 4] = ixy1z1;
						mp_source_fractions[cell * 8 + 5] = ix1yz1;
						mp_source_fractions[cell * 8 + 6] = ix1y1z;
						mp_source_fractions[cell * 8 + 7] = ix1y1z1;

						int zzz = Cell(ix, iy, iz);

						mp_fraction[Cell(ix, iy, iz)] += ixyz;
						mp_fraction[Cell(ix, iy, iz + 1)] += ixyz1;
						mp_fraction[Cell(ix, iy + 1, iz)] += ixy1z;
						mp_fraction[Cell(ix + 1, iy, iz)] += ix1yz;
						mp_fraction[Cell(ix, iy + 1, iz + 1)] += ixy1z1;
						mp_fraction[Cell(ix + 1, iy, iz + 1)] += ix1yz1;
						mp_fraction[Cell(ix + 1, iy + 1, iz)] += ix1y1z;
						mp_fraction[Cell(ix + 1, iy + 1, iz + 1)] += ix1y1z1;
					}
				}
				else
				{
					mp_sources[cell] = -1;  
				}
			}
		}
	}

	// 

	for (int cell = 0; cell<size; cell++)
	{
		// pobranie indeksu komórki
		int cell1 = mp_sources[cell];
		if (cell1 != -1)
		{
			// Pobranie wczeœniej wyliczonych przybli¿en


			float ixyz = mp_source_fractions[cell * 8 + 0];
			float ixyz1 = mp_source_fractions[cell * 8 + 1];
			float ixy1z = mp_source_fractions[cell * 8 + 2];
			float ix1yz = mp_source_fractions[cell * 8 + 3];
			float ixy1z1 = mp_source_fractions[cell * 8 + 4];
			float ix1yz1 = mp_source_fractions[cell * 8 + 5];
			float ix1y1z = mp_source_fractions[cell * 8 + 6];
			float ix1y1z1 = mp_source_fractions[cell * 8 + 7];


	//obliczenie wspó³rzednych X Y Z
			int cell2 = cell1;

			int iz = cell2 / (m_w*m_h);
			cell2 %= m_d*m_w;
			int ix = cell2 % m_w;
			cell2 /= m_w;
			int iy = cell2;


			float fxyz = mp_fraction[Cell(ix, iy, iz)];
			float fxyz1 = mp_fraction[Cell(ix, iy, iz + 1)];
			float fxy1z = mp_fraction[Cell(ix, iy + 1, iz)];
			float fx1yz = mp_fraction[Cell(ix + 1, iy, iz)];
			float fxy1z1 = mp_fraction[Cell(ix, iy + 1, iz + 1)];
			float fx1yz1 = mp_fraction[Cell(ix + 1, iy, iz + 1)];
			float fx1y1z = mp_fraction[Cell(ix + 1, iy + 1, iz)];
			float fx1y1z1 = mp_fraction[Cell(ix + 1, iy + 1, iz + 1)];


			if (fxyz   <1.0f) fxyz = 1.0f;
			if (fxyz1  <1.0f) fxyz1 = 1.0f;
			if (fxy1z  <1.0f) fxy1z = 1.0f;
			if (fx1yz  <1.0f) fx1yz = 1.0f;
			if (fxy1z1 <1.0f) fxy1z1 = 1.0f;
			if (fx1yz1 <1.0f) fx1yz1 = 1.0f;
			if (fx1y1z <1.0f) fx1y1z = 1.0f;
			if (fx1y1z1<1.0f) fx1y1z1 = 1.0f;

			ixyz /= fxyz;
			ixyz1 /= fxyz1;
			ixy1z /= fxy1z;
			ix1yz /= fx1yz;
			ixy1z1 /= fxy1z1;
			ix1yz1 /= fx1yz1;
			ix1y1z /= fx1y1z;
			ix1y1z1 /= fx1y1z1;


			p_out[cell] += (ixyz		  * p_in[Cell(ix, iy, iz)] +
				ixyz1				  * p_in[Cell(ix, iy, iz + 1)] +
				ixy1z				  * p_in[Cell(ix, iy + 1, iz)] +
				ix1yz				  * p_in[Cell(ix + 1, iy, iz)] +
				ixy1z1				  * p_in[Cell(ix, iy + 1, iz + 1)] +
				ix1yz1				  * p_in[Cell(ix + 1, iy, iz + 1)] +
				ix1y1z				  * p_in[Cell(ix + 1, iy + 1, iz)] +
				ix1y1z1				  * p_in[Cell(ix + 1, iy + 1, iz + 1)]);


			p_out[Cell(ix, iy, iz)] -= ixyz								 * p_in[Cell(ix, iy, iz)];
			p_out[Cell(ix, iy, iz + 1)] -= ixyz1						 * p_in[Cell(ix, iy, iz + 1)];
			p_out[Cell(ix, iy + 1, iz)] -= ixy1z						 * p_in[Cell(ix, iy + 1, iz)];
			p_out[Cell(ix + 1, iy, iz)] -= ix1yz						 * p_in[Cell(ix + 1, iy, iz)];
			p_out[Cell(ix, iy + 1, iz + 1)] -= ixy1z1					 * p_in[Cell(ix, iy + 1, iz + 1)];
			p_out[Cell(ix + 1, iy, iz + 1)] -= ix1yz1					 * p_in[Cell(ix + 1, iy, iz + 1)];
			p_out[Cell(ix + 1, iy + 1, iz)] -= ix1y1z					 * p_in[Cell(ix + 1, iy + 1, iz)];
			p_out[Cell(ix + 1, iy + 1, iz + 1)] -= ix1y1z1				 * p_in[Cell(ix + 1, iy + 1, iz + 1)];

		}
	}
}

//szybsza metoda wstecznej adwekcji
void CFluid::ReverseSignedAdvection(float *p_in, float*p_out, float scale)
{

	float a = -m_dt * scale;

	int size = m_w*m_h*m_d;

	CopyField(p_in, p_out);

	for (int x = 0; x < m_w; x++)
	{
		for (int y = 0; y < m_h; y++)
		{
			for (int z = 0; z < m_d; z++)
			{
				int cell = Cell(x, y, z);
				float vx = mp_xv0[cell];
				float vy = mp_yv0[cell];
				float vz = mp_zv0[cell];
				float step = 1.25;
				if (vx > step) {
					vx = step;
				}
				if (vy > step) {
					vy = step;
				}
				if (vz > step) {
					vz = step;
				}
				if (vz < -1 * step) {
					vz = -1 * step;
				}
				if (vx < -1 * step) {
					vx = -1 * step;
				}
				if (vy < -1 * step) {
					vy = -1 * step;
				}
				if (vx != 0.0f || vy != 0.0f || vz != 0.0f)
				{
					float x1 = x + vx * a;
					float y1 = y + vy * a;
					float z1 = z + vz * a;
					Collide(x, y, z, x1, y1, z1);
					int cell1 = Cell((int)x1, (int)y1, (int)z1);

					float fx = x1 - (int)x1;
					float fy = y1 - (int)y1;
					float fz = z1 - (int)z1;

					int ix = (int)x1;
					int iy = (int)y1;
					int iz = (int)z1;

					float ixyz = (1.0f - fx)*(1.0f - fy)*(1.0f - fz) * p_in[Cell(ix, iy, iz)];
					float ixyz1 = (1.0f - fx)*(1.0f - fy)*(fz)*        p_in[Cell(ix, iy, iz + 1)];
					float ixy1z = (1.0f - fx)*(fy)*(1.0f - fz)*        p_in[Cell(ix, iy + 1, iz)];
					float ix1yz = (fx)*(1.0f - fy)*(1.0f - fz)*        p_in[Cell(ix + 1, iy, iz)];
					float ixy1z1 = (1.0f - fx)*(fy)*(fz)*              p_in[Cell(ix, iy + 1, iz + 1)];
					float ix1yz1 = (fx)*(1.0f - fy)*(fz)*              p_in[Cell(ix + 1, iy, iz + 1)];
					float ix1y1z = (fx)*(fy)*(1.0f - fz) *             p_in[Cell(ix + 1, iy + 1, iz)];
					float ix1y1z1 = (fx)*(fy)*(fz)*                    p_in[Cell(ix + 1, iy + 1, iz + 1)];


					p_out[cell] += (ixyz + ixyz1 + ixy1z + ix1yz + ix1yz + ix1yz1 + ix1y1z + ix1y1z1);
				
					p_out[Cell(ix, iy, iz)] -= ixyz;
					p_out[Cell(ix, iy, iz + 1)] -= ixyz1;
					p_out[Cell(ix, iy + 1, iz)] -= ixy1z;
					p_out[Cell(ix + 1, iy, iz)] -= ix1yz;
					p_out[Cell(ix, iy + 1, iz + 1)] -= ixy1z1;
					p_out[Cell(ix + 1, iy, iz + 1)] -= ix1yz1;
					p_out[Cell(ix + 1, iy + 1, iz)] -= ix1y1z;
					p_out[Cell(ix + 1, iy + 1, iz + 1)] -= ix1y1z1;
				}
			}
		}
	}
}


// operacja adwekcji
void CFluid::ForwardAdvection(float *p_in, float*p_out, float scale)
{
	float a = m_dt * scale;

	int w = m_w;
	int h = m_h;
	int d = m_d;

	CopyField(p_in, p_out);

	if (scale == 0.0f)
		return;

	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			for (int z = 0; z < d; z++)
			{
				int cell = Cell(x, y, z);
				float vx = mp_xv0[cell];
				float vy = mp_yv0[cell];
				float vz = mp_zv0[cell];


				if (vx != 0.0f || vy != 0.0f || vz != 0.0f)
				{
					float x1 = x + vx * a;
					float y1 = y + vy * a;
					float z1 = z + vz * a;
					Collide(x, y, z, x1, y1, z1);

					float step = 1.25;
					if (vx > step) {
						vx = step;
					}
					if (vy > step) {
						vy = step;
					}
					if (vz > step) {
						vz = step;
					}
					if (vz < -1 * step) {
						vz = -1 * step;
					}
					if (vx < -1 * step) {
						vx = -1 * step;
					}
					if (vy < -1 * step) {
						vy = -1 * step;
					}
					int ix = (int)x1;
					int iy = (int)y1;
					int iz = (int)z1;

					float fx = x1 - (int)x1;
					float fy = y1 - (int)y1;
					float fz = z1 - (int)z1;

					float in = p_in[cell];


					//wartoœæ dla ka¿dego z 8 wierzcho³ków proporcjonalnie do odleg³oœci w przestrzeni
					float ixyz = (1.0f - fx)*(1.0f - fy)*(1.0f - fz) * in;
					float ixyz1 = (1.0f - fx)*(1.0f - fy)*(fz)* in;
					float ixy1z = (1.0f - fx)*(fy)*(1.0f - fz)* in;
					float ix1yz = (fx)*(1.0f - fy)*(1.0f - fz)* in;

					float ixy1z1 = (1.0f - fx)*(fy)*(fz)* in;
					float ix1yz1 = (fx)*(1.0f - fy)*(fz)* in;
					float ix1y1z = (fx)*(fy)*(1.0f - fz) * in;
					float ix1y1z1 = (fx)*(fy)*(fz)* in;


					// odjêta wartoœæ od przetwarzane elementu
					p_out[cell] -= (ixyz + ixyz1 + ixy1z + ix1yz + ix1yz + ix1yz1 + ix1y1z + ix1y1z1);
					// i dodanie zabranych wartosci do poszczegó³nych komórek
					p_out[Cell(ix, iy, iz)] += ixyz;
					p_out[Cell(ix, iy, iz + 1)] += ixyz1;
					p_out[Cell(ix, iy + 1, iz)] += ixy1z;
					p_out[Cell(ix + 1, iy, iz)] += ix1yz;
					p_out[Cell(ix, iy + 1, iz + 1)] += ixy1z1;
					p_out[Cell(ix + 1, iy, iz + 1)] += ix1yz1;
					p_out[Cell(ix + 1, iy + 1, iz)] += ix1y1z;
					p_out[Cell(ix + 1, iy + 1, iz + 1)] += ix1y1z1;

				}
			}
		}
	}
}



// dodanie wartosci do dystretnych wierzcho³kow proprcjonalnie do rzeczywsitej odleg³oœæi
void CFluid::AddValue(float *p_in, float x, float y, float z, float v)
{
	if (x<0 || y<0 || z<0 || x>(float)m_w - 1.0001f || y>(float)m_h - 1.0001f || z>(float)m_d - 1.0001f)
		return;


	float fx = x - (int)x;
	float fy = y - (int)y;
	float fz = z - (int)z;
	int cell = Cell((int)x, (int)y, (int)z);

	int ix = (int)x;
	int iy = (int)y;
	int iz = (int)z;

	float ixyz = (1.0f - fx)*(1.0f - fy)*(1.0f - fz) * v;
	float ixyz1 = (1.0f - fx)*(1.0f - fy)*(fz)* v;
	float ixy1z = (1.0f - fx)*(fy)*(1.0f - fz)* v;
	float ix1yz = (fx)*(1.0f - fy)*(1.0f - fz)* v;
	float ixy1z1 = (1.0f - fx)*(fy)*(fz)* v;
	float ix1yz1 = (fx)*(1.0f - fy)*(fz)* v;
	float ix1y1z = (fx)*(fy)*(1.0f - fz) * v;
	float ix1y1z1 = (fx)*(fy)*(fz)* v;


	p_in[Cell(ix, iy, iz)] += ixyz;
	p_in[Cell(ix, iy, iz + 1)] += ixyz1;
	p_in[Cell(ix, iy + 1, iz)] += ixy1z;
	p_in[Cell(ix + 1, iy, iz)] += ix1yz;
	p_in[Cell(ix, iy + 1, iz + 1)] += ixy1z1;
	p_in[Cell(ix + 1, iy, iz + 1)] += ix1yz1;
	p_in[Cell(ix + 1, iy + 1, iz)] += ix1y1z;
	p_in[Cell(ix + 1, iy + 1, iz + 1)] += ix1y1z1;


}






// iteracja po wszystkich komórkach i ich wyrenderowanie
void CFluid::Render(float x, float y, float w, float h)
{

	float tot_p = 0;

	float wx = w / m_w;
	float wy = h / m_h;

	float *p_ink = mp_ink0;
	float scale = 255.0f; 


	for (int x0 = 0; x0<m_w; x0++)
	{
		for (int y0 = 0; y0<m_h; y0++)
		{
			for (int z0 = 0; z0 < m_d; z0++)
			{
				int cell = Cell(x0, y0, z0);

				if ((x0 < m_w - 1 && y0 < m_h - 1 && z0 < m_d - 1))
				{
					unsigned int base = 0xff000000;
					int p0 = scale * p_ink[cell];
					if (p0 > 255)
						p0 = 255;
					if (p0 < 0)
						p0 = 0;
				
					drawCube(x0, y0, z0, D3DCOLOR_ARGB(p0, 255, 255, 255));


				}
			}
		}
	}
}


// Metoda obliczaj¹ca stan w nastêpnym kroku na podstawie poprzedniego, serce ca³ego algorytmu
void CFluid::Update(float dt)
{
	//ró¿nica czasu pomiêdzy krokami
	m_dt = dt;


	//Dyfuzje dla posczególnych sk³adowych
	if (m_velocity_diffusion != 0.0f)
	{
		for (int i = 0; i<m_diffusion_iterations; i++)
		{
			Diffusion(mp_xv0, mp_xv1, m_velocity_diffusion / (float)m_diffusion_iterations);
			swap(mp_xv0, mp_xv1);
			Diffusion(mp_yv0, mp_yv1, m_velocity_diffusion / (float)m_diffusion_iterations);
			swap(mp_yv0, mp_yv1);
			Diffusion(mp_zv0, mp_zv1, m_velocity_diffusion / (float)m_diffusion_iterations);
			swap(mp_yv0, mp_yv1);
		}
	}


	if (m_pressure_diffusion != 0.0f)
	{
		for (int i = 0; i<m_diffusion_iterations; i++)
		{
			Diffusion(mp_p0, mp_p1, m_pressure_diffusion / (float)m_diffusion_iterations);
			swap(mp_p0, mp_p1);
		}
	}
	if (m_heat_diffusion != 0.0f)
	{
		for (int i = 0; i<m_diffusion_iterations; i++)
		{
			Diffusion(mp_heat0, mp_heat1, m_heat_diffusion / (float)m_diffusion_iterations);
			swap(mp_heat0, mp_heat1);

		}
	}

	if (m_ink_diffusion != 0.0f)
	{
		for (int i = 0; i<m_diffusion_iterations; i++)
		{
			Diffusion(mp_ink0, mp_ink1, m_ink_diffusion / (float)m_diffusion_iterations);
			swap(mp_ink0, mp_ink1);
		}
	}


	// wp³yw temperatury na unoszenie siê dymu
	ForceFrom(mp_heat0, mp_yv0, m_heat_force);

	// naturalny rozk³ad ciep³a za pomoc¹ funkcji kwadratowej
	QuadraticDecay(mp_heat0, mp_heat0, m_heat_friction_a, m_heat_friction_b, m_heat_friction_c);


	//wp³yw ciœnienia (iloœæi dymu w komórce) na s¹siaduj¹ce komórki
	PressureAcceleration(m_pressure_acc);

	//skala si³y adwekcji
	//float advection_scale = m_w*m_h*m_d / 10000.f;
	float advection_scale = 50;



	//zerowanie macierzy iloœæi dymu w nastêpnym kroku
	SetField(mp_ink1, 1.0f);
	ForwardAdvection(mp_ink1, mp_balance, m_ink_advection * advection_scale);


	//Adwekcji naturalne i wsteczne dla poszczególnych sk³adowych
	ForwardAdvection(mp_ink0, mp_ink1, m_ink_advection * advection_scale);
	swap(mp_ink0, mp_ink1);
	//usuwanie wartosci ujemych postwa³ych na skutek b³êdów numerycznych
	RemoveNegative(mp_ink0);
	ReverseAdvection(mp_ink0, mp_ink1, m_ink_advection * advection_scale);
	swap(mp_ink0, mp_ink1);
	RemoveNegative(mp_ink0);

	ForwardAdvection(mp_heat0, mp_heat1, m_heat_advection * advection_scale);
	swap(mp_heat0, mp_heat1);
	ReverseAdvection(mp_heat0, mp_heat1, m_heat_advection * advection_scale);
	swap(mp_heat0, mp_heat1);


	ForwardAdvection(mp_xv0, mp_xv1, m_velocity_advection * advection_scale);
	ForwardAdvection(mp_yv0, mp_yv1, m_velocity_advection * advection_scale);
	ForwardAdvection(mp_zv0, mp_zv1, m_velocity_advection * advection_scale);


	ReverseSignedAdvection(mp_xv1, mp_xv2, m_velocity_advection * advection_scale);
	ReverseSignedAdvection(mp_yv1, mp_yv2, m_velocity_advection * advection_scale);
	ReverseSignedAdvection(mp_zv1, mp_zv2, m_velocity_advection * advection_scale);
	swap(mp_xv2, mp_xv0);
	swap(mp_yv2, mp_yv0);
	swap(mp_zv2, mp_zv0);

	ForwardAdvection(mp_p0, mp_p1, m_pressure_advection * advection_scale);
	swap(mp_p0, mp_p1);
	
	RemoveNegative(mp_p0);
	ReverseAdvection(mp_p0, mp_p1, m_pressure_advection * advection_scale);
	swap(mp_p0, mp_p1);
	RemoveNegative(mp_p0);
}

//inicjalizcja dymu
void InitFluids()
{

	memset(&g_fluid1, 0, sizeof(CFluid));


	g_fluid1.m_diffusion_iterations = 1;

	g_fluid1.m_velocity_diffusion = 0.0f; 
	g_fluid1.m_pressure_diffusion = 0.0f; 

	g_fluid1.m_heat_diffusion = 0.0f; 

	g_fluid1.m_ink_diffusion = 0.0f;

	g_fluid1.m_velocity_friction_a = 0.0f;
	g_fluid1.m_velocity_friction_b = 0.0f;
	g_fluid1.m_velocity_friction_c = 0.0f;


	g_fluid1.m_vorticity = 0.0f;
	g_fluid1.m_pressure_acc = 0.02; 
	g_fluid1.m_ink_heat = 0.0f;
	g_fluid1.m_heat_force = -0.1f; 
	g_fluid1.m_heat_friction_a = 1.5f;
	g_fluid1.m_heat_friction_b = 0.2f;
	g_fluid1.m_heat_friction_c = 0.01f;
	g_fluid1.m_ink_advection = 150.0f;
	g_fluid1.m_velocity_advection = 150.0f;
	g_fluid1.m_pressure_advection = 150.0f;  
	g_fluid1.m_heat_advection = 150.0f;


	g_fluid1.m_field_test = 0;

	g_fluid1.Init(sizeX, sizeY, sizeZ);


}

void TestApp::Update(float dt)
{
	currentCube = 0;



	//generator dymu
	g_fluid1.AddValue(g_fluid1.mp_ink0, generatorX, generatorY, generatorZ, generatorInk);
	g_fluid1.AddValue(g_fluid1.mp_heat0, generatorX, generatorY, generatorZ, generatorHeat);

	g_fluid1.AddValue(g_fluid1.mp_ink0, generatorX +1, generatorY +1, generatorZ +1, generatorInk);
	g_fluid1.AddValue(g_fluid1.mp_heat0, generatorX +1, generatorY +1, generatorZ +1, generatorHeat);

	g_fluid1.AddValue(g_fluid1.mp_ink0, generatorX + 2, generatorY + 2, generatorZ + 2, generatorInk);
	g_fluid1.AddValue(g_fluid1.mp_heat0, generatorX + 2, generatorY + 2, generatorZ + 2, generatorHeat);


	g_fluid1.AddValue(g_fluid1.mp_ink0, generatorX + 1, generatorY + 2, generatorZ + 3, generatorInk);
	g_fluid1.AddValue(g_fluid1.mp_heat0, generatorX + 1, generatorY + 2, generatorZ + 3, generatorHeat);


	if (!key_a) {
		g_fluid1.Update(0.03f);
	}



	g_fluid1.Render(0, 0, size, size);


	updateBuffers();



	if (key_left) {
		rotY -= 0.1F;
	}
	if (key_right) {
		rotY += 0.1F;
	}
	if (key_up) {
		rotX -= 0.1F;
	}
	if (key_down) {
		rotX += 0.1F;
	}
	//rotX += 0.001f;

	D3DXMatrixRotationX(&Rx, rotX);
	D3DXMatrixRotationY(&Ry, rotY);

	World = Rx * Ry;

}

//metoda startowa
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLinte, int nCmdShow)
{
	TestApp* tApp = new TestApp(hInstance);

	if (!tApp->Init()) {
		return 1;
	}




	InitFluids();

	return tApp->Run();


}

