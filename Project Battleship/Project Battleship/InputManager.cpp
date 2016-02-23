#include "InputManager.h"
#include <windowsx.h>
#include "StateManager.h"

//--------------------------------------------------------------------------------
CInputManager* CInputManager::m_pInstance = 0;
//--------------------------------------------------------------------------------
CInputManager*  CInputManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CInputManager();
	}
	return m_pInstance;
}
//--------------------------------------------------------------------------------
void CInputManager::CreateInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CInputManager();
	}
}
//--------------------------------------------------------------------------------
void CInputManager::DestroyInstance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
	}
	m_pInstance = NULL;
}
//--------------------------------------------------------------------------------
CInputManager::CInputManager()
{
	// clear all the keys and mouse info
	ZeroMemory(&m_Keys[0], 256 * sizeof(tKey));
	ZeroMemory(&m_tMouse, sizeof(m_tMouse));
	m_bAnyKeyEnabled = false;
	m_bKeyboardActive = false;
	m_bInputActive = true;
}
//--------------------------------------------------------------------------------
CInputManager::~CInputManager()
{
}
//--------------------------------------------------------------------------------
void CInputManager::ActivateKey(const unsigned char VKcode)
{
	if (m_Keys[VKcode].unRefCount == 0)
	{
		// create a key and associated events
		m_ActiveKeys.push_back(VKcode);
	}

	m_Keys[VKcode].unRefCount++;
}
//--------------------------------------------------------------------------------
void CInputManager::DeActivateKey(const unsigned char VKcode)
{
	//ensure the key is active
	if (m_Keys[VKcode].unRefCount > 0)
	{
		m_Keys[VKcode].unRefCount--;
		if (m_Keys[VKcode].unRefCount == 0)
		{
			for (unsigned int i = 0; i < m_ActiveKeys.size(); i++)
			{
				if (m_ActiveKeys[i] == VKcode)
				{
					// remove from active list
					m_ActiveKeys.erase(m_ActiveKeys.begin() + i);
					// reset actual key
					m_Keys[VKcode].nKeyState = KEY_CLEAR;
					m_Keys[VKcode].fHeldDuration = 0.f;
					return;
				}
			}
		}
	}
}

