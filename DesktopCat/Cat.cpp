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

#pragma region ��д���෽��
bool Cat::init(const string& spriteName, const string& suffix, const LPoint& size)
{
	if (!LSprite::init(spriteName, suffix, size))
		return false;
	//���þ��鳯��
	m_Direction = CatDirection::Front;
	//��ʼΪ��·״̬������Ļ���߽�����
	m_Status = CatStatus::Idel;
	m_OtherStatus = CatOtherStatus::None;
	m_MotionStatus = CatMotionStatus::None;
	m_AnimationTimerID = Cat_AnimationTimer_ID;
	m_NowContinueTime = 0;
	m_isCommand = false;
	//��ʼ���������
	srand(time(0));
	//���һ�η���ʱ��
	m_ContinueTime = rand() % (Cat_LongIdelTime_Max - Cat_LongIdelTime_Min) + Cat_LongIdelTime_Min;
	//è������ú�����˶��Ķ�ʱ��
	m_IdelTimer = LTimerManager::instance->createTimer(Cat_IdelTimer_ID);
	if (m_IdelTimer == nullptr)	return false;
	return true;
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
	//����Ҽ�������ô�������¶���
	if (m_GlassWndow->GetisRightDown())	return;

	m_NowContinueTime += m_IdelTimer->GetInterval();
	if (m_Status == CatStatus::Idel)
	{
		//OutputDebugString(L"Idel\n");
		UpdateIdel();
	}
	else if (m_Status == CatStatus::Motion)
	{
		UpdateMotion();
	}
	else if (m_Status == CatStatus::Sit)
	{
		//OutputDebugString(L"Sit\n");
		UpdateSit();
	}
	else if (m_Status == CatStatus::Sleep)
	{
		UpdateSleep();
	}
	else if (m_Status == CatStatus::Other)
	{
		//OutputDebugString(L"Other\n");
		UpdateOther();
	}
}

UINT Cat::AddFrontAnimation(UINT fileLine, int animationStyle, UINT addValue, CatStatus status, CatMotionStatus motionStatus)
{
	UINT AddValue = LSprite::AddFrontAnimation(fileLine, animationStyle, addValue);
	BeforeTheChangeStatusOfCatInit(status, motionStatus);
	return AddValue;
}
void Cat::AddBackAnimation(UINT fileLine)
{
	LSprite::AddBackAnimation(fileLine);
}

void Cat::CallAnimationBegin()
{
	LSprite::CallAnimationBegin();
}
void Cat::CallAnimationEnd()
{
	LSprite::CallAnimationEnd();
	if (m_OtherStatus == CatOtherStatus::Stretch)
	{
		Sit(Range(Cat_LongSitTime_Min, Cat_LongSitTime_Max));
	}
}
void Cat::CallAnimationInterrupt()
{
	LSprite::CallAnimationInterrupt();
}
#pragma endregion


void Cat::MotionTo(const LPoint& destination, double speed, int actionDataLine)
{
	//�������յ�ľ��룬x�ᣬy�᷽��
	LPoint D = destination - m_GlassWndow->GetPosition();
	//�����˶�ʱ��
	m_ContinueTime = Division(sqrt(pow(D.m_x, 2) + pow(D.m_y, 2)) * M_To_MS, speed);
	//����è�ķ���
	SetDirection(D);
	//�ڶ���ǰ����
	AddFrontAnimation(GetAnimationDataDirection() + actionDataLine, static_cast<int>(LAnimation::LAnimationStyle::Time), m_ContinueTime, m_Status, m_MotionStatus);		//�������ΪAnimation.txt���߶���������
	//���Action
	vector<float> value2 = { static_cast<float>(m_ContinueTime) };
	LAction* pAction = LAction::create(this, static_cast<int>(LAction::ActionStyle::ToLine), destination, value2);
}

void Cat::BeforeTheChangeStatusOfCatInit(CatStatus status, CatMotionStatus motionStatus)
{
	m_Status = status;
	m_MotionStatus = motionStatus;
	m_NowContinueTime = 0;
	m_IdelTimer->Reset();
}

