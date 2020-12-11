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

#pragma region 重写基类方法
bool Cat::init(const string& spriteName, const string& suffix, const LPoint& size)
{
	if (!LSprite::init(spriteName, suffix, size))
		return false;
	//设置精灵朝向
	m_Direction = CatDirection::Front;
	//初始为走路状态（从屏幕外走进来）
	m_Status = CatStatus::Idel;
	m_MotionStatus = CatMotionStatus::None;
	m_AnimationTimerID = Cat_AnimationTimer_ID;
	m_NowContinueTime = 0;
	//初始化随机种子
	srand(time(0));
	//随机一次发呆时间
	m_EntireIdelTime = rand() % (Cat_IdelEntireTime_Max - Cat_IdelEntireTime_Min) + Cat_IdelEntireTime_Min;
	//猫发呆多久后进行运动的定时器
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
		//发呆到达时间
		if (m_NowContinueTime >= m_EntireIdelTime)
		{
			RangeMotion();
		}
		else
		{
			//如果动画队列不为空，说明需要添加一个站立动画（无限循环）
			if (m_AnimationDeque.size() == 0)
			{
				OutputDebugString(L"添加一个站立动画\n");
				AddFrontAnimation(GetAnimationDataDirection(), static_cast<int>(LAnimation::LAnimationStyle::UnTime), 0, CatStatus::Idel, CatMotionStatus::None);
				LPoint Area = { static_cast<float>(0), static_cast<float>(GetAnimationDataDirection() - FileData_Animation_BeginLine) };
				SetDrawArea(Area);
			}
		}
	}
	else if (m_Status == CatStatus::Motion)
	{
		//如果达到运动时间，更改参数
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
	//OutputDebugString(L"Cat:动画开始\n");
}
void Cat::CallAnimationEnd()
{
	//LSprite::CallAnimationEnd();
	//OutputDebugString(L"Cat:动画结束\n");
	if (m_Status == CatStatus::Other)
	{
		BeforeTheChangeStatusOfCatInit(CatStatus::Idel, CatMotionStatus::None);
	}
}
void Cat::CallAnimationInterrupt()
{
	//LSprite::CallAnimationInterrupt();
	//OutputDebugString(L"Cat:动画被打断\n");
}
#pragma endregion


void Cat::MotionTo(const LPoint& destination, double speed, int actionDataLine)
{
	//计算与终点的距离，x轴，y轴方向
	LPoint D = destination - m_GlassWndow->GetPosition();
	//计算运动时间
	m_MotionTime = Division(sqrt(pow(D.m_x, 2) + pow(D.m_y, 2)) * M_To_MS, speed);
	//计算猫的方向
	SetDirection(D);
	//在队列前插入
	AddFrontAnimation(GetAnimationDataDirection() + actionDataLine, static_cast<int>(LAnimation::LAnimationStyle::Time), m_MotionTime, m_Status, m_MotionStatus);		//这里参数为Animation.txt行走动画的行数
	//添加Action
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
	//随机范围
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
	//播放口哨声
	string Path = ParentPath + GetName() + Music_Whistle;
	vector<wstring> FilePath = { string2wstring(Path) };
	PlayMusic(FilePath);
}

void Cat::Sit()
{
	m_GlassWndow->StopMove();
	//获取动画方向行数
	UINT FileLine = GetAnimationDataDirection();
	//切换坐姿
	AddFrontAnimation(FileLine + Cat_Idel_To_Sit, static_cast<int>(LAnimation::LAnimationStyle::UnTime), 0, CatStatus::Sit, CatMotionStatus::None);
	//播放猫声音
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
//		OutputDebugString(L"初始化失败\n");
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
//		m_Window->StopMove();		//停止窗口运动
//		Idel();
//		m_IdelTime->BeginTiming();	//开始计时发呆时间
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
//	m_IdelTime->EndTiming();			//结束计时发呆时间
//	DWORD IdelTime = m_IdelTime->GetRefreshInterval();	//获取发呆时间
//	if (IdelTime > 1000)				//如果发呆时间超过此时间
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
//	m_PetMode = HousePetMode::Idel;		//设置站立模式
//}