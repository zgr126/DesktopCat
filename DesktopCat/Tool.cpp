#include "Tool.h"
//播放音乐的lib
#pragma comment(lib,"winmm.lib")

double Lin::AngleConversion360(double angle)
{
	while (angle >= 360)
		angle -= 360;
	return angle;
}
double Lin::Minus(double left, double right)
{
	return left - right;
}

double Lin::Division(double top, double bottom)
{
	return top / bottom;
}

int Lin::GetValueCount(const string& str)
{
	if (str == "")	return 0;
	//这是根据逗号数量来计算有多少个有效数据，3个逗号代表有4个有效参数。所以计数先+1
	int Count = 1;
	auto iter = str.begin();
	do
	{
		iter = find(iter, str.end(), ',');
		if (iter == str.end())	break;
		++Count;
		++iter;
	} while (iter < str.end());
	return Count;
}

void Lin::FractureBack(string& str, const char mark)
{
	auto iter = find(str.begin(), str.end(), mark);
	if (iter < str.end())
		str.erase(iter, str.end());
}

void Lin::FractureFront(string& str, const char mark)
{
	auto iter = find(str.begin(), str.end(), mark);
	if (iter < str.end())
		str.erase(str.begin(), iter + 1);
}

string Lin::GetFirst(string& str, const char mark)
{
	//第一个字符一定是mark，所以从第二个字符开始
	auto iter = find(str.begin() + 1, str.end(), mark);
	string First(str.begin() + 1, iter);
	str.erase(str.begin(), iter);
	return First;
}

string Lin::GetData(const string& str, const int count, const char mark)
{
	//忽略第一个标识符mark
	string::const_iterator iterbegin = str.begin() + 1;
	string::const_iterator iterend = find(iterbegin, str.end(), mark);
	for (auto i = 0; i < count; ++i)
	{
		iterbegin = iterend + 1;
		iterend = find(iterend + 1, str.end(), mark);
	}
	return string(iterbegin, iterend);
}

std::wstring Lin::string2wstring(const std::string& str)
{
	std::wstring result;
	//获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//多字节编码转换成宽字节编码  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //添加字符串结尾  
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}

void Lin::WindowAddMenu(HMENU& menu, UINT_PTR ID, LPCWSTR str)
{
	AppendMenu(menu, MF_STRING, ID, str);
	AppendMenu(menu, MF_SEPARATOR, 0, NULL);

}

void Lin::WindowAddBitmapMenu(HMENU& menu, UINT_PTR ID, LPCWSTR str, HBITMAP& hbmp)
{
	hbmp = (HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	AppendMenu(menu, MF_BITMAP, ID, (PTSTR)(LONG)hbmp);
	AppendMenu(menu, MF_SEPARATOR, 0, NULL);
}

void Lin::PlayMusic(vector<wstring>& filePath)
{
	//顺序播放音乐
	for (auto Name : filePath)
	{
		HANDLE handle = CreateThread(NULL, NULL, PlayMusicThread, (void*)Name.c_str(), NULL, NULL);
		CloseHandle(handle);
	}
}

DWORD WINAPI Lin::PlayMusicThread(void* param)
{
	PlaySound(LPCWSTR(param), NULL, SND_ASYNC);
	return 0;
}