void CInputManager::KeyDown(const unsigned char VKcode)
{
	if (m_Keys[VKcode].nKeyState == KEY_CLEAR || m_Keys[VKcode].nKeyState == KEY_UP)
	{
		m_Keys[VKcode].nKeyState = KEY_PRESS;
	}

	if (m_bAnyKeyEnabled)
	{
		if (m_LastKeyState.nKeyState != KEY_PRESS || m_ucLastKeyCode != VKcode)
		{
			m_LastKeyState.nKeyState = KEY_PRESS;
			m_ucLastKeyCode = VKcode;
		}
	}
}
//--------------------------------------------------------------------------------
void CInputManager::KeyUp(unsigned char VKcode)
{
	if (m_Keys[VKcode].nKeyState > KEY_CLEAR)
	{
		m_Keys[VKcode].nKeyState = KEY_RELEASE;
		m_Keys[VKcode].fHeldDuration = 0.f;
	}

	if (m_bAnyKeyEnabled)
	{
		if (m_LastKeyState.nKeyState == KEY_CLEAR)
		{
			m_LastKeyState.nKeyState = KEY_UP;
			m_ucLastKeyCode = VKcode;
		}
	}
}
//--------------------------------------------------------------------------------
bool CInputManager::HandleWindowMessages(const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
	if (m_bInputActive == true)
	{
		switch (uMsg)
		{
			// left mouse button
		case WM_LBUTTONDOWN:
		{
			KeyDown(VK_LBUTTON);
			break;
		}
		case WM_LBUTTONUP:
		{
			KeyUp(VK_LBUTTON);
			break;
		}
		// right mouse button
		case WM_RBUTTONDOWN:
		{
			KeyDown(VK_RBUTTON);
			break;
		}
		case WM_RBUTTONUP:
		{
			KeyUp(VK_RBUTTON);
			break;
		}
		// middle mouse button
		case WM_MBUTTONDOWN:
		{
			KeyDown(VK_MBUTTON);
			break;
		}
		case WM_MBUTTONUP:
		{
			KeyUp(VK_MBUTTON);
			break;
		}
		// X mouse button
		case WM_XBUTTONDOWN:
		{
			switch (LOWORD(wParam))
			{
			case MK_XBUTTON1:
			{
				KeyDown(VK_XBUTTON1);
				break;
			}
			case MK_XBUTTON2:
			{
				KeyDown(VK_XBUTTON2);
				break;
			}
			}
			break;
		}
		case WM_XBUTTONUP:
		{
			switch (wParam)
			{
			case MK_XBUTTON1:
			{
				KeyUp(VK_XBUTTON1);
				break;
			}
			case MK_XBUTTON2:
			{
				KeyUp(VK_XBUTTON2);
				break;
			}
			}
			break;
		}
		// mousewheel
		case WM_MOUSEWHEEL:
		{
			if ((short)(HIWORD(wParam)) > 0)
			{
				ScrollUp();
			}
			else
			{
				ScrollDown();
			}
			break;
		}
		// keyboard
		case WM_CHAR:
		{
			if (m_bKeyboardActive == true)
			{
				KeyDown(unsigned char(wParam & 0xFF));
			}
			int y = 0;
			break;
		}
		case WM_KEYDOWN:
		{
			if (m_bKeyboardActive == false)
			{
				KeyDown(unsigned char(wParam & 0xFF));
			}
			int y = 0;
			break;
		}
		case WM_KEYUP:
		{
			KeyUp(unsigned char(wParam & 0xFF));
			break;
		}
		case WM_MOUSEMOVE:
		{
			m_tMouse.x = GET_X_LPARAM(lParam);
			m_tMouse.y = GET_Y_LPARAM(lParam);
			break;
		}
		case WM_SYSKEYDOWN:
		{
			// full screen
			KeyDown(VK_LMENU);
			KeyDown(unsigned char(wParam & 0xFF));
			if ((unsigned char(wParam & 0xFF)) == VK_RETURN&&!(lParam & 0xA0))
			{
				//EventManager()->SendEvent("OnFullScreen",0,CEventManager::eEventInstant);
			}

			else if ((unsigned char(wParam & 0xFF)) == VK_F4&&!(lParam & 0xA0))
				PostQuitMessage(0);
			else
				break;
		}
		case WM_SYSKEYUP:
		{
			if (lParam & 0x2000000)
			{
				KeyUp(VK_LMENU);
				m_Keys[wParam & 0xFF].nKeyState = KEY_DOWN;
			}
			else
			{
				KeyUp(unsigned char(wParam & 0xFF));
			}
			break;
		}
		case WM_NCMOUSELEAVE:
		{
			// reset the mouse buttons if it leaves the window bounds
			KeyUp(VK_LBUTTON);
			KeyUp(VK_RBUTTON);
			KeyUp(VK_MBUTTON);
			KeyUp(VK_XBUTTON1);
			KeyUp(VK_XBUTTON2);
			break;
		}
		default:
		{
			return true;
		}
		}
	}
	return false;
}
//--------------------------------------------------------------------------------
void CInputManager::ResetInput()
{
	for (unsigned int i = 0; i < m_ActiveKeys.size(); i++)
	{
		KeyUp(m_ActiveKeys[i]);
	}
}
//--------------------------------------------------------------------------------
void CInputManager::UpdateKeyStates(const float fDeltaTime)
{
	if (m_bInputActive == false)
	{
		m_bInputActive = true;
	}
	// any-key handling
	if (m_bAnyKeyEnabled)
	{
		switch (m_LastKeyState.nKeyState)
		{
		case KEY_PRESS:
		{
			m_LastKeyState.nKeyState = KEY_DOWN;
			m_LastKeyState.fHeldDuration = 0.f;
			break;
		}
		case KEY_DOWN:
		{
			m_LastKeyState.fHeldDuration += fDeltaTime;
			break;
		}
		case KEY_UP:
		{
			m_LastKeyState.nKeyState = KEY_CLEAR;
			break;
		}

		}
	}

	for (unsigned int i = 0; i < m_ActiveKeys.size(); i++)
	{
		switch (m_Keys[m_ActiveKeys[i]].nKeyState)
		{
		case KEY_PRESS:
		{
			m_Keys[m_ActiveKeys[i]].nKeyState = KEY_DOWN;
			break;
		}
		case KEY_DOWN:
		{
			m_Keys[m_ActiveKeys[i]].fHeldDuration += fDeltaTime;
			break;
		}
		case KEY_RELEASE:
		{
			m_Keys[m_ActiveKeys[i]].nKeyState = KEY_UP;
			break;
		}
		}
	}

	// update mouse position as well
	m_tMouse.prevX = m_tMouse.x;
	m_tMouse.prevY = m_tMouse.y;
}
//--------------------------------------------------------------------------------
const CInputManager::eKeyStates CInputManager::GetKeyState(const unsigned char VKcode) const
{
	return m_Keys[VKcode].nKeyState;
}
//--------------------------------------------------------------------------------
const bool CInputManager::GetKeyDown(const unsigned char VKcode) const
{
	if (m_bKeyboardActive == false || VK_RETURN == VKcode || VK_BACK == VKcode
		|| VK_LBUTTON == VKcode || VK_RBUTTON == VKcode)
	{
		return (m_Keys[VKcode].nKeyState == KEY_DOWN) ? true : false;
	}
	return false;
}
//--------------------------------------------------------------------------------
const bool CInputManager::GetKeyUp(const unsigned char VKcode) const
{
	if (m_bKeyboardActive == false || VK_RETURN == VKcode || VK_BACK == VKcode
		|| VK_LBUTTON == VKcode || VK_RBUTTON == VKcode)
	{
		return (m_Keys[VKcode].nKeyState == KEY_UP) ? true : false;
	}
	return false;
}
//--------------------------------------------------------------------------------
const bool CInputManager::GetKeyRelease(const unsigned char VKcode)	const
{
	if (m_bKeyboardActive == false || VK_RETURN == VKcode || VK_BACK == VKcode
		|| VK_LBUTTON == VKcode || VK_RBUTTON == VKcode)
	{
		return (m_Keys[VKcode].nKeyState == KEY_RELEASE) ? true : false;
	}
	return false;
}
//--------------------------------------------------------------------------------
const bool CInputManager::GetKeyPress(const unsigned char VKcode) const
{
	if (m_bKeyboardActive == false || VK_RETURN == VKcode || VK_BACK == VKcode
		|| VK_LBUTTON == VKcode || VK_RBUTTON == VKcode)
	{
		return (m_Keys[VKcode].nKeyState == KEY_PRESS) ? true : false;
	}
	return false;
}
//--------------------------------------------------------------------------------
const bool CInputManager::GetAnyKeyPress() const
{
	for (size_t i = 0; i < 256; i++)
	{
		if (m_Keys[i].nKeyState == KEY_PRESS)
			return true;
	}
	return false;
}
//--------------------------------------------------------------------------------
const float CInputManager::GetKeyDownTime(const unsigned char VKcode)const
{
	return m_Keys[VKcode].fHeldDuration;
}

