#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include <string>
enum eTileStatus { tileEmpty, tileAdjecent, tileShipBlue, tileShipRed, tileHitShipBlue, tileHitShipRed, tileHitEmpty, tileShipAdjacentHit,tileShipDead };

class CRenderer;
class CShip;

class CSeaTile
{
public:
	CSeaTile();
	CSeaTile(CRenderer* pRenderer, ID3D11ShaderResourceView* texture, XMFLOAT2 pos, XMFLOAT2 size, int numTile);
	~CSeaTile();

	eTileStatus GetTileStatus() { return eTileCurStatus; }
	ID3D11ShaderResourceView* GetRenderTexture() { return m_pRenderTexture; }
	RECT GetRectPos() { return m_rTilePos; }

	void Update(eTileStatus curStatus);
	void Render();
	void Fire(XMFLOAT2 mousePos);
	bool ShipPlacement(XMFLOAT2 mousePos, int numPlayer);
	bool ShipSearch(XMFLOAT2 mousePos);
	bool CheckAdjecent(XMFLOAT2 mousePos);
	int GetTileNumber() { return nTileNum; }

	void SetShip(CShip* pShip) { m_pShip = pShip; }
	CShip* GetShip() { return m_pShip; }

private:
	eTileStatus eTileCurStatus;
	ID3D11ShaderResourceView* m_pRenderTexture;
	XMFLOAT2 m_fPos;
	XMFLOAT2 m_fSize;
	RECT m_rTilePos;
	int  nTileNum;
	CRenderer* m_pRenderer;
	CShip*     m_pShip;
};

