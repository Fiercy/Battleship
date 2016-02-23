#pragma once
#include "Renderer.h"

enum eLabelType{ labelStatic, labelPlayer1, labelPlayer2 };

class CUITextLabel
{
public:
	CUITextLabel();
	~CUITextLabel();
	CUITextLabel(CRenderer* pRenderer,std::string text, DirectX::XMFLOAT2 pos, eLabelType eLabelType, eFont fType);

	void Render();
	void Update(std::string text);

	void SetRendering(bool bInput) { m_bRendering = bInput; }

	wchar_t* m_wText;
	std::wstring updatedString;
	eLabelType m_eType;
	DirectX::XMFLOAT2 m_vPos;
	int m_nPlayerNum;
	bool m_bRendering;
	eFont fontType;
	CRenderer* m_pRenderer;
};

