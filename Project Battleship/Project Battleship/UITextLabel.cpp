#include "UITextLabel.h"
#include "StateManager.h"

CUITextLabel::CUITextLabel()
{
}


CUITextLabel::~CUITextLabel()
{
}

CUITextLabel::CUITextLabel(CRenderer* pRenderer, std::string text, DirectX::XMFLOAT2 pos, eLabelType eLabelType, eFont fType)
{
	
	updatedString = std::wstring(text.begin(), text.end());
	m_wText = &updatedString[0];
	m_eType = eLabelType;
	m_vPos = pos;
	m_bRendering = true;
	fontType = fType;
	m_pRenderer = pRenderer;
}

void CUITextLabel::Update(std::string text)
{
	updatedString = std::wstring(text.begin(), text.end());
	m_wText = &updatedString[0];
}

void CUITextLabel::Render()
{
	if (m_bRendering == true)
	{
		if (m_eType == labelStatic)
		m_pRenderer->GetFontRenderer(fontType)->DrawString(m_pRenderer->Get2DRenderer(), m_wText, m_vPos, DirectX::Colors::Black);
		else if (m_eType == labelPlayer1)
			m_pRenderer->GetFontRenderer(fontType)->DrawString(m_pRenderer->Get2DRenderer(), m_wText, m_vPos, DirectX::Colors::DarkBlue);
		else if (m_eType == labelPlayer2)
			m_pRenderer->GetFontRenderer(fontType)->DrawString(m_pRenderer->Get2DRenderer(), m_wText, m_vPos, DirectX::Colors::DarkRed);
	}
	
}