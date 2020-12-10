#pragma once
#include "DesktopCat.h"
#include "Animation.h"

#ifndef ParentPath
#define ParentPath			"Assets\\"				//数据文件父目录
#endif
#ifndef ActionDataPath
#define ActionDataPath		"\\ActionData.txt"		//精灵移动数据文本文件
#endif
#ifndef DataMarker
#define DataMarker					'#'				//标记符（用于标记数据与数据之间的间隔）
#endif
#ifndef DataMarkerEnd
#define DataMarkerEnd				'@'				//终止符（用于标记数据结束）
#endif
#ifndef GetFileData_Action_BeginLine
#define GetFileData_Action_BeginLine		2		//ActionData.txt文件有效数据开始行数
#endif


class LAction
{
public:
	static LAction* create(LSprite* pSprite, const UINT& countLine);
	static LAction* create(LSprite* pSprite, const int actionStyle, const LPoint& value1, vector<float>& value2);
	static void release(LAction**);
	//运动轨迹样式
	enum class ActionStyle
	{
		Min = -1,	//范围最小值
		None,		//无运动，静止
		ToLine,		//绝对运动
		ByLine,		//相对运动
		Arc,		//曲线运动
		Max			//范围最大值
	};
private:
	//运动样式
	LFileData<ActionStyle> m_ActionStyle;
	//绝对位置，相对位置，旋转半径。这是一个数据包
	vector<LFileData<UINT>> m_Value1;
	//直线运动只有1个参数：time。曲线运动有4个参数：angle，circleAngle，time，trunStyle。角度，旋转圆心与自己的夹角，运动时间，顺逆旋转。	输入时必须是以参数包的形式输入
	vector<LFileData<float>> m_Value2;
	//精灵指针
	LSprite* m_Sprite;
private:
	LAction():m_ActionStyle(ActionStyle(0)) { }
	~LAction() { }

	bool init(LSprite* pSprite, const UINT& countLine);
	bool init(LSprite* pSprite, const int actionStyle, const LPoint& value1, vector<float>& value2);
	void release();

	//数据审查
	void Censor();
public:
	//开始播放动作
	void PlayAction();
};