void CInputManager::PauseInput()
{
	for (unsigned int i = 0; i < m_ActiveKeys.size(); i++)
	{
		// reset actual key
		m_Keys[m_ActiveKeys[i]].nKeyState = KEY_CLEAR;
		m_Keys[m_ActiveKeys[i]].fHeldDuration = 0.f;
	}
	m_bInputActive = false;
}

void CInputManager::ActivateKeys()
{
	ActivateKey(VK_ESCAPE);
	ActivateKey(VK_UP);
	ActivateKey(VK_DOWN);
	ActivateKey(VK_RIGHT);
	ActivateKey(VK_LEFT);
	ActivateKey(VK_LBUTTON);
	ActivateKey(VK_BACK);
	ActivateKey(VK_RBUTTON);
	ActivateKey(VK_RETURN);
	ActivateKey(VK_SPACE);
	ActivateKey(VK_F1);
	ActivateKey('Q');
	ActivateKey('W');
	ActivateKey('E');
	ActivateKey('R');
	ActivateKey('A');
	ActivateKey('S');
	ActivateKey('D');
	ActivateKey('F');
	ActivateKey('Z');
	ActivateKey('X');
	ActivateKey('C');
	ActivateKey('V');
	ActivateKey('T');
	ActivateKey('H');
	ActivateKey('J');
	ActivateKey('K');
	ActivateKey('L');
	ActivateKey('M');
	ActivateKey('P');
	ActivateKey(VK_MULTIPLY);
	ActivateKey(4);
	ActivateKey(VK_NUMPAD0);
	ActivateKey(VK_NUMPAD5);
	EnableAnyKeyTracking();

}