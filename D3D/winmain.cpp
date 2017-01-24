#include <Windows.h>
#include <atlimage.h>
#include "D3Dapp.h"

int	key_left = 0;
int key_right = 0;
int key_up = 0;
int key_down = 0;
int key_w = 0;
int key_a = 0;
int key_s = 0;
int key_d = 0;
int key_space = 0;

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

const DWORD VertexPositionColor::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX0 ;

class TestApp : public D3DApp
{
public:
	TestApp(HINSTANCE);
	~TestApp();

	bool Init() override;
	void Update(float dt) override;
	void Render() override;
};

IDirect3DVertexBuffer9* VB;
IDirect3DIndexBuffer9* IB;
D3DXMATRIX World;
//rozmiar
int const size = 75;
int const vbSize = size *size *size;
VertexPositionColor  verts[8 * vbSize];
WORD indices[36* vbSize];
int currentCube = 0;

VOID* pVerts;
VOID* pIndices;

TestApp::TestApp(HINSTANCE hInstance):D3DApp(hInstance)
{

}

TestApp::~TestApp()
{

}


void drawCube(float xx, float yy, float zz, D3DCOLOR color) {

	yy = yy * -1;

	int curr = currentCube;
	int vertInd = 8 *  currentCube;
	verts[vertInd]     = VertexPositionColor(xx, yy + 1, zz, color);
	verts[vertInd + 1] = VertexPositionColor(xx + 1, yy + 1, zz, color);
	verts[vertInd + 2] = VertexPositionColor(xx , yy, zz, color);
	verts[vertInd + 3] = VertexPositionColor(xx +1 , yy, zz, color);
		
	verts[vertInd + 4] = VertexPositionColor(xx, yy + 1, zz +1, color);
	verts[vertInd + 5] = VertexPositionColor(xx + 1, yy + 1, zz +1, color);
	verts[vertInd + 6] = VertexPositionColor(xx, yy, zz +1, color);
	verts[vertInd + 7] = VertexPositionColor(xx + 1, yy, zz +1, color);


	int indiciesInd = 36 * currentCube;
	indices[indiciesInd + 0] = 0 +  vertInd;
	indices[indiciesInd + 1] = 1 +  vertInd;
	indices[indiciesInd + 2] = 2 +  vertInd;
	indices[indiciesInd + 3] = 2 +  vertInd;
	indices[indiciesInd + 4] = 1 +  vertInd;
	indices[indiciesInd + 5] = 3 +  vertInd;
	indices[indiciesInd + 6] = 5 +  vertInd;
	indices[indiciesInd + 7] = 4 +  vertInd;
	indices[indiciesInd + 8] = 7 +  vertInd;
	indices[indiciesInd + 9] = 7 +  vertInd;
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
	if (currentCube >= size*size*size) {
		int d = currentCube;
	}
	else {

		currentCube++;
	}
}

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

	D3DXVECTOR3 position = D3DXVECTOR3(2*size, -size /8, 2*size);
	D3DXVECTOR3 target = D3DXVECTOR3(size /2, -size /2 , size / 2);
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

void updateBuffers() {
	
	VB->Lock(0, sizeof(verts), (void**)&pVerts, 0);
	memcpy(pVerts, verts, sizeof(verts));
	VB->Unlock();


	IB->Lock(0, sizeof(indices), (void**)&pIndices, 0);
	memcpy(pIndices, indices, sizeof(indices));
	IB->Unlock();
}

float rotY;
float rotX;
int frame_counter = 1;

D3DXMATRIX Rx, Ry;


void TestApp::Render()
{

	m_pDevice3D->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, d3dColors::black, 1.0f, 0);

	m_pDevice3D->BeginScene();

	m_pDevice3D->SetTransform(D3DTS_WORLD, &World);  

	m_pDevice3D->SetStreamSource(0, VB, 0, sizeof(VertexPositionColor));
	m_pDevice3D->SetIndices(IB);
	m_pDevice3D->SetFVF(VertexPositionColor::FVF);

	int first_prim = 0;
	while (currentCube > 0 && currentCube < size*size*size)
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

 		m_pDevice3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,8 * first_prim, 0, 36 * (chunk), 0, 36 * chunk);


		first_prim += chunk;
		currentCube -= chunk;
	}
	//m_pDevice3D->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 0 , 0, size*size*size*36);


	m_pDevice3D->EndScene();

	m_pDevice3D->Present(0, 0, 0, 0);
	currentCube = 0;

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

	image_->Save(_T(path.c_str()), Gdiplus::ImageFormatJPEG);
	frame_counter++;
	image_->ReleaseDC();

	delete image_;
}



float g_friction = 1.0f;


#pragma warning(disable: 4995)          // don't warn about deprecated functions
#define _CRT_SECURE_NO_DEPRECATE		// ditto




void    debug_log(const char* text, ...);



int has_focus = 1;


bool    g_resize = false;

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

CRITICAL_SECTION	debug_CS;

#if 1
inline void swap(float*&p0, float *&p1)
{
	float *t = p0;
	p0 = p1;
	p1 = t;
}
#else
#define swap(A,B) {float *t=A;A=B;B=t;}
#endif


bool	dragging = 0;
bool	r_dragging = 0;
int		mouse_x = 0;
int		mouse_y = 0;



