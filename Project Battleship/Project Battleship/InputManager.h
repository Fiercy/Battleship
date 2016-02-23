#pragma once
#include <Windows.h>
#include <DirectXMath.h>
using namespace DirectX;
#include <vector>

//enum eKeys{
//	null = 0, LEFT_CLICK, RIGHT_CLICK, BACKSPACE = 8, RETURN = 13, ESCAPE = 27, SPACE = 32, ZERO = 48, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
//	A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, a = 97, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, DEL = 127
//};
class CInputManager
{
public:
	enum eKeyStates { KEY_CLEAR = 0, KEY_PRESS, KEY_DOWN, KEY_RELEASE, KEY_UP };
	struct tMouseInput
	{
		unsigned int x, y, prevX, prevY;
		int m_nScrollPos;
	};
	struct tKey
	{
		eKeyStates		nKeyState;
		float			fHeldDuration;
		unsigned int	unRefCount;
	};
	bool m_bAnyKeyEnabled;
private:
	static CInputManager*  m_pInstance;
	tKey m_Keys[256];
	tMouseInput m_tMouse;
	std::vector<unsigned char> m_ActiveKeys;

	bool m_bKeyboardActive = false;
	bool m_bInputActive = true;

	// for tracking any key useful for typing or waking up from attract states
	unsigned char	m_ucLastKeyCode;
	tKey			m_LastKeyState;

	// helper functions
	void KeyDown(const unsigned char VKcode);
	void KeyUp(const unsigned char VKcode);
	void ScrollUp() { m_tMouse.m_nScrollPos++; }
	void ScrollDown() { m_tMouse.m_nScrollPos--; }


	CInputManager();
	CInputManager(const CInputManager &);
	CInputManager & operator=(const CInputManager &);
public:
	~CInputManager();
	static CInputManager* GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	bool HandleWindowMessages(const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
	// starts tracking the key
	void ActivateKey(const unsigned char VKcode);
	// activates the key if needed
	
	// will unbind all events and no longer track the key
	void DeActivateKey(const unsigned char VKcode);
	// sends events for current key states and then updates the keys for next frame
	// Note: if multiple keys were pressed between the frame the events won't match the order they were pressed.
	void UpdateKeyStates(const float fDeltaTime);
	inline const tMouseInput& GetMouseInput() const { return m_tMouse; }

	void EnableAnyKeyTracking() { m_bAnyKeyEnabled = true; }
	void DisableAnyKeyTracking() { m_bAnyKeyEnabled = false; }
	bool GetAnyKeyTrackingEnabled()const { return m_bAnyKeyEnabled; }

	void ActivateKeyboard() { m_bKeyboardActive = true; }
	void DeactivateKeyboard() { m_bKeyboardActive = false; }

	// used for when losing window focus
	void ResetInput();

	// this is not the most efficient way to do things but is very simple for testing purposes
	// Note: the key to be checked must be active in order for this to return an accurate state
	const eKeyStates GetKeyState(const unsigned char VKcode)const;
	const bool GetKeyDown(const unsigned char VKcode)const;
	const bool GetKeyUp(const unsigned char VKcode)const;
	const bool GetKeyRelease(const unsigned char VKcode)const;
	const bool GetKeyPress(const unsigned char VKcode)const;
	const float GetKeyDownTime(const unsigned char VKcode)const;

	const bool GetAnyKeyPress()const;


	unsigned int GetLastKeyCode() const { return m_ucLastKeyCode; }
	eKeyStates GetLastKeyState() const { return m_LastKeyState.nKeyState; }

	void PauseInput();

	void ActivateKeys();

};