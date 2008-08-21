////////////////////////////////////////////////////////////////
//	File			:	"CSGD_DirectInput.cpp"
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

#include "CSGD_DirectInput.h"

//	MessageBox for Errors that occur within DirectInput.
#ifndef DIERRBOX
	#define DIERRBOX(hWnd, errorText)	{ MessageBox(hWnd, errorText, "CSGD_DirectInput Error", MB_OK | MB_ICONEXCLAMATION); }
#endif

CSGD_DirectInput CSGD_DirectInput::m_Instance;

CSGD_DirectInput::CSGD_DirectInput(void)
{
	m_lpDIObject	= NULL;
	m_pKeyboard		= NULL;
	m_pMouse		= NULL;

}

CSGD_DirectInput::~CSGD_DirectInput(void)
{
	ShutdownDirectInput();
}

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
CSGD_DirectInput* CSGD_DirectInput::GetInstance(void)
{
	return &m_Instance;
}

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
bool CSGD_DirectInput::InitDirectInput(HWND hWnd, HINSTANCE hInstance, unsigned int unInitDevices, unsigned int unExclusiveDevices)
{
	//	Create the DirectInput Object
	if (FAILED( DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_lpDIObject, NULL) ))
		return false;

	//	Initialize the keyboard if the keyboard flag is on
	if (unInitDevices & DI_KEYBOARD)
	{
		//	Initialize the keyboard
		if (!InitKeyboard(hWnd, (unExclusiveDevices & DI_KEYBOARD) ? true : false ))
			return false;
	}

	//	Initialize the mouse if the mouse flag is on
	if (unInitDevices & DI_MOUSE)
	{
		//	Initialize the mouse
		if (!InitMouse(hWnd, (unExclusiveDevices & DI_MOUSE) ? true : false ))
			return false;
	}

	//	Initialize joysticks if the joystick flag is on
	if (unInitDevices & DI_JOYSTICKS)
	{
		//	Initialize joysticks
		if (!InitJoysticks(hWnd, (unExclusiveDevices & DI_JOYSTICKS) ? true : false ))
			return false;
	}

	//	Return success.
	return true;
}

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
void CSGD_DirectInput::ShutdownDirectInput(void)
{
	UnacquireAll();

	if (m_pKeyboard)
	{
		SAFE_DELETE(m_pKeyboard);
	}

	if (m_pMouse)
	{
		SAFE_DELETE(m_pMouse);
	}

	if (m_vpJoysticks.size() > 0)
	{
		for (unsigned int i=0; i < m_vpJoysticks.size(); i++)
		{
			SAFE_DELETE(m_vpJoysticks[i]);
		}

		m_vpJoysticks.clear();
	}
}

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
void CSGD_DirectInput::ReadDevices(void)
{
	if (m_pKeyboard)
	{
		m_pKeyboard->ReadDevice();
		m_pKeyboard->ReadBufferedDevice();
	}

	if (m_pMouse)
	{
		m_pMouse->ReadDevice();
		m_pMouse->ReadBufferedDevice();
	}
	
	if (m_vpJoysticks.size() > 0)
	{
		for (unsigned int i=0; i < m_vpJoysticks.size(); i++)
		{
			if (m_vpJoysticks[i])
			{
				m_vpJoysticks[i]->ReadDevice();
				m_vpJoysticks[i]->ReadBufferedDevice();
			}
		}
	}
}

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
void CSGD_DirectInput::AcquireAll(void)
{
	if (m_pKeyboard)
		m_pKeyboard->Acquire();

	if (m_pMouse)
		m_pMouse->Acquire();
	
	if (m_vpJoysticks.size() > 0)
	{
		for (unsigned int i=0; i < m_vpJoysticks.size(); i++)
		{
			if (m_vpJoysticks[i]) 
				m_vpJoysticks[i]->Acquire();
		}
	}
}

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
void CSGD_DirectInput::UnacquireAll(void)
{
	if (m_pKeyboard)
		m_pKeyboard->Unacquire();

	if (m_pMouse)
		m_pMouse->Unacquire();

	if (m_vpJoysticks.size() > 0)
	{
		for (unsigned int i=0; i < m_vpJoysticks.size(); i++)
		{
			if (m_vpJoysticks[i]) 
				m_vpJoysticks[i]->Unacquire();
		}
	}
}

/***********************	Device specific functions:	********************/

///////////////////////////////////////////////////////////////////
//						KEYBOARD								 //
///////////////////////////////////////////////////////////////////

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
bool CSGD_DirectInput::InitKeyboard(HWND hWnd, bool bIsExclusive)
{
	//	Make sure we aren't trying to do this a second time.
	if (m_pKeyboard)
	{
		DIERRBOX(hWnd, "Keyboard has already been initialized")
		return false;
	}

	m_pKeyboard = new CSGD_DIKeyboard(m_lpDIObject, hWnd, bIsExclusive);
	
	if (m_pKeyboard == NULL)
		return false;

	return true;
}

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
bool CSGD_DirectInput::KeyDown(unsigned char ucDIKey)			{ return (m_pKeyboard && m_pKeyboard->KeyDown(ucDIKey)); }

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
bool CSGD_DirectInput::KeyPressed(unsigned char ucDIKey)		{ return (m_pKeyboard && m_pKeyboard->KeyPressed(ucDIKey)); }

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
bool CSGD_DirectInput::KeyPressedEx(unsigned char ucDIKey)		{ return (m_pKeyboard && m_pKeyboard->KeyPressedEx(ucDIKey)); }

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
bool CSGD_DirectInput::KeyUp(unsigned char ucDIKey)			{ return (m_pKeyboard && m_pKeyboard->KeyUp(ucDIKey)); }

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
bool CSGD_DirectInput::KeyReleased(unsigned char ucDIKey)		{ return (m_pKeyboard && m_pKeyboard->KeyReleased(ucDIKey)); }

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
bool CSGD_DirectInput::KeyReleasedEx(unsigned char ucDIKey)	{ return (m_pKeyboard && m_pKeyboard->KeyReleasedEx(ucDIKey)); }

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
char CSGD_DirectInput::CheckKeys(void)					{ return (m_pKeyboard) ? m_pKeyboard->CheckKeys() : 0; }

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
char CSGD_DirectInput::CheckBufferedKeysEx(void)			{ return (m_pKeyboard) ? m_pKeyboard->CheckBufferedKeysEx() : 0; }

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
unsigned char CSGD_DirectInput::GetDIKCode(void)			{ return (m_pKeyboard) ? m_pKeyboard->GetDIKCode() : 0; }

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
unsigned char CSGD_DirectInput::GetBufferedDIKCodeEx(void)	{ return (m_pKeyboard) ? m_pKeyboard->GetBufferedDIKCodeEx() : 0; }

