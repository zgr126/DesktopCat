#include "GlassWindows.h"
#include "Tool.h"
#include "LFile.h"

using namespace Lin;

#define VK_F3	0x7B

#pragma region 窗口类LWindowEx
LWindowEx* LWindowEx::instance = nullptr;

LWindowEx* LWindowEx::create(HINSTANCE hInstance)
{
	if (instance != nullptr)
		delete instance;

	instance = new WindowEx();
	instance->init(hInstance);
	return instance;
}

void LWindowEx::release(LWindowEx* pWndEx)
{
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
	pWndEx = nullptr;
}

bool LWindowEx::init(HINSTANCE _hInstance)
{
	m_WndEx = { 0 };
	m_WndEx.cbSize = sizeof(WNDCLASSEX);
	m_WndEx.hInstance = _hInstance;
	return true;
}

void LWindowEx::Show(int _CmdShow, POINT postion)
{		//显示窗口
	m_WndEx.style = m_ClassStyle;
	m_WndEx.lpfnWndProc = static_cast<WNDPROC>(m_WndProc);
	//m_WndEx.hIcon = LoadIcon(m_WndEx.hInstance, MAKEINTRESOURCE(IDI_LOGO));
	m_WndEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_WndEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	///窗口注册类名已在SetClassName实现了
	//m_WndEx.hIconSm = LoadIcon(m_WndEx.hInstance, MAKEINTRESOURCE(IDI_LOGO));

	RegisterClassExW(&m_WndEx);

	m_Hwnd = CreateWindowEx(m_WindowExStyle, m_WndEx.lpszClassName, m_WindowName, m_WindowStyle,
		postion.x, postion.y, m_WindowSize.cx, m_WindowSize.cy, NULL, NULL, m_WndEx.hInstance, NULL);

	ShowWindow(m_Hwnd, _CmdShow);
	UpdateWindow(m_Hwnd);
}
#pragma endregion



#pragma region 透明窗口GlassWindow

GlassWindow* GlassWindow::instance = nullptr;

GlassWindow::GlassWindow() :m_isExit(false), m_Window(nullptr),
m_pFactory(nullptr), m_pRT(nullptr), m_pBrush(nullptr), m_ImageFactory(nullptr),
m_Cat(nullptr), m_WindowMoveTimer(nullptr)
{
}

GlassWindow* GlassWindow::create(HINSTANCE _hInstance)
{
	if (instance != nullptr)
		return instance;
	instance = new GlassWindow();
	instance->init(_hInstance);
	return instance;
}

void GlassWindow::release(GlassWindow** _pGW)
{
	if (!(*_pGW))	return;
	(*_pGW)->release();
	delete (*_pGW);
	(*_pGW) = nullptr;
}

LRESULT CALLBACK GlassWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GWindow* GW = GWindow::getInstance();
	switch (message)
	{
		//处理托盘消息
		case WM_TrayMessage:	TrayHandleMsg(lParam);	break;
		//处理托盘菜单消息
		case WM_COMMAND:		TrayHandleMenuMsg(wParam);	break;
		case WM_DESTROY:
			GW->SetExit(true);
			PostQuitMessage(0);
			break; 
		case WM_LBUTTONDOWN:
			//GW->WindowMoveArc({ 100,100 }, 360, 300, 1000, GlassWindow::TrunStyle::Counterclockwise);
			//GW->GetPet()->WalkBy(LPoint{ 400,0 });
			//GW->GetCat()->Scared();
			GW->GetCat()->OnClick();
			break;
		case WM_RBUTTONDOWN: 
		{
			OutputDebugString(L"右键按下\n");
			GW->GetCat()->Idel();
			GW->StopMove();
			GW->SetisRightDown(true);
			POINT pt;
			GetCursorPos(&pt);	//获取当前左键点击的坐标
			GW->SetRightDownPosition(pt);
		}
			break;
		case WM_RBUTTONUP:
			OutputDebugString(L"右键弹起\n");
			GW->SetisRightDown(false);
			GW->SetRightDownPosition({ 0 });
			break;
		case WM_SIZE: {
			LONG_PTR Style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
			Style = Style & ~WS_CAPTION & ~WS_SYSMENU & ~WS_SIZEBOX;
			::SetWindowLongPtr(hwnd, GWL_STYLE, Style);
		}
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

void GlassWindow::release()
{
	LTimerManager::instance->releaseTimer(Window_Move_Timer_ID);		//释放窗口定时器
	Cat::release(&m_Cat);
}

bool GlassWindow::init(HINSTANCE _hInstance)
{
	createWindow(_hInstance);
	initPet();
	createRefreshTimer();
	//重新设置窗口大小
	D2D1_SIZE_U WindowSize = m_Cat->GetSingalSize();
	m_Window->SetWindowSize(SIZE{ static_cast<LONG>(WindowSize.width), static_cast<LONG>(WindowSize.height) });
	m_WindowMoveStyle = WindowMoveStyle::None;
	return true;
}

void GlassWindow::initDirect2D(HWND _hwnd)
{
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);

	RECT WindowRc;
	GetWindowRect(_hwnd, &WindowRc);
	m_pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			_hwnd, D2D1::SizeU(WindowRc.right - WindowRc.left,
				WindowRc.bottom - WindowRc.top)),
		&m_pRT);

	m_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &m_pBrush);


	CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&m_ImageFactory
	);
}

