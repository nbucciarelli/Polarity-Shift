#pragma once
#include "..\gameState.h"

class eventManager;

class CHighScoresState : public gameState
{
private:

	bool CheckScores();
	CHighScoresState(void) {}
	CHighScoresState& operator=(CHighScoresState &ref);
	~CHighScoresState(void) {}
public:
	void enter();
	void exit();
	bool input(float);
	void render(void) const;
	void update(float);

	
	static CHighScoresState* GetInstance(void) 
	{ static CHighScoresState instance; return &instance; }
};