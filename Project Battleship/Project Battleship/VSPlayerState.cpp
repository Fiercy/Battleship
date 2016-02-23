#include "VSPlayerState.h"
#include "StateManager.h"
#include "Ship.h"
#include "SeaTile.h"
#include "InputManager.h"
#include "UITextLabel.h"


CVSPlayerState::CVSPlayerState()
{
	m_pRenderer = CStateManager::GetInstance()->GetRenderer();
	m_pAssetManager = CStateManager::GetInstance()->GetAssetManager();
	m_bPlayer1Placing = true;
	m_bPlayer2Placing = false;
	m_bPlayer1Turn = true;
	m_bGame = false;
	m_bGameOver = false;
	m_nPlayer1Health = 20;
	m_nPlayer2Health = 20;
}


CVSPlayerState::~CVSPlayerState()
{
	for ( unsigned int i = 0; i < seaTilesPlayer1.size(); i++)
	{
		delete seaTilesPlayer1[i];
	}
	for (unsigned int i = 0; i < targetSeaTilesPlayer1.size(); i++)
	{
		delete targetSeaTilesPlayer1[i];
	}
	for (unsigned int i = 0; i < seaTilesPlayer2.size(); i++)
	{
		delete seaTilesPlayer2[i];
	}
	for (unsigned int i = 0; i < targetSeaTilesPlayer2.size(); i++)
	{
		delete targetSeaTilesPlayer2[i];
	}
	for (unsigned int i = 0; i < shipsPlayer1.size(); i++)
	{
		delete shipsPlayer1[i];
	}
	for (unsigned int i = 0; i < shipsPlayer2.size(); i++)
	{
		delete shipsPlayer2[i];
	}

	for (unsigned int i = 0; i < textInfo.size(); i++)
	{
		delete textInfo[i];
	}
}


void CVSPlayerState::Enter()
{
	InitPlayer1(); 
	InitPlayer2();

	std::string hotToPlay = "How to Play: \nClick on ships to start placing them\nRight click while selected to deselect\nHit spacebar while selected to rotate";
	CUITextLabel* labelHowToPlay = new CUITextLabel(m_pRenderer, hotToPlay,XMFLOAT2(380,400), labelStatic, arial9);

	CUITextLabel* labelBlueShipsLeft = new CUITextLabel(m_pRenderer, "", XMFLOAT2(32, 400), labelPlayer1, arial9);
	CUITextLabel* labelRedShipsKilled = new CUITextLabel(m_pRenderer, "", XMFLOAT2(32, 430), labelPlayer1, arial9);
	CUITextLabel* labelBlueTurn = new CUITextLabel(m_pRenderer, "Blue Turn", XMFLOAT2(320, 550), labelPlayer1, arial9);

	CUITextLabel* labelRedShipsLeft = new CUITextLabel(m_pRenderer, "", XMFLOAT2(32, 400), labelPlayer2, arial9);
	CUITextLabel* labelBlueShipsKilled = new CUITextLabel(m_pRenderer, "", XMFLOAT2(32, 430), labelPlayer2, arial9);
	CUITextLabel* labelRedTurn = new CUITextLabel(m_pRenderer, "Red Turn", XMFLOAT2(320, 550), labelPlayer2, arial9);

	CUITextLabel* howToPlayInGame = new CUITextLabel(m_pRenderer, "How to play:\nClick on a tile to fire", XMFLOAT2(380, 400), labelStatic, arial9);
	howToPlayInGame->SetRendering(false);

	CUITextLabel* labelRedWin = new CUITextLabel(m_pRenderer, "Red Won", XMFLOAT2(320, 550), labelPlayer2, arial9);
	labelRedWin->SetRendering(false);
	CUITextLabel* labelBlueWin = new CUITextLabel(m_pRenderer, "Blue Won", XMFLOAT2(320, 550), labelPlayer1, arial9);
	labelBlueWin->SetRendering(false);

	textInfo.push_back(labelHowToPlay);
	textInfo.push_back(labelBlueShipsLeft);
	textInfo.push_back(labelRedShipsKilled);
	textInfo.push_back(labelBlueTurn);

	textInfo.push_back(labelRedShipsLeft);
	textInfo.push_back(labelBlueShipsKilled);
	textInfo.push_back(labelRedTurn);
	textInfo.push_back(howToPlayInGame);
	textInfo.push_back(labelBlueWin);
	textInfo.push_back(labelRedWin);


}

