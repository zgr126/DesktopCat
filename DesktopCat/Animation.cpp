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
	//OutputDebugString(L"删除即时动作！\n");
}

LAnimation::LAnimation()
{

}

bool LAnimation::init(LSprite* _pSprite)
{
	Clear();	//全部归零
	m_AnimationDataFile = DataParent + _pSprite->GetName() + ActionData;		//保存动画数据文件路径
	m_AnimationStyle = LAnimationStyle::Null;
	AnimationBegin();
	return true;
}

void LAnimation::release()
{

}

void LAnimation::AnimationBegin()
{		//动画开始前 执行一些操作
//从文件中读取方法 添加进去
}

void LAnimation::AnimationEnd()
{		//动画结束时 执行一些操作
//从文件中读取方法 添加进去
}

void LAnimation::Cutover(int _FileLine, LAnimationStyle _AnimationStyle, DWORD _Val)
{
	Clear();						//归零所有数据
	float PlayLine;
	vector<string> AnimationData;
	Lin::ReadFileData_To_Vector(m_AnimationDataFile, AnimationData, _FileLine + VaildData);		//读取动画数据文件第_FileLine行
	Lin::Vector_To_Float(AnimationData[VaildData_AnimationPlayLine], PlayLine);					//读取动画选自图片第几行
	Lin::Vector_To_Float(AnimationData[VaildData_AnimationOrder], m_FrameOrder);				//读取一个动画周期内帧的切换顺序
	Lin::Vector_To_Float(AnimationData[VaildData_AnimationDwellTime], m_DwellTime);				//读取每帧直接的间隔
	m_PlayLine = static_cast<UINT>(PlayLine);
	m_FrameNumber = m_FrameOrder.size();			//获取总帧数
	m_NowFrame = 0;									//当前帧
	updataFrameTime();			//更新帧计时
	m_AnimationStyle = _AnimationStyle;
	switch (m_AnimationStyle)
	{
		case LAnimationStyle::Null:
			OutputDebugString(L"动画类型错误\n");
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
	m_NowFrame = 0;				//当前帧
	m_BeginPlayTime = 0;		//帧开始时间
	m_NowFrameTime = 0;			//帧现在时间
	m_EndFrameTime = 0;			//帧结束时间
	m_PlayTime = 0;				//动画持续时间
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
			--m_NowFrame;		//让m_NowFrame值为最后一帧的索引
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
		case LAnimationStyle::Cycle: {		//周期主导
			//不需要添加额外代码
			break;
		}
		case LAnimationStyle::Time: {		//时间主导
			if (m_NowFrameTime == m_PlayTime && !m_isEnd)	//时间一到 立马结束
				m_isEnd = true;
			break;
		}
		case LAnimationStyle::UnTime: {		//无限循环
			//不需要添加额外代码
			break;
		}
	}
	if (m_NowFrameTime >= m_EndFrameTime && !m_isEnd)
	{
		updateFrame();
	}

	//												列↓								行↓
	return LPoint(static_cast<float>(m_FrameOrder[m_NowFrame]), static_cast<float>(m_PlayLine));	//返回绘制区域
}