///////////////////////////////////////////////////////////////////
//						MOUSE								     //
///////////////////////////////////////////////////////////////////

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
bool CSGD_DirectInput::InitMouse(HWND hWnd, bool bIsExclusive)
{
	//	Make sure we aren't trying to do this a second time.
	if (m_pMouse)
	{
		DIERRBOX(hWnd, "Mouse has already been initialized")
		return false;
	}

	m_pMouse = new CSGD_DIMouse(m_lpDIObject, hWnd, bIsExclusive);
	
	if (m_pMouse == NULL)
		return false;

	return true;
}

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
bool CSGD_DirectInput::MouseButtonDown(unsigned char ucButton)			{ return (m_pMouse && m_pMouse->ButtonDown(ucButton)); }

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
bool CSGD_DirectInput::MouseButtonPressed(unsigned char ucButton)		{ return (m_pMouse && m_pMouse->ButtonPressed(ucButton)); }

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
bool CSGD_DirectInput::MouseButtonPressedEx(unsigned char ucButton)	{ return (m_pMouse && m_pMouse->ButtonPressedEx(ucButton)); }

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
bool CSGD_DirectInput::MouseButtonUp(unsigned char ucButton)			{ return (m_pMouse && m_pMouse->ButtonUp(ucButton)); }

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
bool CSGD_DirectInput::MouseButtonReleased(unsigned char ucButton)		{ return (m_pMouse && m_pMouse->ButtonReleased(ucButton)); }

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
bool CSGD_DirectInput::MouseButtonReleasedEx(unsigned char ucButton)	{ return (m_pMouse && m_pMouse->ButtonReleasedEx(ucButton)); }

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
int CSGD_DirectInput::MouseCheckBufferedButtons(void)			{ return (m_pMouse) ? m_pMouse->CheckBufferedButtons() : -1; }

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
int CSGD_DirectInput::MouseCheckBufferedButtonsEx(void)			{ return (m_pMouse) ? m_pMouse->CheckBufferedButtonsEx() : -1; }

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
long CSGD_DirectInput::MouseMovementX(void)						{ return (m_pMouse) ? m_pMouse->MovementX() : 0; }

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
long CSGD_DirectInput::MouseMovementY(void)						{ return (m_pMouse) ? m_pMouse->MovementY() : 0; }

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
long CSGD_DirectInput::MouseWheelMovement(void)					{ return (m_pMouse) ? m_pMouse->WheelMovement() : 0; }

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
int CSGD_DirectInput::MouseGetNumButtons(void)			{ return (m_pMouse) ? m_pMouse->GetNumButtons() : 0; }

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
int CSGD_DirectInput::MouseGetPosX(void)			{ return (m_pMouse) ? m_pMouse->GetPosX() : 0; }

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
int CSGD_DirectInput::MouseGetPosY(void)			{ return (m_pMouse) ? m_pMouse->GetPosY() : 0; }

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
void CSGD_DirectInput::MouseSetPosX(int nPosX)			{ if (m_pMouse) m_pMouse->SetPosX( nPosX ); }

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
void CSGD_DirectInput::MouseSetPosY(int nPosY)			{ if (m_pMouse) m_pMouse->SetPosY( nPosY ); }

///////////////////////////////////////////////////////////////////
//						JOYSTICKS								 //
///////////////////////////////////////////////////////////////////

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
bool CSGD_DirectInput::InitJoysticks(HWND hWnd, bool bIsExclusive)
{
	//	Make sure we aren't trying to do this a second time.
	if (m_vpJoysticks.size() > 0)
	{
		DIERRBOX(hWnd, "Joysticks have already been initialized")
		return false;
	}

	//	Remember the info in a struct to pass along
	tJoystickSetupInfo joySetupInfo = { hWnd, bIsExclusive };

	//	Enumerate all joysticks attached to the system.
	if(FAILED( m_lpDIObject->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, (LPVOID)&joySetupInfo, DIEDFL_ATTACHEDONLY) ))
		return false;

	//	If we don't have any joysticks, we couldn't initialize them
	if (m_vpJoysticks.size() == 0)
		return false;

	return true;
}

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
BOOL CALLBACK CSGD_DirectInput::EnumJoysticksCallback(const DIDEVICEINSTANCE* lpdidi, VOID* pVoid)
{
	CSGD_DirectInput*	pDI = CSGD_DirectInput::GetInstance();

	//	Retrieve the setup info
	tJoystickSetupInfo joySetupInfo = (*(tJoystickSetupInfo*)pVoid);

	//	Create the joystick
	CSGD_DIJoystick* pJoystick = new CSGD_DIJoystick(pDI->m_lpDIObject, joySetupInfo.hWnd, lpdidi, joySetupInfo.bIsExclusive);

	//	Failed to allocate the joystick
	if (pJoystick == NULL)
		return DIENUM_STOP; 

	//	Store the joystick.
	pDI->m_vpJoysticks.push_back(pJoystick);

	//	Continue looking for Joysticks.
	return DIENUM_CONTINUE;
}

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
bool CSGD_DirectInput::JoystickButtonDown(unsigned char ucButton, int nJoyNum)			{ return ((nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) && m_vpJoysticks[nJoyNum]->ButtonDown(ucButton)); }

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
bool CSGD_DirectInput::JoystickButtonPressed(unsigned char ucButton, int nJoyNum)		{ return ((nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) && m_vpJoysticks[nJoyNum]->ButtonPressed(ucButton)); }

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
bool CSGD_DirectInput::JoystickButtonPressedEx(unsigned char ucButton, int nJoyNum)	{ return ((nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) && m_vpJoysticks[nJoyNum]->ButtonPressedEx(ucButton)); }

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
bool CSGD_DirectInput::JoystickButtonUp(unsigned char ucButton, int nJoyNum)			{ return ((nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) && m_vpJoysticks[nJoyNum]->ButtonUp(ucButton)); }

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
bool CSGD_DirectInput::JoystickButtonReleased(unsigned char ucButton, int nJoyNum)		{ return ((nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) && m_vpJoysticks[nJoyNum]->ButtonReleased(ucButton)); }

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
bool CSGD_DirectInput::JoystickButtonReleasedEx(unsigned char ucButton, int nJoyNum)	{ return ((nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) && m_vpJoysticks[nJoyNum]->ButtonReleasedEx(ucButton)); }

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
bool CSGD_DirectInput::JoystickDPadDown(int nDir, int nJoyNum)			{ return ((nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) && m_vpJoysticks[nJoyNum]->DPadDown(nDir)); }

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
bool CSGD_DirectInput::JoystickDPadPressed(int nDir, int nJoyNum)		{ return ((nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) && m_vpJoysticks[nJoyNum]->DPadPressed(nDir)); }

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
bool CSGD_DirectInput::JoystickDPadUp(int nDir, int nJoyNum)			{ return ((nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) && m_vpJoysticks[nJoyNum]->DPadUp(nDir)); }

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
bool CSGD_DirectInput::JoystickDPadReleased(int nDir, int nJoyNum)		{ return ((nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) && m_vpJoysticks[nJoyNum]->DPadReleased(nDir)); }

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
bool CSGD_DirectInput::JoystickGetLStickDirDown(int nDir, int nJoyNum)			{ return ((nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) && m_vpJoysticks[nJoyNum]->GetLStickDirDown(nDir)); }

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
bool CSGD_DirectInput::JoystickGetLStickDirPressed(int nDir, int nJoyNum)		{ return ((nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) && m_vpJoysticks[nJoyNum]->GetLStickDirPressed(nDir)); }

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
int CSGD_DirectInput::JoystickGetLStickXAmount(int nJoyNum)	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->GetLStickXAmount() : 0; }

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
int CSGD_DirectInput::JoystickGetLStickYAmount(int nJoyNum)	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->GetLStickYAmount() : 0; }

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
float CSGD_DirectInput::JoystickGetLStickXNormalized(int nJoyNum)	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->GetLStickXNormalized() : 0.0f; }

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
float CSGD_DirectInput::JoystickGetLStickYNormalized(int nJoyNum)	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->GetLStickYNormalized() : 0.0f; }

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
bool CSGD_DirectInput::JoystickGetRStickDirDown(int nDir, int nJoyNum)			{ return ((nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) && m_vpJoysticks[nJoyNum]->GetRStickDirDown(nDir)); }

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
bool CSGD_DirectInput::JoystickGetRStickDirPressed(int nDir, int nJoyNum)		{ return ((nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) && m_vpJoysticks[nJoyNum]->GetRStickDirPressed(nDir)); }

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
int CSGD_DirectInput::JoystickGetRStickXAmount(int nJoyNum)	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->GetRStickXAmount() : 0; }

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
int CSGD_DirectInput::JoystickGetRStickYAmount(int nJoyNum)	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->GetRStickYAmount() : 0; }

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
float CSGD_DirectInput::JoystickGetRStickXNormalized(int nJoyNum)	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->GetRStickXNormalized() : 0.0f; }

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
float CSGD_DirectInput::JoystickGetRStickYNormalized(int nJoyNum)	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->GetRStickYNormalized() : 0.0f; }

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
int CSGD_DirectInput::JoystickGetLTriggerAmount(int nJoyNum)	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->GetLTriggerAmount() : 0; }