void CVSPlayerState::RenderPlayer1View()
{
	for (unsigned int i = 0; i < seaTilesPlayer1.size(); i++)
	{
		seaTilesPlayer1[i]->Render();
	}

	for (unsigned int i = 0; i < targetSeaTilesPlayer1.size(); i++)
	{
		targetSeaTilesPlayer1[i]->Render();
	}

	if (m_bGame == false)
	{
		for (unsigned int i = 0; i < shipsPlayer1.size(); i++)
		{
			shipsPlayer1[i]->Render();
		}
	}	

}
void CVSPlayerState::RenderPlayer2View()
{
	for (unsigned int i = 0; i < seaTilesPlayer2.size(); i++)
	{
		seaTilesPlayer2[i]->Render();
	}

	for (unsigned int i = 0; i < targetSeaTilesPlayer2.size(); i++)
	{
		targetSeaTilesPlayer2[i]->Render();
	}

	if (m_bGame == false)
	{
		for (unsigned int i = 0; i < shipsPlayer2.size(); i++)
		{
			shipsPlayer2[i]->Render();
		}
	}
}

void CVSPlayerState::InitPlayer1()
{
	seaTilesPlayer1.empty();

	float nMainStartPosX;
	float nMainStartPosY;

	for (int i = 0; i < 100; i++)
	{
		if (i == 0)
		{
			nMainStartPosX = 32;
			nMainStartPosY = 50;
		}
		else if (i % 10 == 0)
		{
			nMainStartPosX = 32;
			nMainStartPosY += 32;
		}
		else
		{
			nMainStartPosX += 32;
		}

		CSeaTile* seaTile = new CSeaTile(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("tileEmpty")->m_pTexture, XMFLOAT2(nMainStartPosX, nMainStartPosY), XMFLOAT2(32.0f, 32.0f), i);
		seaTilesPlayer1.push_back(seaTile);
	}


	for (int i = 0; i < 100; i++)
	{
		if (i == 0)
		{
			nMainStartPosX = 64 + 320;
			nMainStartPosY = 50;
		}
		else if (i % 10 == 0)
		{
			nMainStartPosX = 64 + 320;
			nMainStartPosY += 32;
		}
		else
		{
			nMainStartPosX += 32;
		}

		CSeaTile* seaTile = new CSeaTile(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("tileEmpty")->m_pTexture, XMFLOAT2(nMainStartPosX, nMainStartPosY), XMFLOAT2(32, 32), i);
		targetSeaTilesPlayer1.push_back(seaTile);
	}

	for (int i = 0; i < 100; i++)
	{
		if (i == 0)
		{
			nMainStartPosX = 64 + 320;
			nMainStartPosY = 50;
		}
		else if (i % 10 == 0)
		{
			nMainStartPosX = 64 + 320;
			nMainStartPosY += 32;
		}
		else
		{
			nMainStartPosX += 32;
		}

		CSeaTile* seaTile = new CSeaTile(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("tileEmpty")->m_pTexture, XMFLOAT2(nMainStartPosX, nMainStartPosY), XMFLOAT2(32, 32), i);
		targetSeaTilesPlayer2.push_back(seaTile);
	}

	CShip* blueSubmarine1 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("blueShip")->m_pTexture, XMFLOAT2(32, 400), XMFLOAT2(32, 32), 1, 0, seaTilesPlayer1, targetSeaTilesPlayer2);
	CShip* blueSubmarine2 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("blueShip")->m_pTexture, XMFLOAT2(64 + 10, 400), XMFLOAT2(32, 32), 1, 0, seaTilesPlayer1, targetSeaTilesPlayer2);
	CShip* blueSubmarine3 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("blueShip")->m_pTexture, XMFLOAT2(96 + 20, 400), XMFLOAT2(32, 32), 1, 0, seaTilesPlayer1, targetSeaTilesPlayer2);
	CShip* blueSubmarine4 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("blueShip")->m_pTexture, XMFLOAT2(128 + 30, 400), XMFLOAT2(32, 32), 1, 0, seaTilesPlayer1, targetSeaTilesPlayer2);
	CShip* blueDestroyer1 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("blueShip")->m_pTexture, XMFLOAT2(32, 440), XMFLOAT2(32, 32), 2, 0, seaTilesPlayer1, targetSeaTilesPlayer2);
	CShip* blueDestroyer2 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("blueShip")->m_pTexture, XMFLOAT2(106, 440), XMFLOAT2(32, 32), 2, 0, seaTilesPlayer1, targetSeaTilesPlayer2);
	CShip* blueDestroyer3 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("blueShip")->m_pTexture, XMFLOAT2(180, 440), XMFLOAT2(32, 32), 2, 0, seaTilesPlayer1, targetSeaTilesPlayer2);
	CShip* blueBattleShip1 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("blueShip")->m_pTexture, XMFLOAT2(32, 480), XMFLOAT2(32, 32), 3, 0, seaTilesPlayer1, targetSeaTilesPlayer2);
	CShip* blueBattleShip2 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("blueShip")->m_pTexture, XMFLOAT2(138, 480), XMFLOAT2(32, 32), 3, 0, seaTilesPlayer1, targetSeaTilesPlayer2);
	CShip* blueCarrier = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("blueShip")->m_pTexture, XMFLOAT2(32, 520), XMFLOAT2(32, 32), 4, 0, seaTilesPlayer1, targetSeaTilesPlayer2);

	shipsPlayer1.push_back(blueSubmarine1);
	shipsPlayer1.push_back(blueSubmarine2);
	shipsPlayer1.push_back(blueSubmarine3);
	shipsPlayer1.push_back(blueSubmarine4);
	shipsPlayer1.push_back(blueDestroyer1);
	shipsPlayer1.push_back(blueDestroyer2);
	shipsPlayer1.push_back(blueDestroyer3);
	shipsPlayer1.push_back(blueBattleShip1);
	shipsPlayer1.push_back(blueBattleShip2);
	shipsPlayer1.push_back(blueCarrier);
}
void CVSPlayerState::InitPlayer2()
{
	seaTilesPlayer2.empty();

	float nMainStartPosX;
	float nMainStartPosY;

	for (int i = 0; i < 100; i++)
	{
		if (i == 0)
		{
			nMainStartPosX = 32;
			nMainStartPosY = 50;
		}
		else if (i % 10 == 0)
		{
			nMainStartPosX = 32;
			nMainStartPosY += 32;
		}
		else
		{
			nMainStartPosX += 32;
		}

		CSeaTile* seaTile = new CSeaTile(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("tileEmpty")->m_pTexture, XMFLOAT2(nMainStartPosX, nMainStartPosY), XMFLOAT2(32, 32), i);
		seaTilesPlayer2.push_back(seaTile);
	}



	CShip* redSubmarine1 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("redShip")->m_pTexture, XMFLOAT2(32, 400), XMFLOAT2(32, 32), 1, 1, seaTilesPlayer2,targetSeaTilesPlayer1);
	CShip* redSubmarine2 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("redShip")->m_pTexture, XMFLOAT2(64 + 10, 400), XMFLOAT2(32, 32), 1, 1, seaTilesPlayer2, targetSeaTilesPlayer1);
	CShip* redSubmarine3 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("redShip")->m_pTexture, XMFLOAT2(96 + 20, 400), XMFLOAT2(32, 32), 1, 1, seaTilesPlayer2, targetSeaTilesPlayer1);
	CShip* redSubmarine4 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("redShip")->m_pTexture, XMFLOAT2(128 + 30, 400), XMFLOAT2(32, 32), 1, 1, seaTilesPlayer2, targetSeaTilesPlayer1);
	CShip* redDestroyer1 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("redShip")->m_pTexture, XMFLOAT2(32, 440), XMFLOAT2(32, 32), 2, 1, seaTilesPlayer2, targetSeaTilesPlayer1);
	CShip* redDestroyer2 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("redShip")->m_pTexture, XMFLOAT2(106, 440), XMFLOAT2(32, 32), 2, 1, seaTilesPlayer2, targetSeaTilesPlayer1);
	CShip* redDestroyer3 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("redShip")->m_pTexture, XMFLOAT2(180, 440), XMFLOAT2(32, 32), 2, 1, seaTilesPlayer2, targetSeaTilesPlayer1);
	CShip* redBattleShip1 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("redShip")->m_pTexture, XMFLOAT2(32, 480), XMFLOAT2(32, 32), 3, 1, seaTilesPlayer2, targetSeaTilesPlayer1);
	CShip* redBattleShip2 = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("redShip")->m_pTexture, XMFLOAT2(138, 480), XMFLOAT2(32, 32), 3, 1, seaTilesPlayer2, targetSeaTilesPlayer1);
	CShip* redCarrier = new CShip(m_pRenderer, CStateManager::GetInstance()->GetAssetManager()->getTexture("redShip")->m_pTexture, XMFLOAT2(32, 520), XMFLOAT2(32, 32), 4, 1, seaTilesPlayer2, targetSeaTilesPlayer1);

	shipsPlayer2.push_back(redSubmarine1);
	shipsPlayer2.push_back(redSubmarine2);
	shipsPlayer2.push_back(redSubmarine3);
	shipsPlayer2.push_back(redSubmarine4);
	shipsPlayer2.push_back(redDestroyer1);
	shipsPlayer2.push_back(redDestroyer2);
	shipsPlayer2.push_back(redDestroyer3);
	shipsPlayer2.push_back(redBattleShip1);
	shipsPlayer2.push_back(redBattleShip2);
	shipsPlayer2.push_back(redCarrier);
}



