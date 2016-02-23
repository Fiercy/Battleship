#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include <string>

class CRenderer;

enum eButtonType {buttonVSPlayer,buttonVSAI,buttonExit};

class CUIButton
{
public:
	CUIButton();
	CUIButton(CRenderer* pRender, eButtonType eType, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* hoverTexture,
		XMFLOAT2 fPos,XMFLOAT2 fSize);
	~CUIButton();

	void Execute();
	void Update();
	void Render();

private:
	ID3D11ShaderResourceView* m_pRenderTexture;
	ID3D11ShaderResourceView* m_pButtonTextures[2];
	XMFLOAT2 m_fPos;
	XMFLOAT2 m_fSize;
	RECT m_rButtonPos;
	CRenderer* m_pRenderer;
	eButtonType m_eType;

};

