// include the basic windows header file
#include <windows.h>
#include <windowsx.h>
#include "StateManager.h"
#include "InputManager.h"
#include <vld.h>
void ActivateKeys();
CStateManager* m_pStateManager;
CInputManager* m_pInputManager;

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "Project Battleship";

	// register the window class
	RegisterClassEx(&wc);

	// calculate the size of the client area
	int nScreenWidth = 736;
	int nScreenHeight = 600;


	RECT wr = { 0, 0, nScreenWidth, nScreenHeight };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		"Project Battleship",    // name of the window class
		"Simple Battleship",   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		300,    // x-position of the window
		300,    // y-position of the window
		wr.right - wr.left,    // width of the window
		wr.bottom - wr.top,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL


	// display the window on the screen
	ShowWindow(hWnd, nCmdShow);


	m_pStateManager = CStateManager::GetInstance();
	m_pStateManager->Init(hWnd, nScreenWidth, nScreenHeight);

    m_pInputManager = CInputManager::GetInstance();
	m_pInputManager->ActivateKeys();

	// this struct holds Windows event messages
	MSG msg = { 0 };

	// Enter the infinite message loop
	while (TRUE)
	{
		// Check to see if any messages are waiting in the queue
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);

			// check to see if it's time to quit
			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			m_pStateManager->RunState(0);
			if (GetActiveWindow() == hWnd)
			{
				m_pInputManager->UpdateKeyStates(0);
			}
			else
			{
				m_pInputManager->PauseInput();
			}
		}
	}
	m_pStateManager->Clear();
	delete m_pStateManager;
	CInputManager::DestroyInstance();

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	bool bNoInput = false;
	if (m_pInputManager != nullptr)
	{
		bNoInput = m_pInputManager->HandleWindowMessages(message, wParam, lParam);
	}

	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}