void CVSPlayerState::Exit()
{

}

void CVSPlayerState::Update(float elapsedTime)
{
	if (m_bPlayer1Turn == true && m_bGame == false)
	{
		for (unsigned int i = 0; i < shipsPlayer1.size(); i++)
		{
			shipsPlayer1[i]->Update();
		}
	}
	else if (m_bPlayer1Turn == false && m_bGame == false)
	{
		for (unsigned int i = 0; i < shipsPlayer2.size(); i++)
		{
			shipsPlayer2[i]->Update();
		}
	}

	if (m_bGame == false)
	{
		if (Player1Ready() == true)
		{
			m_bPlayer1Turn = false;
		}
		if (Player2Ready() == true)
		{
			m_bPlayer1Turn = true;
			textInfo[0]->SetRendering(false);
			textInfo[7]->SetRendering(true);
			m_bGame = true;
		}
	}
	else if (m_bGame == true && m_bGameOver != true)
	{
		std::string szShipsLeft = "Ships left: ";
		szShipsLeft += std::to_string(GetNumShipsLeft(1));
		textInfo[1]->Update(szShipsLeft);

		std::string szShipsKilled = "Ships sunken: ";
		szShipsKilled += std::to_string(GetNumShipsKilled(1));
		textInfo[2]->Update(szShipsKilled);


		std::string szShipsLeftRed = "Ships left: ";
		szShipsLeftRed += std::to_string(GetNumShipsLeft(2));
		textInfo[4]->Update(szShipsLeftRed);

		std::string szShipsKilledRed = "Ships sunken: ";
		szShipsKilledRed += std::to_string(GetNumShipsKilled(2));
		textInfo[5]->Update(szShipsKilledRed);

		if (CInputManager::GetInstance()->GetKeyPress(0x01) == true)
		{		
			CInputManager::tMouseInput mousePos = CInputManager::GetInstance()->GetMouseInput();
			XMFLOAT2 m_fPos = XMFLOAT2((float)mousePos.x, (float)mousePos.y);

			if (m_bPlayer1Turn == true)
			{
				for (unsigned int i = 0; i < targetSeaTilesPlayer1.size(); i++)
				{
					if (targetSeaTilesPlayer1[i]->ShipSearch(m_fPos) == true)
					{
						if (seaTilesPlayer2[i]->GetTileStatus() == tileEmpty || seaTilesPlayer2[i]->GetTileStatus() == tileAdjecent)
						{
							sndPlaySound("Assets/fire.wav", SND_ASYNC);
							if (seaTilesPlayer2[i]->GetTileStatus() == tileEmpty)
							{
								seaTilesPlayer2[i]->Update(tileHitEmpty);
								targetSeaTilesPlayer1[i]->Update(tileHitEmpty);
							}
							else if (seaTilesPlayer2[i]->GetTileStatus() == tileAdjecent)
							{
								seaTilesPlayer2[i]->Update(tileShipAdjacentHit);
								targetSeaTilesPlayer1[i]->Update(tileHitEmpty);
							}
							m_bPlayer1Turn = false;
						}
						else if(seaTilesPlayer2[i]->GetTileStatus() == tileShipRed)
						{
							sndPlaySound("Assets/fire.wav", 1);
							sndPlaySound("Assets/hit.wav", 1);
							targetSeaTilesPlayer1[i]->Update(tileHitShipRed);
							seaTilesPlayer2[i]->Update(tileHitShipRed);
							if (seaTilesPlayer2[i]->GetShip() != nullptr)
							seaTilesPlayer2[i]->GetShip()->CheckDamage();
							m_nPlayer2Health -= 1;
						}
					}
				}
			}
			else if (m_bPlayer1Turn == false)
			{
				for (unsigned int i = 0; i < targetSeaTilesPlayer2.size(); i++)
				{
					if (targetSeaTilesPlayer2[i]->ShipSearch(m_fPos) == true)
					{
						if (seaTilesPlayer1[i]->GetTileStatus() == tileEmpty || seaTilesPlayer1[i]->GetTileStatus() == tileAdjecent)
						{
							sndPlaySound("Assets/fire.wav", SND_ASYNC);
							if (seaTilesPlayer1[i]->GetTileStatus() == tileEmpty)
							{
								seaTilesPlayer1[i]->Update(tileHitEmpty);
								targetSeaTilesPlayer2[i]->Update(tileHitEmpty);
							}
							else if (seaTilesPlayer1[i]->GetTileStatus() == tileAdjecent)
							{
								seaTilesPlayer1[i]->Update(tileShipAdjacentHit);
								targetSeaTilesPlayer2[i]->Update(tileHitEmpty);
							}
							m_bPlayer1Turn = true;
						}
						else if (seaTilesPlayer1[i]->GetTileStatus() == tileShipBlue)
						{
							sndPlaySound("Assets/fire.wav", 1);
							sndPlaySound("Assets/hit.wav", 1);
							targetSeaTilesPlayer2[i]->Update(tileHitShipBlue);
							seaTilesPlayer1[i]->Update(tileHitShipBlue);
							if (seaTilesPlayer1[i]->GetShip() != nullptr)
							seaTilesPlayer1[i]->GetShip()->CheckDamage();
							m_nPlayer1Health -= 1;
						}
					}
				}
			}
			
		}

		if (m_nPlayer1Health == 0 || m_nPlayer2Health == 0)
		{
			m_bGameOver = true;
			if (m_nPlayer1Health == 0)
			{
				textInfo[9]->SetRendering(true);
			}
			else if (m_nPlayer2Health == 0)
			{
				textInfo[8]->SetRendering(true);
			}
			sndPlaySound("Assets/win.wav", 0);
		}
	}



}

