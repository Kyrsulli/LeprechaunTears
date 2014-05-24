#pragma once

class LTObject{
public:
	virtual void update();
	virtual void render();
protected:
	bool completed;
};