int	m_w;
int	m_h;
int	m_d;

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
	void GlobalDiffusion(float *p_in, float *p_out, float scale);
	void DiffusionLR(float *p_in, float *p_out, float scale);
	void DiffusionUD(float *p_in, float *p_out, float scale);
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
	float GetValue(float *p_in, float x, float y,  float z);
	void SetValue(float *p_in, float x, float y, float z, float v);
	void EdgeVelocities();
	void ForceFrom(float *p_from, float *p_to, float f);
	void ApplyForce(float *p_to, float f);
	void EdgeForce(float *p_to, float left, float right, float top, float bot);
	float Curl(int i, int j, int k);
	void VorticityConfinement(float scale);
	void Collide(float x, float y, float z, float &x1, float &y1, float &z1);

	bool	m_field_test;

	// width and height
	
	int	m_d;
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



		//if (x >= m_w - 1) {
		//	x = m_w - 2;
		//}

		//if (y >= m_h - 1) {
		//	y = m_h - 2;
		//}

		//if (z >= m_d - 1) {
		//	z = m_d - 2;
		//}











		if (x >= m_w) {
			x = m_w-1;
		}

		if (y >= m_h) {
			y = m_h-1;
		}

		if (z >= m_d) {
			z = m_d-1;
		}
		return x + m_w *y + m_w *m_h*z; }

	// Old and new x and y velocities
	float*	mp_xv0;
	float*	mp_xv1;
	float*	mp_xv2;
	float*	mp_yv0;
	float*	mp_yv1;
	float*	mp_yv2;
	float*	mp_zv0;
	float*	mp_zv1;
	float*	mp_zv2;
	// Old and new pressures
	float*		mp_p0;
	float*		mp_p1;
	// Old and new ink
	float*		mp_ink0;
	float*		mp_ink1;

	float*		mp_heat0;
	float*		mp_heat1;

	// temp accumulators of data for advection
	int*		mp_sources;
	float*		mp_source_fractions;
	float*		mp_fraction;

	// BFECC needs its own buffer
	// can't reuse other, since they might be used by the advection step
	float*		mp_BFECC;

	float*		mp_balance;

	int m_init;
	int m_last_x;
	int m_last_y;
	int m_last_z;


	// "Physical" constants
	float m_velocity_diffusion;

	int	m_diffusion_iterations;

	// With no pressure diffusion, waves of pressure keep moving
	// But only if pressure is advected ahead of velocity
	float m_pressure_diffusion;

	float m_heat_diffusion;

	float m_ink_diffusion;

	// friction applied to velocity.  As a simple fraction each time step
	// so 1.0f will give you no friction

	// Friction is very important to the realism of the simulation
	// changes to these coeffficinets will GREATLY affect the apperence of the simulation.
	// the high factor friction (a) wich is applied to the square of the velocity,
	// acts as a smooth limit to velocity, which causes streams to break up into turbulent flow 
	// The c term will make things stop, which then shows up our border artefacts
	// but a high enough (>0.01) c term is needed to stop things just dissipating too fast
	// Friction is dt*(a*v^2 + b*v + c)
	float m_velocity_friction_a;
	float m_velocity_friction_b;
	float m_velocity_friction_c;
	// Pressure accelleration.  Larger values (>10) are more realistic (like water)
	// values that are too large lead to chaotic waves
	float m_vorticity;
	float m_pressure_acc;
	float m_ink_heat;
	float m_heat_force;
	float m_heat_friction_a;
	float m_heat_friction_b;
	float m_heat_friction_c;
	// high ink advection allows fast moving nice swirlys
	float m_ink_advection;
	// seems nice if both velocity and pressure at same value, which makes sense
	float m_velocity_advection;
	float m_pressure_advection;
	float m_heat_advection;


};

CFluid	g_fluid1;




#ifdef		CRITICAL_SECTION_RND
CRITICAL_SECTION	rnd_CS;
#endif

////
// since rand is weak, I implement my own random numbers

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

// return random number in the range 0 .. a-1 
int rnd(unsigned int a)
{
	return rnd() % a;
}

// return random number in the range 0 .. f
float rndf(float f = 1.0f)
{
	return f*(float)rnd() / 4294967296.0f;
}

float rndf(float f1, float f2)
{
	return f1 + (f2 - f1)*(float)rnd() / 4294967296.0f;
}




//inline float  scale_x(float x) {return x * (float)g_window_width / (float) g_viewport_width;}
//inline float  scale_y(float y) {return y * (float)g_window_height / (float) g_viewport_height;}
inline float  scale_x(float x) { return x; }
inline float  scale_y(float y) { return y; }
inline float  scale_z(float z) { return z; }


// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

static 	LARGE_INTEGER Freq;
static 	LARGE_INTEGER BaseTime;
static 	LARGE_INTEGER PausedTime;
static 	LARGE_INTEGER UnPausedTime;


void Timer_Reset()
{
	QueryPerformanceCounter(&BaseTime);
}

void Timer_Init()
{
	QueryPerformanceFrequency(&Freq);
	Timer_Reset();
}

float Timer_Seconds()
{
	static 	LARGE_INTEGER Time;
	QueryPerformanceCounter(&Time);
	return (float)(Time.QuadPart - BaseTime.QuadPart) / (float)(Freq.QuadPart);
}

void Timer_Pause()
{
	QueryPerformanceCounter(&PausedTime);
}
void Timer_Resume()
{
	QueryPerformanceCounter(&UnPausedTime);
	// adjust the base time by the time we have been paused 
	BaseTime.QuadPart += (UnPausedTime.QuadPart - PausedTime.QuadPart);
}


const int max_strings = 200;
const int max_string_length = 255;
struct SDrawText {
	float x, y;
	char text[max_string_length + 1];
	DWORD color;
};

int num_draw_strings;

SDrawText   texts_to_draw[max_strings];
void    DrawString(float x, float y, const char *p_text, DWORD color = 0xff000000)
{
	if (num_draw_strings == max_strings)
		return;
	texts_to_draw[num_draw_strings].x = scale_x(x);
	texts_to_draw[num_draw_strings].y = scale_y(y);
	texts_to_draw[num_draw_strings].color = color;
	strncpy_s(texts_to_draw[num_draw_strings].text, p_text, max_string_length);
	texts_to_draw[num_draw_strings].text[max_string_length] = '\0'; // NULL terminator for iff p_text is >255 chars
	num_draw_strings++;

}


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


	// add a blob for initial tests

	for (int x = 10; x<20; x++)
	{
		for (int y = 10; y<20; y++)
		{
			//	if (x == 15 && y == 15)
			//		mp_p0[Cell(x,y)] = 1;
			//	else
			//		mp_p0[Cell(x,y)] = 1/sqrtf((15-x)*(15-x) + (15-y)*(15-y));
		}
		//	mp_p0[Cell(30,x)] = 1;
	}
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
		mp_p0[i] = 0.6f; // should be 1.0f, lower is more like gas, higher less compressible
		mp_p1[i] = 0.0f;
		mp_heat0[i] = 0.00f;
		mp_heat1[i] = 0.00f;
		if (i>m_w*(m_h / 2))
			mp_ink0[i] = 0.00f;  // even spread of initial ink
		else
			mp_ink0[i] = 0.00f;  // even spread of initial ink
	}
}

