#include "CPixelShader.h"

CPixelShader::CPixelShader(void)
{
	m_pD3DDevice		=	NULL;
	m_pPixelShader		=	NULL;
	m_pConstantTable	=	NULL;
}

CPixelShader::~CPixelShader(void)
{
	//clean up shaders (NEW)
	if(m_pConstantTable)
	{
		m_pConstantTable->Release();
		m_pConstantTable = NULL;
	}

	if(m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = NULL;
	}
}

bool CPixelShader::Create(const char* szShaderFile, LPDIRECT3DDEVICE9 lpD3DDevice)
{
	LPD3DXBUFFER pCode = NULL;
	LPD3DXBUFFER pErrors = NULL;


	HRESULT result = D3DXCompileShaderFromFile(szShaderFile,    //filepath
                                   NULL,						//macro's
                                   NULL,						//includes
                                   "ps_main",       			//main function
                                   "ps_2_0",        			//shader profile
                                   0,               			//flags
                                   &pCode,          			//compiled operations
                                   &pErrors,            			//errors
                                   &m_pConstantTable);			//constants
	if (result != D3D_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrors->GetBufferPointer(), "Error - CPixelShader::Create()", MB_OK | MB_ICONEXCLAMATION);
		//pCode->Release();
		pErrors->Release();

		return false;
	}

	// Create the pixel shader
	result = lpD3DDevice->CreatePixelShader( (DWORD*)pCode->GetBufferPointer(), &m_pPixelShader);

	if (result != D3D_OK)
	{
		pCode->Release();
		MessageBox(NULL, (LPCSTR)pErrors->GetBufferPointer(), "Error - CPixelShader::Create()", MB_OK | MB_ICONEXCLAMATION);
		pErrors->Release();
		return false;
	}

	pCode->Release();

	m_pD3DDevice = lpD3DDevice;

	return true;
}

bool CPixelShader::Begin(void) const
{
	if (!m_pD3DDevice)
		return false;

	m_pD3DDevice->SetPixelShader(m_pPixelShader);

	return true;
}

bool CPixelShader::End(void) const
{
	if (!m_pD3DDevice)
		return false;

	m_pD3DDevice->SetPixelShader(NULL);

	return true;
}

bool CPixelShader::SetConstantFloat(const char* szConstantName, float fValue)
{
	if (!m_pConstantTable)
		return false;

	D3DXHANDLE handle = m_pConstantTable->GetConstantByName(NULL, szConstantName);

	if (!handle)
		return false;

	m_pConstantTable->SetFloat(m_pD3DDevice, handle, fValue);

	return true;
}