///////////////////////////////////////////////////////////////////
//	Function:	"JoystickGetRTriggerAmount"
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
int CSGD_DirectInput::JoystickGetRTriggerAmount(int nJoyNum)	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->GetRTriggerAmount() : 0; }

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
float CSGD_DirectInput::JoystickGetLTriggerNormalized(int nJoyNum)	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->GetLTriggerNormalized() : 0.0f; }

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
float CSGD_DirectInput::JoystickGetRTriggerNormalized(int nJoyNum)	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->GetRTriggerNormalized() : 0.0f; }

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
int CSGD_DirectInput::JoystickCheckBufferedButtons(int nJoyNum)	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->CheckBufferedButtons() : -1; }

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
int CSGD_DirectInput::JoystickCheckBufferedButtonsEx(int nJoyNum)	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->CheckBufferedButtonsEx() : -1; }

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
int CSGD_DirectInput::JoystickGetNumButtons(int nJoyNum) const	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->GetNumButtons() : 0; }

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
bool CSGD_DirectInput::JoystickIsUnplugged(int nJoyNum) const	{ return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->IsUnplugged() : true; }

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
const char* CSGD_DirectInput::JoystickGetName(int nJoyNum)	const { return (nJoyNum >= 0 && nJoyNum < (int)m_vpJoysticks.size()) ? m_vpJoysticks[nJoyNum]->GetName() : "Doesn't exist"; }

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
//	Purpose	:	To swap the X/Y axes of certain controllers
//
//	NOTE:	Only works on non-Xbox360 controllers.
///////////////////////////////////////////////////////////////////
void CSGD_DirectInput::JoystickSwapRStickAxes(bool bSwapAxes, int nJoyNum) { if (nJoyNum < 0 || nJoyNum >= (int)m_vpJoysticks.size()) return; m_vpJoysticks[nJoyNum]->SwapRStickAxes(bSwapAxes); }

//////////////////////////////////////////////////////////////////////////////////
//	CSGD_DIKeyboard
//////////////////////////////////////////////////////////////////////////////////

CSGD_DIKeyboard::CSGD_DIKeyboard(LPDIRECTINPUT8 pDI, HWND hWnd, bool bIsExclusive)
{
	//	Create the Keyboard Device.
	if (FAILED( pDI->CreateDevice(GUID_SysKeyboard, &m_lpDevice, NULL) ))
	{
		DIERRBOX(hWnd, "Failed to create Keyboard device.")
	}

	//	Set the Data Format for the Keyboard.
	if (FAILED( m_lpDevice->SetDataFormat(&c_dfDIKeyboard) ))
	{
		DIERRBOX(hWnd, "Failed to set data format on Keyboard.")
	}

	//	Set the Cooperative level for the keyboard.
	DWORD dwFlags = DISCL_FOREGROUND;

	if (bIsExclusive)
		dwFlags |= DISCL_EXCLUSIVE;
	else
		dwFlags |= DISCL_NONEXCLUSIVE | DISCL_NOWINKEY;

	if (FAILED( m_lpDevice->SetCooperativeLevel(hWnd, dwFlags) ))
	{
		DIERRBOX(hWnd, "Failed to set cooperative level on Keyboard.")
	}

	//	Set up the device to use buffered input
	//	For Use With Buffered Input.
	DIPROPDWORD dipdw;

	//	Clear the structure to all zeroes.
	memset(&dipdw, 0, sizeof(dipdw));

	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = g_knBUFFER_SIZE;	//	Arbitrary buffer size

	//	Set the buffer size on the device.
	if (FAILED( m_lpDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph) ))
		DIERRBOX(hWnd, "Could not Set the Properties for Buffered Input for Keyboard.");

	//	Acquire the Keyboard.
	if (FAILED( m_lpDevice->Acquire() ))
	{
		DIERRBOX(hWnd, "Failed to acquire Keyboard.")
	}	

	ClearKeys();

	//	Get the Keyboard Layout.
	m_keyLayout = GetKeyboardLayout(0);
}

///////////////////////////////////////////////////////////////////
//	Function:	"ReadDevice"
//
//	Last Modified:		5/25/2008
//
//	Input	:	void
//
//	Return	:	true if it can read the device state, false if it can't.
//
//	Purpose	:	Reads the state of the device every frame.
///////////////////////////////////////////////////////////////////
bool CSGD_DIKeyboard::ReadDevice(void)
{
	//	Make sure the keyboard device was made.
	if (!m_lpDevice)
		return false;

	//	Remember previously pressed keys
	memcpy(m_ucPrevKeysPressed, m_ucKeysPressed, sizeof(m_ucPrevKeysPressed));

	//	Attempt to read the keyboard...
	if (FAILED( m_lpDevice->GetDeviceState(sizeof(m_ucKeysPressed), (LPVOID)m_ucKeysPressed) ))
	{
		//	If we couldn't, try to re-acquire the device.
		if (FAILED( m_lpDevice->Acquire() ))
			return false; //	Could not re-acquire the Keyboard.

		//	Now try reading it
		if (FAILED( m_lpDevice->GetDeviceState(sizeof(m_ucKeysPressed), (LPVOID)m_ucKeysPressed) ))
			return false;
	}

	//	Success.
	return true;
}

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
bool CSGD_DIKeyboard::ReadBufferedDevice(void)
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

	//	Success.
	return true;
}

