#pragma once
#include "DesktopCat.h"
#include "Node.h"
class LSprite;

#define DataParent		"File\\"				//�����ļ��ĸ�Ŀ¼
#define ActionData		"\\ActionData.txt"		//�Ӵ��ļ��ж�ȡ����ߴ��Լ��ܹ��ж����鶯��	(�û��ṩ)
//�и�ʽ
#define VaildData			2					//��Ч���ݿ�ʼ������������������������ͼƬ�ж����鶯����

//��Ч���ݲ�����ʽ
#define VaildData_AnimationPlayLine		0		//��1��λ�õĲ�����ţ��ö���ѡ��ͼƬ�ĵ�n��
#define VaildData_AnimationOrder		1		//��2��λ�õĲ�����ţ��ö���һ��������֡���л�˳�� �磺(0,1,2,3)
#define VaildData_AnimationDwellTime	2		//��3��λ�õĲ�����ţ���1֡��ͣ��ʱ��...�Դ�����

#define M_To_MS		1000.0						//��ת����

//����������
class LAnimation
{
public:
	enum class LAnimationStyle
	{
		Null,		//�û������� ֻ����ϵͳʹ��			(�������մ��������ʱ ���������)
		Cycle,		//�������� ����һ������ֻѭ��1��		(���Ŵ���һ�� ���ܶ������ŵ���֡ ֱ��ɾ�� ���磺����)
		Time,		//ʱ������ ����һ�������̶�����1000ms	(ʱ��δ�� ����������ѭ������ ���磺�ܲ�)
		UnTime		//����ѭ������						(����ֹͣ ���磺��ֹվ��)
	};
	LAnimationStyle m_AnimationStyle;			//��¼��������
	//��̬����������
	static LAnimation* create(LSprite*);
	static void release(LAnimation**);
private:
	/********************************************/
	//���ǹ�������Ҫ�ı���
	string m_AnimationDataFile;	//���������ļ���·��
	//����1������������Ҫ�ı���
	UINT m_PlayLine;			//�ö���ѡ��ͼƬ�ĵ�m_PlayLine��	(���ļ���ȡ)
	vector<float> m_FrameOrder;	//һ���������ڵ�֡�л�˳��			(���ļ���ȡ)
	vector<float> m_DwellTime;	//֡��֮֡��ļ��ʱ��			(���ļ���ȡ)
	UINT m_FrameNumber;			//����1�����ڵ���֡��	
	UINT m_NowFrame;			//��ǰ֡
	DWORD m_BeginPlayTime;		//֡��ʼʱ��
	DWORD m_NowFrameTime;		//֡����ʱ��
	DWORD m_EndFrameTime;		//֡����ʱ��
	DWORD m_PlayTime;			//��������ʱ��
	int m_PlayNumber;			//���Ŵ���
	bool m_isEnd;				//���������Ƿ����
	/*******************************************/
private:
	bool init(LSprite*);
	void release();

	void updateFrame();				//����֡��״̬
	void updataFrameTime();			//���л���һ֡ʱ ����֡��ʱ
	void AnimationBegin();			//�������ſ�ʼʱ����
	void AnimationEnd();			//�������Ž���ʱ����
	void Clear();					//���㶯������ Ȼ���¶������ݸ���

	//��ȡvector�е�_Positionλ�õĲ�����ŵ�_Param��
	template <typename T>
	void ReadVectorData(vector<float>&, int _Index, T& _Param);

public:
	//��������
	LAnimation();

	LPoint update();	//�������ڲ��ŵڼ�֡

	//�л����� ����ɾ��	 �Ḳ���ϸ�����������	
	void Cutover(int _FileLine, LAnimationStyle _Style, DWORD _Val); //��ϸע�ͼ�LPet::PlayAnimation����

public:
	//������
	UINT GetFrameNumber() { return m_FrameNumber; }
	UINT GetNowFrame() { return m_NowFrame; }
	bool GetisEnd() { return m_isEnd; }
};

typedef LAnimation Animation;