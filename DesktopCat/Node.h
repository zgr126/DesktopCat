#pragma once
#include "DesktopCat.h"
struct LPoint final
{
	float m_x;
	float m_y;
	LPoint() :m_x(0), m_y(0) { };
	LPoint(float _x, float _y) :m_x(_x), m_y(_y) { }
	LPoint(float _val) :m_x(_val), m_y(_val) { }
	LPoint operator+(const LPoint& _pt);
	LPoint operator-(const LPoint& _pt);
	LPoint operator*(const LPoint& _pt);
	LPoint operator*(const float& _val);
	LPoint operator/(const LPoint& _pt);
	LPoint operator/(const float& _val);
	bool operator==(const LPoint& _pt);
	bool operator==(const float& _val);
	bool operator!=(const LPoint& _pt);
	bool operator!=(const float& _val);
	void SetLPoint(LPoint& _pt) { *this = _pt; }
};

class LNode
{
protected:
	//成员变量
	LPoint m_Position;			//位置
	LPoint m_Anchor;			//锚点默认(0.5,0.5)	(0,0)在左上角
protected:
	//虚函数
	virtual bool init();
	virtual void release();
public:
	//公开方法
	LNode();

	virtual void Draw(ID2D1HwndRenderTarget*) = 0;
	virtual void Update() = 0;

public:
	//访问器
	LNode* GetNode() { return this; }
	LPoint GetPosition() { return m_Position; }
	LPoint GetAnchor() { return m_Anchor; }

	void SetPosition(const LPoint& point) { m_Position = point; }
	void SetAnchor(const LPoint& anchor) { m_Anchor = anchor; }

};

template<class T>
static void release(T);