#include "Ship.h"
#include "Renderer.h"
#include "SeaTile.h"
#include "StateManager.h"

CShip::CShip()
{
}

CShip::CShip(CRenderer* pRenderer, ID3D11ShaderResourceView* texture, XMFLOAT2 pos, XMFLOAT2 size, int nShipSize, int numPlayer, std::vector<CSeaTile*> pvPlayerTiles, std::vector<CSeaTile*> pvPlayerTargetTiles)
{
	m_pRenderTexture = texture;
	m_fPos = pos;
	m_fStartingPos = pos;
	m_fSize.x = size.x * nShipSize;
	m_fSize.y = size.y;
	m_rShipPos = { (LONG)m_fPos.x, (LONG)m_fPos.y, (LONG)m_fPos.x + (LONG)m_fSize.x, (LONG)m_fPos.y + (LONG)m_fSize.y };
	m_pRenderer = pRenderer;
	m_nShipSize = nShipSize;
	m_bPlacing = false;
	m_pvPlayerTiles = pvPlayerTiles;
	m_bPlaced = false;
	m_bRotated = false;
	m_nPlayer = numPlayer;
	m_nStartingTile = 0;
	m_pvPlayerTargetTiles = pvPlayerTargetTiles;
	m_bAlive = true;
	mousePosSaved = CInputManager::GetInstance()->GetMouseInput();
}


CShip::~CShip()
{
}

void CShip::Render()
{
	for (int i = 0; i < m_nShipSize; i++)
	{
		if (m_bPlaced == false)
		{
			if (m_bRotated == false)
			{
				m_pRenderer->Get2DRenderer()->Draw(m_pRenderTexture, XMFLOAT2(m_fPos.x + 32 * i, m_fPos.y));
			}
			else
			{
				m_pRenderer->Get2DRenderer()->Draw(m_pRenderTexture, XMFLOAT2(m_fPos.x, m_fPos.y + 32 * i));
			}
		}
	}
}

void CShip::Update()
{
	CInputManager::tMouseInput mousePos = CInputManager::GetInstance()->GetMouseInput();
	if (mousePos.x > (unsigned int)m_rShipPos.left &&
		mousePos.x < (unsigned int)m_rShipPos.right
		&& mousePos.y < (unsigned int)m_rShipPos.bottom
		&& mousePos.y >(unsigned int)m_rShipPos.top)
	{
		if (CInputManager::GetInstance()->GetKeyPress(0x01) == true && m_bPlaced == false)
		{
			m_bPlacing = true;
		}
	}

	if (m_bPlacing == true)
	{
		PlaceShip();
	}


}

