////////////////////////////////////////////////////////////////
//	File			:	"CSGD_DirectInput.h"
//
//	Author			:	David Brown (DB)
//	Based in part on:
//		-Input code from the book: "OpenGL Game Programming" by Kevin Hawkins and Dave Astle.
//		-The previous CSGD_DirectInput wrapper by Jensen Rivera.
//
//	Creation Date	:	5/25/2008
//
//	Purpose			:	To wrap DirectInput functionality.  
//						Allows the use of Keyboard, Mouse, 
//						and Joysticks through DirectInput.
//
//	Special thanks to:  Ari Patrick for testing this wrapper and 
//						providing the product name for wireless Xbox360 controllers.
/////////////////////////////////////////////////////////////////

/*
Disclaimer:
This source code was developed for and is the property of
Full Sail University's Game Development Program (c) 2008.
Full Sail students may not redistribute this code, but may
use it in any project used for educational purposes.
*/

#pragma once

//	The version of DirectInput to use.
#ifndef DIRECTINPUT_VERSION
	#define DIRECTINPUT_VERSION 0X800
#endif
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <vector>
using std::vector;

#include "SGD_Util.h"	//	for utility macros

//	Forward declarations:
class CSGD_DIKeyboard;
class CSGD_DIMouse;
class CSGD_DIJoystick;

//	Enumerations:
//		Devices:
enum DeviceFlags { DI_KEYBOARD = 1, DI_MOUSE = 2, DI_JOYSTICKS = 4 };

//		Directions
enum Directions { DIR_LEFT = 0, DIR_RIGHT = 1, DIR_UP = 2, DIR_DOWN = 3, DIR_MAX };

//		Mouse buttons
enum MouseButtons { MOUSE_LEFT = 0, MOUSE_RIGHT = 1, MOUSE_MIDDLE = 2, MOUSE_MAX };

class CSGD_DirectInput
{
private:
	LPDIRECTINPUT8				m_lpDIObject;	//	DirectInput COM Object
	
	CSGD_DIKeyboard*			m_pKeyboard;	//	Keyboard object
	CSGD_DIMouse*				m_pMouse;		//	Mouse object
	vector<CSGD_DIJoystick*>	m_vpJoysticks;	//	Joysticks attached to the system

	// Utility struct for joystick setup:
	struct tJoystickSetupInfo
	{
		HWND hWnd;
		bool bIsExclusive;
	};

	static CSGD_DirectInput	m_Instance;		//	The instance of this class.

	//	Trilogy of EVIL!

	//	Constructor
	CSGD_DirectInput(void);
	//	Copy Constructor
	CSGD_DirectInput(const CSGD_DirectInput&);
	//	Assignment Operator
	CSGD_DirectInput& operator=(const CSGD_DirectInput&);

	//	Plus Destructor
	~CSGD_DirectInput(void);

	//	Private utility functions:
	///////////////////////////////////////////////////////////////////
	//	Function:	"InitKeyboard"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	hWnd			-	The Main window handle.				
	//				bIsExclusive	-	Whether or not you want exclusive control of the device.
	//
	//	Return	:	true if it was able to initialize the Keyboard.
	//
	//	Purpose	:	Initialize the Keyboard for DirectInput.
	///////////////////////////////////////////////////////////////////
	bool InitKeyboard(HWND hWnd, bool bIsExclusive = false);

	///////////////////////////////////////////////////////////////////
	//	Function:	"InitMouse"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	hWnd			-	The Main window handle.				
	//				bIsExclusive	-	Whether or not you want exclusive control of the device.
	//
	//	Return	:	true if it was able to initialize the Mouse.
	//
	//	Purpose	:	Initialize the Mouse for DirectInput.
	///////////////////////////////////////////////////////////////////
	bool InitMouse(HWND hWnd, bool bIsExclusive = false);

	///////////////////////////////////////////////////////////////////
	//	Function:	"InitJoysticks"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	hWnd			-	The Main window handle.				
	//				bIsExclusive	-	Whether or not you want exclusive control of the device.
	//
	//	Return	:	true if it was able to initialize the joysticks.
	//
	//	Purpose	:	Initialize the joysticks for DirectInput.
	///////////////////////////////////////////////////////////////////
	bool InitJoysticks(HWND hWnd, bool bIsExclusive = false);

	///////////////////////////////////////////////////////////////////
	//	Function:	"EnumJoysticksCallback"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	lpddi		-	Pointer to the DirectInput Device Instance of the joystick that was found.
	//				pVoid		-	Pointer to something you want to use for this function.
	//
	//	Return	:	Returns after all of the joysticks connected to the computer have been found.
	//
	//	Purpose	:	Checks to see if there are any Joysticks presently attached to the computer.
	///////////////////////////////////////////////////////////////////
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* lpdidi, VOID* pVoid);

