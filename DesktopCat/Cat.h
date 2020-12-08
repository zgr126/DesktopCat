#pragma once
#include "DesktopCat.h"
#include "Sprite.h"
#include "Animation.h"

#define Cat_AnimationTimer_ID	10		//è������ʱ������ID
#define Cat_IdelTimer_ID		11		//è������ʱ��

//�˺��ӦAnimation�ļ�����
#pragma region è����궨��
//վ��
#define Cat_Front_Idle			(FileData_Animation_BeginLine + 1)		//�泯��Ļ		վ��
#define Cat_Back_Idle			(FileData_Animation_BeginLine + 2) 		//������Ļ		վ��
#define Cat_Left_Idle			(FileData_Animation_BeginLine + 3)		//����			վ��
#define Cat_Right_Idle			(FileData_Animation_BeginLine + 4)		//����			վ��
#define Cat_FrontLeft_Idle		(FileData_Animation_BeginLine + 5)		//������Ļ����	վ��
#define Cat_BackLeft_Idle		(FileData_Animation_BeginLine + 6)		//������Ļ����	վ��
#define Cat_FrontRight_Idle		(FileData_Animation_BeginLine + 7)		//������Ļ����	վ��
#define Cat_BackRight_Idle		(FileData_Animation_BeginLine + 8)		//������Ļ����	վ��
#define Cat_Idle_Min		Cat_Front_Idle
#define Cat_Idle_Max		Cat_BackRight_Idle	//Idle��Χ
//��·
#define Cat_Front_Walk			(Cat_Idle_Max + 1)						//�泯��Ļ		��·
#define Cat_Back_Walk			(Cat_Idle_Max + 2)				 		//������Ļ		��·
#define Cat_Left_Walk			(Cat_Idle_Max + 3)						//����			��·
#define Cat_Right_Walk			(Cat_Idle_Max + 4)						//����			��·
#define Cat_FrontLeft_Walk		(Cat_Idle_Max + 5)						//������Ļ����	��·
#define Cat_BackLeft_Walk		(Cat_Idle_Max + 6)						//������Ļ����	��·
#define Cat_FrontRight_Walk		(Cat_Idle_Max + 7)						//������Ļ����	��·
#define Cat_BackRight_Walk		(Cat_Idle_Max + 8)						//������Ļ����	��·
#define Cat_Walk_Min		Cat_Front_Walk
#define Cat_Walk_Max		Cat_BackRight_Walk	//Walk��Χ
//�ܲ�
#define Cat_Front_Run			(Cat_Walk_Max + 1)						//�泯��Ļ		�ܲ�
#define Cat_Back_Run			(Cat_Walk_Max + 2)				 		//������Ļ		�ܲ�
#define Cat_Left_Run			(Cat_Walk_Max + 3)						//����			�ܲ�
#define Cat_Right_Run			(Cat_Walk_Max + 4)						//����			�ܲ�
#define Cat_FrontLeft_Run		(Cat_Walk_Max + 5)						//������Ļ����	�ܲ�
#define Cat_BackLeft_Run		(Cat_Walk_Max + 6)						//������Ļ����	�ܲ�
#define Cat_FrontRight_Run		(Cat_Walk_Max + 7)						//������Ļ����	�ܲ�
#define Cat_BackRight_Run		(Cat_Walk_Max + 8)						//������Ļ����	�ܲ�
#define Cat_Run_Min			Cat_Front_Run
#define Cat_Run_Max			Cat_BackRight_Run	//Run��Χ
#pragma endregion

//è����һ��ʱ��ķ�Χ
#define Cat_IdelEntireTime_Min	1000
#define Cat_IdelEntireTime_Max	5000


class Cat :public LSprite
{
public:
	static Cat* create(const string& spriteName, const string& suffix, const LPoint& size);
	static void release(Cat**);
#pragma region ��д���෽��	init,release,Draw,Update
protected:
	virtual bool init(const string& spriteName, const string& suffix, const LPoint& size);
	virtual void release();
public:
	virtual void Draw(ID2D1HwndRenderTarget*);
	virtual void Update();
#pragma endregion

	//è״̬ö��
	enum class CatStatus
	{
		Idel,		//վ��
		Walk,		//��·
		Run,		//�ܲ�
		Other		//����
	};
private:
	//è״̬
	CatStatus m_Status;
	//è������ʱ��
	LTimer* m_IdelTimer;
	//è����һ�ε�ʱ��(���1-5��)
	UINT m_EntireIdelTime;
	//è�ӷ�����ʼ�����ڵ�ʱ��
	UINT m_NowIdelTime;
public:

};





