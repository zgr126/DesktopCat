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
		//�˳�
		case Tray_Menu_Exit_ID:		OnTrayMenuExit();		break;
	}
}

//������Ϣ
void GlassWindow::OnTrayLButtonDown()
{
	OutputDebugString(L"�������������ͼ��\n");
	EndMenu();
}
void GlassWindow::OnTrayLButtonUp()
{
	OutputDebugString(L"�������������ͼ��\n");
}
void GlassWindow::OnTrayRButtonDown()
{
	OutputDebugString(L"�Ҽ�����������ͼ��\n");
	EndMenu();//����˵�
}
void GlassWindow::OnTrayRButtonUp()
{
	OutputDebugString(L"�Ҽ�����������ͼ��\n");
	//�����˵�
	HMENU Menu = CreatePopupMenu();
	//Ϊ�˵����ѡ��
	WindowAddMenu(Menu, Tray_Menu_Exit_ID, L"�˳�");
	POINT pt;
	GetCursorPos(&pt);
	TrackPopupMenu(Menu, TPM_LEFTBUTTON | TPM_HORNEGANIMATION, pt.x, pt.y, NULL, GlassWindow::getInstance()->GetWndEx()->GetWindowHWND(), NULL);
	DestroyMenu(Menu);		//������ͷ�Menu
}

//���̲˵��¼�
void GlassWindow::OnTrayMenuExit()
{
	Shell_NotifyIcon(NIM_DELETE, &GlassWindow::getInstance()->GetTray());	//ɾ�����̵�ͼ��
	GlassWindow::getInstance()->SetExit(true);
}