/////////////////////////////////////////////

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
bool CSGD_DIKeyboard::KeyDown(unsigned char ucDIKey)
{
	//	if the high bit is on, it is pressed.
	return (m_ucKeysPressed[ucDIKey] & 0x80) ? true : false;
}

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
bool CSGD_DIKeyboard::KeyPressed(unsigned char ucDIKey)
{
	//	if the key is down and wasn't previously pressed.
	return KeyDown(ucDIKey) && !(m_ucPrevKeysPressed[ucDIKey] & 0x80);
}

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
bool CSGD_DIKeyboard::KeyPressedEx(unsigned char ucDIKey)
{
	//	Go through each element in the Buffer.
	for (DWORD i = 0; i < m_dwBufferElements; i++)
	{
		//	If the offset matches the key we are looking for and the high bit is on, it is pressed.
		if ((m_didod[i].dwOfs == ucDIKey) && (m_didod[i].dwData & 0x80))
			return true;
	}

	//	wasn't pressed.
	return false;
}

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
bool CSGD_DIKeyboard::KeyUp(unsigned char ucDIKey)
{
	//	if the high bit is off, it isn't pressed.
	return !KeyDown(ucDIKey);
}

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
bool CSGD_DIKeyboard::KeyReleased(unsigned char ucDIKey) // was key just released?
{
	//	if the key is up and was previously pressed.
	return KeyUp(ucDIKey) && (m_ucPrevKeysPressed[ucDIKey] & 0x80);
}

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
bool CSGD_DIKeyboard::KeyReleasedEx(unsigned char ucDIKey) // uses DirectX Buffered Input
{
	//	Go through each element in the Buffer.
	for (DWORD i = 0; i < m_dwBufferElements; i++)
	{
		//	If the offset matches the key we are looking for and the high bit is off, it was released.
		if ((m_didod[i].dwOfs == ucDIKey) && !(m_didod[i].dwData & 0x80))
			return true;
	}

	//	wasn't pressed.
	return false;
}

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
char CSGD_DIKeyboard::CheckKeys(void)
{	
	//	Get the Current Keyboard State.
	if ( !GetKeyboardState(m_ucAsciiVals) )
		return 0; // Could not GetKeyboardState.

	unsigned short usNum	= 0;
	unsigned int uiVKCode	= 0;

	//	Go through each keyboard key
	for (int i = 0; i < 256; i++)
	{
		// if the high bit is on, it is pressed
		if (KeyDown( (unsigned char)i ))
		{
			//	Map the Scan Code from DirectInput to a Virtual Key value...
			uiVKCode = MapVirtualKeyEx(i, 1, m_keyLayout);
			//	...and translate that Virtual Key into an Ascii Value.
			ToAsciiEx(uiVKCode, i, m_ucAsciiVals, &usNum, 0, m_keyLayout);
		}
	}

	//	Return the Ascii Value.
	return (char)usNum;
}

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
char CSGD_DIKeyboard::CheckBufferedKeysEx(void)
{
	//	Get the Current Keyboard State.
	if (!(GetKeyboardState(m_ucAsciiVals)))
		return 0; // Could not GetKeyboardState.

	unsigned short usNum	= 0;
	unsigned int uiVKCode	= 0;

	//	Go through each element in the Buffer.
	for (DWORD i = 0; i < m_dwBufferElements; i++)
	{
		//	Go through all keys on the keyboard
		for (unsigned int j = 0; j < 256; j++)
		{
			//	If the offset matches the key we are looking for and the high bit is on, it is pressed.
			if ((m_didod[i].dwOfs == j) && (m_didod[i].dwData & 0x80))
			{
				//	Map the Scan Code from DirectInput to a Virtual Key value...
				uiVKCode = MapVirtualKeyEx(m_didod[i].dwOfs, 1, m_keyLayout);
				//	...and translate that Virtual Key into an Ascii Value.
				ToAsciiEx(uiVKCode, m_didod[i].dwOfs, m_ucAsciiVals, &usNum, 0, m_keyLayout);
			}
		}
	}

	//	Return the Ascii Value.
	return (char)usNum;	
}

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
unsigned char CSGD_DIKeyboard::GetDIKCode(void)
{	
	unsigned char ucNum = 0;

	//	Go through each keyboard key
	for (int i = 0; i < 256; i++)
	{
		// find the first one that is pressed
		if (KeyDown( (unsigned char)i ))
		{
			ucNum = (unsigned char)i;
			break;
		}
	}

	//	Return the DIK Code.
	return ucNum;
}

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
unsigned char CSGD_DIKeyboard::GetBufferedDIKCodeEx(void)
{
	unsigned char ucNum = 0;

	//	Go through each element in the Buffer.
	for (DWORD i = 0; i < m_dwBufferElements; i++)
	{
		//	Go through all keys on the keyboard
		for (unsigned int j = 0; j < 256; j++)
		{
			//	If the offset matches the key we are looking for and the high bit is on, it is pressed.
			if ((m_didod[i].dwOfs == j) && (m_didod[i].dwData & 0x80))
			{
				ucNum = (unsigned char)j;
				break;
			}
		}
	}

	//	Return the DIK Code.
	return ucNum;	
}

///////////////////////////////////////////////////////////////////////////////////////////
//	CSGD_DIMouse
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//	Function:	"CSGD_DIMouse (Constructor)"
//
//	Last Modified:		6/23/2008
//
//	Input	:	pDI				-	Pointer to the DirectInput Object.				
//				hWnd			-	Handle of the window to use the device for.
//				bIsExclusive	-	Whether or not you want exclusive control of the device.
//
//	Return	:	N/A
//
//	Purpose	:	Setup the mouse for use with DirectInput.
///////////////////////////////////////////////////////////////////
CSGD_DIMouse::CSGD_DIMouse(LPDIRECTINPUT8 pDI, HWND hWnd, bool bIsExclusive)
{
	//	Create the Mouse Device.
	if (FAILED( pDI->CreateDevice(GUID_SysMouse, &m_lpDevice, NULL) ))
	{
		DIERRBOX(hWnd, "Failed to create Mouse device.")
	}

	//	Set the Data Format for the Mouse.
	if (FAILED( m_lpDevice->SetDataFormat(&c_dfDIMouse2) ))
	{
		DIERRBOX(hWnd, "Failed to set data format on Mouse.")
	}

	//	Set the Cooperative level for the mouse.
	DWORD dwFlags = DISCL_FOREGROUND;

	if (bIsExclusive)
		dwFlags |= DISCL_EXCLUSIVE;
	else
		dwFlags |= DISCL_NONEXCLUSIVE;

	if (FAILED( m_lpDevice->SetCooperativeLevel(hWnd, dwFlags) ))
	{
		DIERRBOX(hWnd, "Failed to set cooperative level on Mouse.")
	}

	//	Set up the device to use buffered input
	//	For Use With Buffered Input.
	DIPROPDWORD dipdw;

	//	Clear the structure to all zeroes.
	memset(&dipdw, 0, sizeof(dipdw));

	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = g_knBUFFER_SIZE;	//	Arbitrary buffer size

	//	Set the buffer size on the device.
	if (FAILED( m_lpDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph) ))
		DIERRBOX(hWnd, "Could not Set the Properties for Buffered Input for Mouse.");

	//	Acquire the Mouse.
	if (FAILED( m_lpDevice->Acquire() ))
	{
		DIERRBOX(hWnd, "Failed to acquire Mouse.")
	}	

	// DIDC_ATTACHED 
	// DIDC_FORCEFEEDBACK
	// DIDC_POLLEDDATAFORMAT VS DIDC_POLLEDDEVICE 
	// dwAxes 
	// dwButtons 
	// dwPOVs 
	DIDEVCAPS  didCaps;

	// clear out struct
	memset(&didCaps, 0, sizeof(didCaps));
	didCaps.dwSize = sizeof(didCaps); 

	//if (SUCCEEDED( m_lpDevice->GetCapabilities(&didCaps) ))
	m_lpDevice->GetCapabilities(&didCaps);
	{
		if (didCaps.dwFlags & DIDC_POLLEDDATAFORMAT)
			int y = 4;
		if (didCaps.dwFlags & DIDC_POLLEDDEVICE)
			int y = 5;
	}

	// will be zero if it failed because the struct was cleared out
	m_nNumButtons = didCaps.dwButtons;

	//	clear out current state
	memset(&m_diMouseState, 0, sizeof(m_diMouseState));
	//	clear prev state
	memset(&m_diPrevMouseState, 0, sizeof(m_diPrevMouseState));

	//	Set psuedo position of mouse
	SetPosX( 0 );
	SetPosY( 0 );
}

