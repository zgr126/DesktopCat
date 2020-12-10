#include "GlassWindows.h"
#include "Tool.h"
#include "LFile.h"

using namespace Lin;

#pragma region ������LWindowEx
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
{		//��ʾ����
	m_WndEx.style = m_ClassStyle;
	m_WndEx.lpfnWndProc = static_cast<WNDPROC>(m_WndProc);
	m_WndEx.hIcon = nullptr;
	m_WndEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_WndEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	///����ע����������SetClassNameʵ����
	m_WndEx.hIconSm = nullptr;

	RegisterClassExW(&m_WndEx);

	m_Hwnd = CreateWindowEx(m_WindowExStyle, m_WndEx.lpszClassName, m_WindowName, m_WindowStyle,
		postion.x, postion.y, m_WindowSize.cx, m_WindowSize.cy, NULL, NULL, m_WndEx.hInstance, NULL);

	ShowWindow(m_Hwnd, _CmdShow);
	UpdateWindow(m_Hwnd);
}
#pragma endregion



#pragma region ͸������GlassWindow

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

LRESULT CALLBACK GlassWindow::WndProc(HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	GWindow* GW = GWindow::getInstance();
	if (_wParam == ID_TRAY)
	{
		//����������Ϣ
		TrayHandleMsg(_lParam);
		OutputDebugString(L"����\n");
	}
	else if (ID_ALL_TRAYMENU(_wParam))		//�Ƚ����в˵���ID
	{
		//�������̲˵���Ϣ
		TrayHandleMenuMsg(_wParam);
	}
	switch (_message)
	{
		case WM_DESTROY:
			GW->SetExit(true);
			PostQuitMessage(0);
			break; 
		case WM_LBUTTONDOWN:
			//GW->WindowMoveArc({ 100,100 }, 360, 300, 1000, GlassWindow::TrunStyle::Counterclockwise);
			//GW->GetPet()->WalkBy(LPoint{ 400,0 });
			break;
		case WM_RBUTTONDOWN: 
			GW->WindowMoveTo({ 400,150 }, 1000);
			break;
		case WM_SIZE: {
			LONG_PTR Style = ::GetWindowLongPtr(_hwnd, GWL_STYLE);
			Style = Style & ~WS_CAPTION & ~WS_SYSMENU & ~WS_SIZEBOX;
			::SetWindowLongPtr(_hwnd, GWL_STYLE, Style);
		}
		default:
			return DefWindowProc(_hwnd, _message, _wParam, _lParam);
	}
}

void GlassWindow::release()
{
	LTimerManager::instance->releaseTimer(Window_Move_Timer_ID);		//�ͷŴ��ڶ�ʱ��
	Cat::release(&m_Cat);
}

bool GlassWindow::init(HINSTANCE _hInstance)
{
	createWindow(_hInstance);
	initPet();
	createRefreshTimer();
	//�������ô��ڴ�С
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
	//���ļ��ж�ȡ������Ϣ
	string MainFilePath(ParentPath);	MainFilePath += MainFileDataName;
	LFile* pMain = LFile::AddToFilePool(MainFilePath);			//��Main.txt�ļ�
	//��ȡ�������Ƽ������׺
	string SpriteNameData = LReadFileLine(pMain, GetFileData_SpriteNameData);
	FractureBack(SpriteNameData, DataMarkerEnd);
	string SpriteName = GetFirst(SpriteNameData, DataMarker);		//��������
	string SpriteSuffix = GetFirst(SpriteNameData, DataMarker);		//���������׺
	//��ȡ����ߴ�
	string SpriteSize = LReadFileLine(pMain, GetFileData_SpriteSize);
	LFileData<LPoint> Size = LFileData<LPoint>(0);
	string SpriteLength = GetFirst(SpriteSize, DataMarker);
	Size.m_Val.m_x = LDataConversion<float>::Conversion(SpriteLength).m_Val;
	SpriteLength = GetFirst(SpriteSize, DataMarker);
	Size.m_Val.m_y = LDataConversion<float>::Conversion(SpriteLength).m_Val;
	LFile::MoveOutFilePool(MainFilePath);						//�ر�Main.txt�ļ�
	//��������
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
	m_Tray.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_Tray.uCallbackMessage = WM_MESSAGE;		//�Զ������Ϣ����
	m_Tray.hIcon = LoadIcon(m_Window->GetWindowWndClass().hInstance, MAKEINTRESOURCE(IDI_SMALL));		//����ͼ��
	wsprintf(m_Tray.szTip, TEXT("Cat!"));

	Shell_NotifyIcon(NIM_ADD, &m_Tray);//�����������ͼ��
}

