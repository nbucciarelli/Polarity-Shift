#pragma once

class display
{
protected:
	struct _wnd {
		HINSTANCE   hInst;
		HWND        hWnd;
		LPCSTR		className;
		LPCSTR		title;
		HBRUSH		bgBrush;

		UINT		width;
		UINT		height;
		POINT		position;
		UINT		style;

		_wnd();
		~_wnd();
	} wnd;

	//static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

	display(void);
	display(const display&);
	display& operator=(const display&);
	virtual ~display(void);

public:
	static display* getInstance();

	void InitWindow(HINSTANCE);
	void ShutdownWindow();

	void HandleWindowMsgs();

	HWND getHWnd();
};
