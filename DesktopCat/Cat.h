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
#define Cat_Idel_To_Walk	(Cat_Idle_Count)	//վ������ת������������
#define Cat_Idel_To_Run		(Cat_Idle_Count + Cat_Walk_Count)	//վ������ת���ܲ�����
#define Cat_Walk_To_Run		(Cat_Walk_Count)
#define Cat_Idel_To_Sit		(Cat_Idel_To_Run + Cat_Run_Count)	//վ��ת����
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
#define Cat_LongIdelTime_Min	4000
#define Cat_LongIdelTime_Max	8500
//è������ʱ�䷶Χ
#define Cat_LongSitTime_Min		10000
#define Cat_LongSitTime_Max		30000
//è���µ�ʱ�䷶Χ
#define Cat_LongLieTime_Min		10000
#define Cat_LongLieTime_Max		30000
//è���Ի�ȦȦ��ʱ�䷶Χ
#define Cat_LongDigTime_Min		10000
#define Cat_LongDigTime_Max		30000
//è��צ�ӵ�ʱ�䷶Χ
#define Cat_LongLickTime_Min	10000
#define Cat_LongLickTime_Max	30000
//è�Ľǳ����ʱ�䷶Χ
#define Cat_LongRelaexTime_Min	10000
#define Cat_LongRelaexTime_Max	30000
//ץ�ذ��ʱ�䷶Χ
#define Cat_LongScratchTime_Min	3000
#define Cat_LongScratchTime_Max	12000
//�ŵ�ʱ�䷶Χ
#define Cat_LongSniffTime_Min	2000
#define Cat_LongSniffTime_Max	7000
//�˷ܵ�ʱ�䷶Χ
#define Cat_LongSwayTime_Min	1500
#define Cat_LongSwayTime_Max	4000
//è˯����ʽ
#define Cat_Sleep_Style	3

#define Cat_Walk_Speed			80		//80����/��
#define Cat_Run_Speed			150		//150����/��

#define Cat_Range_X			70
#define Cat_Range_Y			70


class Cat :public LSprite
{
public:
	static Cat* create(const string& spriteName, const string& suffix, const LPoint& size);
	static void release(Cat**);
	//è״̬ö��
	enum class CatStatus
	{
		Idel,		//վ��
		Motion,		//��·���ܲ�
		Sit,		//����
		Sleep,		//˯��
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
	//è������״̬
	enum class CatOtherStatus
	{
		None,
		//ֵ�Ƕ�ӦAnimationData.txt������
		Sleep		= 34,		//˯��
		Scared		= 40,		//����
		Lie			= 42,		//ſ��
		Dig			= 44,		//���Ի�ȦȦ
		BoringLie	= 46,		//��Ȥ��ſ��
		Lick		= 48,		//��צ��
		Paw			= 49,		//ץ����
		Relaex		= 51,		//�Ľǳ���
		Scratch		= 53,		//ץ�ذ�
		Sniff		= 55,		//��
		Stretch		= 57,		//������
		Sway		= 59,		//�˷�
		Attack		= 61,		//����
	};
#pragma region ��д���෽��	init,release,Draw,Update
protected:
	virtual bool init(const string& spriteName, const string& suffix, const LPoint& size);
	virtual void release();
public:
	virtual void Draw(ID2D1HwndRenderTarget*);
	virtual void Update();

	virtual UINT AddFrontAnimation(UINT fileLine, int animationStyle = 0, UINT addValue = 0, CatStatus status = CatStatus::Other, CatMotionStatus motionStatus = CatMotionStatus::None);
	virtual void AddBackAnimation(UINT fileLine);

	virtual void CallAnimationBegin();
	virtual void CallAnimationEnd();
	virtual void CallAnimationInterrupt();
#pragma endregion

private:
	//è״̬
	CatStatus m_Status;
	//è״̬��Otherʱ����¼��һ��Other
	CatOtherStatus m_OtherStatus;
	//è�˶�״̬
	CatMotionStatus m_MotionStatus;
	//è����
	CatDirection m_Direction;
	//è������ʱ��
	LTimer* m_IdelTimer;
	//��ǰ״̬�ĳ���ʱ��
	UINT m_ContinueTime;
	//è�Ӷ�����ʼ�����ڵ�ʱ��
	UINT m_NowContinueTime;
	//è�ƶ���Χ
	LPoint m_MoveRange;
	//�Ƿ�����
	bool m_isCommand;
private:
	void MotionTo(const LPoint& destination, double speed, int actionDataLine);
	//�ı�è״̬֮ǰ��һЩ��ʼ��
	void BeforeTheChangeStatusOfCatInit(CatStatus status, CatMotionStatus motionStatus);

	//״̬����
	void UpdateIdel();
	void UpdateMotion();
	void UpdateSit();
	void UpdateSleep();
	void UpdateOther();
	void OnClick();			//�������
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
	//��ȡèˮƽ�����ϵķ���
	CatDirection GetAnimationHorizontal();
	//��ȡè��ֱ�����ϵķ���
	CatDirection GetAnimationVertical();
	//�Զ��ж�CatOtherStatus״̬��������Ӧ����
	void AutoSetOtherStatus();

	//��è�Ľ���
	//����è
	void Call();
	//����
	void Scared();
	//����è�Ľ���
	//վ��
	void Idel();
	//����
	void Sit(UINT addValue);
	//˯��	˯����ʽ����3��	���������˯��
	void Sleep(UINT sleepStyle);
	//ſ��	ſ����ʽ��0Ϊſ�£�1Ϊ���ĵ�ſ��
	void Lie(UINT lieStyle, UINT addValue);
	void BoringLie(UINT addValue);
	//���Ի�ȦȦ
	void Dig(UINT addValue);
	//��צ��
	void Lick(UINT addValue);
	//ץ����
	void Paw(UINT addValue);
	//�Ľǳ���
	void Relaex(UINT addValue);
	//ץ�ذ�
	void Scratch(UINT addValue);
	//��
	void Sniff(UINT addValue);
	//������
	void Stretch();
	//�˷�		�������ͣ����������ڻ���ʱ���������ߴ�������
	void Sway(UINT addValue);
	//����
	void Attack(UINT addValue);
};