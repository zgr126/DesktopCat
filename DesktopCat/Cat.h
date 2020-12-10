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
#define Cat_Idel_To_Walk	(Cat_Idle_Count)					//站立行数转换到行走行数
#define Cat_Idel_To_Run		(Cat_Idle_Count + Cat_Walk_Count)	//站立行数转到跑步行数
#define Cat_Walk_To_Run		(Cat_Walk_Count)
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


//猫发呆一次时间的范围
#define Cat_IdelEntireTime_Min	4000
#define Cat_IdelEntireTime_Max	8500

#define Cat_Walk_Speed			80		//80像素/秒
#define Cat_Run_Speed			150		//150像素/秒

#define Cat_Range_X			70
#define Cat_Range_Y			70


class Cat :public LSprite
{
public:
	static Cat* create(const string& spriteName, const string& suffix, const LPoint& size);
	static void release(Cat**);
#pragma region 重写基类方法	init,release,Draw,Update
protected:
	virtual bool init(const string& spriteName, const string& suffix, const LPoint& size);
	virtual void release();
public:
	virtual void Draw(ID2D1HwndRenderTarget*);
	virtual void Update();
#pragma endregion

	//猫状态枚举
	enum class CatStatus
	{
		Idel,		//站立
		Motion,		//走路或跑步
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
private:
	//猫状态
	CatStatus m_Status;
	//猫运动状态
	CatMotionStatus m_MotionStatus;
	//猫朝向
	CatDirection m_Direction;
	//猫发呆定时器
	LTimer* m_IdelTimer;
	//猫发呆一次的时间(随机1-5秒)
	UINT m_EntireIdelTime;
	//猫从动画开始到现在的时间
	UINT m_NowContinueTime;
	//猫走路/运动时间，通过计算得出
	UINT m_MotionTime;
	//猫移动范围
	LPoint m_MoveRange;
private:
	void MotionTo(const LPoint& destination, double speed, int actionDataLine);
	//改变猫状态之前的一些初始化
	void BeforeTheChangeStatusOfCatInit(CatStatus status, CatMotionStatus motionStatus);
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
};





//#include "Animation.h"
//#include "Timer.h"
//
//class GlassWindow;
//
//#define Cat_Idel_TimerID			10		//猫发呆定时器的固定ID
//
//class Cat :public LSprite
//{
//public:
//	//静态变量、方法
//	static Cat* create(string& _SpriteName, string _Suffix, LPoint& _Size);
//	static void release(Cat**);
//
//public:
//	//枚举
//	enum class HousePetDirection
//	{		//角色朝向(8个方向)
//		Top, Bottom, Left, Right,
//		LeftTop, RightTop, LeftBottom, RightBottom,
//		None,	//Null表示无朝向 特殊动作结束时会设置Null以便AI可以朝随意方向运动（比如朝左伸懒腰结束后 可以往右走）
//	};
//	enum class HousePetMode
//	{			//状态
//		Idel,		//发呆
//		Move,		//运动
//		Special		//专有动作
//	};
//	typedef enum HousePetPrefabAnimation
//	{	//角色预制动画	(枚举值对应文件ActionData文件中动画数据的标号)
//		Attack,		//抓虫子 
//		Balnk,		//眨眼（正坐）
//		Dig,		//背对着抓地板
//		Ennui,		//趴在地上摇尾巴
//		Fly,		//趴在地上无精打采
//		Lick,		//舔爪子
//		Paw,		//抓一下地板
//		Relax,		//四脚朝天
//		Scratch,	//刮地板
//		Sleep1,		//蜷缩身体睡觉
//		Sleep2,		//打呼噜
//		Sleep3,		//困了揉眼睛
//		Sniff,		//嗅（站着）
//		Stretch,	//伸懒腰
//		Sway,		//兴奋地摇尾巴
//		Tail,		//背对着发呆（摇尾巴）
//		BottomRun,		//奔跑	↓
//		LeftRun,		//奔跑	←
//		RightRun,		//奔跑	→
//		TopRun,			//奔跑	↑
//		LeftBottomRun,	//奔跑	↙
//		RightBottomRun,	//奔跑	↘
//		RightTopRun,	//奔跑	↗
//		LeftTopRun,		//奔跑	↖
//		BottomWalk,		//走路	↓
//		LeftWalk,		//走路	←
//		RightWalk,		//走路	→
//		TopWalk,		//走路	↑
//		LeftBottomWalk,	//走路	↙
//		RightBottomWalk,//走路	↘
//		RightTopWalk,	//走路	↗
//		LeftTopWalk,	//走路	↖
//		BottomIdel,		//站立	↓
//		LeftIdel,		//站立	←
//		RightIdel,		//站立	→
//		TopIdel,		//站立	↑
//		LeftBottomIdel,	//站立	↙
//		RightBottomIdel,//站立	↘
//		RightTopIdel,	//站立	↗
//		LeftTopIdel,	//站立	↖
//	} HPA;
//protected:
//	//成员变量
//	Animation* m_Animation;
//	HousePetDirection m_Direction;	//当前朝向
//	HousePetMode m_PetMode;			//运动状态
//	GlassWindow* m_Window;			//宠物窗口句柄
//	LTimer* m_IdelTime;				//发呆时间
//protected:
//	//保护方法
//	virtual bool init(string& _SpriteName, string _Suffix, LPoint& _Size);
//	virtual void release();
//public:
//	//公开方法
//	Cat();
//
//	virtual void Draw(ID2D1HwndRenderTarget*);
//	virtual void Update();
//
//	/*
//		_FileLine	读取文件第_FileLine个动画数据	(从0开始)
//		_Style		动画类型
//		_Val		当_Style为Cycle时	_Val为动画循环次数
//					当_Style为Time时		_Val为动画固定播放时间
//					当_Style为UnTime时	_Val可以是任意值 因为_Val会被忽略*/
//	void PlayAnimation(int _FileLine, LAnimation::LAnimationStyle _Style, DWORD _Val, Cat::HousePetDirection _Direction);
//
//private:
//	void AI();		//动画播完时 交给AI控制
//	void Idel();	//发呆
//
//public:
//	GlassWindow* SetGlassWindow(GlassWindow* GWindow) { m_Window = GWindow; }
//};