void GlassWindow::createWindow(HINSTANCE _hInstance)
{
	m_Window = WindowEx::create(_hInstance);
	m_Window->SetClassName(L"GalssWindow");		//��������
	m_Window->SetWindowName(L"Desktop Cat");	//������
	m_Window->SetClassStyle(CS_HREDRAW | CS_VREDRAW);								//���� ������ ���
	m_Window->SetWindowStyle(WS_OVERLAPPEDWINDOW);									//���� ���� ���
	m_Window->SetWindowExStyle(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED);	//���� ������չ ���
	m_Window->SetWindowSize(SIZE{ 150,200 });		//���ô��ڴ�С
	m_Window->SetWndProc(&GlassWindow::WndProc);	//���ڻص�����
	//m_Window->SetPosition(POINT{ 110,0 });
	m_Window->SethIcon(LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_SMALL)));		//����iconͼ��
	//MoveWindowPosition(0);
}

void GlassWindow::createRefreshTimer()
{
	phWait = CreateWaitableTimer(NULL, FALSE, NULL);
	liDueTime.QuadPart = -1i64;
	SetWaitableTimer(phWait, &liDueTime, 10, NULL, NULL, 0);	//ÿ10ms����һ��
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
	LPoint Position(400,300);
	SIZE size = m_Window->GetWindowSize();
	MoveWindowPosition(Position);
	SetWindowSize({ static_cast<float>(size.cx), static_cast<float>(size.cy) });

	LTimerManager::create();		//��ʼ����ʱ������

	HWND hwnd = m_Window->GetWindowHWND();
	initDirect2D(hwnd);
	//Ϊ�����ȡͼƬ
	string SpritePath = ParentPath + m_Cat->GetName() + "\\" + m_Cat->GetName() + m_Cat->GetSuffix();
	m_Cat->LoadingImage(string2wstring(SpritePath).c_str(), m_pRT, m_ImageFactory);

	SetLayeredWindowAttributes(
		hwnd,
		RGB(255, 255, 255),
		0,
		LWA_COLORKEY);

	//��������
	initTray();
}

