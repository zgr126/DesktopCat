#pragma once
#include "DesktopCat.h"
#include "Node.h"
#include "LoadImage.h"
#include "Timer.h"
#include <deque>

class GlassWindow;
class LAnimation;
class LAction;

class LSprite :public LNode
{
public:
	//静态变量、方法
	static LLoadImage* g_LoadImage;
	static LSprite* create(const string& spriteName, const string& suffix, const LPoint& size);
	static void release(LSprite**);			//调用私有方法release释放成员内的指针 再delete对象
protected:
	//成员变量
	string m_Name;						//精灵名称	(用户提供)
	string m_Suffix;					//文件后缀	(用户提供)	如".png"

	//图片纹理
	ID2D1Bitmap* m_SpriteBitmap;
	//图片纹理画刷
	ID2D1BitmapBrush* m_SpriteBitmapBrush;
	//整个图片资源大小
	D2D1_SIZE_U m_EntireSize;
	//一个精灵的大小
	D2D1_SIZE_U m_SingalSize;
	//被绘制的区域
	LPoint m_Ranks;
	//动画双队列
	deque<LAnimation*> m_AnimationDeque;
	//储存第一个动画
	LAnimation* m_AnimationFront;
	//动画定时器，不在自身使用。而是借给Animation使用
	LTimer* m_AnimationTimer;
	//动画定时器独有ID，1个Sprite对应只有1个ID
	int m_AnimationTimerID;
	//精灵动作
	LAction* m_Action;
	//精灵控制的窗口
	GlassWindow* m_GlassWndow;
protected:
	//重写基类虚函数
	virtual bool init(const string&, const string&, const LPoint& size);
	virtual void release();

protected:
	//保护方法
	void SetDrawArea(LPoint& ranks);	//行、列
public:
	//公开方法
	LSprite();
	virtual void Draw(ID2D1HwndRenderTarget*);
	virtual void Update();

	HRESULT LoadingImage(LPCTSTR, ID2D1RenderTarget*, IWICImagingFactory*);
	//创建一个新的Animation添加进队列的最前面（可以打断动作）如果不指定后面2个参数，则代表使用AnimationData文件，如果指定了后面2个参数则使用代码来指定动画类型和附加参数，一般用于行走，奔跑等需要随机指定位置。
	virtual UINT AddFrontAnimation(UINT fileLine, int animationStyle = 0, UINT addValue = 0);
	//创建一个新的Animation添加进队列的最后面
	virtual void AddBackAnimation(UINT fileLine);
	//动画开始前调用
	virtual void CallAnimationBegin();
	//动画结束后调用
	virtual void CallAnimationEnd();
	//动作被打断调用
	virtual void CallAnimationInterrupt();
public:
	//访问器
	string GetName() { return m_Name; }
	string GetSuffix(){return m_Suffix; }
	D2D1_SIZE_U GetEntireSize() { return m_EntireSize; }
	D2D1_SIZE_U GetSingalSize() { return m_SingalSize; }
	int GetAnimationTimerID() { return m_AnimationTimerID; }
	GlassWindow* GetGlassWindow() { return m_GlassWndow; }

	void SetName(const string& name) { m_Name = name; }
	void SetSuffix(const string& suffix) { m_Suffix = suffix; }
	void SetEntireSize(D2D1_SIZE_U size) { m_EntireSize = size; }
	void SetSingalSize(D2D1_SIZE_U size) { m_SingalSize = size; }
	void SetAnimationTimerID(int id) { m_AnimationTimerID = id; }
	void SetGlassWindow(GlassWindow* glassWindow) { m_GlassWndow = glassWindow; }
};