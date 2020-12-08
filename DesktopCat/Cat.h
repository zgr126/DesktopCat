#pragma once
#include "DesktopCat.h"
#include "Sprite.h"
#include "Animation.h"

#define Cat_AnimationTimer_ID	10		//猫动画定时器独有ID
#define Cat_IdelTimer_ID		11		//猫发呆定时器

//此宏对应Animation文件行数
#pragma region 猫朝向宏定义
//站立
#define Cat_Front_Idle			(FileData_Animation_BeginLine + 1)		//面朝屏幕		站立
#define Cat_Back_Idle			(FileData_Animation_BeginLine + 2) 		//背对屏幕		站立
#define Cat_Left_Idle			(FileData_Animation_BeginLine + 3)		//向左			站立
#define Cat_Right_Idle			(FileData_Animation_BeginLine + 4)		//向右			站立
#define Cat_FrontLeft_Idle		(FileData_Animation_BeginLine + 5)		//面向屏幕朝左	站立
#define Cat_BackLeft_Idle		(FileData_Animation_BeginLine + 6)		//背对屏幕朝左	站立
#define Cat_FrontRight_Idle		(FileData_Animation_BeginLine + 7)		//面向屏幕朝右	站立
#define Cat_BackRight_Idle		(FileData_Animation_BeginLine + 8)		//背对屏幕朝右	站立
#define Cat_Idle_Min		Cat_Front_Idle
#define Cat_Idle_Max		Cat_BackRight_Idle	//Idle范围
//走路
#define Cat_Front_Walk			(Cat_Idle_Max + 1)						//面朝屏幕		走路
#define Cat_Back_Walk			(Cat_Idle_Max + 2)				 		//背对屏幕		走路
#define Cat_Left_Walk			(Cat_Idle_Max + 3)						//向左			走路
#define Cat_Right_Walk			(Cat_Idle_Max + 4)						//向右			走路
#define Cat_FrontLeft_Walk		(Cat_Idle_Max + 5)						//面向屏幕朝左	走路
#define Cat_BackLeft_Walk		(Cat_Idle_Max + 6)						//背对屏幕朝左	走路
#define Cat_FrontRight_Walk		(Cat_Idle_Max + 7)						//面向屏幕朝右	走路
#define Cat_BackRight_Walk		(Cat_Idle_Max + 8)						//背对屏幕朝右	走路
#define Cat_Walk_Min		Cat_Front_Walk
#define Cat_Walk_Max		Cat_BackRight_Walk	//Walk范围
//跑步
#define Cat_Front_Run			(Cat_Walk_Max + 1)						//面朝屏幕		跑步
#define Cat_Back_Run			(Cat_Walk_Max + 2)				 		//背对屏幕		跑步
#define Cat_Left_Run			(Cat_Walk_Max + 3)						//向左			跑步
#define Cat_Right_Run			(Cat_Walk_Max + 4)						//向右			跑步
#define Cat_FrontLeft_Run		(Cat_Walk_Max + 5)						//面向屏幕朝左	跑步
#define Cat_BackLeft_Run		(Cat_Walk_Max + 6)						//背对屏幕朝左	跑步
#define Cat_FrontRight_Run		(Cat_Walk_Max + 7)						//面向屏幕朝右	跑步
#define Cat_BackRight_Run		(Cat_Walk_Max + 8)						//背对屏幕朝右	跑步
#define Cat_Run_Min			Cat_Front_Run
#define Cat_Run_Max			Cat_BackRight_Run	//Run范围
#pragma endregion

//猫发呆一次时间的范围
#define Cat_IdelEntireTime_Min	1000
#define Cat_IdelEntireTime_Max	5000


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
		Walk,		//走路
		Run,		//跑步
		Other		//其他
	};
private:
	//猫状态
	CatStatus m_Status;
	//猫发呆定时器
	LTimer* m_IdelTimer;
	//猫发呆一次的时间(随机1-5秒)
	UINT m_EntireIdelTime;
	//猫从发呆开始到现在的时间
	UINT m_NowIdelTime;
public:

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