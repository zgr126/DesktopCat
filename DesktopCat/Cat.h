#pragma once
#include "DesktopCat.h"
#include "Sprite.h"

#define Cat_AnimationTimer_ID	10		//è������ʱ������ID

class Cat :public LSprite
{
public:
	static Cat* create(const string& spriteName, const string& suffix, const LPoint& size);
	static void release(Cat**);
#pragma region ��д���෽��	init,release,Draw,Update
protected:
	virtual bool init(const string& spriteName, const string& suffix, const LPoint& size);
	virtual void release();
public:
	virtual void Draw(ID2D1HwndRenderTarget*);
	virtual void Update();
#pragma endregion

private:

public:

};





//#include "Animation.h"
//#include "Timer.h"
//
//class GlassWindow;
//
//#define Cat_Idel_TimerID			10		//è������ʱ���Ĺ̶�ID
//
//class Cat :public LSprite
//{
//public:
//	//��̬����������
//	static Cat* create(string& _SpriteName, string _Suffix, LPoint& _Size);
//	static void release(Cat**);
//
//public:
//	//ö��
//	enum class HousePetDirection
//	{		//��ɫ����(8������)
//		Top, Bottom, Left, Right,
//		LeftTop, RightTop, LeftBottom, RightBottom,
//		None,	//Null��ʾ�޳��� ���⶯������ʱ������Null�Ա�AI���Գ����ⷽ���˶������糯�������������� ���������ߣ�
//	};
//	enum class HousePetMode
//	{			//״̬
//		Idel,		//����
//		Move,		//�˶�
//		Special		//ר�ж���
//	};
//	typedef enum HousePetPrefabAnimation
//	{	//��ɫԤ�ƶ���	(ö��ֵ��Ӧ�ļ�ActionData�ļ��ж������ݵı��)
//		Attack,		//ץ���� 
//		Balnk,		//գ�ۣ�������
//		Dig,		//������ץ�ذ�
//		Ennui,		//ſ�ڵ���ҡβ��
//		Fly,		//ſ�ڵ����޾����
//		Lick,		//��צ��
//		Paw,		//ץһ�µذ�
//		Relax,		//�Ľų���
//		Scratch,	//�εذ�
//		Sleep1,		//��������˯��
//		Sleep2,		//�����
//		Sleep3,		//�������۾�
//		Sniff,		//�ᣨվ�ţ�
//		Stretch,	//������
//		Sway,		//�˷ܵ�ҡβ��
//		Tail,		//�����ŷ�����ҡβ�ͣ�
//		BottomRun,		//����	��
//		LeftRun,		//����	��
//		RightRun,		//����	��
//		TopRun,			//����	��
//		LeftBottomRun,	//����	�L
//		RightBottomRun,	//����	�K
//		RightTopRun,	//����	�J
//		LeftTopRun,		//����	�I
//		BottomWalk,		//��·	��
//		LeftWalk,		//��·	��
//		RightWalk,		//��·	��
//		TopWalk,		//��·	��
//		LeftBottomWalk,	//��·	�L
//		RightBottomWalk,//��·	�K
//		RightTopWalk,	//��·	�J
//		LeftTopWalk,	//��·	�I
//		BottomIdel,		//վ��	��
//		LeftIdel,		//վ��	��
//		RightIdel,		//վ��	��
//		TopIdel,		//վ��	��
//		LeftBottomIdel,	//վ��	�L
//		RightBottomIdel,//վ��	�K
//		RightTopIdel,	//վ��	�J
//		LeftTopIdel,	//վ��	�I
//	} HPA;
//protected:
//	//��Ա����
//	Animation* m_Animation;
//	HousePetDirection m_Direction;	//��ǰ����
//	HousePetMode m_PetMode;			//�˶�״̬
//	GlassWindow* m_Window;			//���ﴰ�ھ��
//	LTimer* m_IdelTime;				//����ʱ��
//protected:
//	//��������
//	virtual bool init(string& _SpriteName, string _Suffix, LPoint& _Size);
//	virtual void release();
//public:
//	//��������
//	Cat();
//
//	virtual void Draw(ID2D1HwndRenderTarget*);
//	virtual void Update();
//
//	/*
//		_FileLine	��ȡ�ļ���_FileLine����������	(��0��ʼ)
//		_Style		��������
//		_Val		��_StyleΪCycleʱ	_ValΪ����ѭ������
//					��_StyleΪTimeʱ		_ValΪ�����̶�����ʱ��
//					��_StyleΪUnTimeʱ	_Val����������ֵ ��Ϊ_Val�ᱻ����*/
//	void PlayAnimation(int _FileLine, LAnimation::LAnimationStyle _Style, DWORD _Val, Cat::HousePetDirection _Direction);
//
//private:
//	void AI();		//��������ʱ ����AI����
//	void Idel();	//����
//
//public:
//	GlassWindow* SetGlassWindow(GlassWindow* GWindow) { m_Window = GWindow; }
//};