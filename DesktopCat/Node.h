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
	//��Ա����
	LPoint m_Position;			//λ��
	LPoint m_Anchor;			//ê��Ĭ��(0.5,0.5)	(0,0)�����Ͻ�
protected:
	//�麯��
	virtual bool init();
	virtual void release();
public:
	//��������
	LNode();

	virtual void Draw(ID2D1HwndRenderTarget*) = 0;
	virtual void Update() = 0;

public:
	//������
	LNode* GetNode() { return this; }
	LPoint GetPosition() { return m_Position; }
	LPoint GetAnchor() { return m_Anchor; }

	void SetPosition(const LPoint& point) { m_Position = point; }
	void SetAnchor(const LPoint& anchor) { m_Anchor = anchor; }

};

template<class T>
static void release(T);