void CShip::PlaceShip()
{
	CInputManager::tMouseInput mousePos = CInputManager::GetInstance()->GetMouseInput();

	m_fPos = XMFLOAT2((float)mousePos.x, (float)mousePos.y);
	m_rShipPos = { (LONG)m_fPos.x, (LONG)m_fPos.y, (LONG)m_fPos.x + (LONG)m_fSize.x, (LONG)m_fPos.y + (LONG)m_fSize.y };

	if (CInputManager::GetInstance()->GetKeyPress(VK_SPACE) == true && m_nShipSize > 1)
	{
		if (m_bRotated == false)
		{
			m_bRotated = true;
		}
		else
		{
			m_bRotated = false;
		}
	}

	if (CInputManager::GetInstance()->GetKeyPress(0x02) == true)
	{
		m_bPlacing = false;
		m_bPlaced = false;
		m_fPos = m_fStartingPos;
		m_rShipPos = { (LONG)m_fPos.x, (LONG)m_fPos.y, (LONG)m_fPos.x + (LONG)m_fSize.x, (LONG)m_fPos.y + (LONG)m_fSize.y };
		m_bRotated = false;
		return;
	}

	if (CInputManager::GetInstance()->GetKeyPress(0x01) == true)
	{
		//sndPlaySound("Assets/error.wav", SND_ASYNC);
		for (unsigned int i = 0; i < m_pvPlayerTiles.size(); i++)
		{
			if (m_pvPlayerTiles[i]->ShipPlacement(m_fPos, 0) == true)
			{	
				m_nStartingTile = i;

				if (m_bRotated == false)
				{
					if (m_nShipSize == 1)
					{
						m_bPlacing = false;
						m_bPlaced = true;
						SetUpAdjecentTiles(i, true);
					}
					else if (m_nShipSize == 2)
					{
						if (i % 10 + m_nShipSize <= 10 && m_pvPlayerTiles[i + 1]->GetTileStatus() == tileEmpty)
						{
							m_bPlacing = false;
							m_bPlaced = true;
							SetUpAdjecentTiles(i, true);
						}
					}
					else if (m_nShipSize == 3)
					{
						if (i % 10 + m_nShipSize <= 10 && m_pvPlayerTiles[i + 1]->GetTileStatus() == tileEmpty && m_pvPlayerTiles[i + 2]->GetTileStatus() == tileEmpty)
						{
							m_bPlacing = false;
							m_bPlaced = true;
							SetUpAdjecentTiles(i, true);
						}
					}
					else if (m_nShipSize == 4)
					{
						if (i % 10 + m_nShipSize <= 10 && m_pvPlayerTiles[i + 1]->GetTileStatus() == tileEmpty && m_pvPlayerTiles[i + 2]->GetTileStatus() == tileEmpty == true && m_pvPlayerTiles[i + 3]->GetTileStatus() == tileEmpty)
						{
							m_bPlacing = false;
							m_bPlaced = true;
							SetUpAdjecentTiles(i, true);
						}
					}
				}
				else
				{
					if (i / 10 + m_nShipSize <= 10 && m_nShipSize == 2)
					{
						if (m_pvPlayerTiles[i + 10]->GetTileStatus() == tileEmpty)
						{
							m_bPlacing = false;
							m_bPlaced = true;
							SetUpAdjecentTilesRotated(i, true);
						}
					}
					else if (i / 10 + m_nShipSize <= 10 && m_nShipSize == 3)
					{
						if (m_pvPlayerTiles[i + 10]->GetTileStatus() == tileEmpty && m_pvPlayerTiles[i + 20]->GetTileStatus() == tileEmpty)
						{
							m_bPlacing = false;
							m_bPlaced = true;
							SetUpAdjecentTilesRotated(i, true);
						}
					}
					else if (i / 10 + m_nShipSize <= 10 && m_nShipSize == 4)
					{
						if (m_pvPlayerTiles[i + 10]->GetTileStatus() == tileEmpty && m_pvPlayerTiles[i + 20]->GetTileStatus() == tileEmpty && m_pvPlayerTiles[i + 30]->GetTileStatus() == tileEmpty)
						{
							m_bPlacing = false;
							m_bPlaced = true;
							SetUpAdjecentTilesRotated(i, true);
						}
					}
				}
			}
		}


	}
}

