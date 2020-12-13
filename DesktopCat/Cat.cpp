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
	m_OtherStatus = CatOtherStatus::None;
	m_MotionStatus = CatMotionStatus::None;
	m_AnimationTimerID = Cat_AnimationTimer_ID;
	m_NowContinueTime = 0;
	m_isCommand = false;
	//初始化随机种子
	srand(time(0));
	//随机一次发呆时间
	m_ContinueTime = rand() % (Cat_LongIdelTime_Max - Cat_LongIdelTime_Min) + Cat_LongIdelTime_Min;
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
	//如果右键弹起，那么不给予新动画
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
	//计算与终点的距离，x轴，y轴方向
	LPoint D = destination - m_GlassWndow->GetPosition();
	//计算运动时间
	m_ContinueTime = Division(sqrt(pow(D.m_x, 2) + pow(D.m_y, 2)) * M_To_MS, speed);
	//计算猫的方向
	SetDirection(D);
	//在队列前插入
	AddFrontAnimation(GetAnimationDataDirection() + actionDataLine, static_cast<int>(LAnimation::LAnimationStyle::Time), m_ContinueTime, m_Status, m_MotionStatus);		//这里参数为Animation.txt行走动画的行数
	//添加Action
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
	//发呆到达时间
	if (m_NowContinueTime >= m_ContinueTime)
	{
		//有10%的概率坐下，90%的概率走路
		vector<CatStatus> vCatStatus = { CatStatus::Sit, CatStatus::Motion };
		vector<UINT> vProbability = { 10,90 };
		CatStatus Status = GetRange(vCatStatus, vProbability);
		if (Status == CatStatus::Motion)
		{
			RangeMotion();
		}
		else if (Status == CatStatus::Sit)
		{
			//随机一个久坐时间
			UINT SitTime = rand() % (Cat_LongSitTime_Max - Cat_LongSitTime_Min) + Cat_LongSitTime_Min;
			Sit(SitTime);
		}
	}
	else
	{
		//如果动画队列不为空，说明需要添加一个站立动画（无限循环）
		if (m_AnimationDeque.size() == 0)
		{
			//添加一个站立动画
			Idel();
		}
	}
}
void Cat::UpdateMotion()
{
	//如果时间未到，直接返回
	if (m_NowContinueTime < m_ContinueTime)	return;
	if (m_isCommand)
	{
		//如果是被命令的即呼叫
		m_isCommand = false;	//重置
		BeforeTheChangeStatusOfCatInit(CatStatus::Idel, CatMotionStatus::None);
		return;
	}
	//如果运动结束
	vector<CatStatus> vStatus = { CatStatus::Other, CatStatus::Motion };
	vector<UINT> vProbability = { Walk_To_Other, Walk_To_Idel };
	CatStatus Status = GetRange(vStatus, vProbability);
	switch (Status)
	{
		case CatStatus::Other:		//小概率会发生
		{
			vector<CatOtherStatus> vStatus = { CatOtherStatus::Lie,CatOtherStatus::BoringLie, CatOtherStatus::Lick, CatOtherStatus::Scratch, CatOtherStatus::Sniff };
			vector<UINT> vProbability = { Lie_Probability, BoringLie_Probability, Lick_Probability, Scratch_Probability, Sniff_Probability };
			m_OtherStatus = GetRange(vStatus, vProbability);
			AutoSetOtherStatus();
		}
		break;
		case CatStatus::Motion:		//大概率会发生
			BeforeTheChangeStatusOfCatInit(CatStatus::Idel, CatMotionStatus::None);
			m_OtherStatus = CatOtherStatus::None;
		break;
	}
}
void Cat::UpdateSit()
{
	//时间未到
	if (m_NowContinueTime < m_ContinueTime)	return;
	//如果久坐
	vector<CatOtherStatus> vStatus = { CatOtherStatus::Lie, CatOtherStatus::BoringLie, CatOtherStatus::Dig, CatOtherStatus::Lick, CatOtherStatus::Paw, CatOtherStatus::Relaex, CatOtherStatus::Sleep };
	vector<UINT> vProbability = { LongSit_To_Lie_Probability, LongSit_To_Boring_Probability, LongSit_To_Dig_Probability, LongSit_To_Lick_Probability, LongSit_To_Paw_Probability, LongSit_To_Relaex_Probability, LongSit_To_Sleep_Probability };
	m_OtherStatus = GetRange(vStatus, vProbability);
	AutoSetOtherStatus();
}
void Cat::UpdateSleep()
{
	//不与猫交互，猫永远不会醒来
}
void Cat::UpdateOther()
{
	//时间未到
	if (m_NowContinueTime < m_ContinueTime)	return;
	//时间已到
	switch (m_OtherStatus)
	{
		case CatOtherStatus::Lie:
		{
			vector<CatOtherStatus> vStatus = { CatOtherStatus::Lie, CatOtherStatus::BoringLie, CatOtherStatus::Sleep };
			vector<UINT> vProbability = { Lie_To_Lie_Probability, Lie_To_BoringLie_Probability, Lie_To_Sleep_Probability };
			m_OtherStatus = GetRange(vStatus, vProbability);
		}
		break;
		//惊吓后站立
		case CatOtherStatus::Scared:
		case CatOtherStatus::Sniff: 
		{
			//闻完后站立
			Idel();
			m_OtherStatus = CatOtherStatus::None;
			return;
		}
		break;
		default:
			m_OtherStatus = CatOtherStatus::None;	break;
	}
	//设置动作
	AutoSetOtherStatus();
}
void Cat::OnClick()
{
	switch (m_Status)
	{
		//站立时被点击
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
				//如果坐的时间不够久，直接站立
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
		case CatOtherStatus::Sleep:		this->Sleep(rand() % Cat_Sleep_Style);							break;	//睡觉
		case CatOtherStatus::Scared:	Scared();														break;	//惊吓
		case CatOtherStatus::Lie:		Lie(0, Range(Cat_LongLieTime_Min, Cat_LongLieTime_Max));		break;	//趴下
		case CatOtherStatus::BoringLie:	BoringLie(Range(Cat_LongLieTime_Min, Cat_LongLieTime_Max));		break;	//无聊地趴下
		case CatOtherStatus::Dig:		Dig(Range(Cat_LongDigTime_Min, Cat_LongDigTime_Max));			break;	//背对画圈圈
		case CatOtherStatus::Lick:		Lick(Range(Cat_LongLickTime_Min, Cat_LongLickTime_Max));		break;	//舔爪子
		case CatOtherStatus::Paw:		Paw(1);															break;	//抓虫子
		case CatOtherStatus::Relaex:	Relaex(Range(Cat_LongRelaexTime_Min, Cat_LongRelaexTime_Max));	break;	//四脚朝天
		case CatOtherStatus::Sniff:		Sniff(Range(Cat_LongSniffTime_Min, Cat_LongSniffTime_Max));		break;	//闻
		case CatOtherStatus::Stretch:	Stretch();														break;	//伸懒腰
		case CatOtherStatus::Sway:		Sway(Range(Cat_LongSwayTime_Min, Cat_LongSwayTime_Max));		break;	//兴奋
		case CatOtherStatus::Attack:	Attack(1);														break;	//攻击
		case CatOtherStatus::None:		Sit(Range(Cat_LongSitTime_Min, Cat_LongSitTime_Max));			break;	//坐姿
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
	//这是被命令的
	m_isCommand = true;
}
void Cat::Scared()
{
	m_Direction = GetAnimationHorizontal();
	int Offset = m_Direction == CatDirection::Right ? 1 : 0;
	m_ContinueTime = AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Scared) + Offset);
	m_OtherStatus = CatOtherStatus::Scared;
	//播放惊吓声
	string Path = ParentPath + GetName() + Music_Scared;
	vector<wstring> FilePath = { string2wstring(Path) };
	PlayMusic(FilePath);
}

