#pragma once

#include "resource.h"
#include <Windows.h>
#include <string>
#include <vector>
#include <assert.h>
#include <time.h>

//********D2D相关头文件及lib********
#include <wincodec.h>
#include <d2d1.h>
#include <d2d1_1.h>
#pragma comment(lib, "D2D1.lib")
#pragma comment(lib, "windowscodecs.lib")
//*********************************

//托盘头文件
#include <shellapi.h>

using namespace std;


#define Music_Whistle		"\\吹口哨.wav"
#define Music_CatReply		"\\猫叫声.wav"