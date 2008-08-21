#pragma once

class viewManager;

#define CHARSET "Resource/PS_charset.png"
#define CHARWIDTH 32
#define CHARHEIGHT 48

class bitFont
{
protected:
	viewManager* VM;

	//Charset ID
	int csId;

	int charHeight, charWidth, columns;

	//Singleton protection
	bitFont(void);
	bitFont(const bitFont&);
	bitFont& operator=(const bitFont&);
	~bitFont(void);

	//single char helper
	rect charRect(char ch);
public:

	static bitFont* getInstance();

	void initialize(viewManager* _VM, char* fontImg = CHARSET, int char_width = CHARWIDTH,
		int char_height = CHARHEIGHT);
	void shutdown();

	void drawText(char* text, int x, int y, unsigned int color = 0xffffffff, float scale = 1, float depth = 0);
};
