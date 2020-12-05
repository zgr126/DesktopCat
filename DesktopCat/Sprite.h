#pragma once
#include "DesktopCat.h"
#include "Node.h"
#include "LoadImage.h"


class LAction;


class LSprite :public LNode
{
public:
	//��̬����������
	static LLoadImage* g_LoadImage;
	static void release(LSprite*);			//����˽�з���release�ͷų�Ա�ڵ�ָ�� ��delete����
protected:
	//��Ա����
	string m_Name;						//��������	(�û��ṩ)
	string m_Suffix;					//�ļ���׺	(�û��ṩ)	��".png"

	ID2D1Bitmap* m_SpriteBitmap;
	ID2D1BitmapBrush* m_SpriteBitmapBrush;

	D2D1_SIZE_U m_EntireSize;			//����ͼƬ��Դ��С
	D2D1_SIZE_U m_SingalSize;			//һ������Ĵ�С

	LPoint m_Ranks;				//�����Ƶ�����

protected:
	//��д�����麯��
	virtual bool init(std::string, std::string, LPoint& _Size);
	virtual void release();

protected:
	//��������
	void SetdrawArea(LPoint& _Ranks);	//�С���
public:
	//��������
	LSprite();
	virtual void Draw() = 0;
	virtual void Update() = 0;

	HRESULT LoadingImage(LPCTSTR, ID2D1RenderTarget*, IWICImagingFactory*);

public:
	//������
	string GetName() { return m_Name; }
	string GetSuffix(){return m_Suffix; }
	D2D1_SIZE_U GetEntireSize() { return m_EntireSize; }
	D2D1_SIZE_U GetSingalSize() { return m_SingalSize; }

	void SetName(const string& name) { m_Name = name; }
	void SetSuffix(const string& suffix) { m_Suffix = suffix; }
	void SetEntireSize(D2D1_SIZE_U size) { m_EntireSize = size; }
	void SetSingalSize(D2D1_SIZE_U size) { m_SingalSize = size; }

	friend LAction;

};