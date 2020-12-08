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
	//�ͷ�ǰֹͣ�����˶�
	(*pAction)->m_Sprite->GetGlassWindow()->StopMove();
	(*pAction)->release();
	delete* pAction;
	*pAction = nullptr;
}

bool LAction::init(LSprite* pSprite, const UINT& countLine)
{
	if (countLine < GetFileData_Action_BeginLine)	return false;
	m_Sprite = pSprite;
	//��ʼ��ȡ����
	string ActionFileDataPath = ParentPath + pSprite->GetName() + ActionDataPath;
	LFile* pActionFile = LFile::AddToFilePool(ActionFileDataPath);		//���ļ�

	string EntireData = LReadFileLine(pActionFile, countLine);			//�ļ��ĵ�countLine��ȫ������
	FractureBack(EntireData, DataMarkerEnd);							//ȥ��@���������
	//��ȡ��0������	ActionStyle
	string SingalData = GetFirst(EntireData, DataMarker);
	m_ActionStyle = LDataConversion<ActionStyle>::Conversion(SingalData);
	//��ȡ��1������	����λ�ã����λ�ã���ת�뾶	����һ�����ݰ�
	SingalData = GetFirst(EntireData, DataMarker);
	m_Value1 = LDataPackConversion<UINT>::Conversion(SingalData);
	//��ȡ��2������	��ز���
	SingalData = GetFirst(EntireData, DataMarker);
	m_Value2 = LDataPackConversion<float>::Conversion(SingalData);

	LFile::MoveOutFilePool(ActionFileDataPath);							//�ر��ļ�
	//�������
	Censor();

	//���Կ�ʼ�˶��ˣ�ֻ�õ���һ�Σ��ƶ��жϽ���GlassWindow��
	PlayAction();
}

void LAction::release()
{
}

void LAction::Censor()
{
	//��ʽ������Χ
	if (m_ActionStyle.m_Val <= ActionStyle::Min || m_ActionStyle.m_Val >= ActionStyle::Max)
	{
		m_ActionStyle.m_Val = ActionStyle::None;
		//���涼����Ҫ����ˣ���ΪNone����ֹ
		return;
	}
	//m_Value1���2������
	AdditionOrDeleteValue(m_Value1, 2 - m_Value1.size());
	//���m_Value2
	if (m_ActionStyle.m_Val <= ActionStyle::ByLine)
	{
		//ֱ���˶���ֻ��1������
		AdditionOrDeleteValue(m_Value2, 1 - m_Value2.size());
	}
	else if (m_ActionStyle.m_Val == ActionStyle::Arc)
	{
		//�����˶���4������
		AdditionOrDeleteValue(m_Value2, 4 - m_Value2.size());
	}
}

void LAction::PlayAction()
{
	if (m_ActionStyle.m_Val == ActionStyle::None)
	{
		//ʲô������
		return;
	}
	else if (m_ActionStyle.m_Val == ActionStyle::ToLine)
	{
		//�����˶�
		m_Sprite->GetGlassWindow()->WindowMoveTo({ static_cast<float>(m_Value1[0].m_Val), static_cast<float>(m_Value1[1].m_Val) }, m_Value2[0].m_Val);
	}
	else if (m_ActionStyle.m_Val == ActionStyle::ByLine)
	{
		//����˶�
		m_Sprite->GetGlassWindow()->WindowMoveBy({ static_cast<float>(m_Value1[0].m_Val), static_cast<float>(m_Value1[1].m_Val) }, m_Value2[0].m_Val);
	}
	else if (m_ActionStyle.m_Val == ActionStyle::Arc)
	{
		//�����˶�
		m_Sprite->GetGlassWindow()->WindowMoveArc({ static_cast<float>(m_Value1[0].m_Val), static_cast<float>(m_Value1[1].m_Val) },
			m_Value2[0].m_Val, m_Value2[1].m_Val, m_Value2[2].m_Val, GlassWindow::TrunStyle(static_cast<int>(m_Value2[3].m_Val)));
	}
}