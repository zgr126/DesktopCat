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
	m_MotionStatus = CatMotionStatus::None;
	m_AnimationTimerID = Cat_AnimationTimer_ID;
	m_NowContinueTime = 0;
	//��ʼ���������
	srand(time(0));
	//���һ�η���ʱ��
	m_EntireIdelTime = rand() % (Cat_IdelEntireTime_Max - Cat_IdelEntireTime_Min) + Cat_IdelEntireTime_Min;
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

	m_NowContinueTime += m_IdelTimer->GetInterval();
	if (m_Status == CatStatus::Idel)
	{
		//��������ʱ��
		if (m_NowContinueTime >= m_EntireIdelTime)
		{
			RangeMotion();
		}
		else
		{
			//����������в�Ϊ�գ�˵����Ҫ���һ��վ������������ѭ����
			if (m_AnimationDeque.size() == 0)
			{
				OutputDebugString(L"���һ��վ������\n");
				AddFrontAnimation(GetAnimationDataDirection(), static_cast<int>(LAnimation::LAnimationStyle::UnTime), 0, CatStatus::Idel, CatMotionStatus::None);
				LPoint Area = { static_cast<float>(0), static_cast<float>(GetAnimationDataDirection() - FileData_Animation_BeginLine) };
				SetDrawArea(Area);
			}
		}
	}
	else if (m_Status == CatStatus::Motion)
	{
		//����ﵽ�˶�ʱ�䣬���Ĳ���
		if (m_NowContinueTime >= m_MotionTime)
		{
			BeforeTheChangeStatusOfCatInit(CatStatus::Idel, CatMotionStatus::None);
		}
	}
	else if (m_Status == CatStatus::Other)
	{
		//OutputDebugString(L"Other\n");
	}
}

void Cat::AddFrontAnimation(UINT fileLine, int animationStyle, UINT addValue, CatStatus status, CatMotionStatus motionStatus)
{
	LSprite::AddFrontAnimation(fileLine, animationStyle, addValue);
	BeforeTheChangeStatusOfCatInit(status, motionStatus);
}
void Cat::AddBackAnimation(UINT fileLine)
{
	LSprite::AddBackAnimation(fileLine);
}

void Cat::CallAnimationBegin()
{
	//LSprite::CallAnimationBegin();
	//OutputDebugString(L"Cat:������ʼ\n");
}
void Cat::CallAnimationEnd()
{
	//LSprite::CallAnimationEnd();
	//OutputDebugString(L"Cat:��������\n");
	if (m_Status == CatStatus::Other)
	{
		BeforeTheChangeStatusOfCatInit(CatStatus::Idel, CatMotionStatus::None);
	}
}
void Cat::CallAnimationInterrupt()
{
	//LSprite::CallAnimationInterrupt();
	//OutputDebugString(L"Cat:���������\n");
}
#pragma endregion


void Cat::MotionTo(const LPoint& destination, double speed, int actionDataLine)
{
	//�������յ�ľ��룬x�ᣬy�᷽��
	LPoint D = destination - m_GlassWndow->GetPosition();
	//�����˶�ʱ��
	m_MotionTime = Division(sqrt(pow(D.m_x, 2) + pow(D.m_y, 2)) * M_To_MS, speed);
	//����è�ķ���
	SetDirection(D);
	//�ڶ���ǰ����
	AddFrontAnimation(GetAnimationDataDirection() + actionDataLine, static_cast<int>(LAnimation::LAnimationStyle::Time), m_MotionTime, m_Status, m_MotionStatus);		//�������ΪAnimation.txt���߶���������
	//���Action
	vector<float> value2 = { static_cast<float>(m_MotionTime) };
	LAction* pAction = LAction::create(this, static_cast<int>(LAction::ActionStyle::ToLine), destination, value2);
}