/*
Cells adjacent to border cells have a different diffusion.  They still take in the four surrounding cells
but since the border cells are not updated, the adjacent cells should emit less.
We could use a lookup table for this
or just calculate an adjaceny figure

// ACTUALLY - NOW WE DO UPDATE ALL THE CELLS, SO BORDER CONDITION NOW CHANGED

*/

void CFluid::Diffusion(float *p_in, float *p_out, float scale)
{
	// Build the new pressure values in p1, then swap p0 and p1


	float a = m_dt * scale;
	int cell;
//#if 0
//	// Copy edge as not diffusing into edge
//	CopyEdge(p_in, p_out);
//#else

	// poziome krawedzie 
	for (int x = 1; x < m_w - 1; x++)
	{
		int y = 0;
		int z = 0;
		cell = Cell(x, y, z);
		p_out[cell] = p_in[cell] + a * (p_in[Cell(x -1, y, z)] + p_in[Cell(x + 1, y, z)] + p_in[Cell(x, y + 1, z)] +  p_in[Cell(x, y, z + 1)] - 4.0f * p_in[cell]);

		y = 0;
		z = m_d;
		cell = Cell(x, y, z);
		p_out[cell] = p_in[cell] + a * (p_in[Cell(x - 1, y, z)] + p_in[Cell(x + 1, y, z)] + p_in[Cell(x, y + 1, z)] + p_in[Cell(x, y, z - 1)] - 4.0f * p_in[cell]);

		y = m_h;
		z = m_d;
		cell = Cell(x, y, z);
		p_out[cell] = p_in[cell] + a * (p_in[Cell(x - 1, y, z)] + p_in[Cell(x + 1, y, z)] + p_in[Cell(x, y - 1, z)] + p_in[Cell(x, y, z - 1)] - 4.0f * p_in[cell]);

		y = m_h;
		z = 0;
		cell = Cell(x, y, z);
		p_out[cell] = p_in[cell] + a * (p_in[Cell(x - 1, y, z)] + p_in[Cell(x + 1, y, z)] + p_in[Cell(x, y - 1, z)] + p_in[Cell(x, y, z + 1)] - 4.0f * p_in[cell]);
	}

	// pionowe krawedzie 
	for (int y = 1; y < m_h - 1; y++)
	{
		int x = 0;
		int z = 0;
		cell = Cell(x, y, z);
		p_out[cell] = p_in[cell] + a * (p_in[Cell(x, y +1 , z)] + p_in[Cell(x, y + 1, z)] + p_in[Cell(x + 1 , y, z)] + p_in[Cell(x, y, z + 1)] - 4.0f * p_in[cell]);

		x = m_w;
		z = 0;
		cell = Cell(x, y, z);
		p_out[cell] = p_in[cell] + a * (p_in[Cell(x, y + 1, z)] + p_in[Cell(x, y + 1, z)] + p_in[Cell(x - 1, y, z)] + p_in[Cell(x, y, z + 1)] - 4.0f * p_in[cell]);

		x = m_w;
		z = m_d;
		cell = Cell(x, y, z);
		p_out[cell] = p_in[cell] + a * (p_in[Cell(x, y + 1, z)] + p_in[Cell(x, y + 1, z)] + p_in[Cell(x - 1, y, z)] + p_in[Cell(x, y, z - 1)] - 4.0f * p_in[cell]);

		x = 0;
		z = m_d;
		cell = Cell(x, y, z);
		p_out[cell] = p_in[cell] + a * (p_in[Cell(x, y + 1, z)] + p_in[Cell(x, y + 1, z)] + p_in[Cell(x + 1, y, z)] + p_in[Cell(x, y, z - 1)] - 4.0f * p_in[cell]);
	}

	//œciana przednia i tylna
	


//#endif
	for (int x = 1; x < m_w - 1; x++)
	{
		for (int y = 1; y < m_h - 1; y++)
		{
			for (int z = 1; z < m_d - 1; z++)
			{
				cell = Cell(x, y, z);
				p_out[cell] = p_in[cell] + a * (p_in[Cell(x, y + 1, z)] + p_in[Cell(x, y - 1, z)] + p_in[Cell(x + 1, y, z)] + p_in[Cell(x - 1, y, z)] + p_in[Cell(x, y, z +1)] + p_in[Cell(x, y, z -1)] - 6.0f * p_in[cell]);
			}
		}
	}

}

void CFluid::DiffusionLR(float *p_in, float *p_out, float scale)
{
	// Build the new pressure values in p1, then swap p0 and p1


	float a = m_dt * scale;

	for (int y = 1; y < m_h; y++)
	{
		for (int x = 0; x < m_w - 1; x++)
		{
			for (int z = 1; z < m_d - 1; z++)
			{
				int cell = Cell(x, y, z);
				float m = p_in[cell];
				float n = p_in[cell + 1];
				p_out[cell] = m + a*(n - m);
				p_out[cell + 1] = n + a*(m - n);
				//p_out[cell] = p_in[cell] + a * (p_in[Cell(x,y+1)] + p_in[Cell(x,y-1)] + p_in[Cell(x+1,y)] + p_in[Cell(x-1,y)] - adj * p_in[cell]);
			}
		}
	}
}

