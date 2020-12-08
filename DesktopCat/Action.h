#pragma once
#include "DesktopCat.h"
#include "Animation.h"

class LAction
{
public:
	static LAction* create();
	static void release(LAction**);
private:
	LAction() { }
	~LAction() { }

	bool init();
	void release();
};