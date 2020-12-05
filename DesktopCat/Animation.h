#pragma once
#include "DesktopCat.h"
#include "Node.h"
class LSprite;

#define DataParent		"File\\"				//数据文件的父目录
#define ActionData		"\\ActionData.txt"		//从此文件中读取精灵尺寸以及总共有多少组动作	(用户提供)
//行格式
#define VaildData			2					//有效数据开始行数（动作组总数：即整张图片有多少组动作）

//有效数据参数格式
#define VaildData_AnimationPlayLine		0		//第1个位置的参数存放：该动画选自图片的第n行
#define VaildData_AnimationOrder		1		//第2个位置的参数存放：该动画一个周期内帧的切换顺序 如：(0,1,2,3)
#define VaildData_AnimationDwellTime	2		//第3个位置的参数存放：第1帧的停留时间...以此类推

#define M_To_MS		1000.0						//秒转毫秒

//动作播放器
class LAnimation
{
public:
	enum class LAnimationStyle
	{
		Null,		//用户不可用 只能由系统使用			(当动画刚创建与结束时 赋予的类型)
		Cycle,		//周期主导 比如一个动画只循环1次		(播放次数一到 不管动画播放到哪帧 直接删除 比如：攻击)
		Time,		//时间主导 比如一个动画固定播放1000ms	(时间未到 动画就无限循环播放 比如：跑步)
		UnTime		//无限循环播放						(永不停止 比如：静止站立)
	};
	LAnimationStyle m_AnimationStyle;			//记录动画类型
	//静态变量、方法
	static LAnimation* create(LSprite*);
	static void release(LAnimation**);
private:
	/********************************************/
	//这是管理动画需要的变量
	string m_AnimationDataFile;	//动画数据文件的路径
	//这是1个动画播放需要的变量
	UINT m_PlayLine;			//该动画选自图片的第m_PlayLine行	(从文件获取)
	vector<float> m_FrameOrder;	//一个动画周期的帧切换顺序			(从文件获取)
	vector<float> m_DwellTime;	//帧与帧之间的间隔时间			(从文件获取)
	UINT m_FrameNumber;			//动画1个周期的总帧数	
	UINT m_NowFrame;			//当前帧
	DWORD m_BeginPlayTime;		//帧开始时间
	DWORD m_NowFrameTime;		//帧现在时间
	DWORD m_EndFrameTime;		//帧结束时间
	DWORD m_PlayTime;			//动画持续时间
	int m_PlayNumber;			//播放次数
	bool m_isEnd;				//动画播放是否结束
	/*******************************************/
private:
	bool init(LSprite*);
	void release();

	void updateFrame();				//更新帧的状态
	void updataFrameTime();			//当切换下一帧时 更新帧计时
	void AnimationBegin();			//动画播放开始时调用
	void AnimationEnd();			//动画播放结束时调用
	void Clear();					//归零动画数据 然后被新动作数据覆盖

	//读取vector中第_Position位置的参数存放到_Param中
	template <typename T>
	void ReadVectorData(vector<float>&, int _Index, T& _Param);

public:
	//公开方法
	LAnimation();

	LPoint update();	//返回现在播放第几帧

	//切换动画 不会删除	 会覆盖上个动作的数据	
	void Cutover(int _FileLine, LAnimationStyle _Style, DWORD _Val); //详细注释见LPet::PlayAnimation方法

public:
	//访问器
	UINT GetFrameNumber() { return m_FrameNumber; }
	UINT GetNowFrame() { return m_NowFrame; }
	bool GetisEnd() { return m_isEnd; }
};

typedef LAnimation Animation;