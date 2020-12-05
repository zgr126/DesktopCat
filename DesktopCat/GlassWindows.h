#pragma once
#include "DesktopCat.h"
#include "Node.h"
#include "Cat.h"


//********************文件信息*************************
#ifndef FileDataParent
#define FileDataParent		"File\\"			//数据文件父目录
#endif
#ifndef SpritePathParent
#define SpritePathParent    "Image\\"           //纹理图片父目录
#endif
#ifndef MainFileDataName
#define MainFileDataName	"Main.txt"			//储存程序运行时 精灵的名称 尺寸大小
#endif
#define ValidDataSpriteName 2                   //存放精灵名称位置
#define ValidDataSpriteSize	3					//存放精灵尺寸位置
//*****************************************************
#define PI              3.1415926
#define Angle_To_Radin(_Angle)  (PI/180*(_Angle))    //角度转弧度
#define InitDirection   180.0                        //Arc运动时 初始运动方向
#define WM_MESSAGE	WM_USER + 1	//自定义托盘消息
#define ID_TRAY	1				//托盘ID

#define Window_Move_Timer		0	//窗口移动定时器的固定ID

class LWindowEx
{
public:
	static LWindowEx* instance;
private:
	WNDCLASSEX m_WndEx;
	HWND m_Hwnd;

	wchar_t m_WindowName[32];       //窗口名称
	DWORD m_ClassStyle;             //窗口类 风格        WndEx.style
	DWORD m_WindowStyle;            //窗口 风格          CreateWindowEx第四个参数
	DWORD m_WindowExStyle;          //窗口扩展 风格       CreateWindowEx第一个参数
	SIZE m_WindowSize;              //窗口大小
	void* m_WndProc;                //窗口回调函数
	POINT m_Position;               //窗口左上角坐标
public:
	bool init(HINSTANCE);

	void SetClassName(LPCWSTR _Name) { m_WndEx.lpszClassName = _Name; }			//设置 窗口类 名称
	void SetWindowName(LPCWSTR _Name) { wsprintf(m_WindowName, _Name); }		//设置 窗口 名称
	void SetClassStyle(DWORD _Style) { m_ClassStyle = _Style; }					//设置 窗口类 风格
	void SetWindowStyle(DWORD _Style) { m_WindowStyle = _Style; }				//设置 窗口 风格
	void SetWindowExStyle(DWORD _Style) { m_WindowExStyle = _Style; }			//设置 窗口扩展 风格
	void SetWindowSize(SIZE _size) { m_WindowSize = _size; }					//设置 窗口 大小
	void SetWndProc(void* _proc) { m_WndProc = _proc; }							//设置 窗口回调函数
	void SetPosition(POINT _pt);												//设置 窗口左上角坐标
	void SethIcon(HICON _hicon) { m_WndEx.hIcon = _hicon; }

	SIZE GetWindowSize() { return m_WindowSize; }
	POINT GetPosition() { return m_Position; }

	HWND GetWindowHWND() { return m_Hwnd; }
	WNDCLASSEX GetWindowWndClass() { return m_WndEx; }

	void Show(int _nCmdShow);                                                       //注册 创建 并显示窗口

	//_ClassStyle为窗口类风格 _WindowStyle为创建窗口时的风格
	static LWindowEx* create(HINSTANCE);
	static LWindowEx* getInstance() { return instance; }
	static void release(LWindowEx*);
};

typedef LWindowEx WindowEx;