void Cat::Idel()
{
	//添加一个站立动画
	AddFrontAnimation(GetAnimationDataDirection(), static_cast<int>(LAnimation::LAnimationStyle::UnTime), 0, CatStatus::Idel, CatMotionStatus::None);
	//随机一个猫的发呆时间
	m_ContinueTime = rand() % (Cat_LongIdelTime_Max - Cat_LongIdelTime_Min) + Cat_LongIdelTime_Min;
	LPoint Area = { static_cast<float>(0), static_cast<float>(GetAnimationDataDirection() - FileData_Animation_BeginLine) };
	SetDrawArea(Area);
	OutputDebugString(L"站立\n");
}
void Cat::Sit(UINT addValue)
{
	m_GlassWndow->StopMove();
	//获取动画方向行数
	UINT FileLine = GetAnimationDataDirection();
	//坐姿持续时间
	m_ContinueTime = addValue;
	//切换坐姿
	AddFrontAnimation(FileLine + Cat_Idel_To_Sit, static_cast<int>(LAnimation::LAnimationStyle::Time), addValue, CatStatus::Sit, CatMotionStatus::None);
	OutputDebugString(L"坐下\n");
}
void Cat::Sleep(UINT sleepStyle)
{
	//睡觉持续时间
	m_ContinueTime = 0;
	//计算偏移
	m_Direction = GetAnimationHorizontal();
	int Offest = (m_Direction == CatDirection::Right ? 1 : 0) + (2 * sleepStyle);
	//切换睡觉
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Sleep) + Offest, static_cast<int>(LAnimation::LAnimationStyle::UnTime), 0, CatStatus::Sleep, CatMotionStatus::None);
	OutputDebugString(L"睡觉\n");
}
void Cat::Lie(UINT lieStyle, UINT addValue)
{
	//躺下持续时间
	m_ContinueTime = addValue;
	//获取水平方向	若向右需要进行偏移
	m_Direction = GetAnimationHorizontal();
	//根据方向偏移行数
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0) + (2 * lieStyle);
	//切换躺下
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Lie) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Time), addValue);
	OutputDebugString(L"躺下\n");
}
void Cat::BoringLie(UINT addValue)
{
	Lie(1, addValue);
}
void Cat::Dig(UINT addValue)
{
	m_ContinueTime = addValue;
	//获取水平方向	若向右需要进行偏移
	CatDirection Horizontal = GetAnimationHorizontal();
	//根据方向偏移行数
	int Offset = (Horizontal == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Dig) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Time), addValue);
	OutputDebugString(L"背对画圈圈\n");
}
void Cat::Lick(UINT addValue)
{
	m_ContinueTime = addValue;
	//舔爪子只有1个方向，所以不用获取方向
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Lick), static_cast<int>(LAnimation::LAnimationStyle::Time), addValue);
	m_Direction = CatDirection::Front;
	OutputDebugString(L"舔爪子\n");
}
void Cat::Paw(UINT addValue)
{
	//这是是周期，时间是不需要算的
	m_Direction = GetAnimationHorizontal();
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Paw) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Cycle), addValue);
	OutputDebugString(L"抓虫子\n");
}
void Cat::Relaex(UINT addValue)
{
	m_ContinueTime = addValue;
	m_Direction = GetAnimationHorizontal();
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Relaex) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Time), addValue);
	OutputDebugString(L"四脚朝天\n");
}
void Cat::Scratch(UINT addValue)
{
	m_ContinueTime = addValue;
	m_Direction = GetAnimationHorizontal();
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Scratch) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Time), addValue);
	OutputDebugString(L"抓地板\n");
}
void Cat::Sniff(UINT addValue)
{
	m_ContinueTime = addValue;
	m_Direction = GetAnimationHorizontal();
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Sniff) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Time), addValue);
	OutputDebugString(L"闻\n");
}
void Cat::Stretch()
{
	//不需要计时，因为他是周期
	m_Direction = GetAnimationHorizontal();
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Stretch) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Cycle), 1);
	OutputDebugString(L"伸懒腰\n");
}
void Cat::Sway( UINT addValue)
{
	m_ContinueTime = addValue;
	m_Direction = GetAnimationHorizontal();
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Sway) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Time), addValue);
	OutputDebugString(L"兴奋\n");
}
void Cat::Attack(UINT addValue)
{
	//这是周期，时间是需要要算的
	m_Direction = GetAnimationHorizontal();
	int Offset = (m_Direction == CatDirection::Right ? 1 : 0);
	AddFrontAnimation(static_cast<UINT>(CatOtherStatus::Stretch) + Offset, static_cast<int>(LAnimation::LAnimationStyle::Cycle), addValue);
	OutputDebugString(L"攻击\n");
}