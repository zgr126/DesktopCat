#pragma once
#include "DesktopCat.h"
#include "Node.h"
#include "LoadImage.h"
#include "Timer.h"
#include <deque>

class LAnimation;
class LAction;

class LSprite :public LNode
{
public:
	//��̬����������
	static LLoadImage* g_LoadImage;
	static LSprite* create(const string& spriteName, const string& suffix, const LPoint& size);
	static void release(LSprite**);			//����˽�з���release�ͷų�Ա�ڵ�ָ�� ��delete����
protected:
	//��Ա����
	string m_Name;						//��������	(�û��ṩ)
	string m_Suffix;					//�ļ���׺	(�û��ṩ)	��".png"

	//ͼƬ����
	ID2D1Bitmap* m_SpriteBitmap;
	//ͼƬ����ˢ
	ID2D1BitmapBrush* m_SpriteBitmapBrush;
	//����ͼƬ��Դ��С
	D2D1_SIZE_U m_EntireSize;
	//һ������Ĵ�С
	D2D1_SIZE_U m_SingalSize;
	//�����Ƶ�����
	LPoint m_Ranks;
	//����˫����
	deque<LAnimation*> m_AnimationDeque;
	//�����һ������
	LAnimation* m_AnimationFront;
	//������ʱ������������ʹ�á����ǽ��Animationʹ��
	LTimer* m_AnimationTimer;
	//������ʱ������ID��1��Sprite��Ӧֻ��1��ID
	int m_AnimationTimerID;
	//���鶯��
	LAction* m_Action;
protected:
	//��д�����麯��
	virtual bool init(const string&, const string&, const LPoint& size);
	virtual void release();

protected:
	//��������
	void SetDrawArea(LPoint& ranks);	//�С���
public:
	//��������
	LSprite();
	virtual void Draw(ID2D1HwndRenderTarget*);
	virtual void Update();

	HRESULT LoadingImage(LPCTSTR, ID2D1RenderTarget*, IWICImagingFactory*);
	//����һ���µ�Animation��ӽ�����
	void AddAnimation(UINT fileLine);

public:
	//������
	string GetName() { return m_Name; }
	string GetSuffix(){return m_Suffix; }
	D2D1_SIZE_U GetEntireSize() { return m_EntireSize; }
	D2D1_SIZE_U GetSingalSize() { return m_SingalSize; }
	int GetAnimationTimerID() { return m_AnimationTimerID; }

	void SetName(const string& name) { m_Name = name; }
	void SetSuffix(const string& suffix) { m_Suffix = suffix; }
	void SetEntireSize(D2D1_SIZE_U size) { m_EntireSize = size; }
	void SetSingalSize(D2D1_SIZE_U size) { m_SingalSize = size; }
	void SetAnimationTimerID(int id) { m_AnimationTimerID = id; }
};