void CVSPlayerState::Render()
{
	m_pRenderer->PreRenderInit();

	CommonStates states(m_pRenderer->GetDevice());
	m_pRenderer->Get2DRenderer()->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());



	if (m_bPlayer1Turn == true)
	{
		textInfo[1]->SetRendering(true);
		textInfo[2]->SetRendering(true);
		textInfo[3]->SetRendering(true);
		textInfo[4]->SetRendering(false);
		textInfo[5]->SetRendering(false);
		textInfo[6]->SetRendering(false);
		RenderPlayer1View();
	}
	else if (m_bPlayer1Turn == false)
	{
		textInfo[1]->SetRendering(false);
		textInfo[2]->SetRendering(false);
		textInfo[3]->SetRendering(false);
		textInfo[4]->SetRendering(true);
		textInfo[5]->SetRendering(true);
		textInfo[6]->SetRendering(true);
		RenderPlayer2View();
	}

	if (m_bGameOver == true)
	{
		textInfo[3]->SetRendering(false);
		textInfo[6]->SetRendering(false);

		std::string szShipsLeft = "Ships left: ";
		szShipsLeft += std::to_string(GetNumShipsLeft(1));
		textInfo[1]->Update(szShipsLeft);

		std::string szShipsKilled = "Ships sunken: ";
		szShipsKilled += std::to_string(GetNumShipsKilled(1));
		textInfo[2]->Update(szShipsKilled);


		std::string szShipsLeftRed = "Ships left: ";
		szShipsLeftRed += std::to_string(GetNumShipsLeft(2));
		textInfo[4]->Update(szShipsLeftRed);

		std::string szShipsKilledRed = "Ships sunken: ";
		szShipsKilledRed += std::to_string(GetNumShipsKilled(2));
		textInfo[5]->Update(szShipsKilledRed);
	}

	for (unsigned int i = 0; i < textInfo.size(); i++)
	{
		textInfo[i]->Render();
	}

	m_pRenderer->Get2DRenderer()->End();
	m_pRenderer->GetSwapChain()->Present(0, 0);
}


