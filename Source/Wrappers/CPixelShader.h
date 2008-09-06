#pragma once
#include "IShader.h"

class CPixelShader : public IShader
{
private:
		LPDIRECT3DDEVICE9		m_pD3DDevice;
		IDirect3DPixelShader9*	m_pPixelShader;
		LPD3DXCONSTANTTABLE		m_pConstantTable;


public:
	CPixelShader(void);
	~CPixelShader(void);

	bool Create(const char* szShaderFile, LPDIRECT3DDEVICE9 lpD3DDevice);

	bool Begin(void) const;
	bool End(void) const;

	bool SetConstantFloat(const char* szConstantName, float fValue);
};