bool CShip::CheckDamage()
{
	if (m_bRotated == false)
	{
		for (int z = 0; z < m_nShipSize; z++)
		{
			if (m_nPlayer == 0)
			{
				if (m_pvPlayerTiles[m_nStartingTile + z]->GetTileStatus() != tileHitShipBlue)
					return false;
			}
			else if (m_nPlayer == 1)
			{
				if (m_pvPlayerTiles[m_nStartingTile + z]->GetTileStatus() != tileHitShipRed)
					return false;
			}
		}
	}
	else
	{
		for (int z = 0; z < m_nShipSize * 10; z += 10)
		{
			if (m_nPlayer == 0)
			{
				if (m_pvPlayerTiles[m_nStartingTile + z]->GetTileStatus() != tileHitShipBlue)
					return false;
			}
			else if (m_nPlayer == 1)
			{
				if (m_pvPlayerTiles[m_nStartingTile + z]->GetTileStatus() != tileHitShipRed)
					return false;
			}
		}
	}

	if (m_bRotated == false)
	{
		if (m_nShipSize == 1)
		{
			sndPlaySound("Assets/cruserDown.wav", 1);
		}
		if (m_nShipSize == 2)
		{
			sndPlaySound("Assets/destroyerDown.wav", 1);
		}
		if (m_nShipSize == 3)
		{
			sndPlaySound("Assets/battleshipDown.wav", 1);
		}
		if (m_nShipSize == 4)
		{
			sndPlaySound("Assets/carrierDown.wav", 1);
		}

		for (int z = 0; z < m_nShipSize; z++)
		{
			m_pvPlayerTiles[m_nStartingTile + z]->Update(tileShipDead);
			m_pvPlayerTargetTiles[m_nStartingTile + z]->Update(tileShipDead);
			SetUpAdjecentTiles(m_nStartingTile, false);
		}
	}
	else
	{
		if (m_nShipSize == 1)
		{
			sndPlaySound("Assets/cruserDown.wav", 1);
		}
		if (m_nShipSize == 2)
		{
			sndPlaySound("Assets/destroyerDown.wav", 1);
		}
		if (m_nShipSize == 3)
		{
			sndPlaySound("Assets/battleshipDown.wav", 1);
		}
		if (m_nShipSize == 4)
		{
			sndPlaySound("Assets/carrierDown.wav", 1);
		}

		for (int z = 0; z < m_nShipSize * 10; z += 10)
		{
			m_pvPlayerTiles[m_nStartingTile + z]->Update(tileShipDead);
			m_pvPlayerTargetTiles[m_nStartingTile + z]->Update(tileShipDead);
			SetUpAdjecentTilesRotated(m_nStartingTile, false);
		}
	}

	m_bAlive = false;

	return true;
}


