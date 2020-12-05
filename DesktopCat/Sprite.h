#pragma once
#include "DesktopCat.h"
#include "Node.h"
#include "LoadImage.h"


class LAction;


class LSprite :public LNode
{
public:
	//静态变量、方法
	static LLoadImage* g_LoadImage;
	static void release(LSprite*);			//调用私有方法release释放成员内的指针 再delete对象
protected:
	//成员变量
	string m_Name;						//精灵名称	(用户提供)
	string m_Suffix;					//文件后缀	(用户提供)	如".png"

	ID2D1Bitmap* m_SpriteBitmap;
	ID2D1BitmapBrush* m_SpriteBitmapBrush;

	D2D1_SIZE_U m_EntireSize;			//整个图片资源大小
	D2D1_SIZE_U m_SingalSize;			//一个精灵的大小

	LPoint m_Ranks;				//被绘制的区域

protected:
	//重写基类虚函数
	virtual bool init(std::string, std::string, LPoint& _Size);
	virtual void release();

protected:
	//保护方法
	void SetdrawArea(LPoint& _Ranks);	//行、列
public:
	//公开方法
	LSprite();
	virtual void Draw() = 0;
	virtual void Update() = 0;

	HRESULT LoadingImage(LPCTSTR, ID2D1RenderTarget*, IWICImagingFactory*);

public:
	//访问器
	string GetName() { return m_Name; }
	string GetSuffix(){return m_Suffix; }
	D2D1_SIZE_U GetEntireSize() { return m_EntireSize; }
	D2D1_SIZE_U GetSingalSize() { return m_SingalSize; }

	void SetName(const string& name) { m_Name = name; }
	void SetSuffix(const string& suffix) { m_Suffix = suffix; }
	void SetEntireSize(D2D1_SIZE_U size) { m_EntireSize = size; }
	void SetSingalSize(D2D1_SIZE_U size) { m_SingalSize = size; }

	friend LAction;

};