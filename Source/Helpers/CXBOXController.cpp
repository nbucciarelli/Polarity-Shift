#include "CXBOXController.h"

CXBOXController::CXBOXController(int nPlayerNumber)
{
	// Set the Controller Number
	m_nControllerNum = nPlayerNumber - 1;
}

XINPUT_STATE CXBOXController::GetState()
{
	// Zeroise the state
	ZeroMemory(&m_XControllerState, sizeof(XINPUT_STATE));

	// Get the state
	XInputGetState(m_nControllerNum, &m_XControllerState);

	return m_XControllerState;
}

bool CXBOXController::IsConnected()
{
	// Zeroise the state
	ZeroMemory(&m_XControllerState, sizeof(XINPUT_STATE));

	// Get the state
	DWORD Result = XInputGetState(m_nControllerNum, &m_XControllerState);

	if(Result == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CXBOXController::Vibrate(int leftVal, int rightVal)
{
	// Create a Vibraton State
	XINPUT_VIBRATION Vibration;

	// Zeroise the Vibration
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = leftVal;
	Vibration.wRightMotorSpeed = rightVal;

	// Vibrate the controller
	XInputSetState(m_nControllerNum, &Vibration);
}