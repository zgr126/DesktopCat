#pragma once
#include "DesktopCat.h"

class LLoadImage
{
public:
	//静态变量、方法
	static LLoadImage* instance;
	static LLoadImage* create();
	static LLoadImage* GetInstance();
	static void release();
public:
	//公开方法
	LLoadImage();

	HRESULT LoadintImage(LPCTSTR, ID2D1RenderTarget*, IWICImagingFactory*, ID2D1Bitmap**);
};