typedef class GlassWindow :public LNode
{
public:
	static GlassWindow* instance;
	static LRESULT CALLBACK WndProc(HWND _hwnd, UINT _message, WPARAM _wParam, LPARAM _lParam);	//窗口过程函数

//枚举
private: 
	enum class WindowMoveStyle		//窗口运动样式
	{
		None,
		Line,
		Arc
	};
	public: enum TrunStyle			//窗口旋转样式
	{
		Clockwise = 1,				//顺时针旋转
		Counterclockwise = -1		//逆时针旋转
	};
private:
	//成员变量
	//WNDCLASSEX m_WndEx;		//可能没用，暂时不清楚
	WindowEx* m_Window;
	NOTIFYICONDATA m_Tray;      //托盘结构体
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

	LTimer* m_WindowMoveTimer;	//窗口定时器

	WindowMoveStyle m_WindowMoveStyle;
	bool m_isWindowMove;                //是否有运动
	//LPoint m_WindowPosition;			//窗口位置
	LPoint m_WindowFirst;				//窗口起点位置（只有在窗口运动的时候才有用）记录窗口在一段移动中一开始的位置
	LPoint m_WindowDestination;		    //窗口终点位置（只有在窗口运动的时候才有用）
	LPoint m_WindowSize;                //窗口尺寸
	UINT m_MoveEntireTime;				//一次窗口运动规定的总时间
	UINT m_MoveAlreadyTimer;			//窗口已运动时间
	//LPoint m_MoveStep;			        //窗口运动时 每帧运动的距离
	double m_RotationAngle;				//选择Arc运动时 将旋转角度存入
	LPoint m_CircleRaidus;				//选择Arc运动时 将运动半径存入
	double m_CircleAngle;					//以窗口中心为原点，窗口中心与圆心的连线与x轴正方向夹角成CricleAngle度
	TrunStyle m_TrunStyle;
	LPoint m_CircleCenter;				//选择Arc运动时 将运动圆心存入
private:
	void UpdateWindowMove();          //更新窗口运动
	//窗口直线运动的更新代码
	void UdateWindowMoveLine();
	//窗口圆弧运动的更新代码
	void UpdateWindowMoveArc();

	void createWindow(HINSTANCE);
	void createRefreshTimer();			//窗口绘制、逻辑处理的定时器

	//将窗口移动到指定位置
	void MoveWindowPosition(const LPoint&);

	//计算A绕C旋转angle度后所在的位置，radius为-1时自动计算旋转半径
	LPoint CalculationArc(const LPoint& A, const LPoint& C, double angle, double circleAngle = -1, LPoint radius = -1, TrunStyle trunStyle = TrunStyle::Clockwise);

public:
	//构造析构函数
	GlassWindow();
	~GlassWindow() { }

	virtual void Draw();
	virtual void Update();

	void initDirect2D(HWND);			//初始化Direct2D
	void initPet();						//初始化Pet
	void initTray();					//初始化托盘
	void runrelay(int _CmdShow);
	void run(int _CmdShow);

#pragma region 窗口运动相关
	//窗口直线运动：将窗口在time毫秒内匀速移动到destination（绝对运动）
	void WindowMoveTo(const LPoint& destination, DWORD time);
	//窗口直线运动：将窗口在time毫秒内匀速移动destination（相对运动）
	void WindowMoveBy(const LPoint& destination, DWORD time);
	//窗口圆弧运动：将窗口在time毫秒内匀速绕半径Radius，圆心在以窗口中心为原点与圆心的连线和x轴正方向夹角成CricleAngle度，旋转Angle度数。旋转样式为style
	void WindowMoveArc(LPoint Radius, double angle, double circleAngle, DWORD time, TrunStyle style);
#pragma endregion

	//在窗口运动之前的一些初始化（分直线运动与圆弧运动）
	void BeforeTheMoveOfWindowInit(const LPoint& destination, DWORD time, WindowMoveStyle style);
	void BeforeTheMoveOfWindowInit(const LPoint& destination, const LPoint& center, const LPoint& radius, double Angle, double circleAngle, DWORD time);
	//在窗口运动之后的一些初始化
	void AfterTheMoveOfWindowInit();
	void Start();
	//停止窗口的移动（用于打断之类的操作）
	void StopMove();

	static GlassWindow* create(HINSTANCE _hInstance);
	static GlassWindow* getInstance() { return instance; }
	static void release(GlassWindow** _pGW);

	Cat* GetPet() { return m_Cat; }
	void SetExit(bool _b) { m_isExit = _b; }
	WindowEx* GetWndEx() { return m_Window; }
protected:
	//重写基类方法
	virtual bool init(HINSTANCE _hInstance);
	virtual void release();
public:
	//访问器
	//LPoint GetWindowPosition() { return m_WindowPosition; }
	LPoint GetWindowFirst() { return m_WindowFirst; }
	LPoint GetWindowDestination() { return m_WindowDestination; }
	LPoint GetWindowSize() { return m_WindowSize; }
	//获取窗口中心位置
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
std::wstring string2wstring(const std::string& str);	//string转wstring