void Cat::UpdateIdel()
{
	//��������ʱ��
	if (m_NowContinueTime >= m_ContinueTime)
	{
		//��10%�ĸ������£�90%�ĸ�����·
		vector<CatStatus> vCatStatus = { CatStatus::Sit, CatStatus::Motion };
		vector<UINT> vProbability = { 10,90 };
		CatStatus Status = GetRange(vCatStatus, vProbability);
		if (Status == CatStatus::Motion)
		{
			RangeMotion();
		}
		else if (Status == CatStatus::Sit)
		{
			//���һ������ʱ��
			UINT SitTime = rand() % (Cat_LongSitTime_Max - Cat_LongSitTime_Min) + Cat_LongSitTime_Min;
			Sit(SitTime);
		}
	}
	else
	{
		//����������в�Ϊ�գ�˵����Ҫ���һ��վ������������ѭ����
		if (m_AnimationDeque.size() == 0)
		{
			//���һ��վ������
			Idel();
		}
	}
}
void Cat::UpdateMotion()
{
	//���ʱ��δ����ֱ�ӷ���
	if (m_NowContinueTime < m_ContinueTime)	return;
	if (m_isCommand)
	{
		//����Ǳ�����ļ�����
		m_isCommand = false;	//����
		BeforeTheChangeStatusOfCatInit(CatStatus::Idel, CatMotionStatus::None);
		return;
	}
	//����˶�����
	vector<CatStatus> vStatus = { CatStatus::Other, CatStatus::Motion };
	vector<UINT> vProbability = { Walk_To_Other, Walk_To_Idel };
	CatStatus Status = GetRange(vStatus, vProbability);
	switch (Status)
	{
		case CatStatus::Other:		//С���ʻᷢ��
		{
			vector<CatOtherStatus> vStatus = { CatOtherStatus::Lie,CatOtherStatus::BoringLie, CatOtherStatus::Lick, CatOtherStatus::Scratch, CatOtherStatus::Sniff };
			vector<UINT> vProbability = { Lie_Probability, BoringLie_Probability, Lick_Probability, Scratch_Probability, Sniff_Probability };
			m_OtherStatus = GetRange(vStatus, vProbability);
			AutoSetOtherStatus();
		}
		break;
		case CatStatus::Motion:		//����ʻᷢ��
			BeforeTheChangeStatusOfCatInit(CatStatus::Idel, CatMotionStatus::None);
			m_OtherStatus = CatOtherStatus::None;
		break;
	}
}
void Cat::UpdateSit()
{
	//ʱ��δ��
	if (m_NowContinueTime < m_ContinueTime)	return;
	//�������
	vector<CatOtherStatus> vStatus = { CatOtherStatus::Lie, CatOtherStatus::BoringLie, CatOtherStatus::Dig, CatOtherStatus::Lick, CatOtherStatus::Paw, CatOtherStatus::Relaex, CatOtherStatus::Sleep };
	vector<UINT> vProbability = { LongSit_To_Lie_Probability, LongSit_To_Boring_Probability, LongSit_To_Dig_Probability, LongSit_To_Lick_Probability, LongSit_To_Paw_Probability, LongSit_To_Relaex_Probability, LongSit_To_Sleep_Probability };
	m_OtherStatus = GetRange(vStatus, vProbability);
	AutoSetOtherStatus();
}
void Cat::UpdateSleep()
{
	//����è������è��Զ��������
}
void Cat::UpdateOther()
{
	//ʱ��δ��
	if (m_NowContinueTime < m_ContinueTime)	return;
	//ʱ���ѵ�
	switch (m_OtherStatus)
	{
		case CatOtherStatus::Lie:
		{
			vector<CatOtherStatus> vStatus = { CatOtherStatus::Lie, CatOtherStatus::BoringLie, CatOtherStatus::Sleep };
			vector<UINT> vProbability = { Lie_To_Lie_Probability, Lie_To_BoringLie_Probability, Lie_To_Sleep_Probability };
			m_OtherStatus = GetRange(vStatus, vProbability);
		}
		break;
		//���ź�վ��
		case CatOtherStatus::Scared:
		case CatOtherStatus::Sniff: 
		{
			//�����վ��
			Idel();
			m_OtherStatus = CatOtherStatus::None;
			return;
		}
		break;
		default:
			m_OtherStatus = CatOtherStatus::None;	break;
	}
	//���ö���
	AutoSetOtherStatus();
}
void Cat::OnClick()
{
	switch (m_Status)
	{
		//վ��ʱ�����
		case CatStatus::Idel:
		{
			vector<CatOtherStatus> vOtherStatus = { CatOtherStatus::Sniff, CatOtherStatus::Lie, CatOtherStatus::Relaex };
			vector<UINT> vProbability = { OnClick_Idel_To_Sniff_Probability, OnClick_Idel_To_Lie_Probability, OnClick_Idel_To_Relaex_Probability };
			m_OtherStatus = GetRange(vOtherStatus, vProbability);
			AutoSetOtherStatus();
		}
			break;
		case CatStatus::Motion:
			break;
		case CatStatus::Sit:
		{
			if (m_NowContinueTime <= m_ContinueTime * 0.5)
			{
				//�������ʱ�䲻���ã�ֱ��վ��
				m_OtherStatus = CatOtherStatus::None;
				Idel();
				break;
			}
			vector<CatOtherStatus> vOtherStatus = { CatOtherStatus::Stretch, CatOtherStatus::Scared };
			vector<UINT> vProbability = { OnClick_Sit_To_Stretch_Probability, OnClick_Sit_To_Scared_Probability };
			m_OtherStatus = GetRange(vOtherStatus, vProbability);
			AutoSetOtherStatus();
		}
			break;
		case CatStatus::Sleep:
		{
			vector<CatOtherStatus> vOtherStatus = { CatOtherStatus::Stretch, CatOtherStatus::Scared };
			vector<UINT> vProbability = { OnClick_Sleep_To_Stretch_Probability, OnClick_Sleep_To_Scared_Probability };
			m_OtherStatus = GetRange(vOtherStatus, vProbability);
			AutoSetOtherStatus();
		}
			break;
		case CatStatus::Other:
			break;
	}
}

