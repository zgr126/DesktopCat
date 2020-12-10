#include "Node.h"
#include <math.h>

LPoint LPoint::operator+(const LPoint& _pt)const
{
	LPoint pt;
	pt.m_x = this->m_x + _pt.m_x;
	pt.m_y = this->m_y + _pt.m_y;
	return pt;
}
LPoint LPoint::operator-(const LPoint& _pt)const
{
	LPoint pt;
	pt.m_x = this->m_x - _pt.m_x;
	pt.m_y = this->m_y - _pt.m_y;
	return pt;
}
LPoint LPoint::operator*(const LPoint& _pt)const
{
	LPoint pt;
	pt.m_x = this->m_x * _pt.m_x;
	pt.m_y = this->m_y * _pt.m_y;
	return pt;
}
LPoint LPoint::operator*(const float& _val)const
{
	LPoint pt(_val);
	return operator*(pt);
}
LPoint LPoint::operator/(const LPoint& _pt)const
{
	LPoint pt;
	pt.m_x = this->m_x / _pt.m_x;
	pt.m_y = this->m_y / _pt.m_y;
	return pt;
}
LPoint LPoint::operator/(const float& _val)const
{
	LPoint pt, Val(_val);
	pt = operator/(Val);
	return pt;
}
bool LPoint::operator==(const LPoint& _pt)
{
	LPoint result = operator-(_pt);
	if (fabs(result.m_x) < 0.1f && fabs(result.m_y) < 0.1f)
		return true;
	return false;
}
bool LPoint::operator==(const float& _val)
{
	LPoint pt(_val);
	return operator==(pt);
}
bool LPoint::operator!=(const LPoint& _pt)
{
	return !(operator==(_pt));
}
bool LPoint::operator!=(const float& _val)
{
	return !(operator==(_val));
}


template<class T>
void release(T _p)
{
	_p->release();
	delete _p;
	_p = nullptr;
}

LNode::LNode() :m_Position(LPoint(0.0f, 0.0f)), m_Anchor(LPoint(0.5f, 0.5f))
{
}

void LNode::release()
{
}

bool LNode::init()
{
	return true;
}