//#include "Animation.h"
//#include "Timer.h"
//
//class GlassWindow;
//
//#define Cat_Idel_TimerID			10		//è������ʱ���Ĺ̶�ID
//
//class Cat :public LSprite
//{
//public:
//	//��̬����������
//	static Cat* create(string& _SpriteName, string _Suffix, LPoint& _Size);
//	static void release(Cat**);
//
//public:
//	//ö��
//	enum class HousePetDirection
//	{		//��ɫ����(8������)
//		Top, Bottom, Left, Right,
//		LeftTop, RightTop, LeftBottom, RightBottom,
//		None,	//Null��ʾ�޳��� ���⶯������ʱ������Null�Ա�AI���Գ����ⷽ���˶������糯�������������� ���������ߣ�
//	};
//	enum class HousePetMode
//	{			//״̬
//		Idel,		//����
//		Move,		//�˶�
//		Special		//ר�ж���
//	};
//	typedef enum HousePetPrefabAnimation
//	{	//��ɫԤ�ƶ���	(ö��ֵ��Ӧ�ļ�ActionData�ļ��ж������ݵı��)
//		Attack,		//ץ���� 
//		Balnk,		//գ�ۣ�������
//		Dig,		//������ץ�ذ�
//		Ennui,		//ſ�ڵ���ҡβ��
//		Fly,		//ſ�ڵ����޾����
//		Lick,		//��צ��
//		Paw,		//ץһ�µذ�
//		Relax,		//�Ľų���
//		Scratch,	//�εذ�
//		Sleep1,		//��������˯��
//		Sleep2,		//�����
//		Sleep3,		//�������۾�
//		Sniff,		//�ᣨվ�ţ�
//		Stretch,	//������
//		Sway,		//�˷ܵ�ҡβ��
//		Tail,		//�����ŷ�����ҡβ�ͣ�
//		BottomRun,		//����	��
//		LeftRun,		//����	��
//		RightRun,		//����	��
//		TopRun,			//����	��
//		LeftBottomRun,	//����	�L
//		RightBottomRun,	//����	�K
//		RightTopRun,	//����	�J
//		LeftTopRun,		//����	�I
//		BottomWalk,		//��·	��
//		LeftWalk,		//��·	��
//		RightWalk,		//��·	��
//		TopWalk,		//��·	��
//		LeftBottomWalk,	//��·	�L
//		RightBottomWalk,//��·	�K
//		RightTopWalk,	//��·	�J
//		LeftTopWalk,	//��·	�I
//		BottomIdel,		//վ��	��
//		LeftIdel,		//վ��	��
//		RightIdel,		//վ��	��
//		TopIdel,		//վ��	��
//		LeftBottomIdel,	//վ��	�L
//		RightBottomIdel,//վ��	�K
//		RightTopIdel,	//վ��	�J
//		LeftTopIdel,	//վ��	�I
//	} HPA;
//protected:
//	//��Ա����
//	Animation* m_Animation;
//	HousePetDirection m_Direction;	//��ǰ����
//	HousePetMode m_PetMode;			//�˶�״̬
//	GlassWindow* m_Window;			//���ﴰ�ھ��
//	LTimer* m_IdelTime;				//����ʱ��
//protected:
//	//��������
//	virtual bool init(string& _SpriteName, string _Suffix, LPoint& _Size);
//	virtual void release();
//public:
//	//��������
//	Cat();
//
//	virtual void Draw(ID2D1HwndRenderTarget*);
//	virtual void Update();
//
//	/*
//		_FileLine	��ȡ�ļ���_FileLine����������	(��0��ʼ)
//		_Style		��������
//		_Val		��_StyleΪCycleʱ	_ValΪ����ѭ������
//					��_StyleΪTimeʱ		_ValΪ�����̶�����ʱ��
//					��_StyleΪUnTimeʱ	_Val����������ֵ ��Ϊ_Val�ᱻ����*/
//	void PlayAnimation(int _FileLine, LAnimation::LAnimationStyle _Style, DWORD _Val, Cat::HousePetDirection _Direction);
//
//private:
//	void AI();		//��������ʱ ����AI����
//	void Idel();	//����
//
//public:
//	GlassWindow* SetGlassWindow(GlassWindow* GWindow) { m_Window = GWindow; }
//};