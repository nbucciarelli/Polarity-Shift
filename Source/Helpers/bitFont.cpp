////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: "bitFont.cpp"
// Author: Scott Smallback (SS)
// Purpose: This file is necessary to render bitmap fonts onto the screen.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "datatypes.h"
#include "bitFont.h"
#include "..\Wrappers\viewManager.h"
#include "physics.h"

#define CHARSET "Resource/PS_charset.png"
#define CHARWIDTH 32
#define CHARHEIGHT 48

bitFont::bitFont(void)
{
}

bitFont::~bitFont(void)
{
	
}

bitFont* bitFont::getInstance()
{
	static bitFont Deckard_Cain;
	return &Deckard_Cain;
}

void bitFont::initialize(viewManager* _VM, char* fontImg, int char_width, int char_height)
{
	VM = _VM;

	csId = VM->loadTexture(fontImg);
	charWidth = char_width;
	charHeight = char_height;

	pt dim = VM->getTextureDimensions(csId);
	columns = dim.x / charWidth;
}

void bitFont::shutdown()
{
	VM->releaseTexture(csId);
}

void bitFont::drawText(char* text, int x, int y, unsigned int color, float scale, float depth)
{
	vector3 pos(float(x), float(y), depth);

	matrix scaling;
	calc::matrixScale(scaling, vector3(scale,scale,scale));

	for(int c = 0; text[c] != '\0'; c++)
	{
		if(text[c] != ' ') //Don't bother calling render on a space.
			VM->drawTexture(csId, &pos, &scaling, &charRect(text[c]), NULL, color);
		pos.x += charWidth;
	}
}

rect bitFont::charRect(char ch)
{
	rect holder;

	ch -= 32;

	holder.top = (ch / columns) * charHeight;
	holder.bottom = holder.top + charHeight;
	holder.left = (ch % columns) * charWidth;
	holder.right = holder.left + charWidth;

	return holder;
}