void Cat::WalkTo(const LPoint& destination)
{
	BeforeTheChangeStatusOfCatInit(CatStatus::Motion, CatMotionStatus::Walk);
	MotionTo(destination, Cat_Walk_Speed, Cat_Idel_To_Walk);
}
void Cat::WalkBy(const LPoint& destination)
{
	WalkTo(m_GlassWndow->GetPosition() + destination);
}
void Cat::RunTo(const LPoint& destination)
{
	BeforeTheChangeStatusOfCatInit(CatStatus::Motion, CatMotionStatus::Run);
	MotionTo(destination, Cat_Run_Speed, Cat_Idel_To_Run);
}
void Cat::RunBy(const LPoint& destination)
{
	RunTo(m_GlassWndow->GetPosition() + destination);
}

void Cat::RangeMotion()
{
	HWND Desktop = GetDesktopWindow();
	RECT DesktopRc;
	GetWindowRect(Desktop, &DesktopRc);
	//�����Χ
	int RangeX = (DesktopRc.right - 2 * Cat_Range_X) + Cat_Range_X;
	int RangeY = (DesktopRc.bottom - 2 * Cat_Range_Y) + Cat_Range_Y;
	LPoint Destination = { static_cast<float>(rand() % RangeX), static_cast<float>(rand() % RangeY) };
	WalkTo(Destination);
}


void Cat::SetDirection(const LPoint& vec)
{
	double r = sqrt(pow(vec.m_x, 2) + pow(vec.m_y, 2));
	double Sin = vec.m_y / r;
	double Cos = vec.m_x / r;
	if (Sin > Sin337_5 && Sin <= Sin22_5 && Cos > Cos337_5)
	{
		m_Direction = CatDirection::Right;
	}
	else if (Sin > Sin22_5 && Sin <= Sin67_5 && Cos >= Cos67_5 && Cos < Cos22_5)
	{
		m_Direction = CatDirection::FrontRight;
	}
	else if (Sin > Sin112_5 && Cos >= Cos112_5 && Cos < Cos67_5)
	{
		m_Direction = CatDirection::Front;
	}
	else if (Sin <= Sin112_5 && Sin > Sin157_5 && Cos <= Cos112_5 && Cos > Cos157_5)
	{
		m_Direction = CatDirection::FrontLeft;
	}
	else if (Sin <= Sin157_5 && Sin > Sin202_5 && Cos <= Cos202_5)
	{
		m_Direction = CatDirection::Left;
	}
	else if (Sin <= Sin202_5 && Sin > Sin247_5 && Cos >= Cos202_5 && Cos < Cos247_5)
	{
		m_Direction = CatDirection::BackLeft;
	}
	else if (Sin < Sin292_5 && Cos >= Cos247_5 && Cos < Cos292_5)
	{
		m_Direction = CatDirection::Back;
	}
	else
	{
		m_Direction = CatDirection::BackRight;
	}
}

