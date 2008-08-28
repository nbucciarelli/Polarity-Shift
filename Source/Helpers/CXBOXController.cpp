/////////////////////////////////////////////////////////////
//	File:		"CXBOXController.cpp"
//	Author:		Nick Bucciarelli
//	Purpose:	XInput wrapper
/////////////////////////////////////////////////////////////

#include "CXBOXController.h"

CXBOXController::CXBOXController(int nPlayerNumber)
{
	m_nControllerNum = nPlayerNumber - 1;
}

XINPUT_STATE CXBOXController::GetState()
{
	ZeroMemory(&m_XControllerState, sizeof(XINPUT_STATE));
	XInputGetState(m_nControllerNum, &m_XControllerState);
	return m_XControllerState;
}

bool CXBOXController::IsConnected()
{
	ZeroMemory(&m_XControllerState, sizeof(XINPUT_STATE));

	unsigned int Result = XInputGetState(m_nControllerNum, &m_XControllerState);
	if(Result == ERROR_SUCCESS)
		return true;
	else
		return false;
	
}

void CXBOXController::Vibrate(int leftVal, int rightVal)
{
	XINPUT_VIBRATION Vibration;

	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	Vibration.wLeftMotorSpeed = leftVal;
	Vibration.wRightMotorSpeed = rightVal;

	XInputSetState(m_nControllerNum, &Vibration);
}