void CFluid::GlobalDiffusion(float *p_in, float *p_out, float scale)
{
	float a = scale * m_dt;
	int size = m_w * m_h * m_d;
	float tot = 0;
	for (int cell = 0; cell<size; cell++)
	{
		tot += p_in[cell];
	}

	float avg = tot / size;

	for (int cell = 0; cell<size; cell++)
	{
		p_out[cell] = (1.0f - a) * p_in[cell] + a*avg;
	}
}


void CFluid::DiffusionUD(float *p_in, float *p_out, float scale)
{
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


// NEW - treat cells pairwise, which allows us to handle edge cells
// updates ALL cells
void CFluid::PressureAcceleration(float force)
{

	// Pressure differential between points  
	// to get an accelleration force.
	float a = m_dt * force;

	// First copy the values
	for (int x = 0; x < m_w; x++)
	{
		for (int y = 0; y < m_h; y++)
		{
			for (int z = 0; z < m_d ; z++)
			{
				int cell = Cell(x, y, z);
				mp_xv1[cell] = mp_xv0[cell];
				mp_yv1[cell] = mp_yv0[cell];
				mp_zv1[cell] = mp_zv0[cell];
			}
		}
	}


	//	for (int iterate = 0; iterate<1;iterate++)
	{
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
						mp_yv1[Cell(x, y +1, z)] += a * force_y;

						mp_zv1[cell] += a * force_z;
						mp_zv1[Cell(x, y, z+1)] += a * force_z;
					
				
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

}


// Decay a positive value towards zero via a quadratic function
void CFluid::QuadraticDecay(float *p_in, float *p_out, float a, float b, float c)
{
	float dt = m_dt;
	int size = m_w * m_h * m_d;
	for (int cell = 0; cell<size; cell++)
	{
		float v = p_in[cell];
	
		float v2 = v*v;
		v -= dt*( a*v2 + b*v + c);
		if (v <0.0f)
			v = 0.0f;
		if (v >50.0f)
			v = 50.0f;
		
		p_in[cell] = v;
	}
}


// Given a field p_from, and a field p_to, then add f*p_from to p_to
// can be used to apply a heat field to velocity
void CFluid::ForceFrom(float *p_from, float *p_to, float f)
{
	f *= m_dt;
	int size = m_w * m_h * m_d;
	for (int cell = 0; cell < size; cell++)
	{
		*p_to++ += *p_from++*f;
	}
}

void CFluid::ApplyForce(float *p_to, float f)
{
	f *= m_dt;
	int size = m_w * m_h * m_d;
	for (int cell = 0; cell < size; cell++)
	{
		*p_to++ += f;
	}
}




// Move a scalar along the velocity field

void CFluid::ReverseAdvection(float *p_in, float*p_out, float scale)
{
	// negate advection scale, since it's reverse advection
	float a = -m_dt * scale;

	int size = m_w*m_h*m_d;

	// First copy the scalar values over, since we are adding/subtracting in values, not moving things
	CopyField(p_in, p_out);

	// we need to zero out the fractions 
	ZeroField(mp_fraction);

	// We do all the cells,
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

					float step = 0.25;
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

					// we are taking pressure from four cells (one of which might be the target cell
					// and adding it to the target cell
					// hence we need to subtract the appropiate amounts from each cell
					//
					// Cells are like
					// 
					//    A----B
					//    |    |
					//    |    |
					//    C----D
					// 
					// (Should be square)
					// so we work out the bilinear fraction at each of a,b,c,d
					// NOTE: Using p1 here, to ensure consistency


					/*

					By adding the source value into the destination, we handle the problem of multiple destinations
					but by subtracting it from the source we gloss ove the problem of multiple sources.
					Suppose multiple destinations have the same (partial) source cells, then what happens is the first dest that
					is processed will get all of that source cell (or all of the fraction it needs).  Subsequent dest
					cells will get a reduced fraction.  In extreme cases this will lead to holes forming based on
					the update order.

					Solution:  Maintain an array for dest cells, and source cells.
					For dest cells, store the four source cells and the four fractions
					For source cells, store the number of dest cells that source from here, and the total fraction
					E.G.  Dest cells A, B, C all source from cell D (and explicit others XYZ, which we don't need to store)
					So, dest cells store A->D(0.1)XYZ..., B->D(0.5)XYZ.... C->D(0.7)XYZ...
					Source Cell D is updated with A, B then C
					Update A:   Dests = 1, Tot = 0.1
					Update B:   Dests = 2, Tot = 0.6
					Update C:   Dests = 3, Tot = 1.3

					How much should go to each of A, B and C? They are asking for a total of 1.3, so should they get it all, or
					should they just get 0.4333 in total?
					Ad Hoc answer:
					if total <=1 then they get what they ask for
					if total >1 then is is divided between them proportionally.
					If there were two at 1.0, they would get 0.5 each
					If there were two at 0.5, they would get 0.5 each
					If there were two at 0.1, they would get 0.1 each
					If there were one at 0.6 and one at 0.8, they would get 0.6/1.4 and 0.8/1.4  (0.429 and 0.571) each

					So in our example, total is 1.3,
					A gets 0.1/1.3, B gets 0.6/1.3 C gets 0.7/1.3, all totalling 1.0

					SO we need additional arrays
					int mp_sources[size]
					int mp_source_fractions[size*4]
					float mp_fraction[size]

					*/
					// Just calculaitng fractions for now
							float ixyz = (1.0f - fx)*(1.0f - fy)*(1.0f -fz) ;
					float ixyz1 = (1.0f - fx)*(1.0f - fy)*(fz) ;
					float ixy1z = (1.0f - fx)*(fy)*(1.0f - fz);
					float ix1yz = (fx)*(1.0f - fy)*(1.0f - fz);

					float ixy1z1 = (1.0f - fx)*(fy)*(fz);
					float ix1yz1 = (fx)*(1.0f - fy)*(fz);
					float ix1y1z = (fx)*(fy)*(1.0f - fz);
					float ix1y1z1 = (fx)*(fy)*(fz);
					// Storing the source information for this dest cell (cell)
					mp_sources[cell] = cell1;  // that's cell1, implying cell1+1, cell1+m_w and cell1+1+m_w
											   // and the fractions, unless they are all zero
											   // which is quite possible if advecting something sparse
											   // but since we optimize for a more likely case, this test wastes time
											   //if (ia !=0.0f || ib!=0.0f || ic !=0.0f || id != 0.0f)
					{
						//mp_source_fractions[cell * 4 + 0] = ia;
						//mp_source_fractions[cell * 4 + 1] = ib;
						//mp_source_fractions[cell * 4 + 2] = ic;
						//mp_source_fractions[cell * 4 + 3] = id;

						//// Accumullting the fractions for the four sources 
						//mp_fraction[cell1] += ia;
						//mp_fraction[cell1 + 1] += ib;
						//mp_fraction[cell1 + m_w] += ic;
						//mp_fraction[cell1 + m_w + 1] += id;




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
					// cell has zero velocity, advecting from itself, flag that, and opimize it out later
					mp_sources[cell] = -1;  // 
				}
			}
		}
	}

	// With nice advection, each cell has four sources, and four source fractions

	for (int cell = 0; cell<size; cell++)
	{
		// get the block of four source cells (cell1)
		int cell1 = mp_sources[cell];
		if (cell1 != -1)
		{
			// Get the four fractional amounts we earlier interpolated
			

			float ixyz   =   mp_source_fractions[cell * 8 + 0];
			float ixyz1  =   mp_source_fractions[cell * 8 + 1] ;
			float ixy1z  =   mp_source_fractions[cell * 8 + 2] ;
			float ix1yz  =   mp_source_fractions[cell * 8 + 3] ;
			float ixy1z1 =   mp_source_fractions[cell * 8 + 4];
			float ix1yz1 =   mp_source_fractions[cell * 8 + 5];
			float ix1y1z =   mp_source_fractions[cell * 8 + 6];
			float ix1y1z1 =  mp_source_fractions[cell * 8 + 7] ;


			// get the TOTAL fraction requested from each source cell
			//float fa = mp_fraction[cell1];
			//float fb = mp_fraction[cell1 + 1];
			//float fc = mp_fraction[cell1 + m_w];
			//float fd = mp_fraction[cell1 + 1 + m_w];
			int cell2 = cell1;

			int iz = cell2 / (m_w*m_h);
			cell2 %= m_d*m_w;
			int ix = cell2 % m_w;
			cell2 /= m_w;
			int iy = cell2;
			
																								
			float fxyz   	=					mp_fraction[Cell(ix, iy, iz)            ] 		  ;
			float fxyz1  	=					mp_fraction[Cell(ix, iy, iz + 1)        ] 		  ;
			float fxy1z  	=					mp_fraction[Cell(ix, iy + 1, iz)        ] 		  ;
			float fx1yz  	=					mp_fraction[Cell(ix + 1, iy, iz)        ] 		  ;
			float fxy1z1 	=					mp_fraction[Cell(ix, iy + 1, iz + 1)    ] 		  ;
			float fx1yz1 	=					mp_fraction[Cell(ix + 1, iy, iz + 1)    ] 		  ;
			float fx1y1z 	=					mp_fraction[Cell(ix + 1, iy + 1, iz)    ] 		  ;
			float fx1y1z1	=					mp_fraction[Cell(ix + 1, iy + 1, iz + 1)] 		  ;



			// if less then 1.0 in total, we can have all we want

			if (fxyz   <1.0f) fxyz = 1.0f;
			if (fxyz1  <1.0f) fxyz1 = 1.0f;
			if (fxy1z  <1.0f) fxy1z = 1.0f;
			if (fx1yz  <1.0f) fx1yz = 1.0f;
			if (fxy1z1 <1.0f) fxy1z1 = 1.0f;
			if (fx1yz1 <1.0f) fx1yz1 = 1.0f;
			if (fx1y1z <1.0f) fx1y1z = 1.0f;
			if (fx1y1z1<1.0f) fx1y1z1 = 1.0f;

			// scale the amount we are transferring
			ixyz /= fxyz;
			ixyz1 /= fxyz1;
			ixy1z /= fxy1z;
			ix1yz /= fx1yz;
			ixy1z1 /= fxy1z1;
			ix1yz1 /= fx1yz1;
			ix1y1z /= fx1y1z;
			ix1y1z1 /= fx1y1z1;
			// Give the fraction of the original source, do not alter the original
			// So we are taking fractions from p_in, but not altering those values
			// as they are used again by later cells
			// if the field were mass conserving, then we could simply move the value
			// but if we try that we lose mass
			//p_out[cell] += ia * p_in[] + ib * p_in[cell1 + 1] + ic * p_in[cell1 + m_w] + id * p_in[cell1 + 1 + m_w];

			p_out[cell] += (ixyz		  * p_in[Cell(ix, iy, iz)] +
				ixyz1				  * p_in[Cell(ix, iy, iz + 1)] +
				ixy1z				  * p_in[Cell(ix, iy + 1, iz)] +
				ix1yz				  * p_in[Cell(ix + 1, iy, iz)] +
				ixy1z1				  * p_in[Cell(ix, iy + 1, iz + 1)] +
				ix1yz1				  * p_in[Cell(ix + 1, iy, iz + 1)] +
				ix1y1z				  * p_in[Cell(ix + 1, iy + 1, iz)] +
				ix1y1z1				  * p_in[Cell(ix + 1, iy + 1, iz + 1)]);


			// The accounting happens here, the values we added to the dest in p1 are subtracted from the soruces in p1
			//p_out[cell1] -= ia * p_in[];
			//p_out[cell1 + 1] -= ib * p_in[cell1 + 1];
			//p_out[cell1 + m_w] -= ic * p_in[cell1 + m_w];
			//p_out[cell1 + m_w + 1] -= id * p_in[cell1 + m_w + 1];
																																		  ;
							p_out[Cell(ix, iy, iz)] -= ixyz								 * p_in[Cell(ix, iy, iz)]						  ;
							p_out[Cell(ix, iy, iz + 1)] -= ixyz1						 * p_in[Cell(ix, iy, iz + 1)]					  ;
							p_out[Cell(ix, iy + 1, iz)] -= ixy1z						 * p_in[Cell(ix, iy + 1, iz)]					  ;
							p_out[Cell(ix + 1, iy, iz)] -= ix1yz						 * p_in[Cell(ix + 1, iy, iz)]					  ;
							p_out[Cell(ix, iy + 1, iz + 1)] -= ixy1z1					 * p_in[Cell(ix, iy + 1, iz + 1)]				  ;
							p_out[Cell(ix + 1, iy, iz + 1)] -= ix1yz1					 * p_in[Cell(ix + 1, iy, iz + 1)]				  ;
							p_out[Cell(ix + 1, iy + 1, iz)] -= ix1y1z					 * p_in[Cell(ix + 1, iy + 1, iz)]				  ;
							p_out[Cell(ix + 1, iy + 1, iz + 1)] -= ix1y1z1				 * p_in[Cell(ix + 1, iy + 1, iz + 1)]			  ;

		}
	}
}


