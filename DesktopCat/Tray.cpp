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
		//����
		case Tray_Menu_SitDown_ID:	OnTrayMenuSitDown();	break;
		//�˳�
		case Tray_Menu_Exit_ID:		OnTrayMenuExit();		break;
		default:return;
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
	WindowAddMenu(Menu, Tray_Menu_SitDown_ID, L"����");
	WindowAddMenu(Menu, Tray_Menu_Exit_ID, L"�˳�");

	POINT pt;
	GetCursorPos(&pt);
	TrackPopupMenu(Menu, TPM_RIGHTALIGN | TPM_BOTTOMALIGN, pt.x, pt.y, NULL, GlassWindow::getInstance()->GetWndEx()->GetWindowHWND(), NULL);
}

//���̲˵��¼�
void GlassWindow::OnTrayMenuExit()
{
	Shell_NotifyIcon(NIM_DELETE, &GlassWindow::getInstance()->GetTray());	//ɾ�����̵�ͼ��
	GlassWindow::getInstance()->SetExit(true);
}
void GlassWindow::OnTrayMenuSitDown()
{
	OutputDebugString(L"����\n");
	GlassWindow::getInstance()->GetCat()->Sit();
}

DWORD WINAPI WindowLogicThread(void* param)
{
	static GlassWindow* GW = GlassWindow::instance;
	static LTimer* timer = LTimerManager::instance->createTimer(Window_Logic_Timer_ID);
	static UINT NowContinueTime = 0 , ContinueTime = Window_Logic_FPS;

	while (!GW->GetisExit())
	{
		LTimerManager::instance->Update();		//��ʱ�������ĸ���
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