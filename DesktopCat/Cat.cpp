#include "Cat.h"

Cat* Cat::create(string& _SpriteName, string _Suffix, LPoint& _Size)
{
	Cat* pet = new Cat();
	if (pet->init(_SpriteName, _Suffix, _Size))
		return pet;
	delete pet;
	return pet = nullptr;
}

void Cat::release(Cat** _pet)
{
	if (!(*_pet))	return;
	(*_pet)->release();
	delete* _pet;
	*_pet = nullptr;
}

Cat::Cat() :m_Animation(nullptr), m_IdelTime(nullptr)
{

}

bool Cat::init(string& _SpriteName, string _Suffix, LPoint& _Size)
{
	if (!LSprite::init(_SpriteName, _Suffix, _Size))
	{
		OutputDebugString(L"��ʼ��ʧ��\n");
		return false;
	}
	m_IdelTime = LTimerManager::instance->createTimer(Cat_Idel_TimerID);
	return true;
}

void Cat::release()
{
	Animation::release(&m_Animation);
	LTimerManager::instance->releaseTimer(Cat_Idel_TimerID);
	LSprite::release();
}

void Cat::Draw(ID2D1HwndRenderTarget* _pRT)
{
	//�ڴ˻���
	UINT width = m_SingalSize.width;
	UINT height = m_SingalSize.height;			//�����С

	D2D1_RECT_F Position = D2D1::RectF(
		0 + m_Position.m_x - (m_Anchor.m_x * width), 0 + m_Position.m_y - (m_Anchor.m_y * height),
		m_Position.m_x + width - (m_Anchor.m_x * width), m_Position.m_y + height - (m_Anchor.m_y * height));		//��ͼλ��

	D2D1_RECT_F Tailoring = D2D1::RectF(
		width * UINT(m_Ranks.m_x), height * UINT(m_Ranks.m_y),
		width * UINT(m_Ranks.m_x + 1), height * UINT(m_Ranks.m_y + 1));	//��ͼƬ��ѡ������ʾ����Ļ�ϵĴ�С(����)

	_pRT->DrawBitmap(m_SpriteBitmap,
		Position,
		1,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		Tailoring);
}

void Cat::Update()
{
	if (m_Animation)
	{
		if (m_Animation->GetisEnd())
		{
			m_Window->StopMove();		//ֹͣ�����˶�
			Idel();
			m_IdelTime->BeginTiming();	//��ʼ��ʱ����ʱ��
		}
		//AI();
		LPoint Area = m_Animation->update();
		SetdrawArea(Area);
	}
}

void Cat::PlayAnimation(int _PlayPosition, LAnimation::LAnimationStyle _Style, DWORD _Val, HousePetDirection _Direction)
{
	if (m_Animation == nullptr)
		m_Animation = Animation::create(this);
	m_Direction = _Direction;
	m_Animation->Cutover(_PlayPosition, _Style, _Val);
}

void Cat::AI()
{
	m_IdelTime->EndTiming();			//������ʱ����ʱ��
	DWORD IdelTime = m_IdelTime->GetRefreshInterval();	//��ȡ����ʱ��
	if (IdelTime > 1000)				//�������ʱ�䳬����ʱ��
		PlayAnimation(HPA::Attack, LAnimation::LAnimationStyle::Cycle, 1, HousePetDirection::Left);
}

void Cat::Idel()
{
	switch (m_Direction)
	{
		case HousePetDirection::Top:
			PlayAnimation(HPA::TopIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::Top);
			break;
		case HousePetDirection::Bottom:
			PlayAnimation(HPA::BottomIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::Bottom);
			break;
		case HousePetDirection::Left:
			PlayAnimation(HPA::LeftIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::Left);
			break;
		case HousePetDirection::Right:
			PlayAnimation(HPA::RightIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::Right);
			break;
		case HousePetDirection::LeftTop:
			PlayAnimation(HPA::LeftTopIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::LeftTop);
			break;
		case HousePetDirection::RightTop:
			PlayAnimation(HPA::RightTopIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::RightTop);
			break;
		case HousePetDirection::LeftBottom:
			PlayAnimation(HPA::LeftBottomIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::LeftBottom);
			break;
		case HousePetDirection::RightBottom:
			PlayAnimation(HPA::RightBottomIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::RightBottom);
			break;
	}
	m_PetMode = HousePetMode::Idel;		//����վ��ģʽ
}