// Signed advection is mass conserving, but allows signed quantities 
// so could be used for velocity, since it's faster.
void CFluid::ReverseSignedAdvection(float *p_in, float*p_out, float scale)
{
	// negate advection scale, since it's reverse advection
	float a = -m_dt * scale;

	int size = m_w*m_h*m_d;

	// First copy the scalar values over, since we are adding/subtracting in values, not moving things
	CopyField(p_in, p_out);

	// We do all the cells,
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
			float step = 0.25;
					if (vx > step) {
						vx = step;
					}
					if (vy > step) {
						vy = step;
					}
					if (vz > step) {
						vz = step;
					}
					if (vz < -1*step) {
						vz = -1*step;
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
				Collide(x, y , z, x1, y1, z1);
				int cell1 = Cell((int)x1, (int)y1, (int)z1);

				// get fractional parts
				float fx = x1 - (int)x1;
				float fy = y1 - (int)y1;
				float fz = z1 - (int)z1;

				// Get amounts from (in) source cells
		/*		float ia = (1.0f - fy)*(1.0f - fx) * p_in[cell1];
				float ib = (1.0f - fy)*(fx)* p_in[cell1 + 1];
				float ic = (fy)     *(1.0f - fx) * p_in[cell1 + m_w];
				float id = (fy)     *(fx)* p_in[cell1 + m_w + 1];*/

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


				//// add to (out) dest cell
				//p_out[cell] += ia + ib + ic + id;
				//// and subtract from (out) dest cells
				//p_out[cell1] -= ia;
				//p_out[cell1 + 1] -= ib;
				//p_out[cell1 + m_w] -= ic;
				//p_out[cell1 + m_w + 1] -= id;


				p_out[cell] += (ixyz + ixyz1 + ixy1z + ix1yz + ix1yz + ix1yz1 + ix1y1z + ix1y1z1);
				// Then add them to the four destination cells
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


// Move scalar along the velocity field
// Forward advection moves the value at a point forward along the vector from the same point
// and dissipates it between four points as needed
void CFluid::ForwardAdvection(float *p_in, float*p_out, float scale)
{
	// Pressure differential between points  
	// to get an accelleration force.
	float a = m_dt * scale;

	int w = m_w;
	int h = m_h;
	int d = m_d;

	// First copy the values
	CopyField(p_in, p_out);

	if (scale == 0.0f)
		return;


	// We advect all cells
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

					float step = 0.25;
					if (vx > step) {
						vx = step;
					}
					if (vy > step) {
						vy = step;
					}
					if (vz > step) {
						vz = step;
					}
					if (vz < -1*step) {
						vz = -1*step;
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

				
					// get fractional parts
					float fx = x1 - (int)x1;
					float fy = y1 - (int)y1;
					float fz = z1 - (int)z1;

					// we are taking pressure from four cells (one of which might be the target cell
					// and adding it to the target cell
					// hence we need to subtract the appropiate amounts from each cell
					//
					// Cells are like
					// 
					//    A----B
					//    |    |
					//    |    |
					//    C----D
					// 
					// (Should be square)
					// so we work out the bilinear fraction at each of a,b,c,d

					float in = p_in[cell];


					float ixyz = (1.0f - fx)*(1.0f - fy)*(1.0f -fz) * in;
					float ixyz1 = (1.0f - fx)*(1.0f - fy)*(fz) * in;
					float ixy1z = (1.0f - fx)*(fy)*(1.0f - fz)* in;
					float ix1yz = (fx)*(1.0f - fy)*(1.0f - fz)* in;

					float ixy1z1 = (1.0f - fx)*(fy)*(fz) * in;
					float ix1yz1 = (fx)*(1.0f - fy)*(fz) * in;
					float ix1y1z = (fx)*(fy)*(1.0f - fz) * in;
					float ix1y1z1 = (fx)*(fy)*(fz)* in;

					
					// Subtract them from the source cell
					p_out[cell] -= (ixyz + ixyz1 + ixy1z + ix1yz + ix1yz + ix1yz1 + ix1y1z + ix1y1z1);
					// Then add them to the four destination cells
					p_out[Cell(ix, iy, iz)] += ixyz;
					p_out[Cell(ix, iy, iz + 1)] += ixyz1;
					p_out[Cell(ix, iy + 1, iz)] += ixy1z;
					p_out[Cell(ix + 1, iy, iz)] += ix1yz;
					p_out[Cell(ix, iy + 1, iz + 1)] += ixy1z1;
					p_out[Cell(ix +1, iy, iz + 1)] += ix1yz1;
					p_out[Cell(ix + 1, iy + 1, iz)] += ix1y1z;
					p_out[Cell(ix +1, iy +1, iz +1)] += ix1y1z1;
				
				}
			}
		}
	}
}



