#include "Action.h"
#include "Animation.h"

LAction* LAction::create()
{
	LAction* pAction = new LAction();
	if (pAction->init())
		return pAction;
	delete pAction;
	return pAction = nullptr;
}

void LAction::release(LAction** pAction)
{
	if (!(*pAction))
		return;
	(*pAction)->release();
	delete* pAction;
	*pAction = nullptr;
}

bool LAction::init()
{
}

void LAction::release()
{
}