///////////////////////////////////////////////////////////////////
//	Function:	"ReadDevice"
//
//	Last Modified:		5/25/2008
//
//	Input	:	void
//
//	Return	:	true if it can read the device state, false if it can't.
//
//	Purpose	:	Reads the state of the device every frame.
///////////////////////////////////////////////////////////////////
bool CSGD_DIMouse::ReadDevice(void)
{
	//	Make sure the mouse device was made.
	if (!m_lpDevice)
		return false;

	//	Remember previous state
	memcpy(&m_diPrevMouseState, &m_diMouseState, sizeof(m_diPrevMouseState));

	//	Attempt to read the mouse...
	if (FAILED( m_lpDevice->GetDeviceState(sizeof(m_diMouseState), (LPVOID)&m_diMouseState) ))
	{
		//	If we couldn't, try to re-acquire the device.
		if (FAILED( m_lpDevice->Acquire() ))
			return false; //	Could not re-acquire the Mouse.

		//	Now try reading it
		if (FAILED( m_lpDevice->GetDeviceState(sizeof(m_diMouseState), (LPVOID)&m_diMouseState) ))
			return false;
	}

	// Update "psuedo" position of mouse
	SetPosX( GetPosX() + MovementX() );
	SetPosY( GetPosY() + MovementY() );

	//	Success.
	return true;
}

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
//	NOTE	:	For all Ex functions.
///////////////////////////////////////////////////////////////////
bool CSGD_DIMouse::ReadBufferedDevice(void)
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

	//	Success.
	return true;
}

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
bool CSGD_DIMouse::ButtonDown(unsigned char ucButton)
{
	//	if the high bit is on, it is pressed.
	return (m_diMouseState.rgbButtons[ucButton] & 0x80) ? true : false;
}

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
bool CSGD_DIMouse::ButtonPressed(unsigned char ucButton)
{
	//	if the button is down and wasn't previously pressed.
	return ButtonDown(ucButton) && !(m_diPrevMouseState.rgbButtons[ucButton] & 0x80);
}

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
bool CSGD_DIMouse::ButtonPressedEx(unsigned char ucButton)
{
	//	Go through each element in the Buffer.
	for (DWORD i = 0; i < m_dwBufferElements; i++)
	{
		//	If the offset matches the button we are looking for and the high bit is on, it is pressed.
		if ((m_didod[i].dwOfs == (DWORD)(DIMOFS_BUTTON0 + ucButton)) && (m_didod[i].dwData & 0x80))
			return true;
	}

	//	wasn't pressed.
	return false;
}

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
bool CSGD_DIMouse::ButtonUp(unsigned char ucButton)
{
	//	if the high bit is off, it isn't pressed.
	return !ButtonDown(ucButton);
}

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
bool CSGD_DIMouse::ButtonReleased(unsigned char ucButton)
{
	//	if the button is up and was previously pressed.
	return ButtonUp(ucButton) && (m_diPrevMouseState.rgbButtons[ucButton] & 0x80);
}

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
bool CSGD_DIMouse::ButtonReleasedEx(unsigned char ucButton)
{
	//	Go through each element in the Buffer.
	for (DWORD i = 0; i < m_dwBufferElements; i++)
	{
		//	If the offset matches the button we are looking for and the high bit is off, it was released.
		if ((m_didod[i].dwOfs == (DWORD)(DIMOFS_BUTTON0 + ucButton)) && !(m_didod[i].dwData & 0x80))
			return true;
	}

	//	wasn't pressed.
	return false;
}

///////////////////////////////////////////////////////////////////
//	Function:	"CheckBufferedButtons"
//
//	Last Modified:		6/13/2008
//
//	Input	:	void
//
//	Return	:	The mouse button that was just pressed (i.e. 0, 1, or 2).  -1 if there was no button pressed.
//
//	Purpose	:	To tell if a mouse button was just pressed (without using DirectInput's Buffered Input).
///////////////////////////////////////////////////////////////////
int CSGD_DIMouse::CheckBufferedButtons(void)
{
	int nButton = -1;

	//	Go through all mouse buttons.
	for (int j = 0; j < GetNumButtons(); j++)
	{
		//	Find the first button that was pressed
		if (ButtonPressed(j))
		{
			//	return the first one that was found to be pressed.
			nButton = j;
			break;
		}
	}

	//	return the button.
	return nButton;
}

///////////////////////////////////////////////////////////////////
//	Function:	"CheckBufferedButtonsEx"
//
//	Last Modified:		5/27/2008
//
//	Input	:	void
//
//	Return	:	The mouse button that was just pressed (i.e. 0, 1, or 2).  -1 if there was no button pressed.
//
//	Purpose	:	To tell if a mouse button was just pressed (using DirectInput's Buffered Input).
///////////////////////////////////////////////////////////////////
int CSGD_DIMouse::CheckBufferedButtonsEx(void)
{
	int nButton = -1;

	//	Go through each element in the Buffer.
	for (DWORD i = 0; i < m_dwBufferElements; i++)
	{
		//	Go through all mouse buttons.
		for (int j = 0; j < GetNumButtons(); j++)
		{
			//	If the offset matches the button we are looking for and the high bit is on, it is pressed.
			if ((m_didod[i].dwOfs == (DWORD)(DIMOFS_BUTTON0 + j)) && (m_didod[i].dwData & 0x80))
			{
				//	return the first one that was found to be pressed.
				nButton = j;
				break;
			}
		}
	}

	//	return the button.
	return nButton;
}

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
long CSGD_DIMouse::MovementX(void)
{
	return m_diMouseState.lX;
}

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
long CSGD_DIMouse::MovementY(void)
{
	return m_diMouseState.lY;
}

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
long CSGD_DIMouse::WheelMovement(void)
{
	return m_diMouseState.lZ;
}

///////////////////////////////////////////////////////////////////////////////////////////
//	CSGD_DIJoystick
///////////////////////////////////////////////////////////////////////////////////////////

//	Constants:
const int	JOYSTICK_AXIS_RANGE = 1024;
const float PERCENTAGE_RANGE	= 0.7f;
const int	JOYSTICK_THRESHOLD	= (int)(JOYSTICK_AXIS_RANGE * PERCENTAGE_RANGE);

