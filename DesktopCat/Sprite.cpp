#include "Sprite.h"
#include "GlassWindows.h"
#include "Animation.h"
#include "Action.h"

LLoadImage* LSprite::g_LoadImage = nullptr;

LSprite* LSprite::create(const string& spriteName, const string& suffix, const LPoint& size)
{
	LSprite* pSprite = new LSprite();
	if (pSprite->init(spriteName, suffix, size))
		return pSprite;
	delete pSprite;
	return pSprite = nullptr;
}

void LSprite::release(LSprite** pSprite)
{
	if (!(*pSprite))
		return;
	(*pSprite)->release();
	delete* pSprite;
	*pSprite = nullptr;
}

LSprite::LSprite() :m_SpriteBitmap(nullptr), m_SpriteBitmapBrush(nullptr), m_EntireSize{ 0 }, m_SingalSize{ 0 }, m_Ranks{ 0 }
{
}

bool LSprite::init(const string& name, const string& suffix, const LPoint& size)
{
	if (!LNode::init())
		return false;
	//����/��ȡ ͼƬ������
	g_LoadImage = LLoadImage::create();
	//���þ�������
	SetName(name);
	SetSuffix(suffix);
	SetSingalSize(D2D1::SizeU(size.m_x, size.m_y));
	return true;
}

void LSprite::release()
{
	LNode::release();
	if (m_SpriteBitmap != nullptr)
		m_SpriteBitmap->Release();
	if (m_SpriteBitmapBrush != nullptr)
		m_SpriteBitmapBrush->Release();
	m_SpriteBitmap = nullptr;
	m_SpriteBitmapBrush = nullptr;
}

void LSprite::Draw(ID2D1HwndRenderTarget* pRT)
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

	pRT->DrawBitmap(m_SpriteBitmap,
		Position,
		1,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		Tailoring);
}

void LSprite::Update()
{
	//�������Ϊ��
	if (m_AnimationDeque.size() == 0)
	{
		//ɾ����ʱ��
		LTimerManager::instance->releaseTimer(m_AnimationTimerID);
		m_AnimationTimer = nullptr;
		return;
	}
	//�����ǰ��AnimationFrontָ�벻�Ƕ��е�Front���������Ҫ���´���һ��Timer����µ�Animationʹ��
	if (m_AnimationFront != m_AnimationDeque.front())
	{
		if (m_AnimationTimer == nullptr)
		{
			//��������ڼ�ʱ�����򴴽�һ��
			m_AnimationTimer = LTimerManager::instance->createTimer(m_AnimationTimerID);
		}
		else
		{
			//������ڣ�ֱ�ӽ���ʱ����ʼ����Ч�ʸ���
			m_AnimationTimer->Reset();
		}
		//ֻ���¶��еĵ�һ������
		m_AnimationFront = m_AnimationDeque.front();
		//������ʼ�ص�
		CallAnimationBegin();
	}
	if (m_AnimationFront)
	{
		//����ʱ�����Animationʹ��
		LPoint Area = m_AnimationFront->Update(m_AnimationTimer);
		//������������������򲻸��µ���SetDrawArea
		if (m_AnimationFront->GetisEnd())
		{
			CallAnimationEnd();		//���������ص�
			return;
		}
		SetDrawArea(Area);
	}
}

HRESULT LSprite::LoadingImage(LPCTSTR fileName, ID2D1RenderTarget* pRT, IWICImagingFactory* pWICIF)
{
	HRESULT hr;
	hr = g_LoadImage->LoadintImage(fileName, pRT, pWICIF, &m_SpriteBitmap);
	//����λͼ��ˢ
	pRT->CreateBitmapBrush(m_SpriteBitmap, &m_SpriteBitmapBrush);
	if (SUCCEEDED(hr))
		SetEntireSize(m_SpriteBitmap->GetPixelSize());				//����ͼƬ��С
	return hr;
}

UINT LSprite::AddFrontAnimation(UINT fileLine, int animationStyle, UINT addValue)
{
	//����֮ǰ�����ж����ڲ��ţ����ͷŸö���
	if (m_AnimationDeque.size() > 0)
	{
		CallAnimationInterrupt();
	}
	//����һ������
	auto Animation = LAnimation::create(this);
	//ָ����������
	addValue = Animation->PlayAnimation(fileLine, LAnimation::LAnimationStyle(animationStyle), addValue);
	m_AnimationFront = nullptr;
	//��������ӽ�������ǰ��
	m_AnimationDeque.push_front(Animation);
	return addValue;
}

void LSprite::AddBackAnimation(UINT fileLine)
{
	if (m_AnimationDeque.size() > 0)
	{
		auto BackAnimation = m_AnimationDeque.back();
		if (BackAnimation->GetAnimationStyle() == LAnimation::LAnimationStyle::UnTime)
		{
			//������֮ǰ���������һ����������������ѭ�����ͽ���ɾ��
			m_AnimationDeque.pop_back();
			LAnimation::release(&BackAnimation);
		}
	}
	//����һ������
	auto Animation = LAnimation::create(this);
	//ָ����������
	Animation->PlayAnimation(fileLine);
	m_AnimationFront = nullptr;
	//��������ӽ����������
	m_AnimationDeque.push_back(Animation);
}

void LSprite::CallAnimationBegin()
{
	//ÿ�������������˵���µ�Animation������ʼ�������ڴ˴�����Action
	m_Action = LAction::create(this, m_AnimationFront->GetActionTextLine());		//Action����ԭ����ֱ���ƶ����ڣ�����Ҫ�ƶ����顣��GlassWindow�и��´���λ�á�
}
void LSprite::CallAnimationEnd()
{
	//����һ�������ͷŲ�ɾ��
	auto FutileAnimation = m_AnimationDeque.front();
	LAnimation::release(&FutileAnimation);
	m_AnimationDeque.pop_front();
	//�ͷŶ�����ͬʱ�ͷ�Action����
	LAction::release(&m_Action);
}
void LSprite::CallAnimationInterrupt()
{
	auto FrontAnimation = m_AnimationDeque.front();
	LAnimation::release(&FrontAnimation);
	m_AnimationDeque.pop_front();
	LAction::release(&m_Action);
}

void LSprite::SetDrawArea(LPoint& ranks)
{
	//�������
	m_Ranks = ranks;
}