#pragma once
#include "DesktopCat.h"
#include "Sprite.h"
#include "Timer.h"
#include "LFile.h"
#include "Tool.h"

using namespace Lin;
class LSprite;

#ifndef ParentPath
#define ParentPath				"Assets\\"					//�����ļ��ĸ�Ŀ¼
#endif
#ifndef AnimationDataPath
#define AnimationDataPath		"\\AnimationData.txt"		//�Ӵ��ļ��ж�ȡ����ߴ��Լ��ܹ��ж����鶯��	(�û��ṩ)
#endif
#ifndef DataMarker
#define DataMarker					'#'			//��Ƿ������ڱ������������֮��ļ����
#endif
#ifndef DataMarkerEnd
#define DataMarkerEnd				'@'			//��ֹ�������ڱ�����ݽ�����
#endif
//�и�ʽ
#define FileData_Animation_BeginLine		2		//AnimationData.txt��Ч���ݿ�ʼ����
//��Ч���ݲ�����ʽ
#define GetFileData_Animation_CountLine		0		//��0��λ�ò�����ţ��ö���ѡ��ͼƬ�ĵ�n��
#define GetFileData_Animation_PlayOrder		1		//��1��λ�ò�����ţ��ö���һ��������֡���л�˳�� �磺(0,1,2,3)
#define GetFileData_Animation_StayTime		2		//��2��λ�ò�����ţ���1֡��ͣ��ʱ��...�Դ�����
#define GetFileData_Animation_Style			3		//��3��λ�ò�����ţ��ö������1Ϊ����������2Ϊʱ��������3Ϊ����ѭ������
#define GetFileData_Animation_AddValue		4		//��4��λ�ò�����ţ����Ӳ�������StyleΪ1��2ʱ��Ҫָ�����Ӳ�����Ϊ3ʱ����Ϊ0
#define GetFileData_Animation_Action		5		//��5��λ�ò�����ţ��ö�����ʹ��AcionData.txt��һ�е�����

#define M_To_MS		1000.0						//��ת����

//����������
class LAnimation
{
public:
	//��̬����������
	static LAnimation* create(LSprite*);
	static void release(LAnimation**);
	//��������ö��
	enum class LAnimationStyle
	{
		None,		//�û������� ֻ����ϵͳʹ��			(�������մ��������ʱ ���������)
		Cycle,		//�������� ����һ������ֻѭ��1��		(���Ŵ���һ�� ���ܶ������ŵ���֡ ֱ��ɾ�� ���磺����)
		Time,		//ʱ������ ����һ�������̶�����1000ms	(ʱ��δ�� ����������ѭ������ ���磺�ܲ�)
		UnTime		//����ѭ������						(����ֹͣ ֱ����������� ���磺��ֹվ��)
	};
private:
	/********************************************/
	//���������ļ���·��
	string m_AnimationDataFilePath;

	//����1������������Ҫ�ı���
	//�ö���ѡ��ͼƬ�ĵ�m_CountLine��		(���ļ���ȡ)
	LFileData<UINT> m_CountLine;
	//һ���������ڵ�֡�л�˳��				(���ļ���ȡ)
	vector<LFileData<UINT>> m_vPlayOrder;
	//ÿ֡��ͣ��ʱ��	��λms				(���ļ���ȡ)
	vector<LFileData<UINT>> m_vStayTime;
	//��������							(���ļ���ȡ)
	LFileData<LAnimationStyle>	m_AnimationStyle;
	//��������							(���ļ���ȡ)
	LFileData<UINT> m_AddValue;			//��StyleΪ��������ʱ��AddValue����ѭ��������Ϊʱ������ʱ��AddValueΪ����������ʱ�䡣Ϊ����ѭ��ʱ��AddValueΪ0
	//����ActionData.txt��һ�е����ݡ�	(���ļ���ȡ)
	LFileData<UINT>	m_ActionTextLine;	//���Ϊ0������ȡActionData.txt������(��ԭ�ز���)
	//���ŵ�ǰ֡������	vector������
	UINT m_FrameIndex;
	//ÿ֡ͣ��ʱ�������	vector������
	UINT m_FrameStayTimeIndex;
	//����ÿ֡ͣ��ʱ��
	UINT m_FrameStyleTime;
	//��ǰ֡ͣ��ʱ��
	UINT m_NowFrameStayTime;
	//��ǰ��������ʱ��	(�Ӷ���������ɵ�������������ʱ��)
	UINT m_NowAnimationContinue;
	//���������Ƿ����
	bool m_isEnd;
	/*******************************************/
	LSprite* m_Sprite;
private:
	//ֻ��ͨ��create��������LAnimation����
	LAnimation();
	bool init(LSprite*);
	void release();

	//������飬��һЩ���ݲ�ȫ��vector����
	void Censor();

	//�ڶ������ſ�ʼ֮ǰ��һЩ��ʼ��
	void BeforeTheAnimationOfSpriteInit();
	//�ڶ������Ž������һЩ��ʼ��
	void AfterTheAnimationOfSpriteInit();

	//���ػ�������(����������ָ��������ͼ��ĳһ������λ��)	����LTimer*��ʾAnimation����û�ж�ʱ�����Ǵ�Sprite���Ͻ�����
	LPoint Update(LTimer* animationTimer);
	//������������Ķ�������
	bool isCycle();
	//���ʱ�������Ķ�������
	bool isTime(ULONGLONG interval);
	//�������ʱ�������Ķ�������
	void isUnTime();
	//�л���һ֡
	void NextFrame();
	//�л���һ������
	void NextCycle();

public:
	//�����ļ��е�countLine�ж���
	bool PlayAnimation(UINT fileLine);

	//������
	bool GetisEnd() { return m_isEnd; }
	LAnimationStyle GetAnimationStyle() { return m_AnimationStyle.m_Val; }
	UINT GetActionTextLine() { return  m_ActionTextLine.m_Val; }

	friend class LSprite;
};

typedef LAnimation Animation;