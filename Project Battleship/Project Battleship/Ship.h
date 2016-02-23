#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include "InputManager.h"
using namespace DirectX;
class CSeaTile;
class CRenderer;

class CShip
{
public:
	CShip();
	CShip(CRenderer* pRenderer, ID3D11ShaderResourceView* texture, XMFLOAT2 pos, XMFLOAT2 size, int nShipSize, int sShipOrientation, std::vector<CSeaTile*> playerTiles, std::vector<CSeaTile*> pvPlayerTargetTiles);
	~CShip();

	void Update();
	void Render();
	void PlaceShip();
	bool CheckDamage();
	void SetUpAdjecentTiles(int i, bool bAlive);
	void SetUpAdjecentTilesRotated(int i, bool bAlive);
	bool GetPlaced() { return m_bPlaced; }
	bool GetAlive() { return m_bAlive; }

private:
	//std::vector<CSeaTile> m_vShipPositionTiles;
	std::vector<CSeaTile*> m_pvPlayerTiles;
	std::vector<CSeaTile*> m_pvPlayerTargetTiles;
	ID3D11ShaderResourceView* m_pRenderTexture;
	bool m_bPlacing;
	bool m_bPlaced;
	bool m_bRotated;
	XMFLOAT2 m_fPos;
	XMFLOAT2 m_fStartingPos;
	XMFLOAT2 m_fSize;
	RECT  m_rShipPos;
	int  m_nShipSize;
	int  m_nPlayer;
	int  m_nStartingTile; 
	CRenderer* m_pRenderer;
	bool m_bAlive;
	CInputManager::tMouseInput mousePosSaved;
};

