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
	//创建/获取 图片加载器
	g_LoadImage = LLoadImage::create();
	//设置精灵名称
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
	//在此绘制
	UINT width = m_SingalSize.width;
	UINT height = m_SingalSize.height;			//精灵大小

	D2D1_RECT_F Position = D2D1::RectF(
		0 + m_Position.m_x - (m_Anchor.m_x * width), 0 + m_Position.m_y - (m_Anchor.m_y * height),
		m_Position.m_x + width - (m_Anchor.m_x * width), m_Position.m_y + height - (m_Anchor.m_y * height));		//贴图位置

	D2D1_RECT_F Tailoring = D2D1::RectF(
		width * UINT(m_Ranks.m_x), height * UINT(m_Ranks.m_y),
		width * UINT(m_Ranks.m_x + 1), height * UINT(m_Ranks.m_y + 1));	//在图片中选择需显示在屏幕上的大小(剪裁)

	pRT->DrawBitmap(m_SpriteBitmap,
		Position,
		1,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		Tailoring);
}

void LSprite::Update()
{
	//如果队列为空
	if (m_AnimationDeque.size() == 0)
	{
		//删除计时器
		LTimerManager::instance->releaseTimer(m_AnimationTimerID);
		m_AnimationTimer = nullptr;
		//取(0,0)
		SetDrawArea(LPoint(0));
		return;
	}
	//如果当前的AnimationFront指针不是队列的Front。则代表需要重新创建一个Timer借给新的Animation使用
	if (m_AnimationFront != m_AnimationDeque.front())
	{
		if (m_AnimationTimer == nullptr)
		{
			//如果不存在计时器，则创建一个
			m_AnimationTimer = LTimerManager::instance->createTimer(m_AnimationTimerID);
		}
		else
		{
			//如果存在，直接将定时器初始化。效率更高
			m_AnimationTimer->Reset();
		}
		//只更新队列的第一个动画
		m_AnimationFront = m_AnimationDeque.front();
		//每进入这个if语句，说明新的Animation即将开始，所以在此处创建Action
		m_Action = LAction::create(this, m_AnimationFront->GetActionTextLine());		//Action运行原理是直接移动窗口，不需要移动精灵。在GlassWindow中更新窗口位置。
	}
	if (m_AnimationFront)
	{
		//将定时器借给Animation使用
		LPoint Area = m_AnimationFront->Update(m_AnimationTimer);
		//如果动画生命结束，则不更新调用SetDrawArea
		if (m_AnimationFront->GetisEnd())
		{
			//将第一个动画释放并删除
			auto FutileAnimation = m_AnimationDeque.front();
			LAnimation::release(&FutileAnimation);
			m_AnimationDeque.pop_front();
			//释放动画的同时释放Action数据
			LAction::release(&m_Action);
			return;
		}
		SetDrawArea(Area);
	}
}

HRESULT LSprite::LoadingImage(LPCTSTR fileName, ID2D1RenderTarget* pRT, IWICImagingFactory* pWICIF)
{
	HRESULT hr;
	hr = g_LoadImage->LoadintImage(fileName, pRT, pWICIF, &m_SpriteBitmap);
	//创建位图画刷
	pRT->CreateBitmapBrush(m_SpriteBitmap, &m_SpriteBitmapBrush);
	if (SUCCEEDED(hr))
		SetEntireSize(m_SpriteBitmap->GetPixelSize());				//设置图片大小
	return hr;
}

void LSprite::AddFrontAnimation(UINT fileLine)
{
	//创建之前若已有动画在播放，则将正在播放的动画初始化
	if (m_AnimationDeque.size() > 0)
	{
		auto FrontAnimation = m_AnimationDeque.front();
		FrontAnimation->BeforeTheAnimationOfSpriteInit();
	}
	//创建一个动画
	auto Animation = LAnimation::create(this);
	//指定动画数据
	Animation->PlayAnimation(fileLine);

	//将动画添加进队列最前面
	m_AnimationDeque.push_front(Animation);
}

//void LSprite::AddBackAnimation(UINT fileLine)
//{
//	if (m_AnimationDeque.size() > 0)
//	{
//		auto BackAnimation = m_AnimationDeque.back();
//		if (BackAnimation->GetAnimationStyle() == LAnimation::LAnimationStyle::UnTime)
//		{
//			//如果添加之前，队列最后一个动画类型是无限循环，就将它删除
//			m_AnimationDeque.pop_back();
//			LAnimation::release(&BackAnimation);
//			//LAction::release(&m_Action);
//		}
//	}
//	//创建一个动画
//	auto Animation = LAnimation::create(this);
//	//指定动画数据
//	Animation->PlayAnimation(fileLine);
//
//	//将动画添加进队列最后面
//	m_AnimationDeque.push_back(Animation);
//}

void LSprite::SetDrawArea(LPoint& ranks)
{
	//区域绘制
	m_Ranks = ranks;
}