UINT Cat::GetAnimationDataDirection()
{
	switch (m_Direction)
	{
		case CatDirection::Front:	return	Cat_Front_Idle;
		case CatDirection::Back:	return	Cat_Back_Idle;
		case CatDirection::Left:	return	Cat_Left_Idle;
		case CatDirection::Right:	return	Cat_Right_Idle;
		case CatDirection::FrontLeft:	return	Cat_FrontLeft_Idle;
		case CatDirection::FrontRight:	return	Cat_FrontRight_Idle;
		case CatDirection::BackLeft:	return	Cat_BackLeft_Idle;
		case CatDirection::BackRight:	return	Cat_BackRight_Idle;
	}
}
Cat::CatDirection Cat::GetAnimationHorizontal()
{
	switch (m_Direction)
	{
		case CatDirection::Front:
		case CatDirection::Right:
		case CatDirection::FrontRight:
		case CatDirection::BackRight:		return CatDirection::Right;
		case CatDirection::Back:
		case CatDirection::Left:
		case CatDirection::FrontLeft:
		case CatDirection::BackLeft:		return CatDirection::Left;
	}
}
Cat::CatDirection Cat::GetAnimationVertical()
{
	switch (m_Direction)
	{
		case CatDirection::Front:
		case CatDirection::FrontLeft:
		case CatDirection::FrontRight:
		case CatDirection::Right:			return CatDirection::Front;
		case CatDirection::Back:
		case CatDirection::BackLeft:
		case CatDirection::BackRight:
		case CatDirection::Left:			return CatDirection::Back;
	}
}

void Cat::AutoSetOtherStatus()
{
	switch (m_OtherStatus)
	{
		case CatOtherStatus::Sleep:		this->Sleep(rand() % Cat_Sleep_Style);							break;	//˯��
		case CatOtherStatus::Scared:	Scared();														break;	//����
		case CatOtherStatus::Lie:		Lie(0, Range(Cat_LongLieTime_Min, Cat_LongLieTime_Max));		break;	//ſ��
		case CatOtherStatus::BoringLie:	BoringLie(Range(Cat_LongLieTime_Min, Cat_LongLieTime_Max));		break;	//���ĵ�ſ��
		case CatOtherStatus::Dig:		Dig(Range(Cat_LongDigTime_Min, Cat_LongDigTime_Max));			break;	//���Ի�ȦȦ
		case CatOtherStatus::Lick:		Lick(Range(Cat_LongLickTime_Min, Cat_LongLickTime_Max));		break;	//��צ��
		case CatOtherStatus::Paw:		Paw(1);															break;	//ץ����
		case CatOtherStatus::Relaex:	Relaex(Range(Cat_LongRelaexTime_Min, Cat_LongRelaexTime_Max));	break;	//�Ľų���
		case CatOtherStatus::Sniff:		Sniff(Range(Cat_LongSniffTime_Min, Cat_LongSniffTime_Max));		break;	//��
		case CatOtherStatus::Stretch:	Stretch();														break;	//������
		case CatOtherStatus::Sway:		Sway(Range(Cat_LongSwayTime_Min, Cat_LongSwayTime_Max));		break;	//�˷�
		case CatOtherStatus::Attack:	Attack(1);														break;	//����
		case CatOtherStatus::None:		Sit(Range(Cat_LongSitTime_Min, Cat_LongSitTime_Max));			break;	//����
	}
}

void Cat::Call()
{
	POINT MousePostion;
	GetCursorPos(&MousePostion);
	RunTo({ static_cast<float>(MousePostion.x),static_cast<float>(MousePostion.y) });
	//���ſ�����
	string Path = ParentPath + GetName() + Music_Whistle;
	vector<wstring> FilePath = { string2wstring(Path) };
	PlayMusic(FilePath);
	//���Ǳ������
	m_isCommand = true;
}
void Cat::Scared()
{
	m_Direction = GetAnimationHorizontal();
	int Offset = m_Direction == CatDirection::Right ? 1 : 0;
	m_ContinueTime = AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Scared) + Offset);
	m_OtherStatus = CatOtherStatus::Scared;
	//���ž�����
	string Path = ParentPath + GetName() + Music_Scared;
	vector<wstring> FilePath = { string2wstring(Path) };
	PlayMusic(FilePath);
}