public:
	/////////////////////////////////////////////////////////////////////
	//	Function:	"GetInstance"
	//
	//	Last Modified:	5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	The instance of this class.
	//
	//	Purpose	:	Gets the instance of this class.
	////////////////////////////////////////////////////////////////////
	static CSGD_DirectInput* GetInstance(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"InitDirectInput"
	//
	//	Last Modified:		7/05/2008
	//
	//	Input	:	hWnd				-	The Main window handle.
	//				hInstance			-	The Main application Instance.
	//				unInitDevices		-	A combination of flags representing which
	//										devices to initialize (i.e. DI_KEYBOARD | DI_MOUSE).
	//				unExclusiveDevices	-	A combination of flags representing which
	//										devices to make exclusive (i.e. DI_JOYSTICKS).
	//				
	//	Return	:	true if it initialized the DirectInput object and all
	//				of the requested devices.
	//
	//	Purpose	:	To initialize the DirectInput Object and requested devices.
	///////////////////////////////////////////////////////////////////
	bool InitDirectInput(HWND hWnd, HINSTANCE hInstance, unsigned int unInitDevices, unsigned int unExclusiveDevices = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ShutdownDirectInput"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input:		void
	//
	//	Return:		void
	//
	//	Purpose:	Shuts down DirectInput and any initialized devices.
	///////////////////////////////////////////////////////////////////
	void ShutdownDirectInput(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ReadDevices"
	//
	//	Last Modified:		6/13/2008
	//
	//	Input	:	void
	//
	//	Return	:	void
	//
	//	Purpose	:	Gets the state of all the initialized devices.
	//				Reads buffered data as well (for Ex functions).
	///////////////////////////////////////////////////////////////////	
	void ReadDevices(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"AcquireAll"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	void
	//
	//	Purpose	:	Acquires all the initialized direct input devices (i.e. keyboard, mouse, and joysticks).
	//				Ideally called when the application gains focus.
	///////////////////////////////////////////////////////////////////	
	void AcquireAll(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"UnacquireAll"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	void
	//
	//	Purpose	:	Unacquires all the initialized direct input devices (i.e. keyboard, mouse, and joysticks).
	//				Ideally called when the application loses focus.
	///////////////////////////////////////////////////////////////////	
	void UnacquireAll(void);

	/***********************	Device specific functions:	********************/

	///////////////////////////////////////////////////////////////////
	//						KEYBOARD								 //
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	//	Function:	"KeyDown"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucDIKey	-	The DirectInput Keyboard Constant to check, such as DIK_ESCAPE.
	//
	//	Return	:	true if the Key is currently down.
	//
	//	Purpose	:	Gets the current (immediate) state of a Key.
	///////////////////////////////////////////////////////////////////
	bool KeyDown(unsigned char ucDIKey);
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"KeyPressed"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucDIKey	-	The DirectInput Keyboard Constant to check, such as DIK_ESCAPE.
	//
	//	Return	:	true if Key was pressed this frame.
	//
	//	Purpose	:	To tell if a key was just pressed (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool KeyPressed(unsigned char ucDIKey);

	///////////////////////////////////////////////////////////////////
	//	Function:	"KeyPressedEx"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucDIKey	-	The DirectInput Keyboard Constant to check, such as DIK_ESCAPE.
	//
	//	Return	:	true if the Key was pressed this frame.
	//
	//	Purpose	:	To tell if a key was just pressed (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool KeyPressedEx(unsigned char ucDIKey);

	///////////////////////////////////////////////////////////////////
	//	Function:	"KeyUp"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucDIKey	-	The DirectInput Keyboard Constant to check, such as DIK_ESCAPE.
	//
	//	Return	:	true if the Key is currently up.
	//
	//	Purpose	:	Gets the current (immediate) state of a Key.
	///////////////////////////////////////////////////////////////////
	bool KeyUp(unsigned char ucDIKey);
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"KeyReleased"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucDIKey	-	The DirectInput Keyboard Constant to check, such as DIK_ESCAPE.
	//
	//	Return	:	true if the Key was released this frame.
	//
	//	Purpose	:	To tell if a key was just released (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool KeyReleased(unsigned char ucDIKey);

	///////////////////////////////////////////////////////////////////
	//	Function:	"KeyReleasedEx"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucDIKey	-	The DirectInput Keyboard Constant to check, such as DIK_ESCAPE.
	//
	//	Return	:	true if the Key was released this frame.
	//
	//	Purpose	:	To tell if a key was just released (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool KeyReleasedEx(unsigned char ucDIKey); 

	///////////////////////////////////////////////////////////////////
	//	Function:	"CheckKeys"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	0 if nothing was pressed, otherwise the ascii character 
	//				value of the key the user pressed.
	//
	//	Purpose	:	To get the ascii character value of the key the user typed.
	//				So they can enter letters.
	///////////////////////////////////////////////////////////////////
	char CheckKeys(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"CheckBufferedKeysEx"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	0 if nothing was pressed, otherwise the ascii character 
	//				value of the key the user pressed.
	//
	//	Purpose	:	To get the ascii character value of the key the user just typed.
	//				So they can enter letters (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	char CheckBufferedKeysEx(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetDIKCode"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	0 if nothing was pressed, otherwise the DIK code 
	//				of the key the user pressed.
	//
	//	Purpose	:	To get the DIK code of the key the user is pressing.
	//				For key binding.
	///////////////////////////////////////////////////////////////////
	unsigned char GetDIKCode(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetBufferedDIKCodeEx"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	0 if nothing was pressed, otherwise the DIK code 
	//				of the key the user pressed.
	//
	//	Purpose	:	To get the DIK code of the key the user just pressed.
	//				For key binding (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	unsigned char GetBufferedDIKCodeEx(void);

	///////////////////////////////////////////////////////////////////
	//						MOUSE								     //
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	//	Function:	"MouseButtonDown"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucButton	-	The mouse button to check for (i.e. 0, 1, or 2)
	//								0 = left, 1 = right, 2 = middle
	//
	//	Return	:	true if the mouse button is currently down.
	//
	//	Purpose	:	Gets the current (immediate) state of a mouse button.
	///////////////////////////////////////////////////////////////////
	bool MouseButtonDown(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"MouseButtonPressed"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucButton	-	The mouse button to check for (i.e. 0, 1, or 2)
	//								0 = left, 1 = right, 2 = middle
	//
	//	Return	:	true if the mouse button was pressed this frame.
	//
	//	Purpose	:	To tell if a mouse button was just pressed (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool MouseButtonPressed(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"MouseButtonPressedEx"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucButton	-	The mouse button to check for (i.e. 0, 1, or 2)
	//								0 = left, 1 = right, 2 = middle
	//
	//	Return	:	true if the mouse button was pressed this frame.
	//
	//	Purpose	:	To tell if a mouse button was just pressed (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool MouseButtonPressedEx(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"MouseButtonUp"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucButton	-	The mouse button to check for (i.e. 0, 1, or 2)
	//								0 = left, 1 = right, 2 = middle
	//
	//	Return	:	true if the mouse button is currently up.
	//
	//	Purpose	:	Gets the current (immediate) state of a mouse button.
	///////////////////////////////////////////////////////////////////
	bool MouseButtonUp(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"MouseButtonReleased"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucButton	-	The mouse button to check for (i.e. 0, 1, or 2)
	//								0 = left, 1 = right, 2 = middle
	//
	//	Return	:	true if the mouse button was released this frame.
	//
	//	Purpose	:	To tell if a mouse button was just released (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool MouseButtonReleased(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"MouseButtonReleasedEx"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucButton	-	The mouse button to check for (i.e. 0, 1, or 2)
	//								0 = left, 1 = right, 2 = middle
	//
	//	Return	:	true if the mouse button was released this frame.
	//
	//	Purpose	:	To tell if a mouse button was just released (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool MouseButtonReleasedEx(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"MouseCheckBufferedButtons"
	//
	//	Last Modified:		6/13/2008
	//
	//	Input	:	void
	//
	//	Return	:	The mouse button that was just pressed (i.e. 0, 1, or 2).  -1 if there was no button pressed.
	//
	//	Purpose	:	To tell if a mouse button was just pressed (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	int MouseCheckBufferedButtons(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"MouseCheckBufferedButtonsEx"
	//
	//	Last Modified:		5/27/2008
	//
	//	Input	:	void
	//
	//	Return	:	The mouse button that was just pressed (i.e. 0, 1, or 2).  -1 if there was no button pressed.
	//
	//	Purpose	:	To tell if a mouse button was just pressed (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	int MouseCheckBufferedButtonsEx(void);

	///////////////////////////////////////////////////////////////////
	//	Function		:	"MouseMovementX"
	//
	//	Last Modified	:	5/25/2008
	//
	//	Input			:	void
	//
	//	Return			:	The amount the mouse has moved this frame.
	//
	//	Purpose			:	To report the relative motion of the mouse 
	//						in the X dimension.
	///////////////////////////////////////////////////////////////////
	long MouseMovementX(void);

	///////////////////////////////////////////////////////////////////
	//	Function		:	"MouseMovementY"
	//
	//	Last Modified	:	5/25/2008
	//
	//	Input			:	void
	//
	//	Return			:	The amount the mouse has moved this frame.
	//
	//	Purpose			:	To report the relative motion of the mouse 
	//						in the Y dimension.
	///////////////////////////////////////////////////////////////////
	long MouseMovementY(void);

	///////////////////////////////////////////////////////////////////
	//	Function		:	"MouseWheelMovement"
	//
	//	Last Modified	:	5/25/2008
	//
	//	Input			:	void
	//
	//	Return			:	The amount the mouse wheel has moved this frame.
	//						Positive(+) values mean pushing the wheel away from you.
	//						Negative(-) values mean pulling the wheel toward you.
	//
	//	Purpose			:	To report the relative motion of the mouse 
	//						wheel.
	///////////////////////////////////////////////////////////////////
	long MouseWheelMovement(void);

	///////////////////////////////////////////////////////////////////
	//	Function		:	"MouseGetNumButtons"
	//
	//	Last Modified	:	5/26/2008
	//
	//	Input			:	void
	//
	//	Return			:	The number of buttons supported on the device.
	//
	//	Purpose			:	To report the number of buttons on the device.
	///////////////////////////////////////////////////////////////////
	int MouseGetNumButtons(void);

	///////////////////////////////////////////////////////////////////
	//	Function		:	"MouseGetPosX"
	//
	//	Last Modified	:	6/23/2008
	//
	//	Input			:	void
	//
	//	Return			:	The "psuedo" position of the mouse.
	//
	//	Purpose			:	To help track the "psuedo" position of the mouse.
	///////////////////////////////////////////////////////////////////
	int MouseGetPosX(void);

	///////////////////////////////////////////////////////////////////
	//	Function		:	"MouseGetPosY"
	//
	//	Last Modified	:	6/23/2008
	//
	//	Input			:	void
	//
	//	Return			:	The "psuedo" position of the mouse.
	//
	//	Purpose			:	To help track the "psuedo" position of the mouse.
	///////////////////////////////////////////////////////////////////
	int MouseGetPosY(void);

	///////////////////////////////////////////////////////////////////
	//	Function		:	"MouseSetPosX"
	//
	//	Last Modified	:	7/05/2008
	//
	//	Input			:	The "psuedo" position of the mouse.
	//
	//	Return			:	void
	//
	//	Purpose			:	To set the "psuedo" position of the mouse.
	///////////////////////////////////////////////////////////////////
	void MouseSetPosX(int nPosX);

	///////////////////////////////////////////////////////////////////
	//	Function		:	"MouseSetPosY"
	//
	//	Last Modified	:	7/05/2008
	//
	//	Input			:	The "psuedo" position of the mouse.
	//
	//	Return			:	void
	//
	//	Purpose			:	To set the "psuedo" position of the mouse.
	///////////////////////////////////////////////////////////////////
	void MouseSetPosY(int nPosY);

	///////////////////////////////////////////////////////////////////
	//						JOYSTICKS								 //
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickButtonDown"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	ucButton	-	The joystick button to check for.
	//				nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the joystick button is currently down.
	//
	//	Purpose	:	Gets the current (immediate) state of a joystick button.
	///////////////////////////////////////////////////////////////////
	bool JoystickButtonDown(unsigned char ucButton, int nJoyNum = 0);		
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickButtonPressed"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	ucButton	-	The joystick button to check for.
	//				nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the joystick button was pressed this frame.
	//
	//	Purpose	:	To tell if a joystick button was just pressed (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool JoystickButtonPressed(unsigned char ucButton, int nJoyNum = 0);	
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickButtonPressedEx"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	ucButton	-	The joystick button to check for.
	//				nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the joystick button was pressed this frame.
	//
	//	Purpose	:	To tell if a joystick button was just pressed (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool JoystickButtonPressedEx(unsigned char ucButton, int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickButtonUp"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	ucButton	-	The joystick button to check for.
	//				nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the joystick button is currently up.
	//
	//	Purpose	:	Gets the current (immediate) state of a joystick button.
	///////////////////////////////////////////////////////////////////
	bool JoystickButtonUp(unsigned char ucButton, int nJoyNum = 0);		
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickButtonReleased"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	ucButton	-	The joystick button to check for.
	//				nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the joystick button was released this frame.
	//
	//	Purpose	:	To tell if a joystick button was just released (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool JoystickButtonReleased(unsigned char ucButton, int nJoyNum = 0);	
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickButtonReleasedEx"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	ucButton	-	The joystick button to check for.
	//				nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the joystick button was released this frame.
	//
	//	Purpose	:	To tell if a joystick button was just released (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool JoystickButtonReleasedEx(unsigned char ucButton, int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	D-pad:
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickDPadDown"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir		-	The direction to check (i.e. DIR_RIGHT).
	//				nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the D-pad direction is currently being held down.
	//
	//	Purpose	:	To tell if a D-pad direction is down.
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	bool JoystickDPadDown(int nDir, int nJoyNum = 0); 
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickDPadPressed"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir		-	The direction to check (i.e. DIR_RIGHT).
	//				nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the D-pad was pressed a certain direction.
	//
	//	Purpose	:	To get the (buffered) direction the D-pad was pressed (without using DirectInput's Buffered Input).
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	bool JoystickDPadPressed(int nDir, int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickDPadUp"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir		-	The direction to check (i.e. DIR_RIGHT).
	//				nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the D-pad direction is currently up.
	//
	//	Purpose	:	To tell if a D-pad direction is up.
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	bool JoystickDPadUp(int nDir, int nJoyNum = 0); 
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickDPadReleased"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir		-	The direction to check (i.e. DIR_RIGHT).
	//				nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the D-pad direction was released this frame.
	//
	//	Purpose	:	To tell if a D-pad direction was just released (without using DirectInput's Buffered Input).
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	bool JoystickDPadReleased(int nDir, int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Analog Sticks:
	///////////////////////////////////////////////////////////////////

	//	L-stick:
	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetLStickDirDown"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir		-	The direction to check (i.e. DIR_RIGHT).
	//				nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the left stick is currently held a certain direction.
	//
	//	Purpose	:	To get the (immediate) direction the stick is being held.
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	bool JoystickGetLStickDirDown(int nDir, int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetLStickDirPressed"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir		-	The direction to check (i.e. DIR_RIGHT).
	//				nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the left stick was pressed a certain direction.
	//
	//	Purpose	:	To get the (buffered) direction the stick was pressed (without using DirectInput's Buffered Input).
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	bool JoystickGetLStickDirPressed(int nDir, int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetLStickXAmount"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	-JOYSTICK_RANGE to +JOYSTICK_RANGE ((-) Left and (+) Right)
	//
	//	Purpose	:	To get the position of the joystick in a digital range.
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	int JoystickGetLStickXAmount(int nJoyNum = 0);
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetLStickYAmount"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	-JOYSTICK_RANGE to +JOYSTICK_RANGE (-JOYSTICK_RANGE is Up, +JOYSTICK_RANGE is Down)
	//
	//	Purpose	:	To get the position of the joystick in a digital range.
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	int JoystickGetLStickYAmount(int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetLStickXNormalized"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	-1.0 to +1.0 (-1.0 is Left, +1.0 is Right)
	//
	//	Purpose	:	To get the position of the joystick in a normalized range.
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	float JoystickGetLStickXNormalized(int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetLStickYNormalized"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	-1.0 to +1.0 (-1.0 is Up, +1.0 is Down)
	//
	//	Purpose	:	To get the position of the joystick in a normalized range.
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	float JoystickGetLStickYNormalized(int nJoyNum = 0);

	// R-stick:
	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetRStickDirDown"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir		-	The direction to check (i.e. DIR_RIGHT).
	//				nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the right stick is currently held a certain direction.
	//
	//	Purpose	:	To get the (immediate) direction the stick is being held.
	///////////////////////////////////////////////////////////////////
	bool JoystickGetRStickDirDown(int nDir, int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetRStickDirPressed"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir		-	The direction to check (i.e. DIR_RIGHT).
	//				nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the right stick was pressed a certain direction.
	//
	//	Purpose	:	To get the (buffered) direction the stick was pressed (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////	
	bool JoystickGetRStickDirPressed(int nDir, int nJoyNum = 0);
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetRStickXAmount"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	-JOYSTICK_RANGE to +JOYSTICK_RANGE ((-) Left and (+) Right)
	//
	//	Purpose	:	To get the position of the joystick in a digital range.
	///////////////////////////////////////////////////////////////////
	int JoystickGetRStickXAmount(int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetRStickYAmount"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	-JOYSTICK_RANGE to +JOYSTICK_RANGE (-JOYSTICK_RANGE is Up, +JOYSTICK_RANGE is Down)
	//
	//	Purpose	:	To get the position of the joystick in a digital range.
	///////////////////////////////////////////////////////////////////
	int JoystickGetRStickYAmount(int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetRStickXNormalized"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	-1.0 to +1.0 (-1.0 is Left, +1.0 is Right)
	//
	//	Purpose	:	To get the position of the joystick in a normalized range.
	///////////////////////////////////////////////////////////////////
	float JoystickGetRStickXNormalized(int nJoyNum = 0);
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetRStickYNormalized"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	-1.0 to +1.0 (-1.0 is Up, +1.0 is Down)
	//
	//	Purpose	:	To get the position of the joystick in a normalized range.
	///////////////////////////////////////////////////////////////////
	float JoystickGetRStickYNormalized(int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Triggers:
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetLTriggerAmount"
	//
	//	Last Modified:		6/12/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	0 to JOYSTICK_AXIS_RANGE
	//
	//	Purpose	:	To get the position of the trigger in a digital range.
	//
	//	NOTE: If both triggers are pulled at the same time the reading won't 
	//		  be accurate as their input is combined into one axis by DirectInput.
	///////////////////////////////////////////////////////////////////	
	int JoystickGetLTriggerAmount(int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetRTriggerAmount"
	//
	//	Last Modified:		6/012/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	0 to JOYSTICK_AXIS_RANGE
	//
	//	Purpose	:	To get the position of the trigger in a digital range.
	//
	//	NOTE: If both triggers are pulled at the same time the reading won't 
	//		  be accurate as their input is combined into one axis by DirectInput.
	///////////////////////////////////////////////////////////////////	
	int JoystickGetRTriggerAmount(int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetLTriggerNormalized"
	//
	//	Last Modified:		6/12/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	0.0 to 1.0 to (0.0 is not pressed, +1.0 is pulled all the way down)
	//
	//	Purpose	:	To get the position of the trigger in a normalized range.
	//
	//	NOTE: If both triggers are pulled at the same time the reading won't 
	//		  be accurate as their input is combined into one axis by DirectInput.
	///////////////////////////////////////////////////////////////////
	float JoystickGetLTriggerNormalized(int nJoyNum = 0);
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetRTriggerNormalized"
	//
	//	Last Modified:		6/12/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	0.0 to 1.0 to (0.0 is not pressed, +1.0 is pulled all the way down)
	//
	//	Purpose	:	To get the position of the trigger in a normalized range.
	//
	//	NOTE: If both triggers are pulled at the same time the reading won't 
	//		  be accurate as their input is combined into one axis by DirectInput.
	///////////////////////////////////////////////////////////////////
	float JoystickGetRTriggerNormalized(int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickCheckBufferedButtons"
	//
	//	Last Modified:		5/27/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	The joystick button that was just pressed.  -1 if no button was pressed.
	//
	//	Purpose	:	To tell which joystick button was just pressed (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	int	JoystickCheckBufferedButtons(int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickCheckBufferedButtonsEx"
	//
	//	Last Modified:		6/07/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	The joystick button that was just pressed.  -1 if no button was pressed.
	//
	//	Purpose	:	To tell which joystick button was just pressed (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	int	JoystickCheckBufferedButtonsEx(int nJoyNum = 0);

	///////////////////////////////////////////////////////////////////
	//	Function		:	"JoystickGetNumButtons"
	//
	//	Last Modified	:	5/26/2008
	//
	//	Input			:	nJoyNum		-	The joystick to check.
	//
	//	Return			:	The number of buttons supported on the device.
	//
	//	Purpose			:	To report the number of buttons on the device.
	///////////////////////////////////////////////////////////////////
	int JoystickGetNumButtons(int nJoyNum = 0) const;

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickIsUnplugged"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	true if the joystick is unplugged.
	//
	//	Purpose	:	Returns the true if the joystick is unplugged.
	///////////////////////////////////////////////////////////////////
	bool JoystickIsUnplugged(int nJoyNum = 0) const;

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickGetName"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	nJoyNum		-	The joystick to check.
	//
	//	Return	:	The name of the joystick.
	//
	//	Purpose	:	Returns the name of the joystick (i.e. "Logitech RumblePad 2 USB")
	///////////////////////////////////////////////////////////////////
	const char* JoystickGetName(int nJoyNum = 0) const;

	///////////////////////////////////////////////////////////////////
	//	Function:	"JoystickSwapRStickAxes"
	//
	//	Last Modified:		6/13/2008
	//
	//	Input	:	bSwapAxes	-	Whether or not to swap the X/Y axes of the Rstick.
	//				nJoyNum		-	The joystick to set.
	//
	//	Return	:	void
	//
	//	Purpose	:	To swap the X/Y axes of certain controllers.
	//
	//	NOTE:	Z axis is considered R-stick's X-axis by default.
	//			Only works on non-Xbox360 controllers.
	///////////////////////////////////////////////////////////////////
	void JoystickSwapRStickAxes(bool bSwapAxes, int nJoyNum = 0);

};  //	end class CSGD_DirectInput

////////////////////////////////////////////////////////////////////////////////////////////

const int g_knBUFFER_SIZE = 10;	//	Arbitrary number of elements for Buffered Input.

//	Interface class defining "blue print" functions.
class ISGD_DirectInputDevice
{
public:
	virtual ~ISGD_DirectInputDevice(void) = 0 {};

	virtual bool ReadDevice(void) = 0;
	virtual bool ReadBufferedDevice(void) = 0;
	
	virtual bool Acquire(void) = 0;
	virtual bool Unacquire(void) = 0;

}; 	//	end class ISGD_DirectInputDevice

////////////////////////////////////////////////////////////////////////////////////////////

//	Abstract class with partial functionality.
class CSGD_DirectInputDevice : public ISGD_DirectInputDevice
{
protected:
	LPDIRECTINPUTDEVICE8	m_lpDevice;					//	DirectInput Device pointer.
	
	//	For DirectX Buffered input:
	DIDEVICEOBJECTDATA		m_didod[g_knBUFFER_SIZE];	//	Receives buffered data for the device.
	DWORD					m_dwBufferElements;			//	Number of element in the buffer for the device.

	//	Utility function to clear buffered data.
	void ClearBufferedData(void)
	{
		memset(&m_didod, 0, sizeof(m_didod));	//	clear out device object data.
		m_dwBufferElements = g_knBUFFER_SIZE;	//	set buffer elements to the size of the array.
	}

public:
	//	Constructor.
	CSGD_DirectInputDevice(void)
	{
		m_lpDevice = NULL;

		ClearBufferedData();
	}

	//	Destructor.
	virtual ~CSGD_DirectInputDevice(void)
	{
		if (m_lpDevice)
		{
			m_lpDevice->Unacquire();
			SAFE_RELEASE(m_lpDevice);
		}
	}

	//	Common way to read DirectInput buffered data.
	///////////////////////////////////////////////////////////////////
	//	Function:	"ReadBufferedDevice"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	void
	//
	//	Return	:	true if it can read the device data, false if it can't.
	//
	//	Purpose	:	Gets the DirectInput Buffered Data.
	//
	//	NOTE	:	For all Ex functions.
	///////////////////////////////////////////////////////////////////
	virtual bool ReadBufferedDevice(void)
	{
		//	Make sure the device was made.
		if (!m_lpDevice)
			return false;

		ClearBufferedData();

		//	Attempt to read the device...
		if (FAILED( m_lpDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_didod, &m_dwBufferElements, 0) ))
		{
			//	If we couldn't, try to re-acquire the device.
			if (FAILED( m_lpDevice->Acquire() ))
				return false; //	Could not re-acquire the Device.

			ClearBufferedData();

			//	Now try reading it
			if (FAILED( m_lpDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_didod, &m_dwBufferElements, 0) ))
				return false;
		}

		//	Success
		return true;
	}
	
	//	returns true if the device was able to be acquired.
	virtual bool Acquire(void)		{ return (m_lpDevice) ? SUCCEEDED( m_lpDevice->Acquire() ) : false; }

	//	returns true if the device was able to be unacquired.
	virtual bool Unacquire(void)	{ return (m_lpDevice) ? SUCCEEDED( m_lpDevice->Unacquire() ) : false; }

}; 	//	end class CSGD_DirectInputDevice

////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//	CSGD_DIKeyboard
//////////////////////////////////////////////////////////////////////////////////

// Class to encapsulate the keyboard's functionality.
class CSGD_DIKeyboard : public CSGD_DirectInputDevice
{
private:
	unsigned char		m_ucKeysPressed[256];			//	Holds Keyboard Data.
	unsigned char		m_ucPrevKeysPressed[256];		//	Used for BufferedKeyEx.

	unsigned char		m_ucAsciiVals[256];		//	For CheckKeys and CheckBufferedKeys.
	HKL					m_keyLayout;			//	For CheckKeys and CheckBufferedKeys.

	//	Utility function to clear the state of the pressed keys. 
	void ClearKeys(void)	
	{ 
		//	clear the currently pressed keys
		memset(m_ucKeysPressed, 0, 256 * sizeof(unsigned char)); 
		//	clear the previously pressed keys
		memset(m_ucPrevKeysPressed, 0, 256 * sizeof(unsigned char));
	}

public:

	///////////////////////////////////////////////////////////////////
	//	Function:	"CSGD_DIKeyboard (Constructor)"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	pDI				-	Pointer to the DirectInput Object.				
	//				hWnd			-	Handle of the window to use the device for.
	//				bIsExclusive	-	Whether or not you want exclusive control of the device.
	//
	//	Return	:	N/A
	//
	//	Purpose	:	Setup the keyboard for use with DirectInput.
	///////////////////////////////////////////////////////////////////
	CSGD_DIKeyboard(LPDIRECTINPUT8 pDI, HWND hWnd, bool bIsExclusive=false);
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"ReadDevice"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	true if it can read the device state, false if it can't.
	//
	//	Purpose	:	Reads the state of the device.
	///////////////////////////////////////////////////////////////////
	bool ReadDevice(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ReadBufferedDevice"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	true if it can read the device data, false if it can't.
	//
	//	Purpose	:	Gets the DirectInput Buffered Data.
	//
	//	NOTE	:	For all Ex functions.
	///////////////////////////////////////////////////////////////////
	bool ReadBufferedDevice(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"Acquire"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	true if it could acquire the device
	//
	//	Purpose	:	To acquire the device when the application gains focus.
	//
	//	NOTE	:	Also clears the state of the keys.
	///////////////////////////////////////////////////////////////////
	bool Acquire(void)		{ ClearKeys(); return CSGD_DirectInputDevice::Acquire(); }
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"Unacquire"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	true if it could unacquire the device
	//
	//	Purpose	:	To unacquire the device when the application loses focus.
	//
	//	NOTE	:	Also clears the state of the keys.
	///////////////////////////////////////////////////////////////////
	bool Unacquire(void)	{ ClearKeys(); return CSGD_DirectInputDevice::Unacquire();  }

	///////////////////////////////////////////////////////////////////
	//	Function:	"KeyDown"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucDIKey	-	The DirectInput Keyboard Constant to check, such as DIK_ESCAPE.
	//
	//	Return	:	true if the Key is currently down.
	//
	//	Purpose	:	Gets the current (immediate) state of a Key.
	///////////////////////////////////////////////////////////////////
	bool KeyDown(unsigned char ucDIKey);
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"KeyPressed"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucDIKey	-	The DirectInput Keyboard Constant to check, such as DIK_ESCAPE.
	//
	//	Return	:	true if the Key was pressed this frame.
	//
	//	Purpose	:	To tell if a key was just pressed (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool KeyPressed(unsigned char ucDIKey);
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"KeyPressedEx"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucDIKey	-	The DirectInput Keyboard Constant to check, such as DIK_ESCAPE.
	//
	//	Return	:	true if Key was pressed this frame.
	//
	//	Purpose	:	To tell if a key was just pressed (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool KeyPressedEx(unsigned char ucDIKey); // was key just pressed?

	///////////////////////////////////////////////////////////////////
	//	Function:	"KeyUp"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucDIKey	-	The DirectInput Keyboard Constant to check, such as DIK_ESCAPE.
	//
	//	Return	:	true if the Key is currently up.
	//
	//	Purpose	:	Gets the current (immediate) state of a Key.
	///////////////////////////////////////////////////////////////////
	bool KeyUp(unsigned char ucDIKey);
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"KeyReleased"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucDIKey	-	The DirectInput Keyboard Constant to check, such as DIK_ESCAPE.
	//
	//	Return	:	true if the Key was released this frame.
	//
	//	Purpose	:	To tell if a key was just released (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool KeyReleased(unsigned char ucDIKey); 

	///////////////////////////////////////////////////////////////////
	//	Function:	"KeyReleasedEx"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucDIKey	-	The DirectInput Keyboard Constant to check, such as DIK_ESCAPE.
	//
	//	Return	:	true if the Key was released this frame.
	//
	//	Purpose	:	To tell if a key was just released (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool KeyReleasedEx(unsigned char ucDIKey);

	///////////////////////////////////////////////////////////////////
	//	Function:	"CheckKeys"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	0 if nothing was pressed, otherwise the ascii character 
	//				value of the key the user pressed.
	//
	//	Purpose	:	To get the ascii character value of the key the user typed.
	//				So they can enter letters.
	///////////////////////////////////////////////////////////////////
	char CheckKeys(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"CheckBufferedKeys"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	0 if nothing was pressed, otherwise the ascii character 
	//				value of the key the user pressed.
	//
	//	Purpose	:	To get the ascii character value of the key the user just typed.
	//				So they can enter letters (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	char CheckBufferedKeysEx(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetDIKCode"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	0 if nothing was pressed, otherwise the DIK code 
	//				of the key the user pressed.
	//
	//	Purpose	:	To get the DIK code of the key the user is pressing.
	//				For key binding.
	///////////////////////////////////////////////////////////////////
	unsigned char GetDIKCode(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetBufferedDIKCodeEx"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	0 if nothing was pressed, otherwise the DIK code 
	//				of the key the user pressed.
	//
	//	Purpose	:	To get the DIK code of the key the user just pressed.
	//				For key binding (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	unsigned char GetBufferedDIKCodeEx(void);

};	//	end class CSGD_DIKeyboard

//////////////////////////////////////////////////////////////////////////////////
//	CSGD_DIMouse
//////////////////////////////////////////////////////////////////////////////////

//	Class to encapsulate the mouse
class CSGD_DIMouse : public CSGD_DirectInputDevice
{
private:
	DIMOUSESTATE2	m_diMouseState;			//	Holds Mouse Data (button and movement info).
	DIMOUSESTATE2	m_diPrevMouseState;		//	Holds the previous state.
	
	int				m_nNumButtons;			//	Number of buttons supported.

	int				m_nPosX;				//	Helper positions for mouse cursor.
	int				m_nPosY;			

public:
	///////////////////////////////////////////////////////////////////
	//	Function:	"CSGD_DIMouse (Constructor)"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	pDI				-	Pointer to the DirectInput Object.				
	//				hWnd			-	Handle of the window to use the device for.
	//				bIsExclusive	-	Whether or not you want exclusive control of the device.
	//
	//	Return	:	N/A
	//
	//	Purpose	:	Setup the mouse for use with DirectInput.
	///////////////////////////////////////////////////////////////////
	CSGD_DIMouse(LPDIRECTINPUT8 pDI, HWND hWnd, bool bIsExclusive=false);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ReadDevice"
	//
	//	Last Modified:		6/23/2008
	//
	//	Input	:	void
	//
	//	Return	:	true if it can read the device state, false if it can't.
	//
	//	Purpose	:	Reads the state of the device every frame.
	///////////////////////////////////////////////////////////////////
	bool ReadDevice(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ReadBufferedDevice"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	void
	//
	//	Return	:	true if it can read the device state, false if it can't.
	//
	//	Purpose	:	Gets the DirectInput Buffered Data.
	//
	//	NOTE	:	For use with all Ex functions.
	///////////////////////////////////////////////////////////////////
	bool ReadBufferedDevice(void);
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"ButtonDown"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucButton	-	The mouse button to check for (i.e. 0, 1, or 2)
	//								0 = left, 1 = right, 2 = middle
	//
	//	Return	:	true if the mouse button is currently down.
	//
	//	Purpose	:	Gets the current (immediate) state of a mouse button.
	///////////////////////////////////////////////////////////////////
	bool ButtonDown(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ButtonPressed"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucButton	-	The mouse button to check for (i.e. 0, 1, or 2)
	//								0 = left, 1 = right, 2 = middle
	//
	//	Return	:	true if the mouse button was pressed this frame.
	//
	//	Purpose	:	To tell if a mouse button was just pressed (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool ButtonPressed(unsigned char ucButton);
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"ButtonPressedEx"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucButton	-	The mouse button to check for (i.e. 0, 1, or 2)
	//								0 = left, 1 = right, 2 = middle
	//
	//	Return	:	true if the mouse button was pressed this frame.
	//
	//	Purpose	:	To tell if a mouse button was just pressed (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool ButtonPressedEx(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ButtonUp"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucButton	-	The mouse button to check for (i.e. 0, 1, or 2)
	//								0 = left, 1 = right, 2 = middle
	//
	//	Return	:	true if the mouse button is currently up.
	//
	//	Purpose	:	Gets the current (immediate) state of a mouse button.
	///////////////////////////////////////////////////////////////////
	bool ButtonUp(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ButtonReleased"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucButton	-	The mouse button to check for (i.e. 0, 1, or 2)
	//								0 = left, 1 = right, 2 = middle
	//
	//	Return	:	true if the mouse button was released this frame.
	//
	//	Purpose	:	To tell if a mouse button was just released (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool ButtonReleased(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ButtonReleasedEx"
	//
	//	Last Modified:		5/25/2008
	//
	//	Input	:	ucButton	-	The mouse button to check for (i.e. 0, 1, or 2)
	//								0 = left, 1 = right, 2 = middle
	//
	//	Return	:	true if the mouse button was released this frame.
	//
	//	Purpose	:	To tell if a mouse button was just released (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool ButtonReleasedEx(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"CheckBufferedButtons"
	//
	//	Last Modified:		6/13/2008
	//
	//	Input	:	void
	//
	//	Return	:	The mouse button that was just pressed (i.e. 0, 1, or 2).
	//
	//	Purpose	:	To tell if a mouse button was just pressed (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	int CheckBufferedButtons(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"CheckBufferedButtonsEx"
	//
	//	Last Modified:		5/27/2008
	//
	//	Input	:	void
	//
	//	Return	:	The mouse button that was just pressed (i.e. 0, 1, or 2).
	//
	//	Purpose	:	To tell if a mouse button was just pressed (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	int	CheckBufferedButtonsEx(void);

	///////////////////////////////////////////////////////////////////
	//	Function		:	"MovementX"
	//
	//	Last Modified	:	5/25/2008
	//
	//	Input			:	void
	//
	//	Return			:	The amount the mouse has moved this frame.
	//
	//	Purpose			:	To report the relative motion of the mouse 
	//						in the X dimension.
	///////////////////////////////////////////////////////////////////
	long MovementX(void);

	///////////////////////////////////////////////////////////////////
	//	Function		:	"MovementY"
	//
	//	Last Modified	:	5/25/2008
	//
	//	Input			:	void
	//
	//	Return			:	The amount the mouse has moved this frame.
	//
	//	Purpose			:	To report the relative motion of the mouse 
	//						in the Y dimension.
	///////////////////////////////////////////////////////////////////
	long MovementY(void);

	///////////////////////////////////////////////////////////////////
	//	Function		:	"WheelMovement"
	//
	//	Last Modified	:	5/25/2008
	//
	//	Input			:	void
	//
	//	Return			:	The amount the mouse wheel has moved this frame.
	//						Positive(+) values mean pushing the wheel away from you.
	//						Negative(-) values mean pulling the wheel toward you.
	//
	//	Purpose			:	To report the relative motion of the mouse 
	//						wheel.
	///////////////////////////////////////////////////////////////////
	long WheelMovement(void);

	///////////////////////////////////////////////////////////////////
	//	Function		:	"GetNumButtons"
	//
	//	Last Modified	:	5/26/2008
	//
	//	Input			:	void
	//
	//	Return			:	The number of buttons supported on the device.
	//
	//	Purpose			:	To report the number of buttons on the device.
	///////////////////////////////////////////////////////////////////
	int GetNumButtons(void)		{ return m_nNumButtons; }

	///////////////////////////////////////////////////////////////////
	//	Function		:	"GetPosX"
	//
	//	Last Modified	:	6/23/2008
	//
	//	Input			:	void
	//
	//	Return			:	The "psuedo" position of the mouse on the screen.
	//
	//	Purpose			:	To help track the "psuedo" position of the mouse.
	///////////////////////////////////////////////////////////////////
	int GetPosX(void)			{ return m_nPosX; }

	///////////////////////////////////////////////////////////////////
	//	Function		:	"GetPosY"
	//
	//	Last Modified	:	6/23/2008
	//
	//	Input			:	void
	//
	//	Return			:	The "psuedo" position of the mouse on the screen.
	//
	//	Purpose			:	To help track the "psuedo" position of the mouse.
	///////////////////////////////////////////////////////////////////
	int GetPosY(void)			{ return m_nPosY; }

	///////////////////////////////////////////////////////////////////
	//	Function		:	"SetPosX"
	//
	//	Last Modified	:	6/23/2008
	//
	//	Input			:	nPosX
	//
	//	Return			:	void
	//
	//	Purpose			:	To set the "psuedo" position of the mouse.
	///////////////////////////////////////////////////////////////////
	void SetPosX(int nPosX)			{ m_nPosX = nPosX; }

	///////////////////////////////////////////////////////////////////
	//	Function		:	"SetPosY"
	//
	//	Last Modified	:	6/23/2008
	//
	//	Input			:	nPosY
	//
	//	Return			:	void
	//
	//	Purpose			:	To set the "psuedo" position of the mouse.
	///////////////////////////////////////////////////////////////////
	void SetPosY(int nPosY)			{ m_nPosY = nPosY; }

};	//	end class CSGD_DIMouse

//////////////////////////////////////////////////////////////////////////////////
//	CSGD_DIJoystick
//////////////////////////////////////////////////////////////////////////////////

//	Class to encapsulate the Joystick
class CSGD_DIJoystick : public CSGD_DirectInputDevice
{
private:
	char			m_szJoyName[MAX_PATH];		//	Holds the product name of the joystick.
	
	DIJOYSTATE2		m_diJoyState;				//	Holds Joystick Data (button and movement info).
	DIJOYSTATE2		m_diPrevJoyState;			//	Holds the previous state.

	int				m_nNumButtons;				//	Number of buttons supported.
	bool			m_bIsUnplugged;				//	Tracks if the joystick is currently unplugged.

	bool			m_bIsXbox360Pad;			//	Tracks if it is an Xbox360 pad.
	bool			m_bIsZAxisY;				//	Swaps whether Z or RotationZ is considered the Y-Axis.

	// POV constants
	enum POV_dirs
	{ 
		POV_NEUTRAL		=  -1,
		POV_UP			=   0,
		POV_UPRIGHT		=  45 * DI_DEGREES, 
		POV_RIGHT		=  90 * DI_DEGREES,
		POV_DOWNRIGHT	= 135 * DI_DEGREES,
		POV_DOWN		= 180 * DI_DEGREES,
		POV_DOWNLEFT	= 225 * DI_DEGREES,
		POV_LEFT		= 270 * DI_DEGREES,
		POV_UPLEFT		= 315 * DI_DEGREES
	};

	//	Utility functions:
	///////////////////////////////////////////////////////////////////
	//	Function:	"TranslatePOV"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir		-	The direction to check.
	//				dwPOVDir	-	The value of the D-pad.
	//
	//	Return	:	true if it can translate the direction as what is being pressed, false if it can't.
	//
	//	Purpose	:	Translates a checked for direction from a numerical mess.
	///////////////////////////////////////////////////////////////////
	bool TranslatePOV(int nDir, DWORD dwPOVDir);

	///////////////////////////////////////////////////////////////////
	//	Function:	"TranslateRStickX"
	//
	//	Last Modified:		6/06/2008
	//
	//	Input	:	diJoyState	-	The joystate to translate.
	//
	//	Return	:	-JOYSTICK_RANGE to +JOYSTICK_RANGE
	//
	//	Purpose	:	To get the position of the joystick in a digital range.
	///////////////////////////////////////////////////////////////////	
	LONG TranslateRStickX(DIJOYSTATE2& diJoyState);

	///////////////////////////////////////////////////////////////////
	//	Function:	"TranslateRStickY"
	//
	//	Last Modified:		6/07/2008
	//
	//	Input	:	diJoyState	-	The joystate to translate.
	//
	//	Return	:	-JOYSTICK_RANGE to +JOYSTICK_RANGE
	//
	//	Purpose	:	To get the position of the joystick in a digital range.
	///////////////////////////////////////////////////////////////////	
	LONG TranslateRStickY(DIJOYSTATE2& diJoyState);

public:

	///////////////////////////////////////////////////////////////////
	//	Function:	"CSGD_DIJoystick (Constructor)"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	pDI				-	Pointer to the DirectInput Object.				
	//				hWnd			-	Handle of the window to use the device for.
	//				bIsExclusive	-	Whether or not you want exclusive control of the device.
	//
	//	Return	:	N/A
	//
	//	Purpose	:	Setup the joystick for use with DirectInput.
	///////////////////////////////////////////////////////////////////
	CSGD_DIJoystick(LPDIRECTINPUT8 pDI, HWND hWnd, const DIDEVICEINSTANCE* lpdidi, bool bIsExclusive=false);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ReadDevice"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	void
	//
	//	Return	:	true if it could read the device state, false if it couldn't.
	//
	//	Purpose	:	Reads the state of the device.
	///////////////////////////////////////////////////////////////////
	bool ReadDevice(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ReadBufferedDevice"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	void
	//
	//	Return	:	true if it could read the device state, false if it couldn't.
	//
	//	Purpose	:	Gets the DirectInput Buffered Data.
	//
	//	NOTE	:	For use with all Ex functions.
	///////////////////////////////////////////////////////////////////
	bool ReadBufferedDevice(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"SwitchRStickAxes"
	//
	//	Last Modified:		6/06/2008
	//
	//	Input	:	void
	//
	//	Return	:	void
	//
	//	Purpose	:	Toggles whether the Z or RotationZ axis is considered the Y Axis.
	//
	//	NOTE	:	Defaults to RotationZ is the Y Axis.
	///////////////////////////////////////////////////////////////////
	void SwitchRStickAxes(void);

	///////////////////////////////////////////////////////////////////
	//	Buttons:
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	//	Function:	"ButtonDown"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	ucButton	-	The joystick button to check for.
	//
	//	Return	:	true if the joystick button is currently down.
	//
	//	Purpose	:	Gets the current (immediate) state of a joystick button.
	///////////////////////////////////////////////////////////////////
	bool ButtonDown(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ButtonPressed"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	ucButton	-	The joystick button to check for.
	//
	//	Return	:	true if the joystick button was pressed this frame.
	//
	//	Purpose	:	To tell if a joystick button was just pressed (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool ButtonPressed(unsigned char ucButton);
	
	///////////////////////////////////////////////////////////////////
	//	Function:	"ButtonPressedEx"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	ucButton	-	The joystick button to check for.
	//
	//	Return	:	true if the joystick button was pressed this frame.
	//
	//	Purpose	:	To tell if a joystick button was just pressed (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool ButtonPressedEx(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ButtonUp"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	ucButton	-	The joystick button to check for.
	//
	//	Return	:	true if the joystick button is currently up.
	//
	//	Purpose	:	Gets the current (immediate) state of a joystick button.
	///////////////////////////////////////////////////////////////////
	bool ButtonUp(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ButtonReleased"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	ucButton	-	The joystick button to check for.
	//
	//	Return	:	true if the joystick button was released this frame.
	//
	//	Purpose	:	To tell if a joystick button was just released (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool ButtonReleased(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	Function:	"ButtonReleasedEx"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	ucButton	-	The joystick button to check for.
	//
	//	Return	:	true if the joystick button was released this frame.
	//
	//	Purpose	:	To tell if a joystick button was just released (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool ButtonReleasedEx(unsigned char ucButton);

	///////////////////////////////////////////////////////////////////
	//	D-pad:
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	//	Function:	"DPadDown"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir	-	The direction to check (i.e. DIR_RIGHT).
	//
	//	Return	:	true if the D-pad direction is currently being held down.
	//
	//	Purpose	:	To tell if a D-pad direction is down.
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	bool DPadDown(int nDir); 

	///////////////////////////////////////////////////////////////////
	//	Function:	"DPadPressed"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir	-	The direction to check (i.e. DIR_RIGHT).
	//
	//	Return	:	true if the D-pad was pressed a certain direction.
	//
	//	Purpose	:	To get the (buffered) direction the D-pad was pressed (without using DirectInput's Buffered Input).
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	bool DPadPressed(int nDir);

	///////////////////////////////////////////////////////////////////
	//	Function:	"DPadUp"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir	-	The direction to check (i.e. DIR_RIGHT).
	//
	//	Return	:	true if the D-pad direction is currently up.
	//
	//	Purpose	:	To tell if a D-pad direction is up.
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	bool DPadUp(int nDir); 

	///////////////////////////////////////////////////////////////////
	//	Function:	"DPadReleased"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir	-	The direction to check (i.e. DIR_RIGHT).
	//
	//	Return	:	true if the D-pad direction was released this frame.
	//
	//	Purpose	:	To tell if a D-pad direction was just released (without using DirectInput's Buffered Input).
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	bool DPadReleased(int nDir);

	///////////////////////////////////////////////////////////////////
	//	Analog Sticks:
	///////////////////////////////////////////////////////////////////
	
	//	L-stick:
	///////////////////////////////////////////////////////////////////
	//	Function:	"GetLStickDirDown"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir	-	The direction to check (i.e. DIR_RIGHT).
	//
	//	Return	:	true if the left stick is currently held a certain direction.
	//
	//	Purpose	:	To get the (immediate) direction the stick is being held.
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	bool GetLStickDirDown(int nDir);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetLStickDirPressed"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir	-	The direction to check (i.e. DIR_RIGHT).
	//
	//	Return	:	true if the left stick was pressed a certain direction.
	//
	//	Purpose	:	To get the (buffered) direction the stick was pressed (without using DirectInput's Buffered Input).
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	bool GetLStickDirPressed(int nDir);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetLStickXAmount"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	void
	//
	//	Return	:	-JOYSTICK_RANGE to +JOYSTICK_RANGE
	//
	//	Purpose	:	To get the position of the joystick in a digital range.
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	int GetLStickXAmount(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetLStickYAmount"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	void
	//
	//	Return	:	-JOYSTICK_RANGE to +JOYSTICK_RANGE
	//
	//	Purpose	:	To get the position of the joystick in a digital range.
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	int GetLStickYAmount(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetLStickXNormalized"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	void
	//
	//	Return	:	-1.0 to +1.0 (-1.0 is Left, +1.0 is Right)
	//
	//	Purpose	:	To get the position of the joystick in a normalized range.
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	float GetLStickXNormalized();

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetLStickYNormalized"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	void
	//
	//	Return	:	-1.0 to +1.0 (-1.0 is Up, +1.0 is Down)
	//
	//	Purpose	:	To get the position of the joystick in a normalized range.
	//
	//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
	///////////////////////////////////////////////////////////////////
	float GetLStickYNormalized();

	// R-stick:
	///////////////////////////////////////////////////////////////////
	//	Function:	"GetRStickDirDown"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir	-	The direction to check (i.e. DIR_RIGHT).
	//
	//	Return	:	true if the right stick is currently held a certain direction.
	//
	//	Purpose	:	To get the (immediate) direction the stick is being held.
	///////////////////////////////////////////////////////////////////
	bool GetRStickDirDown(int nDir);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetRStickDirPressed"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	nDir	-	The direction to check (i.e. DIR_RIGHT).
	//
	//	Return	:	true if the right stick was pressed a certain direction.
	//
	//	Purpose	:	To get the (buffered) direction the stick was pressed (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	bool GetRStickDirPressed(int nDir);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetRStickXAmount"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	void
	//
	//	Return	:	-JOYSTICK_RANGE to +JOYSTICK_RANGE ((-) Left and (+) Right)
	//
	//	Purpose	:	To get the position of the joystick in a digital range.
	///////////////////////////////////////////////////////////////////
	int GetRStickXAmount(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetRStickYAmount"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	void
	//
	//	Return	:	-JOYSTICK_RANGE to +JOYSTICK_RANGE (-JOYSTICK_RANGE is Up, +JOYSTICK_RANGE is Down)
	//
	//	Purpose	:	To get the position of the joystick in a digital range.
	///////////////////////////////////////////////////////////////////	
	int GetRStickYAmount(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetRStickXNormalized"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	void
	//
	//	Return	:	-1.0 to +1.0 (-1.0 is Left, +1.0 is Right)
	//
	//	Purpose	:	To get the position of the joystick in a normalized range.
	///////////////////////////////////////////////////////////////////
	float GetRStickXNormalized(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetRStickYNormalized"
	//
	//	Last Modified:		6/01/2008
	//
	//	Input	:	void
	//
	//	Return	:	-1.0 to +1.0 (-1.0 is Up, +1.0 is Down)
	//
	//	Purpose	:	To get the position of the joystick in a normalized range.
	///////////////////////////////////////////////////////////////////
	float GetRStickYNormalized(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetLTriggerAmount"
	//
	//	Last Modified:		6/07/2008
	//
	//	Input	:	void
	//
	//	Return	:	0 to JOYSTICK_AXIS_RANGE
	//
	//	Purpose	:	To get the position of the trigger in a digital range.
	//
	//	NOTE: If both triggers are pulled at the same time the reading won't 
	//		  be accurate as their input is combined into one axis by DirectInput.
	///////////////////////////////////////////////////////////////////	
	int GetLTriggerAmount(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetRTriggerAmount"
	//
	//	Last Modified:		6/07/2008
	//
	//	Input	:	void
	//
	//	Return	:	0 to JOYSTICK_AXIS_RANGE
	//
	//	Purpose	:	To get the position of the trigger in a digital range.
	//
	//	NOTE: If both triggers are pulled at the same time the reading won't 
	//		  be accurate as their input is combined into one axis by DirectInput.
	///////////////////////////////////////////////////////////////////	
	int GetRTriggerAmount(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetLTriggerNormalized"
	//
	//	Last Modified:		6/07/2008
	//
	//	Input	:	void
	//
	//	Return	:	0.0 to 1.0 to (0.0 is not pressed, +1.0 is pulled all the way down)
	//
	//	Purpose	:	To get the position of the trigger in a normalized range.
	//
	//	NOTE: If both triggers are pulled at the same time the reading won't 
	//		  be accurate as their input is combined into one axis by DirectInput.
	///////////////////////////////////////////////////////////////////
	float GetLTriggerNormalized(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetRTriggerNormalized"
	//
	//	Last Modified:		6/07/2008
	//
	//	Input	:	void
	//
	//	Return	:	0.0 to 1.0 to (0.0 is not pressed, +1.0 is pulled all the way down)
	//
	//	Purpose	:	To get the position of the trigger in a normalized range.
	//
	//	NOTE: If both triggers are pulled at the same time the reading won't 
	//		  be accurate as their input is combined into one axis by DirectInput.
	///////////////////////////////////////////////////////////////////
	float GetRTriggerNormalized(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"CheckBufferedButtons"
	//
	//	Last Modified:		5/27/2008
	//
	//	Input	:	void
	//
	//	Return	:	The joystick button that was just pressed.  -1 if no button was pressed.
	//
	//	Purpose	:	To tell which joystick button was just pressed (without using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	int	CheckBufferedButtons(void);

	///////////////////////////////////////////////////////////////////
	//	Function:	"CheckBufferedButtonsEx"
	//
	//	Last Modified:		6/07/2008
	//
	//	Input	:	void
	//
	//	Return	:	The joystick button that was just pressed.  -1 if no button was pressed.
	//
	//	Purpose	:	To tell which joystick button was just pressed (using DirectInput's Buffered Input).
	///////////////////////////////////////////////////////////////////
	int	CheckBufferedButtonsEx(void);

	///////////////////////////////////////////////////////////////////
	//	Function		:	"GetNumButtons"
	//
	//	Last Modified	:	5/26/2008
	//
	//	Input			:	void
	//
	//	Return			:	The number of buttons supported on the device.
	//
	//	Purpose			:	To report the number of buttons on the device.
	///////////////////////////////////////////////////////////////////
	int GetNumButtons(void)	const			{ return m_nNumButtons; }

	///////////////////////////////////////////////////////////////////
	//	Function:	"IsUnplugged"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	void
	//
	//	Return	:	The name of the joystick.
	//
	//	Purpose	:	Returns the true if the joystick is unplugged.
	///////////////////////////////////////////////////////////////////
	bool IsUnplugged(void) const		{ return m_bIsUnplugged; }

	///////////////////////////////////////////////////////////////////
	//	Function:	"GetName"
	//
	//	Last Modified:		5/26/2008
	//
	//	Input	:	void
	//
	//	Return	:	The name of the joystick.
	//
	//	Purpose	:	Returns the name of the joystick (i.e. "Logitech RumblePad 2 USB")
	///////////////////////////////////////////////////////////////////
	const char* GetName(void) const		{ return (const char*)m_szJoyName; }

	///////////////////////////////////////////////////////////////////
	//	Function:	"SwapRStickAxes"
	//
	//	Last Modified:		6/13/2008
	//
	//	Input	:	bSwapAxes	-	Whether or not to swap the X/Y axes of the Rstick.
	//
	//	Return	:	void
	//
	//	Purpose	:	To swap the X/Y axes of certain controllers.
	//
	//	NOTE:	Z axis is considered R-stick's X-axis by default.
	//			Only works on non-Xbox360 controllers.
	///////////////////////////////////////////////////////////////////
	void SwapRStickAxes(bool bSwapAxes)	{ m_bIsZAxisY = bSwapAxes; }

};	//	end class CSGD_DIJoystick
