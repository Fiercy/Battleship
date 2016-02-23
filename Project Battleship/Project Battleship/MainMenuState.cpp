#include "MainMenuState.h"
#include "StateManager.h"
#include "UIButton.h"
#include "UITextLabel.h"

CMainMenuState::CMainMenuState()
{
	m_pRenderer = CStateManager::GetInstance()->GetRenderer();
	m_pAssetManager = CStateManager::GetInstance()->GetAssetManager();

	CUIButton* vsPlayerButton = new CUIButton(m_pRenderer, buttonVSPlayer, m_pAssetManager->getTexture("testButton")->m_pTexture, m_pAssetManager->getTexture("testButtonHover")->m_pTexture,
		XMFLOAT2(295, 400), XMFLOAT2(118, 30));

	m_vMenuUIButtons.push_back(vsPlayerButton);


	CUITextLabel* labelmainName = new CUITextLabel(m_pRenderer, "Simple Battleship", XMFLOAT2(285, 50), labelStatic, arial9);
	CUITextLabel* labelRules = new CUITextLabel(m_pRenderer, "Rules:\nCruser: 1 Tile 4 ships\nDestroyer: 2 Tiles 3 Ships\nBattleship: 3 Tiles 2 Ships\nCarrier: 4 Tiles 1 Ship\nNo ship can be placed adjacently to another\nAfter successful hit player can shoot again until he misses\nClick Start to begin"
		, XMFLOAT2(32, 159), labelStatic, arial9);

	m_vMainMenuLabels.push_back(labelmainName);
	m_vMainMenuLabels.push_back(labelRules);
}


CMainMenuState::~CMainMenuState()
{
	for (unsigned int i = 0; i < m_vMenuUIButtons.size(); i++)
	{
		delete m_vMenuUIButtons[i];
	}

	for (unsigned int i = 0; i < m_vMainMenuLabels.size(); i++)
	{
		delete m_vMainMenuLabels[i];
	}
}


void CMainMenuState::Enter()
{
	int test = 5;
}

void CMainMenuState::Exit()
{
	
}

void CMainMenuState::Update(float elapsedTime)
{
	for (unsigned int i = 0; i < m_vMenuUIButtons.size(); i++)
	{
		m_vMenuUIButtons[i]->Update();
	}
}

void CMainMenuState::Render()
{
	m_pRenderer->PreRenderInit();

	CommonStates states(m_pRenderer->GetDevice());
	m_pRenderer->Get2DRenderer()->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	for (unsigned int i = 0; i < m_vMenuUIButtons.size(); i++)
	{
		m_vMenuUIButtons[i]->Render();
	}

	for (unsigned int i = 0; i < m_vMainMenuLabels.size(); i++)
	{
		m_vMainMenuLabels[i]->Render();
	}

	m_pRenderer->Get2DRenderer()->End();
	m_pRenderer->GetSwapChain()->Present(0, 0);
}
