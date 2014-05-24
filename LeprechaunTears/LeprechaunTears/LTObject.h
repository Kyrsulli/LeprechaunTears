#pragma once

class LTObject{
public:
	virtual void update();
	virtual void render();
	bool levelComplete(){return completed;};
protected:
	bool completed;
};