const float ONE_OVER_RANGE = 1.0f / (float)JOYSTICK_AXIS_RANGE;

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
CSGD_DIJoystick::CSGD_DIJoystick(LPDIRECTINPUT8 pDI, HWND hWnd, const DIDEVICEINSTANCE* lpdidi, bool bIsExclusive)
{
	//	For error reporting
	char szErrorBuffer[256];

	//	Get the Joystick Name and store it.
	strcpy_s(m_szJoyName, _countof(m_szJoyName), (char*)lpdidi->tszProductName);

	// Remember if it is an Xbox 360 pad
	m_bIsXbox360Pad = (strcmp(m_szJoyName, "Xbox 360 Wireless Receiver for Windows") == 0 || strcmp(m_szJoyName, "Controller (XBOX 360 For Windows)") == 0) ? true : false;

	//	Create the Joystick Device.
	if (FAILED( pDI->CreateDevice(lpdidi->guidInstance, &m_lpDevice, NULL) ))
	{
		sprintf_s(szErrorBuffer, _countof(szErrorBuffer), "Failed to create Joystick device: %s", m_szJoyName);
		DIERRBOX(hWnd, szErrorBuffer)
	}

	//	Set the Data Format for the Joystick.
	if (FAILED( m_lpDevice->SetDataFormat(&c_dfDIJoystick2) ))
	{
		sprintf_s(szErrorBuffer, _countof(szErrorBuffer), "Failed to set data format on Joystick: %s", m_szJoyName);
		DIERRBOX(hWnd, szErrorBuffer)
	}

	//	Get Capablities:
	// DIDC_ATTACHED 
	// DIDC_FORCEFEEDBACK
	// DIDC_POLLEDDATAFORMAT VS DIDC_POLLEDDEVICE 
	// dwAxes 
	// dwButtons 
	// dwPOVs 
	DIDEVCAPS  didCaps;

	// clear out struct
	memset(&didCaps, 0, sizeof(didCaps));
	didCaps.dwSize = sizeof(didCaps); 

	//if (SUCCEEDED( m_lpDevice->GetCapabilities(&didCaps) ))
	m_lpDevice->GetCapabilities(&didCaps);
	{
		if (didCaps.dwFlags & DIDC_POLLEDDATAFORMAT)
			int y = 4;
		if (didCaps.dwFlags & DIDC_POLLEDDEVICE)
			int y = 5;

		if (didCaps.dwFlags & DIDC_FORCEFEEDBACK)
			bIsExclusive = true; // forces true so that the device can use ForceFeedback (has to be exclusive to use FF).
	}

	DWORD axes = didCaps.dwAxes;
	DWORD povs = didCaps.dwPOVs;

	// will be zero if it failed because the struct was cleared out
	m_nNumButtons = didCaps.dwButtons;

	//	Set the Cooperative level for the Joystick.
	DWORD dwFlags = DISCL_FOREGROUND;

	if (bIsExclusive)
		dwFlags |= DISCL_EXCLUSIVE;
	else
		dwFlags |= DISCL_NONEXCLUSIVE;

	if (FAILED( m_lpDevice->SetCooperativeLevel(hWnd, dwFlags) ))
	{
		sprintf_s(szErrorBuffer, _countof(szErrorBuffer), "Failed to set cooperative level on Joystick: %s", m_szJoyName);
		DIERRBOX(hWnd, szErrorBuffer)
	}

	//	Set the Properties for the Joystick Axes:

	//	Set the ranges for the axes:
	DIPROPRANGE dipr;
	dipr.diph.dwSize		= sizeof(DIPROPRANGE);
	dipr.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	dipr.diph.dwHow			= DIPH_BYOFFSET;
	dipr.lMin				= -JOYSTICK_AXIS_RANGE;	//	Minimum range.
	dipr.lMax				= +JOYSTICK_AXIS_RANGE;	//	Maximum range.

	dipr.diph.dwObj			= DIJOFS_X;				//	Change the X-Axis.
	m_lpDevice->SetProperty(DIPROP_RANGE, &dipr.diph);

	//	Change the Y-Axis.
	dipr.diph.dwObj			= DIJOFS_Y;
	m_lpDevice->SetProperty(DIPROP_RANGE, &dipr.diph);

	//	Is it not an Xbox360 controller?
	if (!m_bIsXbox360Pad)
	{
		//	Change the Z-Axis //(left/right on R-stick).
		dipr.diph.dwObj			= DIJOFS_Z;
		m_lpDevice->SetProperty(DIPROP_RANGE, &dipr.diph);

		//	Change the RotationZ-Axis //(up/down on R-stick).
		dipr.diph.dwObj			= DIJOFS_RZ;
		m_lpDevice->SetProperty(DIPROP_RANGE, &dipr.diph);
	}
	else // it is an Xbox360 controller
	{
		//	Change the Z-Axis (for L + R triggers).
		dipr.diph.dwObj			= DIJOFS_Z;
		m_lpDevice->SetProperty(DIPROP_RANGE, &dipr.diph);

		//	Change the RotationX-Axis (left/right on R-stick).
		dipr.diph.dwObj			= DIJOFS_RX;
		m_lpDevice->SetProperty(DIPROP_RANGE, &dipr.diph);

		//	Change the RotationY-Axis (up/down on R-stick).
		dipr.diph.dwObj			= DIJOFS_RY;
		m_lpDevice->SetProperty(DIPROP_RANGE, &dipr.diph);
	}

	//	Setup a Dead Zone for the axes.
	DIPROPDWORD deadZone;
	deadZone.diph.dwSize	   = sizeof (deadZone);
	deadZone.diph.dwHeaderSize = sizeof (deadZone.diph);
	deadZone.diph.dwObj		   = DIJOFS_X;
	deadZone.diph.dwHow		   = DIPH_BYOFFSET;
	deadZone.dwData			   = 1000;

	//	Setup the X-Axis Dead Zone.
	m_lpDevice->SetProperty(DIPROP_DEADZONE, &deadZone.diph);

	//	Setup the Y-Axis Dead Zone.
	deadZone.diph.dwObj		   = DIJOFS_Y;
	m_lpDevice->SetProperty(DIPROP_DEADZONE, &deadZone.diph);

	if (!m_bIsXbox360Pad)
	{
		//	Setup the Z-Axis Dead Zone.
		deadZone.diph.dwObj		   = DIJOFS_Z;
		m_lpDevice->SetProperty(DIPROP_DEADZONE, &deadZone.diph);

		//	Setup the ZR-Axis Dead Zone.
		deadZone.diph.dwObj		   = DIJOFS_RZ;
		m_lpDevice->SetProperty(DIPROP_DEADZONE, &deadZone.diph);
	}
	else
	{
		//	Setup the Z-Axis Dead Zone.
		//deadZone.diph.dwObj		   = DIJOFS_Z;
		//m_lpDevice->SetProperty(DIPROP_DEADZONE, &deadZone.diph);

		//	Setup the RotationX-Axis Dead Zone.
		deadZone.diph.dwObj		   = DIJOFS_RX;
		m_lpDevice->SetProperty(DIPROP_DEADZONE, &deadZone.diph);
		
		//	Setup the RotationY-Axis Dead Zone.
		deadZone.diph.dwObj		   = DIJOFS_RY;
		m_lpDevice->SetProperty(DIPROP_DEADZONE, &deadZone.diph);
	}

	//	Set up the device to use buffered input
	//	For Use With Buffered Input.
	DIPROPDWORD dipdw;

	//	Clear the structure to all zeroes.
	memset(&dipdw, 0, sizeof(dipdw));

	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = g_knBUFFER_SIZE;	//	Arbitrary buffer size

	//	Set the buffer size on the device.
	if (FAILED( m_lpDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph) ))
	{
		sprintf_s(szErrorBuffer, _countof(szErrorBuffer), "Could not Set the Properties for Buffered Input for Joystick: %s", m_szJoyName);
		DIERRBOX(hWnd, szErrorBuffer)
	}

	//	Acquire the Joystick.
	if (FAILED( m_lpDevice->Acquire() ))
	{
		sprintf_s(szErrorBuffer, _countof(szErrorBuffer), "Failed to acquire Joystick: %s", m_szJoyName);
		DIERRBOX(hWnd, szErrorBuffer)
	}

	//	clear out struct
	memset(&m_diJoyState, 0, sizeof(m_diJoyState));
	memset(&m_diPrevJoyState, 0, sizeof(m_diPrevJoyState));

	m_bIsUnplugged = false;

	SwapRStickAxes(false); // Z axis is considered R-stick's X-axis
}

///////////////////////////////////////////////////////////////////
//	Function:	"ReadDevice"
//
//	Last Modified:		5/26/2008
//
//	Input	:	void
//
//	Return	:	true if it can read the device state, false if it can't.
//
//	Purpose	:	Reads the state of the device every frame.
///////////////////////////////////////////////////////////////////
bool CSGD_DIJoystick::ReadDevice(void)
{
	//	Make sure the joystick device was made.
	if (!m_lpDevice)
		return false;

	//	Remember the previous state of the joystick
	memcpy(&m_diPrevJoyState, &m_diJoyState, sizeof(m_diPrevJoyState));

	//	Clear out the current state's data
	memset(&m_diJoyState, 0, sizeof(m_diJoyState));

	//	Poll the joystick to update its state
	if (FAILED( m_lpDevice->Poll() ))
	{
		//	Try to Acquire the Joystick.
		HRESULT hr = m_lpDevice->Acquire();

		//	Make sure the joystick is plugged in.
		if (!m_bIsUnplugged)
		{
			//	Check if the joystick was unplugged
			if (hr == DIERR_UNPLUGGED)
				m_bIsUnplugged = true;
		}

		//	Try to re-acquire the joystick if it is unplugged.
		if (m_bIsUnplugged)
		{
			if (SUCCEEDED( m_lpDevice->Acquire() ))
				m_bIsUnplugged = false;
		}

		//	If no input from joystick re-acquire.
		if (hr == DIERR_INPUTLOST)
			hr = m_lpDevice->Acquire();

		//	Success.
		//return true;
	}
	
	//	Attempt to read the joystick state...
	if (FAILED( m_lpDevice->GetDeviceState(sizeof(m_diJoyState), (LPVOID)&m_diJoyState) ))
		return false;

	//	Success.
	return true;
}

