#pragma once
#include "DesktopCat.h"
#include "Sprite.h"
#include "Animation.h"
#include "Action.h"

#define Cat_AnimationTimer_ID	10		//猫动画定时器独有ID
#define Cat_IdelTimer_ID		11		//猫发呆定时器

//此宏对应Animation文件行数
#pragma region 猫朝向宏定义
//站立
#define Cat_Front_Idle			(FileData_Animation_BeginLine + 0)		//面朝屏幕		站立
#define Cat_Left_Idle			(FileData_Animation_BeginLine + 1)		//向左			站立
#define Cat_Right_Idle			(FileData_Animation_BeginLine + 2)		//向右			站立
#define Cat_Back_Idle			(FileData_Animation_BeginLine + 3) 		//背对屏幕		站立
#define Cat_FrontLeft_Idle		(FileData_Animation_BeginLine + 4)		//面向屏幕朝左	站立
#define Cat_FrontRight_Idle		(FileData_Animation_BeginLine + 5)		//面向屏幕朝右	站立
#define Cat_BackRight_Idle		(FileData_Animation_BeginLine + 6)		//背对屏幕朝右	站立
#define Cat_BackLeft_Idle		(FileData_Animation_BeginLine + 7)		//背对屏幕朝左	站立
#define Cat_Idle_Min		Cat_Front_Idle
#define Cat_Idle_Max		Cat_BackLeft_Idle	//Idle范围
#define Cat_Idle_Count		8					//Idle个数
//走路
#define Cat_Front_Walk			(Cat_Idle_Max + 1)						//面朝屏幕		走路
#define Cat_Left_Walk			(Cat_Idle_Max + 2)						//向左			走路
#define Cat_Right_Walk			(Cat_Idle_Max + 3)						//向右			走路
#define Cat_Back_Walk			(Cat_Idle_Max + 4)				 		//背对屏幕		走路
#define Cat_FrontLeft_Walk		(Cat_Idle_Max + 5)						//面向屏幕朝左	走路
#define Cat_FrontRight_Walk		(Cat_Idle_Max + 6)						//面向屏幕朝右	走路
#define Cat_BackRight_Walk		(Cat_Idle_Max + 7)						//背对屏幕朝右	走路
#define Cat_BackLeft_Walk		(Cat_Idle_Max + 8)						//背对屏幕朝左	走路
#define Cat_Walk_Min		Cat_Front_Walk
#define Cat_Walk_Max		Cat_BackLeft_Walk	//Walk范围
#define Cat_Walk_Count		8					//Walk个数
//跑步
#define Cat_Front_Run			(Cat_Walk_Max + 1)						//面朝屏幕		跑步
#define Cat_Left_Run			(Cat_Walk_Max + 2)						//向左			跑步
#define Cat_Right_Run			(Cat_Walk_Max + 3)						//向右			跑步
#define Cat_Back_Run			(Cat_Walk_Max + 4)				 		//背对屏幕		跑步
#define Cat_FrontLeft_Run		(Cat_Walk_Max + 5)						//面向屏幕朝左	跑步
#define Cat_FrontRight_Run		(Cat_Walk_Max + 6)						//面向屏幕朝右	跑步
#define Cat_BackRight_Run		(Cat_Walk_Max + 7)						//背对屏幕朝右	跑步
#define Cat_BackLeft_Run		(Cat_Walk_Max + 8)						//背对屏幕朝左	跑步
#define Cat_Run_Min			Cat_Front_Run
#define Cat_Run_Max			Cat_BackLeft_Run	//Run范围
#define Cat_Run_Count		8					//Run个数
//转换。直接+宏定义就算转换完成
#define Cat_Idel_To_Walk	(Cat_Idle_Count)	//站立行数转换到行走行数
#define Cat_Idel_To_Run		(Cat_Idle_Count + Cat_Walk_Count)	//站立行数转到跑步行数
#define Cat_Walk_To_Run		(Cat_Walk_Count)
#define Cat_Idel_To_Sit		(Cat_Idel_To_Run + Cat_Run_Count)	//站立转坐姿
#pragma endregion

#pragma region 一些三角函数值
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

