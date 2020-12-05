#pragma once
#include "DesktopCat.h"
#include "Node.h"
#include "Cat.h"


//********************�ļ���Ϣ*************************
#ifndef FileDataParent
#define FileDataParent		"File\\"			//�����ļ���Ŀ¼
#endif
#ifndef SpritePathParent
#define SpritePathParent    "Image\\"           //����ͼƬ��Ŀ¼
#endif
#ifndef MainFileDataName
#define MainFileDataName	"Main.txt"			//�����������ʱ ��������� �ߴ��С
#endif
#define ValidDataSpriteName 2                   //��ž�������λ��
#define ValidDataSpriteSize	3					//��ž���ߴ�λ��
//*****************************************************
#define PI              3.1415926
#define Angle_To_Radin(_Angle)  (PI/180*(_Angle))    //�Ƕ�ת����
#define InitDirection   180.0                        //Arc�˶�ʱ ��ʼ�˶�����
#define WM_MESSAGE	WM_USER + 1	//�Զ���������Ϣ
#define ID_TRAY	1				//����ID

#define Window_Move_Timer		0	//�����ƶ���ʱ���Ĺ̶�ID

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
	POINT m_Position;               //�������Ͻ�����
public:
	bool init(HINSTANCE);

	void SetClassName(LPCWSTR _Name) { m_WndEx.lpszClassName = _Name; }			//���� ������ ����
	void SetWindowName(LPCWSTR _Name) { wsprintf(m_WindowName, _Name); }		//���� ���� ����
	void SetClassStyle(DWORD _Style) { m_ClassStyle = _Style; }					//���� ������ ���
	void SetWindowStyle(DWORD _Style) { m_WindowStyle = _Style; }				//���� ���� ���
	void SetWindowExStyle(DWORD _Style) { m_WindowExStyle = _Style; }			//���� ������չ ���
	void SetWindowSize(SIZE _size) { m_WindowSize = _size; }					//���� ���� ��С
	void SetWndProc(void* _proc) { m_WndProc = _proc; }							//���� ���ڻص�����
	void SetPosition(POINT _pt);												//���� �������Ͻ�����
	void SethIcon(HICON _hicon) { m_WndEx.hIcon = _hicon; }

	SIZE GetWindowSize() { return m_WindowSize; }
	POINT GetPosition() { return m_Position; }

	HWND GetWindowHWND() { return m_Hwnd; }
	WNDCLASSEX GetWindowWndClass() { return m_WndEx; }

	void Show(int _nCmdShow);                                                       //ע�� ���� ����ʾ����

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
	public: enum TrunStyle			//������ת��ʽ
	{
		Clockwise = 1,				//˳ʱ����ת
		Counterclockwise = -1		//��ʱ����ת
	};
private:
	//��Ա����
	//WNDCLASSEX m_WndEx;		//����û�ã���ʱ�����
	WindowEx* m_Window;
	NOTIFYICONDATA m_Tray;      //���̽ṹ��
	bool m_isExit;
	bool m_isStart;
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
	//LPoint m_WindowPosition;			//����λ��
	LPoint m_WindowFirst;				//�������λ�ã�ֻ���ڴ����˶���ʱ������ã���¼������һ���ƶ���һ��ʼ��λ��
	LPoint m_WindowDestination;		    //�����յ�λ�ã�ֻ���ڴ����˶���ʱ������ã�
	LPoint m_WindowSize;                //���ڳߴ�
	UINT m_MoveEntireTime;				//һ�δ����˶��涨����ʱ��
	UINT m_MoveAlreadyTimer;			//�������˶�ʱ��
	//LPoint m_MoveStep;			        //�����˶�ʱ ÿ֡�˶��ľ���
	double m_RotationAngle;				//ѡ��Arc�˶�ʱ ����ת�Ƕȴ���
	LPoint m_CircleRaidus;				//ѡ��Arc�˶�ʱ ���˶��뾶����
	double m_CircleAngle;					//�Դ�������Ϊԭ�㣬����������Բ�ĵ�������x��������нǳ�CricleAngle��
	TrunStyle m_TrunStyle;
	LPoint m_CircleCenter;				//ѡ��Arc�˶�ʱ ���˶�Բ�Ĵ���
private:
	void UpdateWindowMove();          //���´����˶�
	//����ֱ���˶��ĸ��´���
	void UdateWindowMoveLine();
	//����Բ���˶��ĸ��´���
	void UpdateWindowMoveArc();

	void createWindow(HINSTANCE);
	void createRefreshTimer();			//���ڻ��ơ��߼�����Ķ�ʱ��

	//�������ƶ���ָ��λ��
	void MoveWindowPosition(const LPoint&);

	//����A��C��תangle�Ⱥ����ڵ�λ�ã�radiusΪ-1ʱ�Զ�������ת�뾶
	LPoint CalculationArc(const LPoint& A, const LPoint& C, double angle, double circleAngle = -1, LPoint radius = -1, TrunStyle trunStyle = TrunStyle::Clockwise);

public:
	//������������
	GlassWindow();
	~GlassWindow() { }

	virtual void Draw();
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
	void WindowMoveArc(LPoint Radius, double angle, double circleAngle, DWORD time, TrunStyle style);
#pragma endregion

	//�ڴ����˶�֮ǰ��һЩ��ʼ������ֱ���˶���Բ���˶���
	void BeforeTheMoveOfWindowInit(const LPoint& destination, DWORD time, WindowMoveStyle style);
	void BeforeTheMoveOfWindowInit(const LPoint& destination, const LPoint& center, const LPoint& radius, double Angle, double circleAngle, DWORD time);
	//�ڴ����˶�֮���һЩ��ʼ��
	void AfterTheMoveOfWindowInit();
	void Start();
	//ֹͣ���ڵ��ƶ������ڴ��֮��Ĳ�����
	void StopMove();

	static GlassWindow* create(HINSTANCE _hInstance);
	static GlassWindow* getInstance() { return instance; }
	static void release(GlassWindow** _pGW);

	Cat* GetPet() { return m_Cat; }
	void SetExit(bool _b) { m_isExit = _b; }
	WindowEx* GetWndEx() { return m_Window; }
protected:
	//��д���෽��
	virtual bool init(HINSTANCE _hInstance);
	virtual void release();
public:
	//������
	//LPoint GetWindowPosition() { return m_WindowPosition; }
	LPoint GetWindowFirst() { return m_WindowFirst; }
	LPoint GetWindowDestination() { return m_WindowDestination; }
	LPoint GetWindowSize() { return m_WindowSize; }
	//��ȡ��������λ��
	LPoint GetWindowCenter() { return m_Position - (m_WindowSize / 2); }

	void SetisWindowMove(const bool is) { m_isWindowMove = is; }
	void SetWindowMoveStyle(const WindowMoveStyle style) { m_WindowMoveStyle = style; }
	//void SetWindowPosition(const LPoint& point) { m_WindowPosition = point; }
	void SetWindowFirst(const LPoint& first) { m_WindowFirst = first; }
	void SetWindowDestination(const LPoint& destination) { m_WindowDestination = destination; }
	void SetWindowSize(const LPoint& size) { m_WindowSize = size; }

} GWindow;




template<typename T>
void ReleaseD2D1Data(T _Data)
{
	if (_Data != nullptr)
		_Data->Release();
}
std::wstring string2wstring(const std::string& str);	//stringתwstring