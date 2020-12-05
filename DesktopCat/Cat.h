#pragma once
#include "GlassWindows.h"
#include "Sprite.h"
#include "Animation.h"
#include "Timer.h"

class GlassWindow;

#define Cat_Idel_TimerID			10		//猫发呆定时器的固定ID

class Cat :public LSprite
{
public:
	//静态变量、方法
	static Cat* create(string& _SpriteName, string _Suffix, LPoint& _Size);
	static void release(Cat**);

public:
	//枚举
	enum class HousePetDirection
	{		//角色朝向(8个方向)
		Top, Bottom, Left, Right,
		LeftTop, RightTop, LeftBottom, RightBottom,
		Null,	//Null表示无朝向 特殊动作结束时会设置Null以便AI可以朝随意方向运动（比如朝左伸懒腰结束后 可以往右走）
	};
	enum class HousePetMode
	{			//状态
		Idel,		//发呆
		Move,		//运动
		Special		//专有动作
	};
	typedef enum HousePetPrefabAnimation
	{	//角色预制动画	(枚举值对应文件ActionData文件中动画数据的标号)
		Attack,		//抓虫子 
		Balnk,		//眨眼（正坐）
		Dig,		//背对着抓地板
		Ennui,		//趴在地上摇尾巴
		Fly,		//趴在地上无精打采
		Lick,		//舔爪子
		Paw,		//抓一下地板
		Relax,		//四脚朝天
		Scratch,	//刮地板
		Sleep1,		//蜷缩身体睡觉
		Sleep2,		//打呼噜
		Sleep3,		//困了揉眼睛
		Sniff,		//嗅（站着）
		Stretch,	//伸懒腰
		Sway,		//兴奋地摇尾巴
		Tail,		//背对着发呆（摇尾巴）
		BottomRun,		//奔跑	↓
		LeftRun,		//奔跑	←
		RightRun,		//奔跑	→
		TopRun,			//奔跑	↑
		LeftBottomRun,	//奔跑	↙
		RightBottomRun,	//奔跑	↘
		RightTopRun,	//奔跑	↗
		LeftTopRun,		//奔跑	↖
		BottomWalk,		//走路	↓
		LeftWalk,		//走路	←
		RightWalk,		//走路	→
		TopWalk,		//走路	↑
		LeftBottomWalk,	//走路	↙
		RightBottomWalk,//走路	↘
		RightTopWalk,	//走路	↗
		LeftTopWalk,	//走路	↖
		BottomIdel,		//站立	↓
		LeftIdel,		//站立	←
		RightIdel,		//站立	→
		TopIdel,		//站立	↑
		LeftBottomIdel,	//站立	↙
		RightBottomIdel,//站立	↘
		RightTopIdel,	//站立	↗
		LeftTopIdel,	//站立	↖
	} HPA;
protected:
	//成员变量
	Animation* m_Animation;
	HousePetDirection m_Direction;	//当前朝向
	HousePetMode m_PetMode;			//运动状态
	GlassWindow* m_Window;			//宠物窗口句柄
	LTimer* m_IdelTime;				//发呆时间
protected:
	//保护方法
	virtual bool init(string& _SpriteName, string _Suffix, LPoint& _Size);
	virtual void release();
public:
	//公开方法
	Cat();

	virtual void Draw(ID2D1HwndRenderTarget*);
	virtual void Draw() { }
	virtual void Update();

	/*
		_FileLine	读取文件第_FileLine个动画数据	(从0开始)
		_Style		动画类型
		_Val		当_Style为Cycle时	_Val为动画循环次数
					当_Style为Time时		_Val为动画固定播放时间
					当_Style为UnTime时	_Val可以是任意值 因为_Val会被忽略*/
	void PlayAnimation(int _FileLine, LAnimation::LAnimationStyle _Style, DWORD _Val, Cat::HousePetDirection _Direction);

private:
	void AI();		//动画播完时 交给AI控制
	void Idel();	//发呆

public:
	GlassWindow* SetGlassWindow(GlassWindow* GWindow) { m_Window = GWindow; }
};