#pragma region 猫的Other动作时间范围
//猫发呆一次时间的范围
#define Cat_LongIdelTime_Min	3500
#define Cat_LongIdelTime_Max	8000
//猫久坐的时间范围
#define Cat_LongSitTime_Min		12000
#define Cat_LongSitTime_Max		30000
//猫躺下的时间范围
#define Cat_LongLieTime_Min		10000
#define Cat_LongLieTime_Max		25000
//猫背对画圈圈的时间范围
#define Cat_LongDigTime_Min		10000
#define Cat_LongDigTime_Max		25000
//猫舔爪子的时间范围
#define Cat_LongLickTime_Min	10000
#define Cat_LongLickTime_Max	30000
//猫四角朝天的时间范围
#define Cat_LongRelaexTime_Min	7000
#define Cat_LongRelaexTime_Max	20000
//抓地板的时间范围
#define Cat_LongScratchTime_Min	3000
#define Cat_LongScratchTime_Max	12000
//闻的时间范围
#define Cat_LongSniffTime_Min	2000
#define Cat_LongSniffTime_Max	4500
//兴奋的时间范围
#define Cat_LongSwayTime_Min	1500
#define Cat_LongSwayTime_Max	2500
#pragma endregion
//猫睡觉样式
#define Cat_Sleep_Style	3

#pragma region 猫的一些动作转换概率
//走路转换站立或者其他的概率
#define Walk_To_Idel		90		//走路转换站立的概率
#define Walk_To_Other		10		//走路转换其他动作的概率
//Other类型每个动作发生的概率
#define Lie_Probability			15	//趴下发生的概率
#define BoringLie_Probability	10	//无聊地趴下发生的概率
#define Lick_Probability		25	//舔爪子发生的概率
#define Scratch_Probability		25	//抓地板发生的概率
#define Sniff_Probability		25	//闻发生的概率
//久坐后会发生一些事情的概率
#define LongSit_To_Lie_Probability		10		//久坐后趴下的概率
#define LongSit_To_Boring_Probability	5		//久坐后无聊地趴下的概率
#define LongSit_To_Dig_Probability		20		//久坐后背对画圈圈的概率
#define LongSit_To_Lick_Probability		20		//久坐后舔抓的概率
#define LongSit_To_Paw_Probability		20		//久坐后抓虫子的概率
#define LongSit_To_Relaex_Probability	20		//久坐后四角朝天的概率
#define LongSit_To_Sleep_Probability	5		//久坐后睡觉的概率
//站立时被点击发生一些事情的概率
#define OnClick_Idel_To_Sniff_Probability	20	//站立时被点击，触发闻的概率
#define OnClick_Idel_To_Lie_Probability		10	//站立时被点击，触发趴下的概率
#define OnClick_Idel_To_Relaex_Probability	10	//站立时被点击，触发四角朝天的概率
#define OnClick_Idel_To_Sway_Probability	30	//站立时被点击，触发兴奋的概率
#define OnClick_Idel_To_Attack_Probability	30	//站立时被点击，触发攻击的概率
//坐下时被点击发生一些事情的概率
#define OnClick_Sit_To_Stretch_Probability	50	//坐下时被点击，触发伸懒腰的概率
#define OnClick_Sit_To_Scared_Probability	50	//坐下时被点击，触发惊吓的概率
//睡觉时被点击发生一些事情的概率
#define OnClick_Sleep_To_Stretch_Probability	75	//睡觉时被点击，触发惊吓的概率
#define OnClick_Sleep_To_Scared_Probability		25	//睡觉时被点击，触发惊吓的概率
//趴下转换概率
#define Lie_To_BoringLie_Probability	50		//趴下转无聊地趴下的概率
#define Lie_To_Sleep_Probability		35		//趴下转睡觉的概率
#define Lie_To_Lie_Probability			15		//继续趴下的概率
//无聊地趴下转换概率
#define BoringLie_To_BoringLie_Probability	15	//继续无聊地趴下的概率			
#define BoringLie_To_Sleep_Probability		80	//转睡觉的概率

#pragma endregion


#define Cat_Walk_Speed			80		//80像素/秒
#define Cat_Run_Speed			150		//150像素/秒

#define Cat_Range_X			70
#define Cat_Range_Y			70