bool CVSPlayerState::Player1Ready()
{
	for (unsigned int i = 0; i < shipsPlayer1.size(); i++)
	{
		if (shipsPlayer1[i]->GetPlaced() == false)
			return false;
	}

	return true;
}

bool CVSPlayerState::Player2Ready()
{
	for (unsigned int i = 0; i < shipsPlayer2.size(); i++)
	{
		if (shipsPlayer2[i]->GetPlaced() == false)
			return false;
	}
	return true;
}

int CVSPlayerState::GetNumShipsLeft(int numPlayer)
{
	int shipsLeft = 0;
	if (numPlayer == 1)
	{
		for (unsigned int i = 0; i < shipsPlayer1.size(); i++)
		{
			if (shipsPlayer1[i]->GetAlive() == true)
				shipsLeft++;
		}
	}
	else if (numPlayer == 2)
	{
		for (unsigned int i = 0; i < shipsPlayer2.size(); i++)
		{
			if (shipsPlayer2[i]->GetAlive() == true)
				shipsLeft++;
		}
	}

	return shipsLeft;
}

int CVSPlayerState::GetNumShipsKilled(int numPlayer)
{
	int shipsKilled = 0;
	if (numPlayer == 1)
	{
		for (unsigned int i = 0; i < shipsPlayer2.size(); i++)
		{
			if (shipsPlayer2[i]->GetAlive() == false)
				shipsKilled++;
		}
	}
	else if (numPlayer == 2)
	{
		for (unsigned int i = 0; i < shipsPlayer1.size(); i++)
		{
			if (shipsPlayer1[i]->GetAlive() == false)
				shipsKilled++;
		}
	}

	return shipsKilled;
}