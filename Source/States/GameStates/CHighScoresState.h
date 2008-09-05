#pragma once
#include "menuState.h"


class eventManager;

class CHighScoresState : public menuState
{
private:
	
	enum menuOptions{ EXIT , TOTAL};

	void menuHandler(void);
		
	//bool CheckScores();
	CHighScoresState(void);
	CHighScoresState& operator=(CHighScoresState &ref);
	~CHighScoresState(void) {}
public:
	void enter();
	void exit();
	bool input(float);
	void render(void) const;
	void update(float);
	static bool AddHighScore(unsigned int nScore);
	

	
	static CHighScoresState* GetInstance(void) 
	{ static CHighScoresState instance; return &instance; }
};