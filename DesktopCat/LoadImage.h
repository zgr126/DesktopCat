#pragma once
#include "DesktopCat.h"

class LLoadImage
{
public:
	//��̬����������
	static LLoadImage* instance;
	static LLoadImage* create();
	static LLoadImage* GetInstance();
	static void release();
public:
	//��������
	LLoadImage();

	HRESULT LoadintImage(LPCTSTR, ID2D1RenderTarget*, IWICImagingFactory*, ID2D1Bitmap**);
};