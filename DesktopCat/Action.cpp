#include "GlassWindows.h"
#include "Action.h"
#include "Animation.h"

LAction* LAction::create(LSprite* pSprite, const UINT& countLine)
{
	LAction* pAction = new LAction();
	if (pAction->init(pSprite, countLine))
		return pAction;
	delete pAction;
	return pAction = nullptr;
}

void LAction::release(LAction** pAction)
{
	if (!(*pAction))
		return;
	//释放前停止窗口运动
	(*pAction)->m_Sprite->GetGlassWindow()->StopMove();
	(*pAction)->release();
	delete* pAction;
	*pAction = nullptr;
}

bool LAction::init(LSprite* pSprite, const UINT& countLine)
{
	if (countLine < GetFileData_Action_BeginLine)	return false;
	m_Sprite = pSprite;
	//开始读取数据
	string ActionFileDataPath = ParentPath + pSprite->GetName() + ActionDataPath;
	LFile* pActionFile = LFile::AddToFilePool(ActionFileDataPath);		//打开文件

	string EntireData = LReadFileLine(pActionFile, countLine);			//文件的第countLine行全部数据
	FractureBack(EntireData, DataMarkerEnd);							//去掉@后面的数据
	//读取第0个数据	ActionStyle
	string SingalData = GetFirst(EntireData, DataMarker);
	m_ActionStyle = LDataConversion<ActionStyle>::Conversion(SingalData);
	//读取第1个数据	绝对位置，相对位置，旋转半径	这是一个数据包
	SingalData = GetFirst(EntireData, DataMarker);
	m_Value1 = LDataPackConversion<UINT>::Conversion(SingalData);
	//读取第2个数据	相关参数
	SingalData = GetFirst(EntireData, DataMarker);
	m_Value2 = LDataPackConversion<float>::Conversion(SingalData);

	LFile::MoveOutFilePool(ActionFileDataPath);							//关闭文件
	//检查数据
	Censor();

	//可以开始运动了，只用调用一次，移动判断交给GlassWindow。
	PlayAction();
}

void LAction::release()
{
}

void LAction::Censor()
{
	//样式超出范围
	if (m_ActionStyle.m_Val <= ActionStyle::Min || m_ActionStyle.m_Val >= ActionStyle::Max)
	{
		m_ActionStyle.m_Val = ActionStyle::None;
		//后面都不需要检查了，因为None代表静止
		return;
	}
	//m_Value1最多2个参数
	AdditionOrDeleteValue(m_Value1, 2 - m_Value1.size());
	//检查m_Value2
	if (m_ActionStyle.m_Val <= ActionStyle::ByLine)
	{
		//直线运动，只有1个参数
		AdditionOrDeleteValue(m_Value2, 1 - m_Value2.size());
	}
	else if (m_ActionStyle.m_Val == ActionStyle::Arc)
	{
		//曲线运动有4个参数
		AdditionOrDeleteValue(m_Value2, 4 - m_Value2.size());
	}
}

void LAction::PlayAction()
{
	if (m_ActionStyle.m_Val == ActionStyle::None)
	{
		//什么都不做
		return;
	}
	else if (m_ActionStyle.m_Val == ActionStyle::ToLine)
	{
		//绝对运动
		m_Sprite->GetGlassWindow()->WindowMoveTo({ static_cast<float>(m_Value1[0].m_Val), static_cast<float>(m_Value1[1].m_Val) }, m_Value2[0].m_Val);
	}
	else if (m_ActionStyle.m_Val == ActionStyle::ByLine)
	{
		//相对运动
		m_Sprite->GetGlassWindow()->WindowMoveBy({ static_cast<float>(m_Value1[0].m_Val), static_cast<float>(m_Value1[1].m_Val) }, m_Value2[0].m_Val);
	}
	else if (m_ActionStyle.m_Val == ActionStyle::Arc)
	{
		//曲线运动
		m_Sprite->GetGlassWindow()->WindowMoveArc({ static_cast<float>(m_Value1[0].m_Val), static_cast<float>(m_Value1[1].m_Val) },
			m_Value2[0].m_Val, m_Value2[1].m_Val, m_Value2[2].m_Val, GlassWindow::TrunStyle(static_cast<int>(m_Value2[3].m_Val)));
	}
}