class Cat :public LSprite
{
public:
	static Cat* create(const string& spriteName, const string& suffix, const LPoint& size);
	static void release(Cat**);
	//猫状态枚举
	enum class CatStatus
	{
		Test,		//测试
		Idel,		//站立
		Motion,		//走路或跑步
		Sit,		//坐下
		Sleep,		//睡觉
		Other		//其他
	};
	enum class CatMotionStatus
	{
		None, Walk, Run
	};
	//猫方向枚举
	enum class CatDirection
	{
		Front = 10,					//面朝屏幕
		Back = -10,					//背对屏幕
		Left = -1,
		Right = 1,
		FrontLeft = Front + Left,	//面朝屏幕向左
		FrontRight = Front + Right,	//面朝屏幕向右
		BackRight = Back + Right,
		BackLeft = Back + Left
	};
	//猫的其他状态
	enum class CatOtherStatus
	{
		None,
		//值是对应AnimationData.txt的行数
		Sleep		= 35,		//睡觉
		Scared		= 41,		//惊吓
		Lie			= 43,		//趴下
		Dig			= 45,		//背对画圈圈
		BoringLie	= 47,		//无趣的趴下
		Lick		= 49,		//舔爪子
		Paw			= 50,		//抓虫子
		Relaex		= 52,		//四角朝天
		Scratch		= 54,		//抓地板
		Sniff		= 56,		//闻
		Stretch		= 58,		//伸懒腰
		Sway		= 60,		//兴奋
		Attack		= 62,		//攻击
	};
#pragma region 重写基类方法	init,release,Draw,Update
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
	//猫状态
	CatStatus m_Status;
	//猫状态在Other时，记录哪一种Other
	CatOtherStatus m_OtherStatus;
	//猫运动状态
	CatMotionStatus m_MotionStatus;
	//猫朝向
	CatDirection m_Direction;
	//猫发呆定时器
	LTimer* m_IdelTimer;
	//当前状态的持续时间
	UINT m_ContinueTime;
	//猫从动画开始到现在的时间
	UINT m_NowContinueTime;
	//猫移动范围
	LPoint m_MoveRange;
	//是否被命令
	bool m_isCommand;
private:
	void MotionTo(const LPoint& destination, double speed, int actionDataLine);
	//改变猫状态之前的一些初始化
	void BeforeTheChangeStatusOfCatInit(CatStatus status, CatMotionStatus motionStatus);

	//状态更新
	void UpdateIdel();
	void UpdateMotion();
	void UpdateSit();
	void UpdateSleep();
	void UpdateOther();
public:
	//猫运动
	void WalkTo(const LPoint& destination);
	void WalkBy(const LPoint& destination);
	void RunTo(const LPoint& destination);
	void RunBy(const LPoint& destination);
	void RangeMotion();
	//暂时不做圆周运动
	void PlayMotionArc();

	//设置方向，计算vec与x轴夹角判断方向
	void SetDirection(const LPoint& vec);

	//通过猫的方向来获取对应Animation.txt的猫方向	注意返回的是站立方向的行数，若要转换成行走方向的行数+Cat_Idel_To_Walk
	UINT GetAnimationDataDirection();
	//获取猫水平方向上的方向
	CatDirection GetAnimationHorizontal();
	//获取猫竖直方向上的方向
	CatDirection GetAnimationVertical();
	//自动判断CatOtherStatus状态并设置相应动画
	void AutoSetOtherStatus();
	void OnClick();			//被鼠标点击

	//与猫的交互
	//呼叫猫
	void Call();
	//惊吓
	void Scared();
	//非与猫的交互
	//站立
	void Idel();
	//坐下
	void Sit(UINT addValue);
	//睡觉	睡觉样式：有3种	这个是无限睡觉
	void Sleep(UINT sleepStyle);
	//趴下	趴的样式：0为趴下，1为无聊的趴下
	void Lie(UINT lieStyle, UINT addValue);
	void BoringLie(UINT addValue);
	//背对画圈圈
	void Dig(UINT addValue);
	//舔爪子
	void Lick(UINT addValue);
	//抓虫子
	void Paw(UINT addValue);
	//四角朝天
	void Relaex(UINT addValue);
	//抓地板
	void Scratch(UINT addValue);
	//闻
	void Sniff(UINT addValue);
	//伸懒腰
	void Stretch();
	//兴奋		动画类型，可以是周期或者时间主导或者次数主导
	void Sway(UINT addValue);
	//攻击
	void Attack(UINT addValue);
};