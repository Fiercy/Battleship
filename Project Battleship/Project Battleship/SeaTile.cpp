#include "SeaTile.h"
#include "Renderer.h"
#include "StateManager.h"

CSeaTile::CSeaTile()
{
	m_pShip = nullptr;
}


CSeaTile::~CSeaTile()
{
}

CSeaTile::CSeaTile(CRenderer* pRenderer, ID3D11ShaderResourceView* texture, XMFLOAT2 pos, XMFLOAT2 size, int numTile)
{
	m_pRenderTexture = texture;
	m_fPos = pos;
	m_fSize = size;
	m_rTilePos = { (LONG)m_fPos.x, (LONG)m_fPos.y, (LONG)m_fPos.x + (LONG)m_fSize.x, (LONG)m_fPos.y + (LONG)m_fSize.y };
	nTileNum = numTile;
	m_pRenderer = pRenderer;
	eTileCurStatus = tileEmpty;
}

void CSeaTile::Update(eTileStatus curStatus)
{
	if (curStatus == tileEmpty)
	{
		m_pRenderTexture = CStateManager::GetInstance()->GetAssetManager()->getTexture("tileEmpty")->m_pTexture;
		eTileCurStatus = tileEmpty;
	}
	else if (curStatus == tileAdjecent)
	{
		m_pRenderTexture = CStateManager::GetInstance()->GetAssetManager()->getTexture("tileShipAdjacent")->m_pTexture;
		eTileCurStatus = tileAdjecent;
	}
	else if (curStatus == tileShipBlue)
	{
		m_pRenderTexture = CStateManager::GetInstance()->GetAssetManager()->getTexture("tileBlueShip")->m_pTexture;
		eTileCurStatus = tileShipBlue;
	}
	else if (curStatus == tileShipRed)
	{
		m_pRenderTexture = CStateManager::GetInstance()->GetAssetManager()->getTexture("tileRedShip")->m_pTexture;
		eTileCurStatus = tileShipRed;
	}
	else if (curStatus == tileHitShipBlue)
	{
		m_pRenderTexture = CStateManager::GetInstance()->GetAssetManager()->getTexture("tileBlueShipHit")->m_pTexture;
		eTileCurStatus = tileHitShipBlue;
	}
	else if (curStatus == tileHitShipRed)
	{
		m_pRenderTexture = CStateManager::GetInstance()->GetAssetManager()->getTexture("tileRedShipHit")->m_pTexture;
		eTileCurStatus = tileHitShipRed;
	}
	else if (curStatus == tileHitEmpty)
	{
		m_pRenderTexture = CStateManager::GetInstance()->GetAssetManager()->getTexture("tileHitEmpty")->m_pTexture;
		eTileCurStatus = tileHitEmpty;
	}
	else if (curStatus == tileShipAdjacentHit)
	{
		m_pRenderTexture = CStateManager::GetInstance()->GetAssetManager()->getTexture("tileShipAdjacentHit")->m_pTexture;
		eTileCurStatus = tileShipAdjacentHit;
	}
	else if (curStatus == tileShipDead)
	{
		m_pRenderTexture = CStateManager::GetInstance()->GetAssetManager()->getTexture("tileShipDead")->m_pTexture;
		eTileCurStatus = tileShipDead;
	}
}

bool CSeaTile::ShipPlacement(XMFLOAT2 mousePos,int numPlayer)
{
	if (mousePos.x > (unsigned int)m_rTilePos.left &&
		mousePos.x < (unsigned int)m_rTilePos.right
		&& mousePos.y < (unsigned int)m_rTilePos.bottom
		&& mousePos.y >(unsigned int)m_rTilePos.top)
	{
		if (eTileCurStatus == tileEmpty)
		{
			sndPlaySound("Assets/click.wav", SND_ASYNC);
			return true;
		}
	}
	//sndPlaySound("Assets/error.wav", SND_ASYNC);
	return false;

}

bool CSeaTile::ShipSearch(XMFLOAT2 mousePos)
{
	if (mousePos.x > (unsigned int)m_rTilePos.left &&
		mousePos.x < (unsigned int)m_rTilePos.right
		&& mousePos.y < (unsigned int)m_rTilePos.bottom
		&& mousePos.y >(unsigned int)m_rTilePos.top)
	{
		if (eTileCurStatus == tileEmpty)
		{
			return true;
		}
	}

	return false;
}

bool CSeaTile::CheckAdjecent(XMFLOAT2 mousePos)
{
	if (mousePos.x >= (unsigned int)m_rTilePos.left &&
		mousePos.x <= (unsigned int)m_rTilePos.right
		&& mousePos.y <= (unsigned int)m_rTilePos.bottom
		&& mousePos.y >= (unsigned int)m_rTilePos.top)
	{
		return true;
	}

	return false;
}


void CSeaTile::Render()
{
	m_pRenderer->Get2DRenderer()->Draw(m_pRenderTexture, m_rTilePos);
}