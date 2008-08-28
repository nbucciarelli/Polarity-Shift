/////////////////////////////////////////////////////////////
//	File:		"CXBOXController.H"
//	Author:		Nick Bucciarelli
//	Purpose:	XInput wrapper
/////////////////////////////////////////////////////////////

#pragma once

#include <windows.h>
#include <XInput.h>
//XInput Library
#pragma comment(lib, "XInput.lib")

/*
typedef struct _XINPUT_GAMEPAD {
WORD wButtons;
BYTE bLeftTrigger;
BYTE bRightTrigger;
SHORT sThumbLX;
SHORT sThumbLY;
SHORT sThumbRX;
SHORT sThumbRY;
} XINPUT_GAMEPAD, *PXINPUT_GAMEPAD;
*/
/*
XINPUT_GAMEPAD_DPAD_UP          0x00000001
XINPUT_GAMEPAD_DPAD_DOWN        0x00000002
XINPUT_GAMEPAD_DPAD_LEFT        0x00000004
XINPUT_GAMEPAD_DPAD_RIGHT       0x00000008
XINPUT_GAMEPAD_START            0x00000010
XINPUT_GAMEPAD_BACK             0x00000020
XINPUT_GAMEPAD_LEFT_THUMB       0x00000040
XINPUT_GAMEPAD_RIGHT_THUMB      0x00000080
XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
XINPUT_GAMEPAD_A                0x1000
XINPUT_GAMEPAD_B                0x2000
XINPUT_GAMEPAD_X                0x4000
XINPUT_GAMEPAD_Y                0x8000
*/

class CXBOXController
{
private:
	XINPUT_STATE m_XControllerState;
	int m_nControllerNum;
public:
	CXBOXController(int nPlayerNumber);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"GetState"
	//	Last Modified:	August 28th, 2008
	//	Purpose:		Gets the state of the controller
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	XINPUT_STATE GetState();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"IsConnected"
	//	Last Modified:	August 28th, 2008
	//	Purpose:		Is the controller connected?
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool IsConnected();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Function:		"Vibrate"
	//	Last Modified:	August 28th, 2008
	//	Purpose:		Vibrates the controller
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void Vibrate(int nLeftVal = 0, int nRightVal = 0);
};

