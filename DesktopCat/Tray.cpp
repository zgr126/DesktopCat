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
		//退出
		case Tray_Menu_Exit_ID:		OnTrayMenuExit();		break;
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
	WindowAddMenu(Menu, Tray_Menu_Exit_ID, L"退出");
	POINT pt;
	GetCursorPos(&pt);
	TrackPopupMenu(Menu, TPM_LEFTBUTTON | TPM_HORNEGANIMATION, pt.x, pt.y, NULL, GlassWindow::getInstance()->GetWndEx()->GetWindowHWND(), NULL);
	DestroyMenu(Menu);		//用完就释放Menu
}

//托盘菜单事件
void GlassWindow::OnTrayMenuExit()
{
	Shell_NotifyIcon(NIM_DELETE, &GlassWindow::getInstance()->GetTray());	//删除托盘的图标
	GlassWindow::getInstance()->SetExit(true);
}