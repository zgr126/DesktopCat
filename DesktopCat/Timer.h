#pragma once
#include <Windows.h>
#include <list>

using namespace std;

class LTimerManager;

class LTimer
{
private:
	UINT m_ID;						//ÿ����ʱ������ID
	ULONGLONG m_BeginTime;			//��ʼʱ��
	ULONGLONG m_EndTime;			//����ʱ��
	ULONGLONG m_Interval;			//��һ��ˢ�µ����ڵ�ʱ����

protected:
	LTimer(UINT ID);
	~LTimer() { }

	void Begin();			//��ʼ��ʱ
	void End();				//������ʱ
	void Arrive();			//�������һ��Begin���˷�������ʱ�����ʱ��
	void Update();
public:
	//����ʱ�����ã��ظ�����
	void Reset();
	//������
	ULONGLONG GetInterval() { return m_Interval; }		//��һ��ˢ�µ����ڵ�ʱ����

	friend LTimerManager;
};

class LTimerManager
{
public:
	static LTimerManager* instance;
	static LTimerManager* create();
	static void release();
private:
	list<LTimer*> m_listTimer;		//������Timer

private:
	bool init();
public:
	LTimer* createTimer(UINT ID);	//������ʱ��
	void releaseTimer(UINT ID);		//�ͷŶ�ʱ��

	void Update();
};