// Given a floating point position within the field, add to it 
// with bilinear interpolation
void CFluid::AddValue(float *p_in, float x, float y, float z, float v)
{
	if (x<0 || y<0 || z<0 || x>(float)m_w - 1.0001f || y>(float)m_h - 1.0001f || z>(float)m_d - 1.0001f)
		return;

	v = 1;	
	// get fractional parts
	float fx = x - (int)x;
	float fy = y - (int)y;
	float fz = z - (int)z;
	// get the corner cell (a)
	int cell = Cell((int)x, (int)y, (int)z);

	int ix = (int)x;
	int iy = (int)y;
	int iz = (int)z;

	float ixyz    = (1.0f - fx)*(1.0f - fy)*(1.0f - fz) * v;
	float ixyz1   = (1.0f - fx)*(1.0f - fy)*(fz)* v;
	float ixy1z   = (1.0f - fx)*(fy)*(1.0f - fz)* v;
	float ix1yz   = (fx)*(1.0f - fy)*(1.0f - fz)* v;
	float ixy1z1  = (1.0f - fx)*(fy)*(fz)* v;
	float ix1yz1  = (fx)*(1.0f - fy)*(fz)* v;
	float ix1y1z  = (fx)*(fy)*(1.0f - fz) * v;
	float ix1y1z1 = (fx)*(fy)*(fz)* v;

	// Add into each cell
	p_in[Cell(ix, iy, iz)] += ixyz;
	p_in[Cell(ix, iy, iz + 1)] += ixyz1;
	p_in[Cell(ix, iy + 1, iz)] += ixy1z;
	p_in[Cell(ix + 1, iy, iz)] += ix1yz;
	p_in[Cell(ix, iy + 1, iz + 1)] += ixy1z1;
	p_in[Cell(ix + 1, iy, iz + 1)] += ix1yz1;
	p_in[Cell(ix + 1, iy + 1, iz)] += ix1y1z;
	p_in[Cell(ix + 1, iy + 1, iz + 1)] += ix1y1z1;
	

}


