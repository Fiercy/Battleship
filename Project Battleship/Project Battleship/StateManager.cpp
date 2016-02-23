#include "StateManager.h"
#include "State.h"
#include "VSPlayerState.h"
#include "MainMenuState.h"

CStateManager* CStateManager::m_pInstance = 0;

CStateManager*  CStateManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CStateManager();
	}
	return m_pInstance;
}

CStateManager::CStateManager()
{
}


CStateManager::~CStateManager()
{
	delete m_pRenderer;
	delete m_pAssetManager;

	for (unsigned int i = 0; i < m_GameStates.size();i++)
	{
		delete m_GameStates[i];
	}
}

void CStateManager::Init(HWND hWnd, int nWidth, int nHeight)
{
	m_pRenderer = new CRenderer();
	m_pRenderer->Init(hWnd, nWidth, nHeight);

	m_pAssetManager = new CAssetManager();
	m_pAssetManager->Init(m_pRenderer->GetDevice(), m_pRenderer->GetDeviceContext());

	m_GameStates.push_back(new CMainMenuState());
	m_GameStates.push_back(new CVSPlayerState());
	//m_GameStates.push_back(new CVSAIState());
	
	PushState(0);
}



void CStateManager::PushState(unsigned int new_State)
{
	if (!m_StateStack.empty())
	{
		m_StateStack.top()->Exit();
	}
	switch (new_State)
	{
	case mainMenu:
	{
		m_StateStack.push(m_GameStates[mainMenu]);
		break;
	}
	case vsPlayer:
	{
		m_StateStack.push(m_GameStates[vsPlayer]);
		break;
	}
	default:
		return;
	}

	m_StateStack.top()->Enter();
}

void CStateManager::PopState()
{
	if (!m_StateStack.empty())
	{	
		m_StateStack.top()->Exit();
		m_StateStack.pop();
		m_StateStack.top()->Enter();
	}
}

void CStateManager::RunState(float elapsedTime)
{
	if (!m_StateStack.empty())
	{	
		m_StateStack.top()->Update(elapsedTime);
		m_StateStack.top()->Render();
	}
}

void CStateManager::Clear()
{
	while (!m_StateStack.empty())
	{
		m_StateStack.pop();
	}
}