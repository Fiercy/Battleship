#pragma once
#include "State.h"
#include <vector>

class CSeaTile;
class CShip;
class CUITextLabel;

class CVSPlayerState : public CState
{
public:
	CVSPlayerState();
	~CVSPlayerState();

	// Loads and sets up whats needed for the state
	virtual void Enter();
	// Clears and removes whats not needed in the state
	virtual void Exit();
	// Updates the state
	virtual void Update(float elapsedTime);
	// Renders the state
	virtual void Render();

	void RenderPlayer1View();
	void RenderPlayer2View();

	void InitPlayer1();
	void InitPlayer2();

	bool Player1Ready();
	bool Player2Ready();

	void RenderPlayer1ShipStatus();

	eState GetType() const { return eState::vsPlayer; }

	std::vector<CSeaTile*> GetPlayer1Tiles() { return seaTilesPlayer1; };

	int GetNumShipsLeft(int numPlayer);
	int GetNumShipsKilled(int numPlayer);

private:
	std::vector<CSeaTile*> seaTilesPlayer1;
	std::vector<CSeaTile*> targetSeaTilesPlayer1;
	std::vector<CSeaTile*> seaTilesPlayer2;
	std::vector<CSeaTile*> targetSeaTilesPlayer2;

	std::vector<CShip*> shipsPlayer1;
	std::vector<CShip*> shipsPlayer2;
	std::vector<CUITextLabel*> textInfo;

	int  m_nPlayer1Health;
	int  m_nPlayer2Health;

	bool m_bPlayer1Placing;
	bool m_bPlayer2Placing;
	bool m_bPlayer1Turn;
	bool m_bGame;
	bool m_bGameOver;
};