void CFluid::SetValue(float *p_in, float x, float y, float z, float v)
{
	// get fractional parts
	float fx = x - (int)x;
	float fy = y - (int)y;
	// get the corner cell (a)
	int cell = Cell((int)x, (int)y, (int)z);

	// get fractions of the values for each target cell
	float ia = (1.0f - fy)*(1.0f - fx) * v;
	float ib = (1.0f - fy)*(fx)* v;
	float ic = (fy)     *(1.0f - fx) * v;
	float id = (fy)     *(fx)* v;

	// Set into each cell
	p_in[cell] = ia;
	p_in[cell + 1] = ib;
	p_in[cell + m_w] = ic;
	p_in[cell + m_w + 1] = id;
}

float CFluid::GetValue(float *p_in, float x, float y, float z)
{
	// get fractional parts
	float fx = x - (int)x;
	float fy = y - (int)y;
	float fz = z - (int)z;
	// get the corner cell (a)
	int cell = Cell((int)x, (int)y, int(z));

	// get fractions of the values from each cell
	float ia = (1.0f - fy)*(1.0f - fx) * p_in[cell];
	float ib = (1.0f - fy)*(fx)* p_in[cell + 1];
	float ic = (fy)     *(1.0f - fx) * p_in[cell + m_w];
	float id = (fy)     *(fx)* p_in[cell + m_w + 1];

	return ia + ib + ic + id;
}




