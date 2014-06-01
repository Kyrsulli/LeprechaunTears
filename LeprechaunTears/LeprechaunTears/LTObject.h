#pragma once

class LTObject{
public:
	virtual void update();
	virtual void render();
	virtual void onGUI();
	bool levelComplete(){return completed;};
protected:
	bool completed;
};