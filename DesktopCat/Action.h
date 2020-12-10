#pragma once
#include "DesktopCat.h"
#include "Animation.h"

#ifndef ParentPath
#define ParentPath			"Assets\\"				//�����ļ���Ŀ¼
#endif
#ifndef ActionDataPath
#define ActionDataPath		"\\ActionData.txt"		//�����ƶ������ı��ļ�
#endif
#ifndef DataMarker
#define DataMarker					'#'				//��Ƿ������ڱ������������֮��ļ����
#endif
#ifndef DataMarkerEnd
#define DataMarkerEnd				'@'				//��ֹ�������ڱ�����ݽ�����
#endif
#ifndef GetFileData_Action_BeginLine
#define GetFileData_Action_BeginLine		2		//ActionData.txt�ļ���Ч���ݿ�ʼ����
#endif


class LAction
{
public:
	static LAction* create(LSprite* pSprite, const UINT& countLine);
	static LAction* create(LSprite* pSprite, const int actionStyle, const LPoint& value1, vector<float>& value2);
	static void release(LAction**);
	//�˶��켣��ʽ
	enum class ActionStyle
	{
		Min = -1,	//��Χ��Сֵ
		None,		//���˶�����ֹ
		ToLine,		//�����˶�
		ByLine,		//����˶�
		Arc,		//�����˶�
		Max			//��Χ���ֵ
	};
private:
	//�˶���ʽ
	LFileData<ActionStyle> m_ActionStyle;
	//����λ�ã����λ�ã���ת�뾶������һ�����ݰ�
	vector<LFileData<UINT>> m_Value1;
	//ֱ���˶�ֻ��1��������time�������˶���4��������angle��circleAngle��time��trunStyle���Ƕȣ���תԲ�����Լ��ļнǣ��˶�ʱ�䣬˳����ת��	����ʱ�������Բ���������ʽ����
	vector<LFileData<float>> m_Value2;
	//����ָ��
	LSprite* m_Sprite;
private:
	LAction():m_ActionStyle(ActionStyle(0)) { }
	~LAction() { }

	bool init(LSprite* pSprite, const UINT& countLine);
	bool init(LSprite* pSprite, const int actionStyle, const LPoint& value1, vector<float>& value2);
	void release();

	//�������
	void Censor();
public:
	//��ʼ���Ŷ���
	void PlayAction();
};