void GlassWindow::initPet()
{	
	//从文件中读取精灵信息
	string MainFilePath(ParentPath);	MainFilePath += MainFileDataName;
	LFile* pMain = LFile::AddToFilePool(MainFilePath);			//打开Main.txt文件
	//读取精灵名称及纹理后缀
	string SpriteNameData = LReadFileLine(pMain, GetFileData_SpriteNameData);
	FractureBack(SpriteNameData, DataMarkerEnd);
	string SpriteName = GetFirst(SpriteNameData, DataMarker);		//精灵名称
	string SpriteSuffix = GetFirst(SpriteNameData, DataMarker);		//精灵纹理后缀
	//读取精灵尺寸
	string SpriteSize = LReadFileLine(pMain, GetFileData_SpriteSize);
	LFileData<LPoint> Size = LFileData<LPoint>(0);
	string SpriteLength = GetFirst(SpriteSize, DataMarker);
	Size.m_Val.m_x = LDataConversion<float>::Conversion(SpriteLength).m_Val;
	SpriteLength = GetFirst(SpriteSize, DataMarker);
	Size.m_Val.m_y = LDataConversion<float>::Conversion(SpriteLength).m_Val;
	LFile::MoveOutFilePool(MainFilePath);						//关闭Main.txt文件
	//创建精灵
	m_Cat = Cat::create(SpriteName, SpriteSuffix, Size.m_Val);
	m_Cat->SetGlassWindow(this);
	m_Cat->SetAnchor({ 0 });
	//m_Cat->WalkBy({ 400,0 });
	//m_Cat->AddFrontAnimation(3);
	//m_Cat->AddFrontAnimation(28);
	//m_Cat->AddAnimation(4);
}

void GlassWindow::initTray()
{
	m_Tray.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_Tray.hWnd = m_Window->GetWindowHWND();
	m_Tray.uID = ID_TRAY;
	m_Tray.uFlags = NIF_GUID | NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_Tray.uCallbackMessage = WM_TrayMessage;		//自定义的消息名称
	m_Tray.hIcon = LoadIcon(m_Window->GetWindowWndClass().hInstance, MAKEINTRESOURCE(IDI_LOGO));		//托盘图标
	wsprintf(m_Tray.szTip, TEXT("喵~!"));

	Shell_NotifyIcon(NIM_ADD, &m_Tray);//在托盘区添加图标
}

void GlassWindow::createWindow(HINSTANCE _hInstance)
{
	m_Window = WindowEx::create(_hInstance);
	m_Window->SetClassName(L"GalssWindow");		//窗口类名
	m_Window->SetWindowName(L"Desktop Cat");	//窗口名
	m_Window->SetClassStyle(CS_HREDRAW | CS_VREDRAW);								//设置 窗口类 风格
	m_Window->SetWindowStyle(WS_OVERLAPPEDWINDOW);									//设置 窗口 风格
	m_Window->SetWindowExStyle(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED);	//设置 窗口扩展 风格
	m_Window->SetWindowSize(SIZE{ 150,200 });		//设置窗口大小
	m_Window->SetWndProc(&GlassWindow::WndProc);	//窗口回调函数
	//m_Window->SetPosition(POINT{ 110,0 });
	m_Window->SethIcon(LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_SMALL)));		//设置icon图标
	//m_Window->SethIcon((HICON)LoadImage(NULL, L"Logo.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE));		//设置icon图标
	//MoveWindowPosition(0);
}

