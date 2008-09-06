#pragma once
#include <d3d9.h>
#include <D3dx9shader.h>

class IShader
{
public:
	IShader(void) {}
	virtual ~IShader(void) {}

	virtual bool Create(const char* szShaderFile, LPDIRECT3DDEVICE9 lpD3DDevice) = 0;

	virtual bool Begin(void) const = 0;
	virtual bool End(void) const = 0;

	virtual bool SetConstantFloat(const char* szConstantName, float fValue) = 0;
};