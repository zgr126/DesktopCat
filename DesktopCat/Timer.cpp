#include "Timer.h"


#pragma region LTimer
LTimer::LTimer(UINT id) :m_ID(id), m_BeginTime(0), m_EndTime(0), m_Interval(0)
{
}

void LTimer::Begin()
{
	m_BeginTime = m_EndTime = GetTickCount64();
}

void LTimer::End()
{
	m_BeginTime = m_EndTime = 0;
}

void LTimer::Arrive()
{
	m_EndTime = GetTickCount64();
	m_Interval = m_EndTime - m_BeginTime;
	if (m_Interval != 0)
	{
		//如果时间间隔太小则不更新m_BeginTimer
		m_BeginTime = m_EndTime;
	}
}

void LTimer::Update()
{
	Arrive();
}

void LTimer::Reset()
{
	Begin();
}
#pragma endregion


#pragma region LTimerManager

LTimerManager* LTimerManager::instance = nullptr;

LTimerManager* LTimerManager::create()
{
	if (instance == nullptr)
	{
		instance = new LTimerManager();
		if (instance->init())
			return instance;
		release();
		return nullptr;
	}
	return instance;
}

void LTimerManager::release()
{
	for (auto& timer : instance->m_listTimer)
	{
		instance->m_listTimer.erase(find(instance->m_listTimer.begin(), instance->m_listTimer.end(), timer));
		delete timer;
	}
	delete instance;
	instance = nullptr;
}

bool LTimerManager::init()
{
	return true;
}

LTimer* LTimerManager::createTimer(UINT ID)
{
	for (auto& timer : m_listTimer)
	{
		if (ID == timer->m_ID)
		{
			OutputDebugString(L"创建的定时器的ID重复");
			return nullptr;
		}
	}
	LTimer* Timer = new LTimer(ID);
	m_listTimer.push_back(Timer);
	Timer->Begin();			//创建完成后便开始计时
	return Timer;
}

void LTimerManager::releaseTimer(UINT ID)
{
	for (auto& Timer : m_listTimer)
	{
		if (Timer->m_ID == ID)
		{
			m_listTimer.erase(find(m_listTimer.begin(), m_listTimer.end(), Timer));
			delete Timer;
			return;
		}
	}
}

void LTimerManager::Update()
{
	for (auto& timer : m_listTimer)
	{
		timer->Update();
	}
}

#pragma endregion
