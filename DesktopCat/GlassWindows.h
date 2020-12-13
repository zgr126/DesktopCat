#pragma once
#include "DesktopCat.h"
#include "Node.h"
#include "Cat.h"
#include "Sprite.h"
#include "Timer.h"


//********************�ļ���Ϣ*************************
#ifndef ParentPath
#define ParentPath		"Assets\\"				//�����ļ���Ŀ¼
#endif
#ifndef MainFileDataName
#define MainFileDataName	"Main.txt"			//�����������ʱ ��������� �ߴ��С
#endif
#define GetFileData_SpriteNameData	2			//��ž�����������
#define GetFileData_SpriteSize		3			//��ž���ߴ�����
#ifndef DataMarker
#define DataMarker					'#'			//��Ƿ������ڱ������������֮��ļ����
#endif
#ifndef DataMarkerEnd
#define DataMarkerEnd				'@'			//��ֹ�������ڱ�����ݽ�����
#endif
//*****************************************************
#define PI              3.1415926
#define Angle_To_Radin(_Angle)  (PI/180*(_Angle))    //�Ƕ�ת����
#define InitDirection   180.0                        //Arc�˶�ʱ ��ʼ�˶�����
#define WM_TrayMessage	WM_USER + 1	//�Զ���������Ϣ
#define ID_TRAY	1				//����ID

//���̲˵�ID
#define Tray_Menu_Exit_ID		301		//�˳�
#define Tray_Menu_SitDown_ID	302		//����
#define ID_ALL_TRAYMENU(ID)\
	ID==Tray_Menu_Exit_ID?1:0 || ID==Tray_Menu_SitDown_ID?1:0

#define Window_Logic_FPS			15	//15ms����һ���߼�
#define Window_Logic_Timer_ID		0	//�����߼�����ʱ���Ĺ̶�ID
#define Window_Move_Timer_ID		1	//�����ƶ���ʱ���Ĺ̶�ID

class LWindowEx
{
public:
	static LWindowEx* instance;
private:
	WNDCLASSEX m_WndEx;
	HWND m_Hwnd;

	wchar_t m_WindowName[32];       //��������
	DWORD m_ClassStyle;             //������ ���        WndEx.style
	DWORD m_WindowStyle;            //���� ���          CreateWindowEx���ĸ�����
	DWORD m_WindowExStyle;          //������չ ���       CreateWindowEx��һ������
	SIZE m_WindowSize;              //���ڴ�С
	void* m_WndProc;                //���ڻص�����
public:
	bool init(HINSTANCE);

	void SetClassName(LPCWSTR name) { m_WndEx.lpszClassName = name; }		//���� ������ ����
	void SetWindowName(LPCWSTR name) { wsprintf(m_WindowName, name); }		//���� ���� ����
	void SetClassStyle(DWORD style) { m_ClassStyle = style; }				//���� ������ ���
	void SetWindowStyle(DWORD style) { m_WindowStyle = style; }				//���� ���� ���
	void SetWindowExStyle(DWORD style) { m_WindowExStyle = style; }			//���� ������չ ���
	void SetWindowSize(SIZE size) { m_WindowSize = size; }					//���� ���� ��С
	void SetWndProc(void* proc) { m_WndProc = proc; }						//���� ���ڻص�����
	void SethIcon(HICON hicon) { m_WndEx.hIcon = hicon; }

	SIZE GetWindowSize() { return m_WindowSize; }

	HWND GetWindowHWND() { return m_Hwnd; }
	WNDCLASSEX GetWindowWndClass() { return m_WndEx; }

	void Show(int _nCmdShow, POINT position);				//ע�� ���� ����ʾ����

	//_ClassStyleΪ�������� _WindowStyleΪ��������ʱ�ķ��
	static LWindowEx* create(HINSTANCE);
	static LWindowEx* getInstance() { return instance; }
	static void release(LWindowEx*);
};

typedef LWindowEx WindowEx;

