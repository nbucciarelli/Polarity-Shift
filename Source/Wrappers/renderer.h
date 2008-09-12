#pragma once

//Renderer H:
//Abstract base class for graphics renderer.  Makes going between DX and OGL much easier.

class spriteData;
class camera;
struct textureData;
struct meshData;
union matrix;
union vector3;
struct rect;
#ifndef WIN_INTERNAL
struct HWND__;
typedef HWND__* HWND;
#endif

typedef unsigned int color;
typedef unsigned int uint;

#define DEFAULT_WNDWIDTH 1024
#define DEFAULT_WNDHEIGHT 600

class renderer
{
protected:
	HWND hWnd;
	camera * cam;

public:
	renderer();
	virtual ~renderer() {}

	virtual void Init(HWND, int width = DEFAULT_WNDWIDTH, int height = DEFAULT_WNDHEIGHT,
					bool isWindowed = true, bool vsync = true) = 0;

	virtual void changeResolution(int height, int width, bool isWindowed = true) = 0;

	//Allocate texture memory according to the renderer's proceedures, store data for return
	//in dat.
	virtual void LoadTexture(textureData* dat, const char filename[], uint colorKey = 0) = 0;
	virtual void LoadMesh(meshData* dat, const char filename[]) = 0;

	//tex should be a pointer to the memory space of the texture
	virtual void ReleaseTexture(void* tex) = 0;
	virtual void ReleaseMesh(void* mesh) = 0;

	virtual void BeginScene() = 0;
	virtual void BeginSprites() = 0;
	virtual void BeginLines() = 0;

	virtual void RenderMesh(meshData * obj) = 0;
	virtual void RenderSprite(const void* texture, const vector3* pos, const matrix * transform = NULL,
		const rect* section = NULL, const vector3* center = NULL, uint color = 0xffffffff) = 0;
	virtual void RenderText(int _x, int _y,
		const char* text,
		const color _color = 0xffffffff) const = 0;

	virtual void drawLine(vector3& pt1, vector3& pt2, color fillColor = 0xffffffff) = 0;

	virtual void EndLines() = 0;
	virtual void EndSprites() = 0;
	virtual void EndScene() = 0;
	virtual void EndNoPresent() = 0;
	virtual void Shutdown() = 0;

	virtual void AttachCamera(camera* _cam);
	virtual void ReleaseCamera();

//	virtual void setRefreshRate(uint);
//	virtual void setWindowed(bool);
};