void CShip::SetUpAdjecentTiles(int i, bool bAlive)
{
	CInputManager::tMouseInput mousePos = CInputManager::GetInstance()->GetMouseInput();
	if (bAlive == true)
		mousePosSaved = mousePos;

	if (bAlive == false)
	{
		mousePos = mousePosSaved;
	}

	if (i - 1 >= 0)
	{
		if (m_pvPlayerTiles[i - 1]->CheckAdjecent(XMFLOAT2((float)mousePos.x - 32, (float)mousePos.y)) == true)
		{
			if (bAlive == true)
				m_pvPlayerTiles[i - 1]->Update(tileAdjecent);
			else
			{
				m_pvPlayerTiles[i - 1]->Update(tileShipAdjacentHit);
				m_pvPlayerTargetTiles[i - 1]->Update(tileShipAdjacentHit);
			}

		}
	}
	if (i - 10 >= 0)
	{

		if (m_pvPlayerTiles[i - 10]->CheckAdjecent(XMFLOAT2((float)mousePos.x, (float)mousePos.y - 32)) == true)
		{
			if (bAlive == true)
				m_pvPlayerTiles[i - 10]->Update(tileAdjecent);
			else
			{
				m_pvPlayerTiles[i - 10]->Update(tileShipAdjacentHit);
				m_pvPlayerTargetTiles[i - 10]->Update(tileShipAdjacentHit);
			}

		}
	}
	if (i - 11 >= 0)
	{
		if (m_pvPlayerTiles[i - 11]->CheckAdjecent(XMFLOAT2((float)mousePos.x - 32, (float)mousePos.y - 32)) == true)
		{
			if (bAlive == true)
				m_pvPlayerTiles[i - 11]->Update(tileAdjecent);
			else
			{
				m_pvPlayerTiles[i - 11]->Update(tileShipAdjacentHit);
				m_pvPlayerTargetTiles[i - 11]->Update(tileShipAdjacentHit);
			}

		}
	}
	if (i - 9 >= 0)
	{
		if (m_pvPlayerTiles[i - 9]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 32, (float)mousePos.y - 32)) == true)
		{
			if (bAlive == true)
				m_pvPlayerTiles[i - 9]->Update(tileAdjecent);
			else
			{
				m_pvPlayerTiles[i - 9]->Update(tileShipAdjacentHit);
				m_pvPlayerTargetTiles[i - 9]->Update(tileShipAdjacentHit);
			}

		}
	}
	if (i + 9 <= 99)
	{
		if (m_pvPlayerTiles[i + 9]->CheckAdjecent(XMFLOAT2((float)mousePos.x - 32, (float)mousePos.y + 32)) == true)
		{
			if (bAlive == true)
				m_pvPlayerTiles[i + 9]->Update(tileAdjecent);
			else
			{
				m_pvPlayerTiles[i + 9]->Update(tileShipAdjacentHit);
				m_pvPlayerTargetTiles[i + 9]->Update(tileShipAdjacentHit);
			}
		}
	}
	if (i + 10 <= 99)
	{
		if (m_pvPlayerTiles[i + 10]->CheckAdjecent(XMFLOAT2((float)mousePos.x, (float)mousePos.y + 32)) == true)
		{
			if (bAlive == true)
				m_pvPlayerTiles[i + 10]->Update(tileAdjecent);
			else
			{
				m_pvPlayerTiles[i + 10]->Update(tileShipAdjacentHit);
				m_pvPlayerTargetTiles[i + 10]->Update(tileShipAdjacentHit);
			}
		}
	}
	if (i + 11 <= 99)
	{
		if (m_pvPlayerTiles[i + 11]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 32, (float)mousePos.y + 32)) == true)
		{
			if (bAlive == true)
				m_pvPlayerTiles[i + 11]->Update(tileAdjecent);
			else
			{
				m_pvPlayerTiles[i + 11]->Update(tileShipAdjacentHit);
				m_pvPlayerTargetTiles[i + 11]->Update(tileShipAdjacentHit);
			}
		}
	}

	if (m_nShipSize == 1)
	{
		if (i + 1 <= 99)
		{
			if (m_pvPlayerTiles[i + 1]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 32, (float)mousePos.y)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i + 1]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 1]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 1]->Update(tileShipAdjacentHit);
				}

			}
		}
	}
	else if (m_nShipSize == 2)
	{
		if (i + 2 <= 99)
		{
			if (m_pvPlayerTiles[i + 2]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 64, (float)mousePos.y)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i + 2]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 2]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 2]->Update(tileShipAdjacentHit);
				}
			}
		}
		if (i + 12 <= 99)
		{
			if (m_pvPlayerTiles[i + 12]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 64, (float)mousePos.y + 32)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i + 12]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 12]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 12]->Update(tileShipAdjacentHit);
				}

			}
		}
		if (i - 8 >= 0)
		{
			if (m_pvPlayerTiles[i - 8]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 64, (float)mousePos.y - 32)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i - 8]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i - 8]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i - 8]->Update(tileShipAdjacentHit);
				}

			}
		}
	}
	else if (m_nShipSize == 3)
	{
		if (i + 12 <= 99)
		{
			if (m_pvPlayerTiles[i + 12]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 64, (float)mousePos.y + 32)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i + 12]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 12]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 12]->Update(tileShipAdjacentHit);
				}

			}
		}
		if (i - 8 >= 0)
		{
			if (m_pvPlayerTiles[i - 8]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 64, (float)mousePos.y - 32)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i - 8]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i - 8]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i - 8]->Update(tileShipAdjacentHit);
				}

			}
		}
		if (i + 3 <= 99)
		{
			if (m_pvPlayerTiles[i + 3]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 96, (float)mousePos.y)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i + 3]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 3]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 3]->Update(tileShipAdjacentHit);
				}

			}
		}
		if (i + 13 <= 99)
		{
			if (m_pvPlayerTiles[i + 13]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 96, (float)mousePos.y + 32)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i + 13]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 13]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 13]->Update(tileShipAdjacentHit);
				}

			}
		}
		if (i - 7 >= 0)
		{
			if (m_pvPlayerTiles[i - 7]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 96, (float)mousePos.y - 32)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i - 7]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i - 7]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i - 7]->Update(tileShipAdjacentHit);
				}

			}
		}
	}
	else if (m_nShipSize == 4)
	{
		if (i + 12 <= 99)
		{
			if (m_pvPlayerTiles[i + 12]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 64, (float)mousePos.y + 32)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i + 12]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 12]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 12]->Update(tileShipAdjacentHit);
				}

			}
		}
		if (i - 8 >= 0)
		{
			if (m_pvPlayerTiles[i - 8]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 64, (float)mousePos.y - 32)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i - 8]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i - 8]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i - 8]->Update(tileShipAdjacentHit);
				}

			}
		}
		if (i + 13 <= 99)
		{
			if (m_pvPlayerTiles[i + 13]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 96, (float)mousePos.y + 32)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i + 13]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 13]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 13]->Update(tileShipAdjacentHit);
				}

			}
		}
		if (i - 7 >= 0)
		{
			if (m_pvPlayerTiles[i - 7]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 96, (float)mousePos.y - 32)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i - 7]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i - 7]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i - 7]->Update(tileShipAdjacentHit);
				}
			}
		}
		if (i + 4 <= 99)
		{
			if (m_pvPlayerTiles[i + 4]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 128, (float)mousePos.y)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i + 4]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 4]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 4]->Update(tileShipAdjacentHit);
				}

			}
		}
		if (i + 14 <= 99)
		{
			if (m_pvPlayerTiles[i + 14]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 128, (float)mousePos.y + 32)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i + 14]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 14]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 14]->Update(tileShipAdjacentHit);
				}

			}
		}
		if (i - 6 >= 0)
		{
			if (m_pvPlayerTiles[i - 6]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 128, (float)mousePos.y - 32)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i - 6]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i - 6]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i - 6]->Update(tileShipAdjacentHit);
				}

			}
		}
	}

	if (m_nShipSize >= 0 && bAlive == true)
	{
		for (int z = 0; z < m_nShipSize; z++)
		{
			m_pvPlayerTiles[i + z]->SetShip(this);

			if (m_nPlayer == 0)
				m_pvPlayerTiles[i + z]->Update(tileShipBlue);
			else if (m_nPlayer == 1)
				m_pvPlayerTiles[i + z]->Update(tileShipRed);
		}
	}
}