void GlassWindow::createRefreshTimer()
{
	phWait = CreateWaitableTimer(NULL, FALSE, NULL);
	liDueTime.QuadPart = -1i64;
	SetWaitableTimer(phWait, &liDueTime, 10, NULL, NULL, 0);	//每10ms触发一次
	dwRet = 0;
}

void GlassWindow::MoveWindowPosition(const LPoint& destination)
{
	m_Position = destination;
	float width = GetWindowSize().cx;
	float height = GetWindowSize().cy;
	MoveWindow(m_Window->GetWindowHWND(), destination.m_x - m_Anchor.m_x * width, destination.m_y - m_Anchor.m_y * height, width, height, FALSE);
}

void GlassWindow::runrelay(int _CmdShow)
{
	m_Window->Show(_CmdShow, { 0,0 });
	//HBRUSH brush;
	//HDC dc = GetDC(m_Window->GetWindowHWND());
	//brush = (HBRUSH)GetStockObject(DC_BRUSH);//参数必须是 DC_BURSH

	//SelectObject(dc, brush);//载入设备

	//SetDCBrushColor(dc, RGB(255, 255, 255));//成功修改画刷颜色。哇嘎嘎~
	//ReleaseDC(m_Window->GetWindowHWND(), dc);
	LPoint Position(-400,300);
	SIZE size = m_Window->GetWindowSize();
	MoveWindowPosition(Position);
	SetWindowSize({ static_cast<float>(size.cx), static_cast<float>(size.cy) });

	LTimerManager::create();		//初始化定时管理器

	HWND hwnd = m_Window->GetWindowHWND();
	initDirect2D(hwnd);


	//创建托盘
	initTray();
}

