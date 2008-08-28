#ifndef CANIMATIONMANAGER_H_
#define CANIMATIONMANAGER_H_

#include "../Wrappers/viewManager.h"
#include "../Objects/baseObj.h"
#include <vector>
using std::vector;
class CAnimationEngine;

class CAnimationManager
{
private:
	viewManager* m_pTM;

	CAnimationManager(void) { m_pTM = viewManager::getInstance(); }
	CAnimationManager(const CAnimationManager&);
	CAnimationManager& operator=(const CAnimationManager&);

	~CAnimationManager(void) {};
public:
	vector<CAnimationEngine*> m_pAE;
	baseObj* m_pBase;

	static CAnimationManager* GetInstance(void);
	void Load(char* szFileName, baseObj* object);
	void Shutdown(void);
	void Render(int ID, int nPosX, int nPosY, float fScaleX = 1.0f, float fScaleY = 1.0f, float fRotationX = 0.0f, 
		float fRotationY = 0.0f, float fRotation = 0.0f, unsigned color = 0xFFFFFFFF);
	void Update(float fDelta);
	CAnimationEngine* GetEngine(unsigned int nIndex);
};

#endif