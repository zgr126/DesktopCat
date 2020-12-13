#include "Animation.h"
#include "Sprite.h"
#include "Cat.h"

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
	//OutputDebugString(L"删除即时动作！\n");
}

LAnimation::LAnimation() :m_AnimationStyle(LAnimationStyle::None), m_CountLine(0), m_AddValue(0), m_ActionTextLine(0), m_Sprite(nullptr)
{
}

bool LAnimation::init(LSprite* pSprite)
{
	//保存精灵指针
	m_Sprite = pSprite;
	//保存动画数据文件路径	如：Assest\\Cat\\AnimationData.txt
	m_AnimationDataFilePath = ParentPath + pSprite->GetName() + AnimationDataPath;
	return true;
}

void LAnimation::release() { }

void LAnimation::Censor()
{
	//检查m_vStayTime
	AdditionOrDeleteValue(m_vStayTime, m_vPlayOrder.size() - m_vStayTime.size());
}

void LAnimation::BeforeTheAnimationOfSpriteInit()
{
	//从第0帧开始的索引
	m_FrameIndex = 0;
	//第0帧需要的停留时间的索引
	m_FrameStayTimeIndex = 0;
	//储存m_FrameStayTimeIndex帧的需停留时间
	m_FrameStyleTime = m_vStayTime[m_FrameStayTimeIndex].m_Val;
	//初始化第一帧当前停留时间
	m_NowFrameStayTime = 0;
	//初始化当前动画持续时间
	m_NowAnimationContinue = 0;
	//动画未结束
	m_isEnd = false;
}

void LAnimation::AfterTheAnimationOfSpriteInit()
{
	//动画总经历时间初始化
	m_NowAnimationContinue = 0;
	//动画风格初始化
	m_AnimationStyle.m_Val = LAnimationStyle::None;
}

UINT LAnimation::PlayAnimation(UINT fileLine, LAnimation::LAnimationStyle animationStyle, UINT addValue)
{
	LFile* pFile = LFile::AddToFilePool(m_AnimationDataFilePath);		//打开文件
	//开始读取数据
	string EntireData = LReadFileLine(pFile, fileLine);		//文件的第fileLine行全部数据
	FractureBack(EntireData, DataMarkerEnd);				//去掉@后面的数据
	//OutputDebugString(string2wstring(EntireData).c_str());
	//OutputDebugString(L"\n");
	//读取第0个数据		图片第几行
	string SingalData = GetFirst(EntireData, DataMarker);
	m_CountLine = LDataConversion<UINT>::Conversion(SingalData);
	//读取第1个数据		帧播放顺序
	SingalData = GetFirst(EntireData, DataMarker);
	m_vPlayOrder = LDataPackConversion<UINT>::Conversion(SingalData);
	//读取第2个数据		每帧停留时间
	SingalData = GetFirst(EntireData, DataMarker);
	m_vStayTime = LDataPackConversion<UINT>::Conversion(SingalData);
	do
	{
		//如果第二第三参数指定了，说明是不读取第3，4，5个数据的。
		if (animationStyle != LAnimationStyle::None)
		{
			m_AnimationStyle.m_Val = animationStyle;
			m_AddValue = addValue;
			break;	//直接跳出循环
		}
		//读取第3个数据		动画类型
		SingalData = GetFirst(EntireData, DataMarker);
		m_AnimationStyle = LDataConversion<LAnimationStyle>::Conversion(SingalData);
		//读取第4个数据		附加数据
		SingalData = GetFirst(EntireData, DataMarker);
		m_AddValue = LDataConversion<UINT>::Conversion(SingalData);
		//读取第5个数据		使用Action哪一行的数据
		SingalData = GetFirst(EntireData, DataMarker);
		m_ActionTextLine = LDataConversion<UINT>::Conversion(SingalData);
		LFile::MoveOutFilePool(m_AnimationDataFilePath);					//关闭文件
	} while (0);

	//进行数据审查
	Censor();
	BeforeTheAnimationOfSpriteInit();
	return m_AddValue.m_Val;
}

LPoint LAnimation::Update(LTimer* animationTimer)
{
	if (animationTimer == nullptr)
	{
		return LPoint(static_cast<float>(m_vPlayOrder[0].m_Val), static_cast<float>(m_CountLine.m_Val));
	}
	//动画生命结束	等待被释放
	if (m_isEnd)
	{
		return	LPoint(static_cast<float>(m_vPlayOrder[0].m_Val), static_cast<float>(m_CountLine.m_Val));
	}
	//先判断动画类型由时间主导的情况
	if (!isTime(animationTimer->GetInterval()))
	{
		return	LPoint(static_cast<float>(m_vPlayOrder[0].m_Val), static_cast<float>(m_CountLine.m_Val));
	}
	//帧停留时间累加
	m_NowFrameStayTime += animationTimer->GetInterval();
	if (m_NowFrameStayTime >= m_FrameStyleTime)
	{
		//如果完成一个周期
		if (++m_FrameIndex == m_vPlayOrder.size())
		{
			NextCycle();
		}
		else	//否则切换下一帧
		{
			NextFrame();
		}
	}
	//												行↓											列↓
	LPoint Area(static_cast<float>(m_vPlayOrder[m_FrameIndex].m_Val), static_cast<float>(m_CountLine.m_Val));
	return Area;
}

bool LAnimation::isCycle()
{
	//如果周期循环未达到指定次数		返回true
	if (--m_AddValue.m_Val > 0)
	{
		NextFrame();
		return true;
	}
	//达到指定次数返回fasle
	else
	{
		//动画播放结束
		m_isEnd = true;
		return false;
	}
}

bool LAnimation::isTime(ULONGLONG interval)
{
	//动画总经历时间累加
	m_NowAnimationContinue += interval;
	if (m_AnimationStyle.m_Val == LAnimationStyle::Time)
	{
		if (m_NowAnimationContinue >= m_AddValue.m_Val)
		{
			//达到或超过生存时间
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
	//初始化当前帧停留时间
	m_NowFrameStayTime = 0;
	//自增帧停留时间索引
	++m_FrameStayTimeIndex;
	//获取当前帧的停留时间
	m_FrameStyleTime = m_vStayTime[m_FrameStayTimeIndex].m_Val;
}

void LAnimation::NextCycle()
{
	if (m_AnimationStyle.m_Val == LAnimationStyle::Cycle)
	{
		//如果动画风格是周期主导
		if (!isCycle())
			return;
	}
	//直接循环周期
	m_FrameIndex = 0;				//帧索引初始化
	m_FrameStayTimeIndex = -1;		//每帧停留时间索引初始化	这里设为-1是因为接下来调用NextFrame,此变量会自增后从0开始
	NextFrame();
}