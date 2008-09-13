#pragma once
#include "renderer.h"

struct IDirect3D9;
struct IDirect3DDevice9;
struct ID3DXFont;
struct ID3DXSprite;
struct ID3DXLine;
struct _D3DPRESENT_PARAMETERS_;
struct D3DXMATRIX;
struct D3DXVECTOR3;
union vector3;
union matrix;
typedef _D3DPRESENT_PARAMETERS_ D3DPRESENT_PARAMETERS;

class dxRenderer : public renderer
{
protected:
	struct _com {
		IDirect3D9 *d3d;
		IDirect3DDevice9 *device;
		ID3DXFont *dxFont;
		ID3DXSprite *dxSprite;
		ID3DXLine *dxLine;

		_com();
		~_com();
	} com;

	D3DPRESENT_PARAMETERS* d3dpp;

	//Converstion functions, to switch game data to DX data
	void convertVec3(D3DXVECTOR3* out, vector3* in);
	void convertMatrix(D3DXMATRIX* out, matrix* in);
	const D3DXMATRIX* getMatrix(const matrix* in) const;
	const D3DXVECTOR3* getVec3(const vector3* in) const;

	dxRenderer(void);
	dxRenderer(const dxRenderer&);
	dxRenderer& operator=(const dxRenderer&);
	~dxRenderer(void);

public:
	static dxRenderer* getInstance();

	void Init(HWND, int width = DEFAULT_WNDWIDTH, int height = DEFAULT_WNDHEIGHT,
					bool isWindowed = true, bool vsync = true);	

	void changeResolution(int height, int width, bool isWindowed = true);

	void LoadTexture(textureData* dat, const char filename[], uint colorKey = 0);
	void LoadMesh(meshData* dat, const char filename[]);

	IDirect3DDevice9*	GetDirect3DDevice(void) {return com.device;}

	void ReleaseTexture(void* tex);
	void ReleaseMesh(void* mesh);

	void BeginScene();
	void BeginSprites();
	void BeginLines();

	void RenderMesh(meshData * obj);
	void RenderSprite(const void* texture, const vector3* pos, const matrix * transform = NULL,
		const rect* section = NULL, const vector3* center = NULL, uint color = 0xffffffff);
	void RenderText(int _x, int _y,
		const char* text,
		const color _color = 0xffffffff) const;

	void drawLine(const vector3& pt1, const vector3& pt2, color fillColor = 0xffffffff);
	void drawRect(const rect& box, color fillColor = 0xffffffff);

	void EndLines();
	void EndSprites();
	void EndScene();
	void Shutdown();

	void EndNoPresent();
	void Present();
	void Clear(unsigned char cRed, unsigned char cGreen, unsigned char cBlue);

};