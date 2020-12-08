#include "GlassWindows.h"
#include "Cat.h"

Cat* Cat::create(const string& spriteName, const string& suffix, const LPoint& size)
{
	Cat* pCat = new Cat();
	if (pCat->init(spriteName, suffix, size))
		return pCat;
	delete pCat;
	return pCat = nullptr;
}
void Cat::release(Cat** pCat)
{
	if (!(*pCat))
		return;
	(*pCat)->release();
	delete* pCat;
	*pCat = nullptr;
}

#pragma region 重写基类方法
bool Cat::init(const string& spriteName, const string& suffix, const LPoint& size)
{
	return LSprite::init(spriteName, suffix, size);
}

void Cat::release()
{
	LSprite::release();
}

void Cat::Draw(ID2D1HwndRenderTarget* pRT)
{
	LSprite::Draw(pRT);
}

void Cat::Update()
{
	LSprite::Update();
}
#pragma endregion





//
//Cat* Cat::create(string& _SpriteName, string _Suffix, LPoint& _Size)
//{
//	Cat* pet = new Cat();
//	if (pet->init(_SpriteName, _Suffix, _Size))
//		return pet;
//	delete pet;
//	return pet = nullptr;
//}
//
//void Cat::release(Cat** _pet)
//{
//	if (!(*_pet))	return;
//	(*_pet)->release();
//	delete* _pet;
//	*_pet = nullptr;
//}
//
//Cat::Cat() :m_Animation(nullptr), m_IdelTime(nullptr)
//{
//
//}
//
//bool Cat::init(string& _SpriteName, string _Suffix, LPoint& _Size)
//{
//	if (!LSprite::init(_SpriteName, _Suffix, _Size))
//	{
//		OutputDebugString(L"初始化失败\n");
//		return false;
//	}
//	m_IdelTime = LTimerManager::instance->createTimer(Cat_Idel_TimerID);
//	return true;
//}
//
//void Cat::release()
//{
//	Animation::release(&m_Animation);
//	LTimerManager::instance->releaseTimer(Cat_Idel_TimerID);
//	LSprite::release();
//}
//
//void Cat::Draw(ID2D1HwndRenderTarget* pRT)
//{
//	LSprite::Draw(pRT);
//}
//
//void Cat::Update()
//{
//	LSprite::Update();
//	if (Animation->GetisEnd())
//	{
//		m_Window->StopMove();		//停止窗口运动
//		Idel();
//		m_IdelTime->BeginTiming();	//开始计时发呆时间
//	}
//}
//
//void Cat::PlayAnimation(int _PlayPosition, LAnimation::LAnimationStyle _Style, DWORD _Val, HousePetDirection _Direction)
//{
//	if (m_Animation == nullptr)
//		m_Animation = Animation::create(this);
//	m_Direction = _Direction;
//	m_Animation->Cutover(_PlayPosition, _Style, _Val);
//}
//
//void Cat::AI()
//{
//	m_IdelTime->EndTiming();			//结束计时发呆时间
//	DWORD IdelTime = m_IdelTime->GetRefreshInterval();	//获取发呆时间
//	if (IdelTime > 1000)				//如果发呆时间超过此时间
//		PlayAnimation(HPA::Attack, LAnimation::LAnimationStyle::Cycle, 1, HousePetDirection::Left);
//}
//
//void Cat::Idel()
//{
//	switch (m_Direction)
//	{
//		case HousePetDirection::Top:
//			PlayAnimation(HPA::TopIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::Top);
//			break;
//		case HousePetDirection::Bottom:
//			PlayAnimation(HPA::BottomIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::Bottom);
//			break;
//		case HousePetDirection::Left:
//			PlayAnimation(HPA::LeftIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::Left);
//			break;
//		case HousePetDirection::Right:
//			PlayAnimation(HPA::RightIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::Right);
//			break;
//		case HousePetDirection::LeftTop:
//			PlayAnimation(HPA::LeftTopIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::LeftTop);
//			break;
//		case HousePetDirection::RightTop:
//			PlayAnimation(HPA::RightTopIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::RightTop);
//			break;
//		case HousePetDirection::LeftBottom:
//			PlayAnimation(HPA::LeftBottomIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::LeftBottom);
//			break;
//		case HousePetDirection::RightBottom:
//			PlayAnimation(HPA::RightBottomIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::RightBottom);
//			break;
//	}
//	m_PetMode = HousePetMode::Idel;		//设置站立模式
//}