///////////////////////////////////////////////////////////////////
//	Function:	"ReadBufferedDevice"
//
//	Last Modified:		5/26/2008
//
//	Input	:	void
//
//	Return	:	true if it can read the device state, false if it can't.
//
//	Purpose	:	Gets the Buffered Device Data every frame.
//
//	NOTE	:	Not for use with Ex functions (use ReadDevice() for that).
///////////////////////////////////////////////////////////////////
bool CSGD_DIJoystick::ReadBufferedDevice(void)
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

	//	Success.
	return true;
}

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
//	NOTE	:	Defaults to RotationZ is Y Axis.
///////////////////////////////////////////////////////////////////
void CSGD_DIJoystick::SwitchRStickAxes(void)
{
	m_bIsZAxisY = !m_bIsZAxisY;
}

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
bool CSGD_DIJoystick::ButtonDown(unsigned char ucButton)
{
	//	if the high bit is on, it is pressed.
	return (m_diJoyState.rgbButtons[ucButton] & 0x80) ? true : false;
}

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
bool CSGD_DIJoystick::ButtonPressed(unsigned char ucButton)
{
	//	if the button is down and wasn't previously pressed.
	return ButtonDown(ucButton) && !(m_diPrevJoyState.rgbButtons[ucButton] & 0x80);
}

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
bool CSGD_DIJoystick::ButtonPressedEx(unsigned char ucButton)
{
	//	Go through each element in the Buffer.
	for (DWORD i = 0; i < m_dwBufferElements; i++)
	{
		//	If the offset matches the button we are looking for and the high bit is on, it is pressed.
		if ((m_didod[i].dwOfs == (DWORD)DIJOFS_BUTTON(ucButton)) && (m_didod[i].dwData & 0x80))
			return true;
	}

	//	wasn't pressed.
	return false;
}

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
bool CSGD_DIJoystick::ButtonUp(unsigned char ucButton)
{
	//	if the high bit is off, it isn't pressed.
	return !ButtonDown(ucButton);
}

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
bool CSGD_DIJoystick::ButtonReleased(unsigned char ucButton)
{
	//	if the button is up and was previously pressed.
	return ButtonUp(ucButton) && (m_diPrevJoyState.rgbButtons[ucButton] & 0x80);
}

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
bool CSGD_DIJoystick::ButtonReleasedEx(unsigned char ucButton)
{
	//	Go through each element in the Buffer.
	for (DWORD i = 0; i < m_dwBufferElements; i++)
	{
		//	If the offset matches the button we are looking for and the high bit is off, it was released.
		if ((m_didod[i].dwOfs == (DWORD)(DWORD)DIJOFS_BUTTON(ucButton)) && !(m_didod[i].dwData & 0x80))
			return true;
	}

	//	wasn't pressed.
	return false;
}

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
bool CSGD_DIJoystick::TranslatePOV(int nDir, DWORD dwPOVDir)
{
	// means neutral
	bool bIsPOVCentered = (LOWORD(dwPOVDir) == 0xFFFF);
	
	if (bIsPOVCentered)
		return false;

	int nPOV = (int)LOWORD(dwPOVDir);

	switch(nDir)
	{
		case DIR_UP: // could be up/left, up, or up/right.
		{
			return (nPOV > POV_LEFT || nPOV < POV_RIGHT);
		}
		break;

		case DIR_DOWN: // could be dwn/left, dwn, or dwn/right.
		{
			return (nPOV > POV_RIGHT && nPOV < POV_LEFT);
		}
		break;

		case DIR_LEFT: // could be up/left, left, or dwn/left.
		{
			return (/*dwPOVDir > POV_UP && */nPOV > POV_DOWN);
		}
		break;

		case DIR_RIGHT: // could be up/right, right, or dwn/right.
		{
			return (nPOV > POV_UP && nPOV < POV_DOWN);
		}
		break;
	};

	return false;
}

///////////////////////////////////////////////////////////////////
//	Function:	"TranslateRStickX"
//
//	Last Modified:		6/06/2008
//
//	Input	:	diJoyState	-	The joystate to translate.
//
//	Return	:	-JOYSTICK_RANGE to +JOYSTICK_RANGE (which is 1024)
//
//	Purpose	:	To get the position of the joystick in a digital range.
///////////////////////////////////////////////////////////////////	
LONG CSGD_DIJoystick::TranslateRStickX(DIJOYSTATE2& diJoyState)
{
	//	Make sure the device is valid.
	if (!m_lpDevice)
		return 0;

	//	Check for the Direction.
	if (!m_bIsXbox360Pad)
	{
		return (m_bIsZAxisY) ? diJoyState.lRz : diJoyState.lZ;
	}
	else	//	is Xbox 360 pad
	{
		return diJoyState.lRx;
	}
}

///////////////////////////////////////////////////////////////////
//	Function:	"TranslateRStickY"
//
//	Last Modified:		6/07/2008
//
//	Input	:	diJoyState	-	The joystate to translate.
//
//	Return	:	-JOYSTICK_RANGE to +JOYSTICK_RANGE (which is 1024)
//
//	Purpose	:	To get the position of the joystick in a digital range.
///////////////////////////////////////////////////////////////////	
LONG CSGD_DIJoystick::TranslateRStickY(DIJOYSTATE2& diJoyState)
{
	//	Make sure the device is valid.
	if (!m_lpDevice)
		return 0;

	//	Check for the Direction.
	if (!m_bIsXbox360Pad)
	{
		return (m_bIsZAxisY) ? diJoyState.lZ : diJoyState.lRz; 
	}
	else	//	is Xbox 360 pad
	{
		return diJoyState.lRy;
	}
}

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
bool CSGD_DIJoystick::DPadDown(int nDir)
{
	DWORD dwDir = m_diJoyState.rgdwPOV[0];

	return TranslatePOV(nDir, m_diJoyState.rgdwPOV[0]);
}

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
bool CSGD_DIJoystick::DPadPressed(int nDir)
{
	//	If the d-pad is down and wasn't previously.
	return DPadDown(nDir) && !TranslatePOV(nDir, m_diPrevJoyState.rgdwPOV[0]);
}

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
bool CSGD_DIJoystick::DPadUp(int nDir)
{
	return !DPadDown(nDir);
}

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
bool CSGD_DIJoystick::DPadReleased(int nDir)
{
	//	if the direction isn't down and was previously pressed.
	return !DPadDown(nDir) && TranslatePOV(nDir, m_diPrevJoyState.rgdwPOV[0]);
}

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
bool CSGD_DIJoystick::GetLStickDirDown(int nDir)
{
	//	Make sure the device is valid.
	if (!m_lpDevice)
		return false;

	//	Check for the Direction.
	switch(nDir)
	{
		case DIR_LEFT:
		{
			if (m_diJoyState.lX < 0) 
				return true;
		}		
		break;

		case DIR_RIGHT:
		{
			if (m_diJoyState.lX > 0) 
				return true;
		}	
		break;

		case DIR_UP:
		{
			if (m_diJoyState.lY < 0) 
				return true;
		}
		break;

		case DIR_DOWN:
		{
			if (m_diJoyState.lY > 0) 
				return true;
		}	
		break;
	};

	//	The direction wasn't pressed.
	return false;
}

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
bool CSGD_DIJoystick::GetLStickDirPressed(int nDir)
{
	//	Make sure the device is valid.
	if (!m_lpDevice)
		return false;

	//	Check for the Direction.
	switch(nDir)
	{
		case DIR_LEFT:
		{
			if (m_diJoyState.lX < -JOYSTICK_THRESHOLD && !(m_diPrevJoyState.lX < -JOYSTICK_THRESHOLD)) 
				return true;
		}		
		break;

		case DIR_RIGHT:
		{
			if (m_diJoyState.lX > JOYSTICK_THRESHOLD && !(m_diPrevJoyState.lX > JOYSTICK_THRESHOLD)) 
				return true;
		}	
		break;

		case DIR_UP:
		{
			if (m_diJoyState.lY < -JOYSTICK_THRESHOLD && !(m_diPrevJoyState.lY < -JOYSTICK_THRESHOLD)) 
				return true;
		}
		break;

		case DIR_DOWN:
		{
			if (m_diJoyState.lY > JOYSTICK_THRESHOLD && !(m_diPrevJoyState.lY > JOYSTICK_THRESHOLD)) 
				return true;
		}	
		break;
	};

	//	The direction wasn't pressed.
	return false;
}

