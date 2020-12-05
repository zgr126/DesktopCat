// DesktopCat.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "DesktopCat.h"
#include "GlassWindows.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    GWindow* GW = GWindow::create(hInstance);
    GW->run(nCmdShow);
    GWindow::release(&GW);
    return 1;
}