#pragma once
#include <stack>
#include <vector>
#include "State.h"
#include "Renderer.h"
#include "AssetManager.h"
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

class CStateManager
{
public:
	CStateManager();
	~CStateManager();
	static CStateManager* GetInstance();

	void Init(HWND hWnd, int nWidth, int nHeight);

	void PushState(unsigned int new_State);
	void PopState();
	void RunState(float elapsedTime);
	void CStateManager::Clear();
	CRenderer* GetRenderer() { return m_pRenderer; }
	CAssetManager* GetAssetManager() { return m_pAssetManager; }

	//CState* GetState(int numState)  {}

private:
	static CStateManager*                  m_pInstance;
	std::stack<CState*>                   m_StateStack;
	std::vector<CState*> m_GameStates;
	CRenderer*          m_pRenderer;
	CAssetManager*      m_pAssetManager;
};

