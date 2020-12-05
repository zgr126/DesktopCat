#include "Animation.h"
#include "Sprite.h"


LAnimation* LAnimation::create(LSprite* _pSprite)
{
	LAnimation* animation = new Animation();
	if (animation->init(_pSprite))
		return animation;
	delete animation;
	return animation = nullptr;
}

void LAnimation::release(LAnimation** _pAnimation)
{
	if (!(*_pAnimation))
		return;
	(*_pAnimation)->release();
	delete* _pAnimation;
	*_pAnimation = nullptr;
	//OutputDebugString(L"ɾ����ʱ������\n");
}

LAnimation::LAnimation()
{

}

bool LAnimation::init(LSprite* _pSprite)
{
	Clear();	//ȫ������
	m_AnimationDataFile = DataParent + _pSprite->GetName() + ActionData;		//���涯�������ļ�·��
	m_AnimationStyle = LAnimationStyle::Null;
	AnimationBegin();
	return true;
}

void LAnimation::release()
{

}

void LAnimation::AnimationBegin()
{		//������ʼǰ ִ��һЩ����
//���ļ��ж�ȡ���� ��ӽ�ȥ
}

void LAnimation::AnimationEnd()
{		//��������ʱ ִ��һЩ����
//���ļ��ж�ȡ���� ��ӽ�ȥ
}

void LAnimation::Cutover(int _FileLine, LAnimationStyle _AnimationStyle, DWORD _Val)
{
	Clear();						//������������
	float PlayLine;
	vector<string> AnimationData;
	Lin::ReadFileData_To_Vector(m_AnimationDataFile, AnimationData, _FileLine + VaildData);		//��ȡ���������ļ���_FileLine��
	Lin::Vector_To_Float(AnimationData[VaildData_AnimationPlayLine], PlayLine);					//��ȡ����ѡ��ͼƬ�ڼ���
	Lin::Vector_To_Float(AnimationData[VaildData_AnimationOrder], m_FrameOrder);				//��ȡһ������������֡���л�˳��
	Lin::Vector_To_Float(AnimationData[VaildData_AnimationDwellTime], m_DwellTime);				//��ȡÿֱ֡�ӵļ��
	m_PlayLine = static_cast<UINT>(PlayLine);
	m_FrameNumber = m_FrameOrder.size();			//��ȡ��֡��
	m_NowFrame = 0;									//��ǰ֡
	updataFrameTime();			//����֡��ʱ
	m_AnimationStyle = _AnimationStyle;
	switch (m_AnimationStyle)
	{
		case LAnimationStyle::Null:
			OutputDebugString(L"�������ʹ���\n");
			break;
		case LAnimationStyle::Cycle:
			m_PlayNumber = static_cast<int>(_Val);
			m_PlayTime = 0;
			break;
		case LAnimationStyle::Time:
			m_PlayNumber = -1;
			m_PlayTime = _Val;
			break;
		case LAnimationStyle::UnTime:
			m_PlayNumber = -1;
			m_PlayTime = 0;
			break;
	}
}

void LAnimation::Clear()
{
	m_AnimationStyle = LAnimationStyle::Null;
	m_PlayLine = 0;
	m_FrameNumber = 0;
	m_FrameOrder.clear();
	m_DwellTime.clear();
	m_NowFrame = 0;				//��ǰ֡
	m_BeginPlayTime = 0;		//֡��ʼʱ��
	m_NowFrameTime = 0;			//֡����ʱ��
	m_EndFrameTime = 0;			//֡����ʱ��
	m_PlayTime = 0;				//��������ʱ��
	m_isEnd = false;
}

void LAnimation::updateFrame()
{
	if (++m_NowFrame == m_FrameNumber)
	{
		if (m_PlayNumber == -1 || --m_PlayNumber)
		{
			m_NowFrame = 0;
		}
		else
		{
			m_isEnd = true;
			--m_NowFrame;		//��m_NowFrameֵΪ���һ֡������
		}
	}
	if (!m_isEnd)
		updataFrameTime();
}

void LAnimation::updataFrameTime()
{
	float DwellTime;
	ReadVectorData(m_DwellTime, m_NowFrame, DwellTime);
	DwellTime *= M_To_MS;
	m_BeginPlayTime = m_EndFrameTime = m_NowFrameTime = GetTickCount();
	m_EndFrameTime += static_cast<DWORD>(DwellTime);
}

template<typename T>
void LAnimation::ReadVectorData(vector<float>& _vector, int _Index, T& _Param)
{
	if (_vector.size() == 0)return;
	float data = _vector[_Index];
	_Param = static_cast<T>(data);
}

LPoint LAnimation::update()
{
	m_NowFrameTime = GetTickCount();
	switch (m_AnimationStyle)
	{
		case LAnimationStyle::Cycle: {		//��������
			//����Ҫ��Ӷ������
			break;
		}
		case LAnimationStyle::Time: {		//ʱ������
			if (m_NowFrameTime == m_PlayTime && !m_isEnd)	//ʱ��һ�� �������
				m_isEnd = true;
			break;
		}
		case LAnimationStyle::UnTime: {		//����ѭ��
			//����Ҫ��Ӷ������
			break;
		}
	}
	if (m_NowFrameTime >= m_EndFrameTime && !m_isEnd)
	{
		updateFrame();
	}

	//												�С�								�С�
	return LPoint(static_cast<float>(m_FrameOrder[m_NowFrame]), static_cast<float>(m_PlayLine));	//���ػ�������
}