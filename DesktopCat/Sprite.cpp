#include "Sprite.h"

LLoadImage* LSprite::g_LoadImage = nullptr;

void LSprite::release(LSprite* _pet)
{
	if (_pet != nullptr)
	{
		_pet->release();
		delete _pet;
		_pet = nullptr;
	}
}

LSprite::LSprite() :m_SpriteBitmap(nullptr), m_SpriteBitmapBrush(nullptr), m_EntireSize{ 0 }, m_SingalSize{ 0 }, m_Ranks{ 0 }
{
}

bool LSprite::init(std::string _Name, std::string _Suffix, LPoint& _Size)
{
	if (!LNode::init())
		return false;
	//����/��ȡ ͼƬ������
	g_LoadImage = LLoadImage::create();
	//���þ�������
	SetName(_Name);
	SetSuffix(_Suffix);
	SetSingalSize(D2D1::SizeU(_Size.m_x, _Size.m_y));
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

HRESULT LSprite::LoadingImage(LPCTSTR _fileName, ID2D1RenderTarget* _pRT, IWICImagingFactory* _pWICIF)
{
	HRESULT hr;
	hr = g_LoadImage->LoadintImage(_fileName, _pRT, _pWICIF, &m_SpriteBitmap);
	//����λͼ��ˢ
	_pRT->CreateBitmapBrush(m_SpriteBitmap, &m_SpriteBitmapBrush);
	if (SUCCEEDED(hr))
		SetEntireSize(m_SpriteBitmap->GetPixelSize());				//����ͼƬ��С
	return hr;
}

void LSprite::SetdrawArea(LPoint& _Ranks)
{
	//�������
	m_Ranks = _Ranks;
}