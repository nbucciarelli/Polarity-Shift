///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File:		"CBitmampFont.cpp"
//
//	Author:		Nick Bucciarelli (NB)
//
//	Purpose:	Display text with bitmaps
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CBitmapFont.h"
#include "CSGD_TextureManager.h"

CBitmapFont::CBitmapFont()
{
	SetFontImageID(-1);
	SetCharWidth(0);
	SetNumCols(0);
	SetCharHeight(0);
}

void CBitmapFont::DrawString(const char* szText, int nPosX, int nPosY, float fScaleX, float fScaleY, int nSpacing, DWORD color)
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	RECT r;
	int id;
	for (unsigned int i = 0; i < strlen(szText); ++i)
	{
		char ch = szText[i];
		ch = toupper(ch);
		id = ch - (int)GetStartChar();
		r = CalcRect(id);
		pTM->Draw(m_nFontImageID, (int)(nPosX + (i*GetCharWidth()*fScaleX) + (i*nSpacing)), (int)(nPosY), fScaleX, fScaleY, &r, 0.0f, 0.0f, 0.0f, color);
	}
}


void CBitmapFont::DrawLetter(const char szLetter, int nPosX, int nPosY, float fScaleX, float fScaleY, int nSpacing, DWORD color)
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	int id = toupper(szLetter) - GetStartChar();
	RECT r = CalcRect(id);
	pTM->Draw(m_nFontImageID, (int)(nPosX + (GetCharWidth()*fScaleX) + (nSpacing)), (int)(nPosY), fScaleX, fScaleY, &r, 0.0f, 0.0f, 0.0f, color);

}

RECT CBitmapFont::CalcRect(int id)
{
	RECT r;

	r.left		= (id % GetNumCols()) * GetCharWidth();
	r.right		= r.left + GetCharWidth();
	r.top		= (id / GetNumCols()) * GetCharHeight();
	r.bottom	= r.top + GetCharHeight();

	return r;
}
