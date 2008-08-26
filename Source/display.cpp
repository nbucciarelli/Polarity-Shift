#include <windows.h>
#include "display.h"
#include "game.h"

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 600

#pragma region windows messages
namespace {
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
	case ( WM_DESTROY ):
		game::GetInstance()->setIsRunning(false);
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}
}

void display::HandleWindowMsgs()
{
	MSG msg; ZeroMemory( &msg, sizeof( msg ) );
	if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
	{ 
		TranslateMessage( &msg );
		DispatchMessage( &msg ); 
	}
}
#pragma endregion

#pragma region constructors/destructors
display::display(void) {}
display::~display(void) {}

display::_wnd::_wnd() : 
hInst(NULL), hWnd(NULL),
className("GameDisplay"), title("Polarity Shift"),
width(DEFAULT_SCREEN_WIDTH), height(DEFAULT_SCREEN_HEIGHT),
style(WS_OVERLAPPEDWINDOW), bgBrush((HBRUSH)(COLOR_WINDOWFRAME))
{ position.x = position.y = CW_USEDEFAULT; }

display::_wnd::~_wnd() {}
#pragma endregion

#pragma region singleton
display* display::getInstance()
{
	static display ygdrassil;

	return &ygdrassil;
}
#pragma endregion

#pragma region init/shutdown
void display::InitWindow(HINSTANCE hInst)
{
	wnd.hInst = hInst;

	WNDCLASSEX  wndClass;
    // register a new type of window
    ZeroMemory( &wndClass, sizeof( wndClass ) );
    wndClass.cbSize         = sizeof( WNDCLASSEX );				// size of window structure
    wndClass.lpfnWndProc    = (WNDPROC)WndProc;					// message callback
	wndClass.lpszClassName  = wnd.className;					// class name
    wndClass.hInstance      = wnd.hInst;						// handle to application
    wndClass.hCursor        = LoadCursor( NULL, IDC_ARROW );    // default cursor
    wndClass.hbrBackground  = wnd.bgBrush;  // background brush
    RegisterClassEx( &wndClass );

    // create window of registered type
	wnd.hWnd = CreateWindow(
		wnd.className, wnd.title,			// class name, window title 
		wnd.style,							// window style
        wnd.position.x, wnd.position.y,		// x & y coordinates
		wnd.width, wnd.height,				// width & height of window
        NULL, NULL,							// parent window & menu
        wnd.hInst,							// handle to application
        NULL );

	ShowWindow( wnd.hWnd, SW_NORMAL );
	UpdateWindow( wnd.hWnd );
}

void display::ShutdownWindow()
{
	UnregisterClass(wnd.className, wnd.hInst);
}
#pragma endregion



HWND display::getHWnd()
{
	return wnd.hWnd;
}