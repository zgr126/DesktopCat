#pragma once
#include "DesktopCat.h"
#include "Sprite.h"
#include "Animation.h"
#include "Action.h"

#define Cat_AnimationTimer_ID	10		//è������ʱ������ID
#define Cat_IdelTimer_ID		11		//è������ʱ��

//�˺��ӦAnimation�ļ�����
#pragma region è����궨��
//վ��
#define Cat_Front_Idle			(FileData_Animation_BeginLine + 0)		//�泯��Ļ		վ��
#define Cat_Left_Idle			(FileData_Animation_BeginLine + 1)		//����			վ��
#define Cat_Right_Idle			(FileData_Animation_BeginLine + 2)		//����			վ��
#define Cat_Back_Idle			(FileData_Animation_BeginLine + 3) 		//������Ļ		վ��
#define Cat_FrontLeft_Idle		(FileData_Animation_BeginLine + 4)		//������Ļ����	վ��
#define Cat_FrontRight_Idle		(FileData_Animation_BeginLine + 5)		//������Ļ����	վ��
#define Cat_BackRight_Idle		(FileData_Animation_BeginLine + 6)		//������Ļ����	վ��
#define Cat_BackLeft_Idle		(FileData_Animation_BeginLine + 7)		//������Ļ����	վ��
#define Cat_Idle_Min		Cat_Front_Idle
#define Cat_Idle_Max		Cat_BackLeft_Idle	//Idle��Χ
#define Cat_Idle_Count		8					//Idle����
//��·
#define Cat_Front_Walk			(Cat_Idle_Max + 1)						//�泯��Ļ		��·
#define Cat_Left_Walk			(Cat_Idle_Max + 2)						//����			��·
#define Cat_Right_Walk			(Cat_Idle_Max + 3)						//����			��·
#define Cat_Back_Walk			(Cat_Idle_Max + 4)				 		//������Ļ		��·
#define Cat_FrontLeft_Walk		(Cat_Idle_Max + 5)						//������Ļ����	��·
#define Cat_FrontRight_Walk		(Cat_Idle_Max + 6)						//������Ļ����	��·
#define Cat_BackRight_Walk		(Cat_Idle_Max + 7)						//������Ļ����	��·
#define Cat_BackLeft_Walk		(Cat_Idle_Max + 8)						//������Ļ����	��·
#define Cat_Walk_Min		Cat_Front_Walk
#define Cat_Walk_Max		Cat_BackLeft_Walk	//Walk��Χ
#define Cat_Walk_Count		8					//Walk����
//�ܲ�
#define Cat_Front_Run			(Cat_Walk_Max + 1)						//�泯��Ļ		�ܲ�
#define Cat_Left_Run			(Cat_Walk_Max + 2)						//����			�ܲ�
#define Cat_Right_Run			(Cat_Walk_Max + 3)						//����			�ܲ�
#define Cat_Back_Run			(Cat_Walk_Max + 4)				 		//������Ļ		�ܲ�
#define Cat_FrontLeft_Run		(Cat_Walk_Max + 5)						//������Ļ����	�ܲ�
#define Cat_FrontRight_Run		(Cat_Walk_Max + 6)						//������Ļ����	�ܲ�
#define Cat_BackRight_Run		(Cat_Walk_Max + 7)						//������Ļ����	�ܲ�
#define Cat_BackLeft_Run		(Cat_Walk_Max + 8)						//������Ļ����	�ܲ�
#define Cat_Run_Min			Cat_Front_Run
#define Cat_Run_Max			Cat_BackLeft_Run	//Run��Χ
#define Cat_Run_Count		8					//Run����
//ת����ֱ��+�궨�����ת�����
#define Cat_Idel_To_Walk	(Cat_Idle_Count)					//վ������ת������������
#define Cat_Idel_To_Run		(Cat_Idle_Count + Cat_Walk_Count)	//վ������ת���ܲ�����
#define Cat_Walk_To_Run		(Cat_Walk_Count)
#pragma endregion

#pragma region һЩ���Ǻ���ֵ
#define Sin22_5		(0.38268343)
#define Cos22_5		(0.92387953)
#define Sin67_5		(0.92387953)
#define Cos67_5		(0.38268343)
#define Sin112_5	(0.92387953)
#define Cos112_5	(-0.38268343)
#define Sin157_5	(0.38268343)
#define Cos157_5	(-0.92387953)
#define Sin202_5	(-0.38268343)
#define Cos202_5	(-0.92387953)
#define Sin247_5	(-0.92387953)
#define Cos247_5	(-0.38268343)
#define Sin292_5	(-0.92387953)
#define Cos292_5	(0.38268343)
#define Sin337_5	(-0.38268343)
#define Cos337_5	(0.92387953)
#pragma endregion


//è����һ��ʱ��ķ�Χ
#define Cat_IdelEntireTime_Min	4000
#define Cat_IdelEntireTime_Max	8500

#define Cat_Walk_Speed			80		//80����/��
#define Cat_Run_Speed			150		//150����/��

#define Cat_Range_X			70
#define Cat_Range_Y			70


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
		Motion,		//��·���ܲ�
		Other		//����
	};
	enum class CatMotionStatus
	{
		None, Walk, Run
	};
	//è����ö��
	enum class CatDirection
	{
		Front = 10,					//�泯��Ļ
		Back = -10,					//������Ļ
		Left = -1,
		Right = 1,
		FrontLeft = Front + Left,	//�泯��Ļ����
		FrontRight = Front + Right,	//�泯��Ļ����
		BackRight = Back + Right,
		BackLeft = Back + Left
	};
private:
	//è״̬
	CatStatus m_Status;
	//è�˶�״̬
	CatMotionStatus m_MotionStatus;
	//è����
	CatDirection m_Direction;
	//è������ʱ��
	LTimer* m_IdelTimer;
	//è����һ�ε�ʱ��(���1-5��)
	UINT m_EntireIdelTime;
	//è�Ӷ�����ʼ�����ڵ�ʱ��
	UINT m_NowContinueTime;
	//è��·/�˶�ʱ�䣬ͨ������ó�
	UINT m_MotionTime;
	//è�ƶ���Χ
	LPoint m_MoveRange;
private:
	void MotionTo(const LPoint& destination, double speed, int actionDataLine);
	//�ı�è״̬֮ǰ��һЩ��ʼ��
	void BeforeTheChangeStatusOfCatInit(CatStatus status, CatMotionStatus motionStatus);
public:
	//è�˶�
	void WalkTo(const LPoint& destination);
	void WalkBy(const LPoint& destination);
	void RunTo(const LPoint& destination);
	void RunBy(const LPoint& destination);
	void RangeMotion();
	//��ʱ����Բ���˶�
	void PlayMotionArc();

	//���÷��򣬼���vec��x��н��жϷ���
	void SetDirection(const LPoint& vec);

	//ͨ��è�ķ�������ȡ��ӦAnimation.txt��è����	ע�ⷵ�ص���վ���������������Ҫת�������߷��������+Cat_Idel_To_Walk
	UINT GetAnimationDataDirection();
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