#pragma once
#include "../helpers/datatypes.h"
#include "../helpers/criticalsectionmacros.h"

class CSGD_DirectInput;

enum mouseButtons {
	MOUSEBTN_LEFT,
	MOUSEBTN_RIGHT,
	MOUSEBTN_MIDDLE,
	MOUSEBTNS
};

class mouse
{
protected:
	vector3 position;

	CRITICAL_VARS;

	int imgID;
	pt clickPos;

	CSGD_DirectInput* theInput;

	mouse();
	mouse(const mouse&);
	mouse& operator=(const mouse&);
	~mouse();
public:
	static mouse* getInstance();

	void initialize();
	void shutdown();

	void update(float dt);
	void render() const;

	void setPos(const vector3& val);
	void setClickPos(const pt& val) { clickPos = val; }
	void setImgID(int val);

	const vector3& getPos() const { return position; }
	const pt& getClickPos() const { return clickPos; }
	int getImgID() const { return imgID; }
};