// render the array on screen as a bunch of rectangles
void CFluid::Render(float x, float y, float w, float h)
{
	m_screen_x = x;
	m_screen_y = y;
	m_screen_w = w;
	m_screen_h = h;



	float tot_p = 0;

	float wx = w / m_w;
	float wy = h / m_h;

	float *p_ink = mp_ink0;
	float scale = 100.0f; //255.0f; 


	for (int x0 = 0; x0<m_w; x0++)
	{
		for (int y0 = 0; y0<m_h; y0++)
		{
			for (int z0 = 0; z0 < m_d; z0++)
			{
				int cell = Cell(x0, y0, z0);

				if ((x0 < m_w - 1 && y0 < m_h - 1 && z0 < m_d -1))
				{
					unsigned int base = 0xff000000;
					int p0 = scale * p_ink[cell];
					if (p0 > 255) p0 = 255;
					int p1 = scale * p_ink[cell + 1];
					if (p1 > 255) p1 = 255;
					int p2 = scale * p_ink[cell + 1 + m_w];
					if (p2 > 255) p2 = 255;
					int p3 = scale * p_ink[cell + m_w];
					if (p3 > 255) p3 = 255;
					if (p0 < 0 || p1 < 0 || p2 < 0 || p3 < 0)
						base = 0xffff0000;
					int lun2t = sizeof(mp_heat0);

					int lunt = mp_heat0[cell];


					int h0 = scale * mp_heat0[cell];
					if (h0 > 255) h0 = 255;
					int h1 = scale * mp_heat0[cell + 1];
					if (h1 > 255) h1 = 255;
					int h2 = scale * mp_heat0[cell + 1 + m_w];
					if (h2 > 255) h2 = 255;
					int h3 = scale * mp_heat0[cell + m_w];
					if (h3 > 255) h3 = 255;
					if (h0 < 0 || h1 < 0 || h2 < 0 || h3 < 0)
						base = 0xffff0000;


			
						p0 = p0 + (p0 << 8) + (p0 << 16) + (p0 << 24);
						p1 = p1 + (p1 << 8) + (p1 << 16) + (p1 << 24);
						p2 = p2 + (p2 << 8) + (p2 << 16) + (p2 << 24);
						p3 = p3 + (p3 << 8) + (p3 << 16) + (p3 << 24);

					//DrawRect(x + x0 * wx, y + y0*wy, wx, wy, base + p0, base + p1, base + p2, base + p3);
					D3DCOLOR col = base + p0;
					//drawCube(x0, y0, z0, base + p0);
			//if(p0 + p1 + p2 + p3 > 0)
					//drawCube(x0, y0, z0, D3DCOLOR_ARGB(p0 + p1 + p2 + p3, 255, 255, 255));

					//drawCube(x0, y0, z0, D3DCOLOR_ARGB(255, 255, 255, 255));
					//drawCube(x0, y0, z0, D3DCOLOR_ARGB(3, rand(), rand(), rand()));
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

	// wp³yw temperatury na unoszenie siê dymu
		ForceFrom(mp_heat0, mp_yv0, m_heat_force);

	// naturalny rozk³ad ciep³a za pomoc¹ funkcji kwadratowej
		QuadraticDecay(mp_heat0, mp_heat0, m_heat_friction_a, m_heat_friction_b, m_heat_friction_c);


		//wp³yw ciœnienia (iloœæi dymu w komórce) na s¹siaduj¹ce komórki
		PressureAcceleration(m_pressure_acc);
	
		//skala si³y adwekcji
	float advection_scale = m_w*m_h*m_d / 10000.f;



	//zerowanie macierzy iloœæi dymu w nastêpnym kroku
	SetField(mp_ink1, 1.0f);
	ForwardAdvection(mp_ink1, mp_balance, m_ink_advection * advection_scale);


	ForwardAdvection(mp_ink0, mp_ink1, m_ink_advection * advection_scale);
	swap(mp_ink0, mp_ink1);
	ReverseAdvection(mp_ink0, mp_ink1, m_ink_advection * advection_scale);
	swap(mp_ink0, mp_ink1);

	
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

	ReverseAdvection(mp_p0, mp_p1, m_pressure_advection * advection_scale);
	swap(mp_p0, mp_p1);
}


void InitFluids()
{

	memset(&g_fluid1, 0, sizeof(CFluid));



	// Fluid1 is the one with the double time step

	g_fluid1.m_diffusion_iterations = 1;

	g_fluid1.m_velocity_diffusion = 0.0f; //3.0f; //3.5f; // 3.5f works nicely

										  // With no pressure diffusion, waves of pressure keep moving
										  // But only if pressure is advected ahead of velocity
	g_fluid1.m_pressure_diffusion = 0.0f; //6.0f; // 3.5f; // too big a diffusion scale creates innacuracies, but it's fine around here

	g_fluid1.m_heat_diffusion = 0.0f; // 3.0f;

	g_fluid1.m_ink_diffusion = 0.0f; //1.0f; // 3.0f;

									 // friction applied to velocity.  As a simple fraction each time step
									 // so 1.0f will give you no friction

									 // Friction is very important to the realism of the simulation
									 // changes to these coeffficinets will GREATLY affect the apperence of the simulation.
									 // the high factor friction (a) wich is applied to the square of the velocity,
									 // acts as a smooth limit to velocity, which causes streams to break up into turbulent flow 
									 // The c term will make things stop, which then shows up our border artefacts
								 // but a high enough (>0.01) c term is needed to stop things just dissipating too fast
						 // Friction is dt*(a*v^2 + b*v + c)

	g_fluid1.m_velocity_friction_a = 0.0f;
	g_fluid1.m_velocity_friction_b = 0.0f;
	g_fluid1.m_velocity_friction_c = 0.0f;


	g_fluid1.m_vorticity = 0.0f;

	// Pressure accelleration.  Larger values (>0.5) are more realistic (like water)
	// values that are too large lead to chaotic waves
	g_fluid1.m_pressure_acc = 0.02; // 2.0 worked well for smoke-like stuff

								 // upwards force applied by ink

								 // heat follows ink in advection, but is a seperate scalar value
	g_fluid1.m_ink_heat = 0.0f;
	g_fluid1.m_heat_force = -0.1f; //-0.1f;
	g_fluid1.m_heat_friction_a = 1.5f;
	g_fluid1.m_heat_friction_b = 0.2f;
	g_fluid1.m_heat_friction_c = 0.01f;

	// high ink advection allows fast moving nice swirlys
	g_fluid1.m_ink_advection = 150.0f;
	// seems nice if both velocity and pressure at same value, which makes sense
	g_fluid1.m_velocity_advection = 150.0f;
	g_fluid1.m_pressure_advection = 150.0f;  // 130, lag behind, to allow vel to dissipate???
	g_fluid1.m_heat_advection = 150.0f;

	//////////////////////////////////////////////////////////////

	g_fluid1.m_field_test = 0;

	g_fluid1.Init(size, size, size);


}
MSG msg;

void TestApp::Update(float dt)
{
	currentCube = 0;
	//int z = 0;
	//for (int i = 0; i < size; i++) {
	//	for (int j = 0; j < size; j++) {
	//		for (int k = 0; k < size; k++) {

	//			drawCube(i, j, k, D3DCOLOR_ARGB(255, ( 100 + 20*i)%255, (20 * j) % 255, (20 * j) % 255));
	//		}
	//	}
	//}

	//wartosc
	g_fluid1.AddValue(g_fluid1.mp_ink0, 25, 25, 25, 45);
	g_fluid1.AddValue(g_fluid1.mp_heat0, 25, 25, 25, 45);
	//g_fluid1.mp_yv0[g_fluid1.Cell(size / 2, size - 4, size / 2)] = 10;

	//g_fluid1.AddValue(g_fluid1.mp_xv0, size, size, size / 2, 0.04f);
	//g_fluid1.AddValue(g_fluid1.mp_yv0, size, size, size / 2, 0.04f);
	//g_fluid1.AddValue(g_fluid1.mp_zv0, size, size, size / 2, 0.04f);













				if (!key_a) {
					g_fluid1.Update(0.02f);
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
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLinte, int nCmdShow)
{
	TestApp* tApp = new TestApp(hInstance);

	if (!tApp->Init()) {
		return 1;
	}

	// Enter the message loop
	ZeroMemory(&msg, sizeof(msg));
	Timer_Init();


	InitFluids();

		return tApp->Run();


}