///////////////////////////////////////////////////////////////////
//	Function:	"GetLStickXAmount"
//
//	Last Modified:		6/01/2008
//
//	Input	:	void
//
//	Return	:	-JOYSTICK_RANGE to +JOYSTICK_RANGE ((-) Left and (+) Right)
//
//	Purpose	:	To get the position of the joystick in a digital range.
//
//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
///////////////////////////////////////////////////////////////////
int CSGD_DIJoystick::GetLStickXAmount(void)
{
	//	Make sure the device is valid.
	if (!m_lpDevice)
		return 0;

	return m_diJoyState.lX;
}

///////////////////////////////////////////////////////////////////
//	Function:	"GetLStickYAmount"
//
//	Last Modified:		6/01/2008
//
//	Input	:	void
//
//	Return	:	-JOYSTICK_RANGE to +JOYSTICK_RANGE (-JOYSTICK_RANGE is Up, +JOYSTICK_RANGE is Down)
//
//	Purpose	:	To get the position of the joystick in a digital range.
//
//	NOTE:	L-stick and D-pad can usually be swapped by pressing a "Mode" button on the controller.
///////////////////////////////////////////////////////////////////
int CSGD_DIJoystick::GetLStickYAmount(void)
{
	//	Make sure the device is valid.
	if (!m_lpDevice)
		return 0;

	return m_diJoyState.lY;
}

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
float CSGD_DIJoystick::GetLStickXNormalized()
{
	return (float)GetLStickXAmount() * ONE_OVER_RANGE;			// same as dividing by JOYSTICK_AXIS_RANGE (but twice as fast!)
}

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
float CSGD_DIJoystick::GetLStickYNormalized()
{
	return (float)GetLStickYAmount() * ONE_OVER_RANGE;			// same as dividing by JOYSTICK_AXIS_RANGE (but twice as fast!)
}

/////////////////////////////////

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
bool CSGD_DIJoystick::GetRStickDirDown(int nDir)
{
	//	Make sure the device is valid.
	if (!m_lpDevice)
		return false;

	switch(nDir)
	{
		case DIR_LEFT:
		{
			if ( TranslateRStickX(m_diJoyState) < 0 ) 
				return true;
		}		
		break;

		case DIR_RIGHT:
		{
			if ( TranslateRStickX(m_diJoyState) > 0 ) 
				return true;
		}	
		break;

		case DIR_UP:
		{
			if ( TranslateRStickY(m_diJoyState) < 0 ) 
				return true;
		}
		break;

		case DIR_DOWN:
		{
			if ( TranslateRStickY(m_diJoyState) > 0 ) 
				return true;
		}	
		break;
	};

	//	The direction wasn't pressed.
	return false;
}

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
bool CSGD_DIJoystick::GetRStickDirPressed(int nDir)
{
	//	Make sure the device is valid.
	if (!m_lpDevice)
		return false;

	//	Check for the Direction.
	switch(nDir)
	{
		case DIR_LEFT:
		{
			if ( TranslateRStickX(m_diJoyState) < -JOYSTICK_THRESHOLD && !(TranslateRStickX(m_diPrevJoyState) < -JOYSTICK_THRESHOLD) ) 
				return true;
		}		
		break;

		case DIR_RIGHT:
		{
			if ( TranslateRStickX(m_diJoyState) > JOYSTICK_THRESHOLD && !(TranslateRStickX(m_diPrevJoyState) > JOYSTICK_THRESHOLD) ) 
				return true;
		}	
		break;

		case DIR_UP:
		{
			if ( TranslateRStickY(m_diJoyState) < -JOYSTICK_THRESHOLD && !(TranslateRStickY(m_diPrevJoyState) < -JOYSTICK_THRESHOLD) ) 
				return true;
		}
		break;

		case DIR_DOWN:
		{
			if ( TranslateRStickY(m_diJoyState) > JOYSTICK_THRESHOLD && !(TranslateRStickY(m_diPrevJoyState) > JOYSTICK_THRESHOLD) ) 
				return true;
		}	
		break;
	};

	//	The direction wasn't pressed.
	return false;
}

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
int CSGD_DIJoystick::GetRStickXAmount(void)
{
	//	Make sure the device is valid.
	if (!m_lpDevice)
		return 0;

	return TranslateRStickX(m_diJoyState);
}

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
int CSGD_DIJoystick::GetRStickYAmount(void)
{
	//	Make sure the device is valid.
	if (!m_lpDevice)
		return 0;

	return TranslateRStickY(m_diJoyState);
}

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
float CSGD_DIJoystick::GetRStickXNormalized()
{
	return (float)GetRStickXAmount() * ONE_OVER_RANGE;			// same as dividing by JOYSTICK_AXIS_RANGE (but twice as fast!)
}

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
float CSGD_DIJoystick::GetRStickYNormalized()
{
	return (float)GetRStickYAmount() * ONE_OVER_RANGE;			// same as dividing by JOYSTICK_AXIS_RANGE (but twice as fast!)
}

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
int CSGD_DIJoystick::GetLTriggerAmount(void)
{
	if (!m_bIsXbox360Pad) return 0;

	return m_diJoyState.lZ;
}

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
int CSGD_DIJoystick::GetRTriggerAmount(void)
{
	if (!m_bIsXbox360Pad) return 0;

	return -m_diJoyState.lZ; // - to take into account that it is actually 0 to -JOYSTICK_AXIS_RANGE
}

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
float CSGD_DIJoystick::GetLTriggerNormalized(void)
{
	return GetRTriggerAmount() * ONE_OVER_RANGE;	// same as dividing by JOYSTICK_AXIS_RANGE (but twice as fast!)
}

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
float CSGD_DIJoystick::GetRTriggerNormalized(void)
{
	return GetRTriggerAmount() * ONE_OVER_RANGE;	// same as dividing by JOYSTICK_AXIS_RANGE (but twice as fast!)
}

/////////////////////////////////

///////////////////////////////////////////////////////////////////
//	Function:	"CheckBufferedButtons"
//
//	Last Modified:		6/07/2008
//
//	Input	:	void
//
//	Return	:	The joystick button that was just pressed.  -1 if no button was pressed.
//
//	Purpose	:	To tell which joystick button was just pressed (without using DirectInput's Buffered Input).
///////////////////////////////////////////////////////////////////
int	CSGD_DIJoystick::CheckBufferedButtons(void)
{
	int nButton = -1;

	//	Go through all joystick buttons.
	for (int j = 0; j < GetNumButtons(); j++)
	{
		//	Check if the button was just pressed
		if (ButtonPressed((unsigned char)j))
		{
			//	return the first one that was found to be pressed.
			nButton = j;
			break;
		}
	}

	//	return the button.
	return nButton;
}

///////////////////////////////////////////////////////////////////
//	Function:	"CheckBufferedButtonsEx"
//
//	Last Modified:		5/27/2008
//
//	Input	:	void
//
//	Return	:	The joystick button that was just pressed.  -1 if no button was pressed.
//
//	Purpose	:	To tell which joystick button was just pressed (using DirectInput's Buffered Input).
///////////////////////////////////////////////////////////////////
int CSGD_DIJoystick::CheckBufferedButtonsEx(void)
{
	int nButton = -1;

	//	Go through each element in the Buffer.
	for (DWORD i = 0; i < m_dwBufferElements; i++)
	{
		//	Go through all joystick buttons.
		for (int j = 0; j < GetNumButtons(); j++)
		{
			//	If the offset matches the button we are looking for and the high bit is on, it is pressed.
			if ((m_didod[i].dwOfs == (DWORD)(DWORD)DIJOFS_BUTTON(j)) && (m_didod[i].dwData & 0x80))
			{
				//	return the first one that was found to be pressed.
				nButton = j;
				break;
			}
		}
	}

	//	return the button.
	return nButton;
}