#pragma once
#include <Windows.h>
#include <list>

using namespace std;

class LTimerManager;

class LTimer
{
private:
	UINT m_ID;						//每个定时器独有ID
	ULONGLONG m_BeginTime;			//开始时间
	ULONGLONG m_EndTime;			//结束时间
	ULONGLONG m_Interval;			//上一次刷新到现在的时间间隔

protected:
	LTimer(UINT ID);
	~LTimer() { }

	void Begin();			//开始计时
	void End();				//结束计时
	void Arrive();			//计算从上一次Begin到此方法调用时间隔的时间
	void Update();
public:
	//将计时器重置，重复利用
	void Reset();
	//访问器
	ULONGLONG GetInterval() { return m_Interval; }		//上一次刷新到现在的时间间隔

	friend LTimerManager;
};

class LTimerManager
{
public:
	static LTimerManager* instance;
	static LTimerManager* create();
	static void release();
private:
	list<LTimer*> m_listTimer;		//储存活动的Timer

private:
	bool init();
public:
	LTimer* createTimer(UINT ID);	//创建定时器
	void releaseTimer(UINT ID);		//释放定时器

	void Update();
};