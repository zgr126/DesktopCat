#include "Animation.h"
#include "Sprite.h"

//using namespace Lin;

LAnimation* LAnimation::create(LSprite* pSprite)
{
	LAnimation* animation = new Animation();
	if (animation->init(pSprite))
		return animation;
	delete animation;
	return animation = nullptr;
}

void LAnimation::release(LAnimation** pAnimation)
{
	if (!(*pAnimation))
		return;
	(*pAnimation)->release();
	delete* pAnimation;
	*pAnimation = nullptr;
	//OutputDebugString(L"ɾ����ʱ������\n");
}

LAnimation::LAnimation() :m_AnimationStyle(LAnimationStyle::None), m_CountLine(0), m_AddValue(0)
{
}

bool LAnimation::init(LSprite* pSprite)
{
	//���涯�������ļ�·��	�磺Assest\\Cat\\AnimationData.txt
	m_AnimationDataFilePath = ParentPath + pSprite->GetName() + AnimationDataPath;
	return true;
}

void LAnimation::release() { }

void LAnimation::Censor()
{
	int Minus = m_vPlayOrder.size() - m_vStayTime.size();
	if (Minus > 0)
	{
		for (int i = 0; i < Minus; ++i)
		{
			m_vStayTime.push_back(LFileData<UINT>(m_vStayTime.back().m_Val));
		}
	}
	else if (Minus < 0)
	{
		for (int i = 0; i > Minus; --i)
		{
			m_vStayTime.pop_back();
		}
	}
}

void LAnimation::BeforeTheAnimationOfSpriteInit()
{
	//�ӵ�0֡��ʼ������
	m_FrameIndex = 0;
	//��0֡��Ҫ��ͣ��ʱ�������
	m_FrameStayTimeIndex = 0;
	//����m_FrameStayTimeIndex֡����ͣ��ʱ��
	m_FrameStyleTime = m_vStayTime[m_FrameStayTimeIndex].m_Val;
	//��ʼ����һ֡��ǰͣ��ʱ��
	m_NowFrameStayTime = 0;
	//��ʼ����ǰ��������ʱ��
	m_NowAnimationContinue = 0;
	//����δ����
	m_isEnd = false;
}

void LAnimation::AfterTheAnimationOfSpriteInit()
{
	//�����ܾ���ʱ���ʼ��
	m_NowAnimationContinue = 0;
	//��������ʼ��
	m_AnimationStyle.m_Val = LAnimationStyle::None;
}

bool LAnimation::PlayAnimation(UINT fileLine)
{
	LFile* pFile = LFile::AddToFilePool(m_AnimationDataFilePath);		//���ļ�
	//��ʼ��ȡ����
	string EntireData = LReadFileLine(pFile, fileLine);		//�ļ��ĵ�fileLine��ȫ������
	FractureBack(EntireData, DataMarkerEnd);		//ȥ��@���������
	//��ȡ��0������		ͼƬ�ڼ���
	string SingalData = GetFirst(EntireData, DataMarker);
	m_CountLine = LDataConversion<UINT>::Conversion(SingalData);
	//��ȡ��1������		֡����˳��
	SingalData = GetFirst(EntireData, DataMarker);
	m_vPlayOrder = LDataPackConversion<UINT>::Conversion(SingalData);
	//��ȡ��2������		ÿ֡ͣ��ʱ��
	SingalData = GetFirst(EntireData, DataMarker);
	m_vStayTime = LDataPackConversion<UINT>::Conversion(SingalData);
	//��ȡ��3������		��������
	SingalData = GetFirst(EntireData, DataMarker);
	m_AnimationStyle = LDataConversion<LAnimationStyle>::Conversion(SingalData);
	//��ȡ��4������		��������
	SingalData = GetFirst(EntireData, DataMarker);
	m_AddValue = LDataConversion<UINT>::Conversion(SingalData);
	LFile::MoveOutFilePool(m_AnimationDataFilePath);					//�ر��ļ�
	//�����������
	Censor();
	BeforeTheAnimationOfSpriteInit();
}

LPoint LAnimation::Update(LTimer* animationTimer)
{
	//������������	�ȴ����ͷ�
	if (m_isEnd)	return	LPoint(static_cast<float>(m_vPlayOrder[0].m_Val, static_cast<float>(m_CountLine.m_Val)));
	//���ж϶���������ʱ�����������
	if (!isTime(animationTimer->GetInterval()))
	{
		return	LPoint(static_cast<float>(m_vPlayOrder[0].m_Val, static_cast<float>(m_CountLine.m_Val)));
	}
	//֡ͣ��ʱ���ۼ�
	m_NowFrameStayTime += animationTimer->GetInterval();
	if (m_NowFrameStayTime >= m_FrameStyleTime)
	{
		//������һ������
		if (++m_FrameIndex == m_vPlayOrder.size())
		{
			NextCycle();
		}
		else	//�����л���һ֡
		{
			NextFrame();
		}
	}
	//												�С�											�С�
	LPoint Area(static_cast<float>(m_vPlayOrder[m_FrameIndex].m_Val), static_cast<float>(m_CountLine.m_Val));
	return Area;
}

bool LAnimation::isCycle()
{
	//�������ѭ��δ�ﵽָ������		����true
	if (--m_AddValue.m_Val > 0)
	{
		NextFrame();
		return true;
	}
	//�ﵽָ����������fasle
	else
	{
		//�������Ž���
		m_isEnd = true;
		return false;
	}
}

bool LAnimation::isTime(ULONGLONG interval)
{
	//�����ܾ���ʱ���ۼ�
	m_NowAnimationContinue += interval;
	if (m_AnimationStyle.m_Val == LAnimationStyle::Time)
	{
		if (m_NowAnimationContinue >= m_AddValue.m_Val)
		{
			//�ﵽ�򳬹�����ʱ��
			m_isEnd = true;
			return false;
		}
	}
	return true;
}

void LAnimation::isUnTime()
{

}

void LAnimation::NextFrame()
{
	//��ʼ����ǰ֡ͣ��ʱ��
	m_NowFrameStayTime = 0;
	//����֡ͣ��ʱ������
	++m_FrameStayTimeIndex;
	//��ȡ��ǰ֡��ͣ��ʱ��
	m_FrameStyleTime = m_vStayTime[m_FrameStayTimeIndex].m_Val;
}

void LAnimation::NextCycle()
{
	if (m_AnimationStyle.m_Val == LAnimationStyle::Cycle)
	{
		//��������������������
		if (!isCycle())
			return;
	}
	//ֱ��ѭ������
	m_FrameIndex = 0;				//֡������ʼ��
	m_FrameStayTimeIndex = -1;		//ÿ֡ͣ��ʱ��������ʼ��	������Ϊ-1����Ϊ����������NextFrame,�˱������������0��ʼ
	NextFrame();
}