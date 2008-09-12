#include <windows.h>
#include "..\Helpers\datatypes.h"
#include "dxRenderer.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <cstring>
#include <stdio.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define SAFE_RELEASE(p) if (p) { p->Release(); p = NULL; }

#pragma region constructors / destructors

dxRenderer::dxRenderer(void)
{
}

dxRenderer::~dxRenderer(void) {}

dxRenderer::_com::_com() : d3d(0), device(0), dxFont(0), dxSprite(0), dxLine(0) {}

dxRenderer::_com::~_com()
{
	SAFE_RELEASE(dxLine);
	SAFE_RELEASE(dxFont);
	SAFE_RELEASE(dxSprite);
	SAFE_RELEASE(device);
	SAFE_RELEASE(d3d);
}

dxRenderer* dxRenderer::getInstance()
{
	static dxRenderer mrX;
	return &mrX;
}

#pragma endregion

#pragma region Initialization/Shutdown Functions
void dxRenderer::Init(HWND _hWnd, int width, int height, bool isWindowed, bool vsync)
{
	hWnd = _hWnd;

	com.d3d = Direct3DCreate9(D3D_SDK_VERSION);	

	d3dpp = new D3DPRESENT_PARAMETERS;

	ZeroMemory(d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	
	d3dpp->BackBufferWidth = width;
	d3dpp->BackBufferHeight = height;
	d3dpp->BackBufferFormat = (isWindowed) ? D3DFMT_UNKNOWN : D3DFMT_R5G6B5;
	d3dpp->BackBufferCount = 1;
	d3dpp->MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp->MultiSampleQuality = 0;
	d3dpp->SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp->hDeviceWindow	 = hWnd;
	d3dpp->Windowed = isWindowed;
	d3dpp->EnableAutoDepthStencil = true;
	d3dpp->AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp->Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dpp->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp->PresentationInterval = (vsync) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;

	com.d3d->CreateDevice(0, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, d3dpp, &com.device);

	com.device->SetRenderState(D3DRS_ZENABLE, true);	
	com.device->SetRenderState(D3DRS_LIGHTING, true);

	D3DXCreateSprite(com.device, &com.dxSprite);
	D3DXCreateFont(com.device, 0,0,0,0,0,0,0,0,0,0, &com.dxFont);

	D3DXCreateLine(com.device, &com.dxLine);
}

void dxRenderer::Shutdown()
{
	delete d3dpp;
	//note that com struct releases the COM devices
}

void dxRenderer::changeResolution(int height, int width, bool isWindowed)
{
	d3dpp->BackBufferFormat = (isWindowed) ? D3DFMT_UNKNOWN : D3DFMT_R5G6B5;
	d3dpp->Windowed = isWindowed;
	d3dpp->BackBufferWidth = width;
	d3dpp->BackBufferHeight = height;

	com.dxLine->OnLostDevice();
	com.dxFont->OnLostDevice();
	com.dxSprite->OnLostDevice();
	com.device->Reset(d3dpp);
	com.dxSprite->OnResetDevice();
	com.dxFont->OnResetDevice();

	DWORD styleFlags = WS_VISIBLE;

	if (isWindowed)
	{
		styleFlags |= WS_OVERLAPPEDWINDOW;
		ShowCursor(TRUE); // show the mouse cursor
	}
	else
	{
		styleFlags |= WS_POPUP;
		ShowCursor(FALSE); // hide the mouse cursor
	}

	SetWindowLong(hWnd, GWL_STYLE, styleFlags);

	if (isWindowed)
	{
		RECT window;
		window.left	= 0;
		window.top		= 0;
		window.right	= width;
		window.bottom	= height;

		AdjustWindowRectEx(&window, 
							styleFlags,
							FALSE, 
							WS_EX_APPWINDOW);
		
		int windowWidth		= window.right - window.left;
		int windowHeight	= window.bottom - window.top;

		SetWindowPos(hWnd, HWND_TOP,	(GetSystemMetrics(SM_CXSCREEN)>>1) - (windowWidth>>1),
										(GetSystemMetrics(SM_CYSCREEN)>>1) - (windowHeight>>1),
										windowWidth, windowHeight, SWP_SHOWWINDOW);
	}
	else
	{
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	}
}

#pragma endregion

#pragma region load/unload functions
void dxRenderer::LoadTexture(textureData* dat, const char filename[], uint colorKey)
{
	strcpy_s(dat->filename, strlen(filename)+ 1, filename);
	//wcscpy_s(dat->filename, wcslen(filename) + 1, filename);

	dat->texture = NULL;
	LPDIRECT3DTEXTURE9 temp = NULL;

	// Load the texture from the given file.
	HRESULT hr = 0;
	if (FAILED(hr = D3DXCreateTextureFromFileEx(com.device, filename, 0, 0, D3DX_DEFAULT, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, colorKey, 0, 0, &temp )))
	{
		// Failed.  Give it one more try, just to be sure.
		if (FAILED(hr = D3DXCreateTextureFromFileEx(com.device, filename, 0, 0, D3DX_DEFAULT, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, colorKey, 0, 0, &temp )))
		{
			char szBuffer[256] = {0};
			sprintf_s(szBuffer, "Failed to Create Texture - %s", filename); 
			MessageBox(0, szBuffer, "Texture Error", MB_OK);
			return;
		}
	}

	// Get surface description (to find Width/Height of the texture)
	D3DSURFACE_DESC d3dSurfDesc;
	ZeroMemory(&d3dSurfDesc, sizeof(d3dSurfDesc));

	temp->GetLevelDesc(0, &d3dSurfDesc);
	// Remember the Width and Height
	dat->width	= d3dSurfDesc.Width;
	dat->height	= d3dSurfDesc.Height;

	dat->texture = temp;
}

void dxRenderer::LoadMesh(meshData* dat, const char filename[])
{
	//TODO:  Implement for 3D fun
}

void dxRenderer::ReleaseTexture(void* tex)
{
	((LPDIRECT3DTEXTURE9)tex)->Release();
}

void dxRenderer::ReleaseMesh(void* mesh)
{
	//TODO: Implement for 3D fun
}

#pragma endregion

#pragma region conversion functions
/*LPDIRECT3DDEVICE9	GetDirect3DDevice(void)
{
	return com.device;
}*/
const D3DXMATRIX* dxRenderer::getMatrix(const matrix* in) const 
{
	return reinterpret_cast<const D3DXMATRIX*>(in);
}
const D3DXVECTOR3* dxRenderer::getVec3(const vector3* in) const 
{
	return reinterpret_cast<const D3DXVECTOR3*>(in);
}

#pragma endregion

#pragma region begin functions

void dxRenderer::BeginScene()
{
	com.device->Clear(0,0,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(10,10,40), 1.0f, 0);
	com.device->BeginScene();
}

void dxRenderer::BeginSprites()
{
	com.dxSprite->Begin(D3DXSPRITE_SORT_DEPTH_BACKTOFRONT | D3DXSPRITE_ALPHABLEND);	
}
void dxRenderer::BeginLines()
{
	com.dxLine->Begin();
}
#pragma endregion

#pragma region render functions
void dxRenderer::RenderMesh(meshData * obj)
{
}

void dxRenderer::RenderSprite(const void* texture, const vector3* pos, const matrix * transform,
		const rect* section, const vector3* center, uint color)
{
	if(!transform)
		com.dxSprite->SetTransform(getMatrix(&matrix()));
	else
		com.dxSprite->SetTransform(getMatrix(transform));

	com.dxSprite->Draw((LPDIRECT3DTEXTURE9)texture, reinterpret_cast<const RECT*>(section), getVec3(center), getVec3(pos), color);
}

void dxRenderer::RenderText(int _x, int _y,
				const char* text,
				const color _color) const
{
	LPDIRECT3DSURFACE9 d3dBackBuffer = 0;

	const char* rgb = reinterpret_cast<const char*>(&_color);

	com.device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &d3dBackBuffer);

	HDC hdc = 0;
	d3dBackBuffer->GetDC(&hdc);

	SetTextColor(hdc, RGB(rgb[1], rgb[2], rgb[3]));
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, _x, _y, text, (int)strlen(text));

	d3dBackBuffer->ReleaseDC(hdc);
	SAFE_RELEASE(d3dBackBuffer);
}

void dxRenderer::drawLine(vector3 &pt1, vector3 &pt2, color fillColor)
{
	D3DXVECTOR2 lp[2];
	unsigned char* c = (unsigned char*)&fillColor;

	lp[0].x = pt1.x;
	lp[0].y = pt1.y;
	lp[1].x = pt2.x;
	lp[1].y = pt2.y;

	com.dxLine->Draw(lp, 2, D3DCOLOR_ARGB(c[0], c[1], c[2], c[3]));
}
#pragma endregion

#pragma region end functions
void dxRenderer::EndLines()
{
	com.dxLine->End();
}
void dxRenderer::EndSprites()
{
	com.dxSprite->End();
}
void dxRenderer::Clear(unsigned char cRed, unsigned char cGreen, unsigned char cBlue)
{
	com.device->Clear(0,0,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(cRed,cGreen,cBlue), 1.0f, 0);
}
void dxRenderer::EndScene()
{
	com.device->EndScene();	
	com.device->Present(0,0,0,0);
}
void dxRenderer::EndNoPresent()
{
	com.device->EndScene();		
}
void dxRenderer::Present()
{
	com.device->Present(0,0,0,0);	
}
#pragma endregion