void GlassWindow::run(int _CmdShow)
{
	runrelay(_CmdShow);
	while (!m_isExit)
	{
		dwRet = MsgWaitForMultipleObjects(1, &phWait, FALSE, INFINITE, QS_ALLINPUT);
		switch (dwRet)
		{
			case WAIT_OBJECT_0: {
				Draw(nullptr);
				Update();
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
	//�ͷ�ȫ�ֹ������ڴ�
	LTimerManager::release();
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
	LTimerManager::instance->Update();		//��ʱ�������ĸ���
	m_Cat->Update();
	if (m_isWindowMove)
		UpdateWindowMove();
	UpdateKeyInput();
}

void GlassWindow::UpdateWindowMove()
{
	//�ۼ����˶�ʱ��
	m_MoveAlreadyTimer += m_WindowMoveTimer->GetInterval();
	if (m_MoveAlreadyTimer >= m_MoveBothTime)
	{
		//�ﵽ�򳬳������˶�ʱ��	ֱ�ӽ������ƶ����յ�
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
	//����2��ʱ��İٷֱ�
	float Percentage = m_MoveAlreadyTimer / static_cast<double>(m_MoveBothTime);
	//���ݰٷֱȼ��㴰�ڵ�λ��
	LPoint Destination = (m_WindowDestination - m_WindowFirst) * Percentage + m_WindowFirst;
	MoveWindowPosition(Destination);
}

void GlassWindow::UpdateWindowMoveArc()
{
	//����2��ʱ��İٷֱ�
	double Percentage = m_MoveAlreadyTimer / static_cast<double>(m_MoveBothTime);
	//���ݰٷֱȼ�������ת�Ƕ�
	double Angle = Percentage * m_RotationAngle;
	//��Angle����С��360��Ľ�
	Angle = AngleConversion360(Angle);
	//��������ת�Ƕȼ��㴰�ڵ�λ��
	LPoint Destnation = CalculationArc(GetPosition(), m_CircleCenter, Angle, m_CircleAngle, m_CircleRaidus, m_TrunStyle);
	MoveWindowPosition(Destnation);
}

#define KEY_DOWM(vk_c) (GetAsyncKeyState(vk_c) & 0x8000 ? 1 : 0)
void GlassWindow::UpdateKeyInput()
{
	static int ControlKey = 0;
	if (ControlKey && !KEY_DOWM(VK_CONTROL))
	{
		//����Control��
		//OutputDebugString(L"����Control��\n");
		OutputDebugString(L"���ڴ�С��");
		OutputDebugString(to_wstring(m_Window->GetWindowSize().cx).c_str());
		OutputDebugString(L",");
		OutputDebugString(to_wstring(m_Window->GetWindowSize().cy).c_str());
		OutputDebugString(L"\n");
		OutputDebugString(L"�������Ͻ�λ�ã�");
		OutputDebugString(to_wstring(GetWindowPosition().m_x).c_str());
		OutputDebugString(L",");
		OutputDebugString(to_wstring(GetWindowPosition().m_y).c_str());
		OutputDebugString(L"\n");
		OutputDebugString(L"��������Ļ�ϵ�λ�ã�");
		OutputDebugString(to_wstring(GetPosition().m_x).c_str());
		OutputDebugString(L",");
		OutputDebugString(to_wstring(GetPosition().m_y).c_str());
		OutputDebugString(L"\n");
		OutputDebugString(L"��������λ�ã�");
		OutputDebugString(to_wstring(GetWindowCenter().m_x).c_str());
		OutputDebugString(L",");
		OutputDebugString(to_wstring(GetWindowCenter().m_y).c_str());
		OutputDebugString(L"\n");
		POINT MousePostion;
		GetCursorPos(&MousePostion);
		m_Cat->RunTo({ static_cast<float>(MousePostion.x),static_cast<float>(MousePostion.y) });
	}
	ControlKey = KEY_DOWM(VK_CONTROL);
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
	//Բ��λ��
	LPoint CircleCenter;
	//��������λ��
	LPoint WindowCenter = GetPosition();
	//��circleAngle����С��360��Ľ�
	circleAngle = AngleConversion360(circleAngle);
	//����Բ��λ��
	CircleCenter.m_x = static_cast<int>(WindowCenter.m_x + radius.m_x * cos(Angle_To_Radin(circleAngle)) + 0.5);	//��static_casst��ȥ�������� +0.5����������
	CircleCenter.m_y = static_cast<int>(WindowCenter.m_y + (radius.m_y * sin(Angle_To_Radin(circleAngle)) * -1) + 0.5);		//����*-1����Ϊ��Ļy��������������
	//����Ŀ�ĵ�
	LPoint destination;
	double Angle = angle;
	//��angle����С��360��Ľ�
	angle = AngleConversion360(angle);
	//�����յ㣨��������ͣ����λ�ã�
	destination = CalculationArc(WindowCenter, CircleCenter, angle, circleAngle, radius, trunStyle);
	//�ڴ����˶���ʼǰ����һЩ��ʼ��
	BeforeTheMoveOfWindowInit(destination, CircleCenter, radius, Angle, circleAngle, time);
}

LPoint GlassWindow::CalculationArc(const LPoint& A, const LPoint& C, double angle, double circleAngleAC, LPoint radius, TrunStyle trunStyle)
{
	if (circleAngleAC < 0);		//���Զ�����A��C��x��ļн�
	if (radius.m_x < 0);		//���Զ�����A��C�ľ���		�˴��Ⱥ���
	//��circleAngleAC����С��360��Ľ�
	circleAngleAC = AngleConversion360(circleAngleAC);
	//A��C����ԽǶȣ�circleAngleAC��C��A����ԽǶȣ��������������벻Ҫ���
	double CircleAngleCA;
	if (circleAngleAC < 180)
		CircleAngleCA = 180 + circleAngleAC;
	else
		CircleAngleCA = circleAngleAC - 180;
	//Ŀ�ĵ�
	LPoint Destination;
	//����Ŀ�ĵ�	��������angleһ��С��360�㣬����ġ�
	Destination.m_x = static_cast<int>(C.m_x + radius.m_x * cos(Angle_To_Radin(CircleAngleCA + (trunStyle * angle))) + 0.5);	//��static_casst��ȥ�������� +0.5����������
	Destination.m_y = static_cast<int>(C.m_y + (radius.m_y * sin(Angle_To_Radin(CircleAngleCA + (trunStyle * angle)) * -1)) + 0.5);	//����*-1����Ϊ��Ļy��������������
	return Destination;
}

void GlassWindow::BeforeTheMoveOfWindowInit(const LPoint& destination, DWORD time, WindowMoveStyle MoveStyle)
{
	//����UpdateWindowMove
	SetisWindowMove(true);
	//���ô������λ��
	SetWindowFirst(m_Position);
	//���ô����յ�λ��
	SetWindowDestination(destination);
	//�����˶�ʱ��
	m_MoveBothTime = time;
	//�������˶�ʱ��
	m_MoveAlreadyTimer = 0;
	//�����˶�����
	SetWindowMoveStyle(MoveStyle);
	//Ϊ�����˶�����һ����ʱ��		�ƶ��������ͷ�
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
	//�ر�UpdateWindowMove
	SetisWindowMove(false);
	//��ʼ�������λ��
	SetWindowFirst(0);
	//��ʼ�����յ�λ��
	SetWindowDestination(0);
	//��ʼ�˶���ʱ��
	m_MoveBothTime = 0;
	//��ʼ���˶�ʱ��
	m_MoveAlreadyTimer = 0;
	//���¶�Arc�˶��ĳ�ʼ��
	m_CircleCenter = 0;		//��ʼԲ��
	m_CircleRaidus = 0;		//��ʼ����ת�뾶
	m_RotationAngle = 0;	//��ʼ����ת�Ƕ�
	m_CircleAngle = 0;		//��ʼ����תԲ�ĽǶ�
	SetWindowMoveStyle(WindowMoveStyle::None);
	//�ͷŴ����˶���ʱ��
	LTimerManager::instance->releaseTimer(Window_Move_Timer_ID);
	m_WindowMoveTimer = nullptr;
}

void GlassWindow::StopMove()
{
	SetWindowDestination(m_Position);			//���ô����յ�λ��
	AfterTheMoveOfWindowInit();
}
#pragma endregion