void GlassWindow::run(int _CmdShow)
{
	runrelay(_CmdShow);
	//创建线程用于更新窗口逻辑
	HANDLE thread = CreateThread(NULL, NULL, WindowLogicThread, NULL, 0, 0);
	while (!m_isExit)
	{
		dwRet = MsgWaitForMultipleObjects(1, &phWait, FALSE, INFINITE, QS_ALLINPUT);
		switch (dwRet)
		{
			case WAIT_OBJECT_0: {
				//通知子线程，更新窗口逻辑
				//m_isLogic = true;
				//LTimerManager::instance->Update();
				//Update();
				//Draw(nullptr);
			}
			break;
			case WAIT_OBJECT_0 + 1: {
				MSG msg;
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			break;
		}
	}
	//CloseHandle(thread);
}

void GlassWindow::Draw(ID2D1HwndRenderTarget*)
{
	m_pRT->BeginDraw();
	m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
	m_Cat->Draw(m_pRT);
	m_pRT->EndDraw();
}

void GlassWindow::Update()
{
	m_Cat->Update();
	if (m_isWindowMove)
		UpdateWindowMove();
	UpdateKeyInput();
	//右键窗口移动的的处理
	if (m_isRightDown)
	{
		POINT pt;
		POINT MouseDown = m_RightDownPosition;	//获取鼠标左键点击时的位置
		POINT WindowPoint = { static_cast<LONG>(GetPosition().m_x), static_cast<LONG>(GetPosition().m_y) };
		SIZE Size = GetWindowSize();
		RECT DesktopSize;
		GetWindowRect(GetDesktopWindow(), &DesktopSize);
		GetCursorPos(&pt);		//获取当前鼠标移动的位置
		MoveWindowPosition({ static_cast<float>(WindowPoint.x += (pt.x - MouseDown.x)), static_cast<float>(WindowPoint.y += (pt.y - MouseDown.y)) });;
		m_RightDownPosition = pt;
	}
}

void GlassWindow::UpdateWindowMove()
{
	//累加已运动时间
	m_MoveAlreadyTimer += m_WindowMoveTimer->GetInterval();
	if (m_MoveAlreadyTimer >= m_MoveBothTime)
	{
		//达到或超出窗口运动时间	直接将窗口移动到终点
		MoveWindowPosition(m_WindowDestination);
		AfterTheMoveOfWindowInit();
	}
	switch (m_WindowMoveStyle)
	{
		case WindowMoveStyle::Line:
			UdateWindowMoveLine();
			break;
		case WindowMoveStyle::Arc:
			UpdateWindowMoveArc();
			break;
		case WindowMoveStyle::None:
		default:
			return;
	}
}

void GlassWindow::UdateWindowMoveLine()
{
	//计算2个时间的百分比
	float Percentage = m_MoveAlreadyTimer / static_cast<double>(m_MoveBothTime);
	//根据百分比计算窗口的位置
	LPoint Destination = (m_WindowDestination - m_WindowFirst) * Percentage + m_WindowFirst;
	MoveWindowPosition(Destination);
}

void GlassWindow::UpdateWindowMoveArc()
{
	//计算2个时间的百分比
	double Percentage = m_MoveAlreadyTimer / static_cast<double>(m_MoveBothTime);
	//根据百分比计算已旋转角度
	double Angle = Percentage * m_RotationAngle;
	//把Angle换成小于360°的角
	Angle = AngleConversion360(Angle);
	//根据已旋转角度计算窗口的位置
	LPoint Destnation = CalculationArc(GetPosition(), m_CircleCenter, Angle, m_CircleAngle, m_CircleRaidus, m_TrunStyle);
	MoveWindowPosition(Destnation);
}

#define KEY_DOWM(vk_c) (GetAsyncKeyState(vk_c) & 0x8000 ? 1 : 0)
void GlassWindow::UpdateKeyInput()
{
	static int F4Key = 0;
	if (F4Key && !KEY_DOWM(VK_F4))
	{
		{
			//OutputDebugString(L"窗口大小：");
			//OutputDebugString(to_wstring(m_Window->GetWindowSize().cx).c_str());
			//OutputDebugString(L",");
			//OutputDebugString(to_wstring(m_Window->GetWindowSize().cy).c_str());
			//OutputDebugString(L"\n");
			//OutputDebugString(L"窗口左上角位置：");
			//OutputDebugString(to_wstring(GetWindowPosition().m_x).c_str());
			//OutputDebugString(L",");
			//OutputDebugString(to_wstring(GetWindowPosition().m_y).c_str());
			//OutputDebugString(L"\n");
			//OutputDebugString(L"中心在屏幕上的位置：");
			//OutputDebugString(to_wstring(GetPosition().m_x).c_str());
			//OutputDebugString(L",");
			//OutputDebugString(to_wstring(GetPosition().m_y).c_str());
			//OutputDebugString(L"\n");
			//OutputDebugString(L"窗口中心位置：");
			//OutputDebugString(to_wstring(GetWindowCenter().m_x).c_str());
			//OutputDebugString(L",");
			//OutputDebugString(to_wstring(GetWindowCenter().m_y).c_str());
			//OutputDebugString(L"\n");
		}
		//测试动画行
		//m_Cat->AddFrontAnimation(60, static_cast<int>(LAnimation::LAnimationStyle::Cycle), 3, Cat::CatStatus::Other, Cat::CatMotionStatus::None);
		//m_Cat->GetGlassWindow()->StopMove();
		m_Cat->AddFrontAnimation(2, 0, 0, Cat::CatStatus::Test);
	}
	F4Key = KEY_DOWM(VK_F4);
	//F3呼叫猫
	static int F3Key = 0;
	if (F3Key && !KEY_DOWM(VK_F3))
	{
		//呼叫猫
		m_Cat->Call();
	}
	F3Key = KEY_DOWM(VK_F3);

}

void GlassWindow::WindowMoveTo(const LPoint& destination, DWORD time)
{
	LPoint Destination(static_cast<int>(destination.m_x), static_cast<int>(destination.m_y));
	BeforeTheMoveOfWindowInit(destination, time, WindowMoveStyle::Line);
}

void GlassWindow::WindowMoveBy(const LPoint& destination, DWORD time)
{
	LPoint Destination = destination;
	Destination = Destination + m_Position;
	WindowMoveTo(Destination, time);
}

void GlassWindow::WindowMoveArc(LPoint radius, double angle, double circleAngle, DWORD time, TrunStyle trunStyle)
{
	m_TrunStyle = trunStyle;
	//圆心位置
	LPoint CircleCenter;
	//窗口中心位置
	LPoint WindowCenter = GetPosition();
	//将circleAngle换成小于360°的角
	circleAngle = AngleConversion360(circleAngle);
	//计算圆心位置
	CircleCenter.m_x = static_cast<int>(WindowCenter.m_x + radius.m_x * cos(Angle_To_Radin(circleAngle)) + 0.5);	//加static_casst是去掉浮点数 +0.5是四舍五入
	CircleCenter.m_y = static_cast<int>(WindowCenter.m_y + (radius.m_y * sin(Angle_To_Radin(circleAngle)) * -1) + 0.5);		//这里*-1是因为屏幕y轴是向下增长的
	//窗口目的地
	LPoint destination;
	double Angle = angle;
	//将angle换成小于360°的角
	angle = AngleConversion360(angle);
	//计算终点（窗口最终停留的位置）
	destination = CalculationArc(WindowCenter, CircleCenter, angle, circleAngle, radius, trunStyle);
	//在窗口运动开始前进行一些初始化
	BeforeTheMoveOfWindowInit(destination, CircleCenter, radius, Angle, circleAngle, time);
}

LPoint GlassWindow::CalculationArc(const LPoint& A, const LPoint& C, double angle, double circleAngleAC, LPoint radius, TrunStyle trunStyle)
{
	if (circleAngleAC < 0);		//请自动计算A与C与x轴的夹角
	if (radius.m_x < 0);		//请自动计算A与C的距离		此处先忽略
	//将circleAngleAC换成小于360°的角
	circleAngleAC = AngleConversion360(circleAngleAC);
	//A在C的相对角度（circleAngleAC是C在A的相对角度）这是两个变量请不要搞混
	double CircleAngleCA;
	if (circleAngleAC < 180)
		CircleAngleCA = 180 + circleAngleAC;
	else
		CircleAngleCA = circleAngleAC - 180;
	//目的地
	LPoint Destination;
	//计算目的地	传进来的angle一定小于360°，请放心。
	Destination.m_x = static_cast<int>(C.m_x + radius.m_x * cos(Angle_To_Radin(CircleAngleCA + (trunStyle * angle))) + 0.5);	//加static_casst是去掉浮点数 +0.5是四舍五入
	Destination.m_y = static_cast<int>(C.m_y + (radius.m_y * sin(Angle_To_Radin(CircleAngleCA + (trunStyle * angle)) * -1)) + 0.5);	//这里*-1是因为屏幕y轴是向下增长的
	return Destination;
}

void GlassWindow::BeforeTheMoveOfWindowInit(const LPoint& destination, DWORD time, WindowMoveStyle MoveStyle)
{
	//开启UpdateWindowMove
	SetisWindowMove(true);
	//设置窗口起点位置
	SetWindowFirst(m_Position);
	//设置窗口终点位置
	SetWindowDestination(destination);
	//设置运动时间
	m_MoveBothTime = time;
	//设置已运动时间
	m_MoveAlreadyTimer = 0;
	//设置运动类型
	SetWindowMoveStyle(MoveStyle);
	//为窗口运动创建一个计时器		移动结束后释放
	if(m_WindowMoveTimer == nullptr)
		m_WindowMoveTimer = LTimerManager::instance->createTimer(Window_Move_Timer_ID);
}
void GlassWindow::BeforeTheMoveOfWindowInit(const LPoint& destination, const LPoint& circleCenter, const LPoint& radius, double angle, double circleAngle, DWORD time)
{
	m_CircleCenter = circleCenter;
	m_CircleRaidus = radius;
	m_RotationAngle = angle;
	m_CircleAngle = circleAngle;
	BeforeTheMoveOfWindowInit(destination, time, WindowMoveStyle::Arc);
}

void GlassWindow::AfterTheMoveOfWindowInit()
{
	//关闭UpdateWindowMove
	SetisWindowMove(false);
	//初始窗口起点位置
	SetWindowFirst(0);
	//初始窗口终点位置
	SetWindowDestination(0);
	//初始运动总时间
	m_MoveBothTime = 0;
	//初始已运动时间
	m_MoveAlreadyTimer = 0;
	//以下对Arc运动的初始化
	m_CircleCenter = 0;		//初始圆心
	m_CircleRaidus = 0;		//初始化旋转半径
	m_RotationAngle = 0;	//初始化旋转角度
	m_CircleAngle = 0;		//初始化旋转圆心角度
	SetWindowMoveStyle(WindowMoveStyle::None);
	//释放窗口运动计时器
	LTimerManager::instance->releaseTimer(Window_Move_Timer_ID);
	m_WindowMoveTimer = nullptr;
}

void GlassWindow::StopMove()
{
	SetWindowDestination(m_Position);			//设置窗口终点位置
	AfterTheMoveOfWindowInit();
}
#pragma endregion