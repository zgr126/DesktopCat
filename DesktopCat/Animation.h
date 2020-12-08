#pragma once
#include "DesktopCat.h"
#include "Sprite.h"
#include "Timer.h"
#include "LFile.h"
#include "Tool.h"

using namespace Lin;
class LSprite;

#ifndef ParentPath
#define ParentPath				"Assets\\"					//数据文件的父目录
#endif
#ifndef AnimationDataPath
#define AnimationDataPath		"\\AnimationData.txt"		//从此文件中读取精灵尺寸以及总共有多少组动作	(用户提供)
#endif
#ifndef DataMarker
#define DataMarker					'#'			//标记符（用于标记数据与数据之间的间隔）
#endif
#ifndef DataMarkerEnd
#define DataMarkerEnd				'@'			//终止符（用于标记数据结束）
#endif
//行格式
#define FileData_Animation_BeginLine		2		//AnimationData.txt有效数据开始行数
//有效数据参数格式
#define GetFileData_Animation_CountLine		0		//第0个位置参数存放：该动画选自图片的第n行
#define GetFileData_Animation_PlayOrder		1		//第1个位置参数存放：该动画一个周期内帧的切换顺序 如：(0,1,2,3)
#define GetFileData_Animation_StayTime		2		//第2个位置参数存放：第1帧的停留时间...以此类推
#define GetFileData_Animation_Style			3		//第3个位置参数存放：该动画风格，1为周期主导，2为时间主导，3为无限循环播放
#define GetFileData_Animation_AddValue		4		//第4个位置参数存放：附加参数，当Style为1，2时需要指定附加参数，为3时请设为0
#define GetFileData_Animation_Action		5		//第5个位置参数存放：该动画将使用AcionData.txt哪一行的数据

#define M_To_MS		1000.0						//秒转毫秒

//动作播放器
class LAnimation
{
public:
	//静态变量、方法
	static LAnimation* create(LSprite*);
	static void release(LAnimation**);
	//动画类型枚举
	enum class LAnimationStyle
	{
		None,		//用户不可用 只能由系统使用			(当动画刚创建与结束时 赋予的类型)
		Cycle,		//周期主导 比如一个动画只循环1次		(播放次数一到 不管动画播放到哪帧 直接删除 比如：攻击)
		Time,		//时间主导 比如一个动画固定播放1000ms	(时间未到 动画就无限循环播放 比如：跑步)
		UnTime		//无限循环播放						(永不停止 直到动画被打断 比如：静止站立)
	};
private:
	/********************************************/
	//动画数据文件的路径
	string m_AnimationDataFilePath;

	//这是1个动画播放需要的变量
	//该动画选自图片的第m_CountLine行		(从文件获取)
	LFileData<UINT> m_CountLine;
	//一个动画周期的帧切换顺序				(从文件获取)
	vector<LFileData<UINT>> m_vPlayOrder;
	//每帧的停留时间	单位ms				(从文件获取)
	vector<LFileData<UINT>> m_vStayTime;
	//动画类型							(从文件获取)
	LFileData<LAnimationStyle>	m_AnimationStyle;
	//附加数据							(从文件获取)
	LFileData<UINT> m_AddValue;			//当Style为周期主导时，AddValue代表循环次数。为时间主导时，AddValue为动画持续总时间。为无限循环时，AddValue为0
	//调用ActionData.txt哪一行的数据。	(从文件获取)
	LFileData<UINT>	m_ActionTextLine;	//如果为0代表不读取ActionData.txt的内容(即原地不动)
	//播放当前帧的索引	vector的索引
	UINT m_FrameIndex;
	//每帧停留时间的索引	vector的索引
	UINT m_FrameStayTimeIndex;
	//储存每帧停留时间
	UINT m_FrameStyleTime;
	//当前帧停留时间
	UINT m_NowFrameStayTime;
	//当前动画持续时间	(从动画创建完成到现在所经历的时间)
	UINT m_NowAnimationContinue;
	//动画播放是否结束
	bool m_isEnd;
	/*******************************************/
	LSprite* m_Sprite;
private:
	//只能通过create方法创建LAnimation对象
	LAnimation();
	bool init(LSprite*);
	void release();

	//数据审查，将一些数据不全的vector补齐
	void Censor();

	//在动画播放开始之前的一些初始化
	void BeforeTheAnimationOfSpriteInit();
	//在动画播放结束后的一些初始化
	void AfterTheAnimationOfSpriteInit();

	//返回绘制区域(绘制区域是指整个纹理图的某一个精灵位置)	参数LTimer*表示Animation本身没有定时器，是从Sprite身上借来的
	LPoint Update(LTimer* animationTimer);
	//针对周期主导的动画更新
	bool isCycle();
	//针对时间主导的动画更新
	bool isTime(ULONGLONG interval);
	//针对无限时间主导的动画更新
	void isUnTime();
	//切换下一帧
	void NextFrame();
	//切换下一个周期
	void NextCycle();

public:
	//播放文件中第countLine行动画
	bool PlayAnimation(UINT fileLine);

	//访问器
	bool GetisEnd() { return m_isEnd; }
	LAnimationStyle GetAnimationStyle() { return m_AnimationStyle.m_Val; }
	UINT GetActionTextLine() { return  m_ActionTextLine.m_Val; }

	friend class LSprite;
};

typedef LAnimation Animation;