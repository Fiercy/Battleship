#include "UIButton.h"
#include "InputManager.h"
#include "Renderer.h"
#include "StateManager.h"

CUIButton::CUIButton()
{
}


CUIButton::~CUIButton()
{
}

CUIButton::CUIButton(CRenderer* pRender, eButtonType eType, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* hoverTexture,
	XMFLOAT2 fPos, XMFLOAT2 fSize)
{
	m_pButtonTextures[0] = texture;
	m_pButtonTextures[1] = hoverTexture;
	m_fPos = fPos;
	m_fSize = fSize;
	m_rButtonPos = { (LONG)m_fPos.x, (LONG)m_fPos.y, (LONG)m_fPos.x + (LONG)m_fSize.x, (LONG)m_fPos.y + (LONG)m_fSize.y };
	m_pRenderer = pRender;
	m_eType = eType;
	m_pRenderTexture = m_pButtonTextures[0];
}

void CUIButton::Render()
{
	m_pRenderer->Get2DRenderer()->Draw(m_pRenderTexture, m_rButtonPos);
}

void CUIButton::Update()
{
	CInputManager::tMouseInput mousePos = CInputManager::GetInstance()->GetMouseInput();
	if (mousePos.x > (unsigned int)m_rButtonPos.left &&
		mousePos.x < (unsigned int)m_rButtonPos.right
		&& mousePos.y < (unsigned int)m_rButtonPos.bottom
		&& mousePos.y >(unsigned int)m_rButtonPos.top)
	{	
		if (m_pRenderTexture != m_pButtonTextures[1])
		{
			m_pRenderTexture = m_pButtonTextures[1];
		}
		if (CInputManager::GetInstance()->GetKeyPress(0x01) == true)
		{
		   Execute();		
		}
	}
	else
	{
		m_pRenderTexture = m_pButtonTextures[0];
	}
}

void CUIButton::Execute()
{
	if (m_eType == buttonVSPlayer)
	{
		CStateManager::GetInstance()->PushState(1);
	}
}