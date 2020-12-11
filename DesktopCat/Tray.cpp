#include "GlassWindows.h"


void GlassWindow::TrayHandleMsg(LPARAM lParam)
{
	switch (lParam)
	{
		case WM_LBUTTONDOWN:
			OnTrayLButtonDown();		break;
		case WM_LBUTTONUP:
			OnTrayLButtonUp();			break;
		case WM_RBUTTONDOWN:
			OnTrayRButtonDown();		break;
		case WM_RBUTTONUP:
			OnTrayRButtonUp();			break;
	}
}
void GlassWindow::TrayHandleMenuMsg(WPARAM wParam)
{
	switch (wParam)
	{
		//坐下
		case Tray_Menu_SitDown_ID:	OnTrayMenuSitDown();	break;
		//退出
		case Tray_Menu_Exit_ID:		OnTrayMenuExit();		break;
		default:return;
	}
}

//托盘消息
void GlassWindow::OnTrayLButtonDown()
{
	OutputDebugString(L"左键按下了托盘图标\n");
	EndMenu();
}
void GlassWindow::OnTrayLButtonUp()
{
	OutputDebugString(L"左键弹起了托盘图标\n");
}
void GlassWindow::OnTrayRButtonDown()
{
	OutputDebugString(L"右键按下了托盘图标\n");
	EndMenu();//收起菜单
}
void GlassWindow::OnTrayRButtonUp()
{
	OutputDebugString(L"右键弹起了托盘图标\n");
	//创建菜单
	HMENU Menu = CreatePopupMenu();
	//为菜单添加选项
	WindowAddMenu(Menu, Tray_Menu_SitDown_ID, L"坐下");
	WindowAddMenu(Menu, Tray_Menu_Exit_ID, L"退出");

	POINT pt;
	GetCursorPos(&pt);
	TrackPopupMenu(Menu, TPM_RIGHTALIGN | TPM_BOTTOMALIGN, pt.x, pt.y, NULL, GlassWindow::getInstance()->GetWndEx()->GetWindowHWND(), NULL);
}

//托盘菜单事件
void GlassWindow::OnTrayMenuExit()
{
	Shell_NotifyIcon(NIM_DELETE, &GlassWindow::getInstance()->GetTray());	//删除托盘的图标
	GlassWindow::getInstance()->SetExit(true);
}
void GlassWindow::OnTrayMenuSitDown()
{
	OutputDebugString(L"坐下\n");
	GlassWindow::getInstance()->GetCat()->Sit();
}

DWORD WINAPI WindowLogicThread(void* param)
{
	static GlassWindow* GW = GlassWindow::instance;
	static LTimer* timer = LTimerManager::instance->createTimer(Window_Logic_Timer_ID);
	static UINT NowContinueTime = 0 , ContinueTime = Window_Logic_FPS;

	while (!GW->GetisExit())
	{
		LTimerManager::instance->Update();		//定时管理器的更新
		NowContinueTime += timer->GetInterval();
		if(NowContinueTime >= ContinueTime)
		{
			GW->Draw(nullptr);
			GW->Update();
			NowContinueTime = 0;
		}
		Sleep(1);
	}
	return 0;
}