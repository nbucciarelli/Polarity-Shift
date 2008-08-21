#pragma once
#include "..\EventSystem\listener.h"
#include "..\Helpers\dataTypes.h"

class baseObj;

class camera : public listener
{
protected:
	baseObj* track;

	vector3 camPos, trackPos;
	const rect* bounds;

	matrix viewMatrix;

	camera();
	camera(const camera&);
	camera& operator=(const camera&);
	~camera();
public:
	static camera* getInstance();

	void initialize(const vector3& tracking = vector3(400,450,0));
	
	virtual void HandleEvent(gameEvent* ev);

	baseObj* getTrack() const { return track; }
	const vector3& getPos() const { return camPos; }
	const matrix& getViewMatrix() const { return viewMatrix; }
	
	void setTrackPos(const vector3& pos) { trackPos = pos; }
	void setBounds(const rect* bound) { bounds = bound; }

	void update(float dt);
};