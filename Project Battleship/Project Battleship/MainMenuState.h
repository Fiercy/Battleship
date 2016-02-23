#pragma once
#include "State.h"
#include <vector>

class CUIButton;
class CUITextLabel;

class CMainMenuState : public CState
{
public:
	CMainMenuState();
	~CMainMenuState();

	// Loads and sets up whats needed for the state
	virtual void Enter();
	// Clears and removes whats not needed in the state
	virtual void Exit();
	// Updates the state
	virtual void Update(float elapsedTime);
	// Renders the state
	virtual void Render();

	eState GetType() const { return eState::mainMenu; }

private:
	std::vector<CUIButton*> m_vMenuUIButtons;
	std::vector<CUITextLabel*> m_vMainMenuLabels;
	
};