void Cat::BeforeTheChangeStatusOfCatInit(CatStatus status, CatMotionStatus motionStatus)
{
	m_Status = status;
	m_MotionStatus = motionStatus;
	m_NowContinueTime = 0;
	m_IdelTimer->Reset();
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


void Cat::Call()
{
	POINT MousePostion;
	GetCursorPos(&MousePostion);
	RunTo({ static_cast<float>(MousePostion.x),static_cast<float>(MousePostion.y) });
	//���ſ�����
	string Path = ParentPath + GetName() + Music_Whistle;
	vector<wstring> FilePath = { string2wstring(Path) };
	PlayMusic(FilePath);
}

void Cat::Sit()
{
	m_GlassWndow->StopMove();
	//��ȡ������������
	UINT FileLine = GetAnimationDataDirection();
	//�л�����
	AddFrontAnimation(FileLine + Cat_Idel_To_Sit, static_cast<int>(LAnimation::LAnimationStyle::UnTime), 0, CatStatus::Sit, CatMotionStatus::None);
	//����è����
	string Path = ParentPath + GetName() + Music_CatReply;
	vector<wstring> FilePath = { string2wstring(Path) };
	PlayMusic(FilePath);
}

//
//Cat* Cat::create(string& _SpriteName, string _Suffix, LPoint& _Size)
//{
//	Cat* pet = new Cat();
//	if (pet->init(_SpriteName, _Suffix, _Size))
//		return pet;
//	delete pet;
//	return pet = nullptr;
//}
//
//void Cat::release(Cat** _pet)
//{
//	if (!(*_pet))	return;
//	(*_pet)->release();
//	delete* _pet;
//	*_pet = nullptr;
//}
//
//Cat::Cat() :m_Animation(nullptr), m_IdelTime(nullptr)
//{
//
//}
//
//bool Cat::init(string& _SpriteName, string _Suffix, LPoint& _Size)
//{
//	if (!LSprite::init(_SpriteName, _Suffix, _Size))
//	{
//		OutputDebugString(L"��ʼ��ʧ��\n");
//		return false;
//	}
//	m_IdelTime = LTimerManager::instance->createTimer(Cat_Idel_TimerID);
//	return true;
//}
//
//void Cat::release()
//{
//	Animation::release(&m_Animation);
//	LTimerManager::instance->releaseTimer(Cat_Idel_TimerID);
//	LSprite::release();
//}
//
//void Cat::Draw(ID2D1HwndRenderTarget* pRT)
//{
//	LSprite::Draw(pRT);
//}
//
//void Cat::Update()
//{
//	LSprite::Update();
//	if (Animation->GetisEnd())
//	{
//		m_Window->StopMove();		//ֹͣ�����˶�
//		Idel();
//		m_IdelTime->BeginTiming();	//��ʼ��ʱ����ʱ��
//	}
//}
//
//void Cat::PlayAnimation(int _PlayPosition, LAnimation::LAnimationStyle _Style, DWORD _Val, HousePetDirection _Direction)
//{
//	if (m_Animation == nullptr)
//		m_Animation = Animation::create(this);
//	m_Direction = _Direction;
//	m_Animation->Cutover(_PlayPosition, _Style, _Val);
//}
//
//void Cat::AI()
//{
//	m_IdelTime->EndTiming();			//������ʱ����ʱ��
//	DWORD IdelTime = m_IdelTime->GetRefreshInterval();	//��ȡ����ʱ��
//	if (IdelTime > 1000)				//�������ʱ�䳬����ʱ��
//		PlayAnimation(HPA::Attack, LAnimation::LAnimationStyle::Cycle, 1, HousePetDirection::Left);
//}
//
//void Cat::Idel()
//{
//	switch (m_Direction)
//	{
//		case HousePetDirection::Top:
//			PlayAnimation(HPA::TopIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::Top);
//			break;
//		case HousePetDirection::Bottom:
//			PlayAnimation(HPA::BottomIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::Bottom);
//			break;
//		case HousePetDirection::Left:
//			PlayAnimation(HPA::LeftIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::Left);
//			break;
//		case HousePetDirection::Right:
//			PlayAnimation(HPA::RightIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::Right);
//			break;
//		case HousePetDirection::LeftTop:
//			PlayAnimation(HPA::LeftTopIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::LeftTop);
//			break;
//		case HousePetDirection::RightTop:
//			PlayAnimation(HPA::RightTopIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::RightTop);
//			break;
//		case HousePetDirection::LeftBottom:
//			PlayAnimation(HPA::LeftBottomIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::LeftBottom);
//			break;
//		case HousePetDirection::RightBottom:
//			PlayAnimation(HPA::RightBottomIdel, LAnimation::LAnimationStyle::UnTime, 0, HousePetDirection::RightBottom);
//			break;
//	}
//	m_PetMode = HousePetMode::Idel;		//����վ��ģʽ
//}