void CShip::SetUpAdjecentTilesRotated(int i, bool bAlive)
{
	CInputManager::tMouseInput mousePos = CInputManager::GetInstance()->GetMouseInput();
	if (bAlive == true)
		mousePosSaved = mousePos;

	if (bAlive == false)
	{
		mousePos = mousePosSaved;
	}
	 
	if (i - 1 >= 0)
	{
		if (m_pvPlayerTiles[i - 1]->CheckAdjecent(XMFLOAT2((float)mousePos.x - 32, (float)mousePos.y)) == true)
		{
			if (bAlive == true)
			m_pvPlayerTiles[i - 1]->Update(tileAdjecent);
			else
			{
				m_pvPlayerTiles[i - 1]->Update(tileAdjecent);
				m_pvPlayerTargetTiles[i - 1]->Update(tileShipAdjacentHit);
			}
		}
	}
	if (i - 10 >= 0)
	{

		if (m_pvPlayerTiles[i - 10]->CheckAdjecent(XMFLOAT2((float)mousePos.x, (float)mousePos.y - 32)) == true)
		{
			if (bAlive == true)
			m_pvPlayerTiles[i - 10]->Update(tileAdjecent);
			else
			{
				m_pvPlayerTiles[i - 10]->Update(tileShipAdjacentHit);
				m_pvPlayerTargetTiles[i - 10]->Update(tileShipAdjacentHit);
			}
		}
	}
	if (i - 11 >= 0)
	{
		if (m_pvPlayerTiles[i - 11]->CheckAdjecent(XMFLOAT2((float)mousePos.x - 32, (float)mousePos.y - 32)) == true)
		{
			if (bAlive == true)
			m_pvPlayerTiles[i - 11]->Update(tileAdjecent);
			else
			{
				m_pvPlayerTiles[i - 11]->Update(tileShipAdjacentHit);
				m_pvPlayerTargetTiles[i - 11]->Update(tileShipAdjacentHit);
			}
		}
	}
	if (i - 9 >= 0)
	{
		if (m_pvPlayerTiles[i - 9]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 32, (float)mousePos.y - 32)) == true)
		{
			if (bAlive == true)
			m_pvPlayerTiles[i - 9]->Update(tileAdjecent);
			else
			{
				m_pvPlayerTiles[i - 9]->Update(tileShipAdjacentHit);
				m_pvPlayerTargetTiles[i - 9]->Update(tileShipAdjacentHit);
			}
		}
	}
	if (i + 9 <= 99)
	{
		if (m_pvPlayerTiles[i + 9]->CheckAdjecent(XMFLOAT2((float)mousePos.x - 32, (float)mousePos.y + 32)) == true)
		{
			if (bAlive == true)
			m_pvPlayerTiles[i + 9]->Update(tileAdjecent);
			else
			{
				m_pvPlayerTiles[i + 9]->Update(tileShipAdjacentHit);
				m_pvPlayerTargetTiles[i + 9]->Update(tileShipAdjacentHit);
			}
		}
	}
	if (i + 1 <= 99)
	{
		if (m_pvPlayerTiles[i + 1]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 32, (float)mousePos.y)) == true)
		{
			if (bAlive == true)
			m_pvPlayerTiles[i + 1]->Update(tileAdjecent);
			else
			{
				m_pvPlayerTiles[i + 1]->Update(tileShipAdjacentHit);
				m_pvPlayerTargetTiles[i + 1]->Update(tileShipAdjacentHit);
			}
		}
	}
	if (i + 11 <= 99)
	{
		if (m_pvPlayerTiles[i + 11]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 32, (float)mousePos.y + 32)) == true)
		{
			if (bAlive == true)
			m_pvPlayerTiles[i + 11]->Update(tileAdjecent);
			else
			{
				m_pvPlayerTiles[i + 11]->Update(tileShipAdjacentHit);
				m_pvPlayerTargetTiles[i + 11]->Update(tileShipAdjacentHit);
			}
		}
	}

	if (m_nShipSize == 2)
	{
		if (i + 20 <= 99)
		{
			if (m_pvPlayerTiles[i + 20]->CheckAdjecent(XMFLOAT2((float)mousePos.x, (float)mousePos.y + 64)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i + 20]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 20]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 20]->Update(tileShipAdjacentHit);
				}
			}
		}
		if (i + 21 <= 99)
		{
			if (m_pvPlayerTiles[i + 21]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 32, (float)mousePos.y + 64)) == true)
			{
				if (bAlive == true)
				m_pvPlayerTiles[i + 21]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 21]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 21]->Update(tileShipAdjacentHit);
				}
			}
		}
		if (i + 19 <= 99)
		{
			if (m_pvPlayerTiles[i + 19]->CheckAdjecent(XMFLOAT2((float)mousePos.x - 32, (float)mousePos.y + 64)) == true)
			{
				if (bAlive == true)
				m_pvPlayerTiles[i + 19]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 19]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 19]->Update(tileShipAdjacentHit);
				}
			}
		}
	}
	else if (m_nShipSize == 3)
	{
		if (i + 21 <= 99)
		{
			if (m_pvPlayerTiles[i + 21]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 32, (float)mousePos.y + 64)) == true)
			{
				if (bAlive == true)
				m_pvPlayerTiles[i + 21]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 21]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 21]->Update(tileShipAdjacentHit);
				}
			}
		}
		if (i + 19 >= 0)
		{
			if (m_pvPlayerTiles[i + 19]->CheckAdjecent(XMFLOAT2((float)mousePos.x - 32, (float)mousePos.y + 64)) == true)
			{
				if (bAlive == true)
				m_pvPlayerTiles[i + 19]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 19]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 19]->Update(tileShipAdjacentHit);
				}
			}
		}
		if (i + 31 <= 99)
		{
			if (m_pvPlayerTiles[i + 31]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 32, (float)mousePos.y + 96)) == true)
			{
				if (bAlive == true)
				m_pvPlayerTiles[i + 31]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 31]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 31]->Update(tileShipAdjacentHit);
				}
			}
		}
		if (i + 29 <= 99)
		{
			if (m_pvPlayerTiles[i + 29]->CheckAdjecent(XMFLOAT2((float)mousePos.x - 32, (float)mousePos.y + 96)) == true)
			{
				if (bAlive == true)
					m_pvPlayerTiles[i + 29]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 29]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 29]->Update(tileShipAdjacentHit);
				}
			}
		}
		if (i + 30 <= 99)
		{
			if (m_pvPlayerTiles[i + 30]->CheckAdjecent(XMFLOAT2((float)mousePos.x, (float)mousePos.y + 96)) == true)
			{
				if (bAlive == true)
				m_pvPlayerTiles[i + 30]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 30]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 30]->Update(tileShipAdjacentHit);
				}
			}
		}
	}
	else if (m_nShipSize == 4)
	{
		if (i + 21 <= 99)
		{
			if (m_pvPlayerTiles[i + 21]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 32, (float)mousePos.y + 64)) == true)
			{
				if (bAlive == true)
				m_pvPlayerTiles[i + 21]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 21]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 21]->Update(tileShipAdjacentHit);
				}
			}
		}
		if (i + 19 <= 99)
		{
			if (m_pvPlayerTiles[i + 19]->CheckAdjecent(XMFLOAT2((float)mousePos.x - 32, (float)mousePos.y + 64)) == true)
			{
				if (bAlive == true)
				m_pvPlayerTiles[i + 19]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 19]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 19]->Update(tileShipAdjacentHit);
				}
			}
		}
		if (i + 31 <= 99)
		{
			if (m_pvPlayerTiles[i + 31]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 32, (float)mousePos.y + 96)) == true)
			{
				if (bAlive == true)
				m_pvPlayerTiles[i + 31]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 31]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 31]->Update(tileShipAdjacentHit);
				}
			}
		}
		if (i + 29 <= 99)
		{
			if (m_pvPlayerTiles[i + 29]->CheckAdjecent(XMFLOAT2((float)mousePos.x - 32, (float)mousePos.y + 96)) == true)
			{
				if (bAlive == true)
				m_pvPlayerTiles[i + 29]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 29]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 29]->Update(tileShipAdjacentHit);
				}
			}
		}
		if (i + 41 <= 99)
		{
			if (m_pvPlayerTiles[i + 41]->CheckAdjecent(XMFLOAT2((float)mousePos.x + 32, (float)mousePos.y + 128)) == true)
			{
				if (bAlive == true)
				m_pvPlayerTiles[i + 41]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 41]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 41]->Update(tileShipAdjacentHit);
				}
			}
		}
		if (i + 39 <= 99)
		{
			if (m_pvPlayerTiles[i + 39]->CheckAdjecent(XMFLOAT2((float)mousePos.x - 32, (float)mousePos.y + 128)) == true)
			{
				if (bAlive == true)
				m_pvPlayerTiles[i + 39]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 39]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 39]->Update(tileShipAdjacentHit);
				}
			}
		}
		if (i + 40 <= 99)
		{
			if (m_pvPlayerTiles[i + 40]->CheckAdjecent(XMFLOAT2((float)mousePos.x, (float)mousePos.y + 128)) == true)
			{
				if (bAlive == true)
				m_pvPlayerTiles[i + 40]->Update(tileAdjecent);
				else
				{
					m_pvPlayerTiles[i + 40]->Update(tileShipAdjacentHit);
					m_pvPlayerTargetTiles[i + 40]->Update(tileShipAdjacentHit);
				}
			}
		}
	}

	if (m_nShipSize >= 0 && bAlive == true)
	{
		for (int z = 0; z < m_nShipSize * 10; z += 10)
		{
			if (m_nPlayer == 0)
				m_pvPlayerTiles[i + z]->Update(tileShipBlue);
			else if (m_nPlayer == 1)
				m_pvPlayerTiles[i + z]->Update(tileShipRed);

			m_pvPlayerTiles[i + z]->SetShip(this);
		}

	}
}