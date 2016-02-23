#pragma once
/***********************************************
* Filename:  		State.h
* Date:      		2/21/2016
* Mod. Initials:	CH
* Author:    		Volodymyr Golovan
* Purpose:   		This is the Abstract Base
*					Class for the Game States
************************************************/

class CRenderer;
class CStateManager;
class CAssetManager; 

enum eState
{
	mainMenu,vsPlayer
};

class CState
{
public:
	CState() {};
	virtual ~CState() {};

	// Loads and sets up whats needed for the state
	virtual void Enter() = 0;
	// Clears and removes whats not needed in the state
	virtual void Exit() = 0;
	// Updates the state
	virtual void Update(float elapsedTime) = 0;
	// Renders the state
	virtual void Render() = 0;


protected:
	CRenderer* m_pRenderer;
	CStateManager* m_pStateManager;
	CAssetManager* m_pAssetManager;



};