typedef class GlassWindow :public LNode
{
public:
	static GlassWindow* instance;
	static LRESULT CALLBACK WndProc(HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam);	//���ڹ��̺���

//ö��
private: 
	enum class WindowMoveStyle		//�����˶���ʽ
	{
		None,
		Line,
		Arc
	};
	public:
	enum TrunStyle			//������ת��ʽ
	{
		Clockwise = 1,				//˳ʱ����ת
		Counterclockwise = -1		//��ʱ����ת
	};
private:
	//��Ա����
	WindowEx* m_Window;
	NOTIFYICONDATA m_Tray;			//���̽ṹ��
	bool m_isExit;
	bool m_isRightDown;				//�Ƿ��Ҽ������϶�����
	POINT m_RightDownPosition;		//�Ҽ����µ�λ��
	HANDLE phWait;
	LARGE_INTEGER liDueTime;
	DWORD dwRet;
	//Direct2D
	ID2D1Factory* m_pFactory;
	ID2D1HwndRenderTarget* m_pRT;
	ID2D1SolidColorBrush* m_pBrush;
	IWICImagingFactory* m_ImageFactory;

	Cat* m_Cat;			//Cat

	LTimer* m_WindowMoveTimer;	//���ڶ�ʱ��

	WindowMoveStyle m_WindowMoveStyle;
	bool m_isWindowMove;                //�Ƿ����˶�
	LPoint m_WindowFirst;				//�������λ�ã�ֻ���ڴ����˶���ʱ������ã���¼������һ���ƶ���һ��ʼ��λ��
	LPoint m_WindowDestination;		    //�����յ�λ�ã�ֻ���ڴ����˶���ʱ������ã�
	UINT m_MoveBothTime;				//һ�δ����˶��涨����ʱ��
	UINT m_MoveAlreadyTimer;			//�������˶�ʱ��
	double m_RotationAngle;				//ѡ��Arc�˶�ʱ ����ת�Ƕȴ���
	LPoint m_CircleRaidus;				//ѡ��Arc�˶�ʱ ���˶��뾶����
	double m_CircleAngle;				//�Դ�������Ϊԭ�㣬����������Բ�ĵ�������x��������нǳ�CricleAngle��
	TrunStyle m_TrunStyle;
	LPoint m_CircleCenter;				//ѡ��Arc�˶�ʱ ���˶�Բ�Ĵ���
private:
	void UpdateWindowMove();			//���´����˶�
	//����ֱ���˶��ĸ��´���
	void UdateWindowMoveLine();
	//����Բ���˶��ĸ��´���
	void UpdateWindowMoveArc();
	//�����������
	void UpdateKeyInput();

	void createWindow(HINSTANCE);
	void createRefreshTimer();			//���ڻ��ơ��߼�����Ķ�ʱ��


	//����A��C��תangle�Ⱥ����ڵ�λ�ã�radiusΪ-1ʱ�Զ�������ת�뾶
	LPoint CalculationArc(const LPoint& A, const LPoint& C, double angle, double circleAngle = -1, LPoint radius = -1, TrunStyle trunStyle = TrunStyle::Clockwise);

public:
	//������������
	GlassWindow();
	~GlassWindow() { }

	virtual void Draw(ID2D1HwndRenderTarget*);
	virtual void Update();

	void initDirect2D(HWND);			//��ʼ��Direct2D
	void initPet();						//��ʼ��Pet
	void initTray();					//��ʼ������
	void runrelay(int _CmdShow);
	void run(int _CmdShow);

#pragma region �����˶����
	//����ֱ���˶�����������time�����������ƶ���destination�������˶���
	void WindowMoveTo(const LPoint& destination, DWORD time);
	//����ֱ���˶�����������time�����������ƶ�destination������˶���
	void WindowMoveBy(const LPoint& destination, DWORD time);
	//����Բ���˶�����������time�����������ư뾶Radius��Բ�����Դ�������Ϊԭ����Բ�ĵ����ߺ�x��������нǳ�CricleAngle�ȣ���תAngle��������ת��ʽΪstyle
	void WindowMoveArc(LPoint Radius, double angle, double circleAngle, DWORD time, TrunStyle TrunStyle);
#pragma endregion

	//�ڴ����˶�֮ǰ��һЩ��ʼ������ֱ���˶���Բ���˶���
	void BeforeTheMoveOfWindowInit(const LPoint& destination, DWORD time, WindowMoveStyle MoveStyle);
	void BeforeTheMoveOfWindowInit(const LPoint& destination, const LPoint& center, const LPoint& radius, double Angle, double circleAngle, DWORD time);
	//�ڴ����˶�֮���һЩ��ʼ��
	void AfterTheMoveOfWindowInit();
	//ֹͣ���ڵ��ƶ������ڴ��֮��Ĳ�����
	void StopMove();
	//�������ƶ���ָ��λ��
	void MoveWindowPosition(const LPoint&);

	static GlassWindow* create(HINSTANCE _hInstance);
	static GlassWindow* getInstance() { return instance; }
	static void release(GlassWindow** _pGW);

	Cat* GetCat() { return m_Cat; }
	void SetExit(bool _b) { m_isExit = _b; }
	WindowEx* GetWndEx() { return m_Window; }
protected:
	//��д���෽��
	virtual bool init(HINSTANCE _hInstance);
	virtual void release();
public:
	//������
	bool GetisExit() { return m_isExit; }
	bool GetisRightDown() { return m_isRightDown; }
	LPoint GetWindowFirst() { return m_WindowFirst; }
	LPoint GetWindowDestination() { return m_WindowDestination; }
	SIZE GetWindowSize() { return m_Window->GetWindowSize(); }
	//��ȡ��������λ��
	LPoint GetWindowCenter() { return { static_cast<float>(m_Window->GetWindowSize().cx / 2), static_cast<float>(m_Window->GetWindowSize().cy / 2) }; }
	//��ȡ�������Ͻ�����Ļ�ϵ�λ��
	LPoint GetWindowPosition()
	{
		//��ȡ��������Ļ�ϵ�λ��
		LPoint Position = m_Position;
		Position.m_x = static_cast<int>(Position.m_x - m_Anchor.m_x * m_Window->GetWindowSize().cx + 0.5);	//+0.5��������
		Position.m_y = static_cast<int>(Position.m_y - m_Anchor.m_y * m_Window->GetWindowSize().cy + 0.5);
		return Position;
	}
	NOTIFYICONDATA GetTray() { return m_Tray; }

	void SetisWindowMove(const bool is) { m_isWindowMove = is; }
	void SetisRightDown(const bool is) { m_isRightDown = is; }
	void SetRightDownPosition(const POINT& position) { m_RightDownPosition = position; }
	void SetWindowMoveStyle(const WindowMoveStyle style) { m_WindowMoveStyle = style; }
	void SetWindowFirst(const LPoint& first) { m_WindowFirst = first; }
	void SetWindowDestination(const LPoint& destination) { m_WindowDestination = destination; }
	void SetWindowSize(const LPoint& size) { m_Window->SetWindowSize( { static_cast<LONG>(size.m_x), static_cast<LONG>(size.m_y)} ); }


public:
	//����Ϊ������Ϣ
	static void TrayHandleMsg(LPARAM);
	static void TrayHandleMenuMsg(WPARAM);

	//Ϊ�����¼�
	static void OnTrayLButtonDown();
	static void OnTrayLButtonUp();
	static void OnTrayRButtonDown();
	static void OnTrayRButtonUp();


	//���̲˵��¼�
	static void OnTrayMenuExit();
	static void OnTrayMenuSitDown();

} GWindow;
//ʹ��һ���߳������ºͻ���
DWORD WINAPI WindowLogicThread(void* param);