void Cat::Idel()
{
	//���һ��վ������
	AddFrontAnimation(GetAnimationDataDirection(), static_cast<int>(LAnimation::LAnimationStyle::UnTime), 0, CatStatus::Idel, CatMotionStatus::None);
	//���һ��è�ķ���ʱ��
	m_ContinueTime = rand() % (Cat_LongIdelTime_Max - Cat_LongIdelTime_Min) + Cat_LongIdelTime_Min;
	LPoint Area = { static_cast<float>(0), static_cast<float>(GetAnimationDataDirection() - FileData_Animation_BeginLine) };
	SetDrawArea(Area);
	OutputDebugString(L"վ��\n");
}
void Cat::Sit(UINT addValue)
{
	m_GlassWndow->StopMove();
	//��ȡ������������
	UINT FileLine = GetAnimationDataDirection();
	//���˳���ʱ��
	m_ContinueTime = addValue;
	//�л�����
	AddFrontAnimation(FileLine + Cat_Idel_To_Sit, static_cast<int>(LAnimation::LAnimationStyle::Time), addValue, CatStatus::Sit, CatMotionStatus::None);
	OutputDebugString(L"����\n");
}
void Cat::Sleep(UINT sleepStyle)
{
	//˯������ʱ��
	m_ContinueTime = 0;
	//����ƫ��
	m_Direction = GetAnimationHorizontal();
	int Offest = (m_Direction == CatDirection::Right ? 1 : 0) + (2 * sleepStyle);
	//�л�˯��
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Sleep) + Offest, static_cast<int>(LAnimation::LAnimationStyle::UnTime), 0, CatStatus::Sleep, CatMotionStatus::None);
	OutputDebugString(L"˯��\n");
}
void Cat::Lie(UINT lieStyle, UINT addValue)
{
	//���³���ʱ��
	m_ContinueTime = addValue;
	//��ȡˮƽ����	��������Ҫ����ƫ��
	m_Direction = GetAnimationHorizontal();
	//���ݷ���ƫ������
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0) + (2 * lieStyle);
	//�л�����
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Lie) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Time), addValue);
	OutputDebugString(L"����\n");
}
void Cat::BoringLie(UINT addValue)
{
	Lie(1, addValue);
}
void Cat::Dig(UINT addValue)
{
	m_ContinueTime = addValue;
	//��ȡˮƽ����	��������Ҫ����ƫ��
	CatDirection Horizontal = GetAnimationHorizontal();
	//���ݷ���ƫ������
	int Offset = (Horizontal == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Dig) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Time), addValue);
	OutputDebugString(L"���Ի�ȦȦ\n");
}
void Cat::Lick(UINT addValue)
{
	m_ContinueTime = addValue;
	//��צ��ֻ��1���������Բ��û�ȡ����
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Lick), static_cast<int>(LAnimation::LAnimationStyle::Time), addValue);
	m_Direction = CatDirection::Front;
	OutputDebugString(L"��צ��\n");
}
void Cat::Paw(UINT addValue)
{
	//���������ڣ�ʱ���ǲ���Ҫ���
	m_Direction = GetAnimationHorizontal();
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Paw) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Cycle), addValue);
	OutputDebugString(L"ץ����\n");
}
void Cat::Relaex(UINT addValue)
{
	m_ContinueTime = addValue;
	m_Direction = GetAnimationHorizontal();
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Relaex) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Time), addValue);
	OutputDebugString(L"�Ľų���\n");
}
void Cat::Scratch(UINT addValue)
{
	m_ContinueTime = addValue;
	m_Direction = GetAnimationHorizontal();
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Scratch) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Time), addValue);
	OutputDebugString(L"ץ�ذ�\n");
}
void Cat::Sniff(UINT addValue)
{
	m_ContinueTime = addValue;
	m_Direction = GetAnimationHorizontal();
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Sniff) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Time), addValue);
	OutputDebugString(L"��\n");
}
void Cat::Stretch()
{
	//����Ҫ��ʱ����Ϊ��������
	m_Direction = GetAnimationHorizontal();
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Stretch) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Cycle), 1);
	OutputDebugString(L"������\n");
}
void Cat::Sway( UINT addValue)
{
	m_ContinueTime = addValue;
	m_Direction = GetAnimationHorizontal();
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Sway) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Time), addValue);
	OutputDebugString(L"�˷�\n");
}
void Cat::Attack(UINT addValue)
{
	//�������ڣ�ʱ������ҪҪ���
	m_Direction = GetAnimationHorizontal();
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Stretch) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Cycle), addValue);
	OutputDebugString(L"����\n");
}