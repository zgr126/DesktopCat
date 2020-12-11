#include "Tool.h"
//�������ֵ�lib
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
	//���Ǹ��ݶ��������������ж��ٸ���Ч���ݣ�3�����Ŵ�����4����Ч���������Լ�����+1
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
	//��һ���ַ�һ����mark�����Դӵڶ����ַ���ʼ
	auto iter = find(str.begin() + 1, str.end(), mark);
	string First(str.begin() + 1, iter);
	str.erase(str.begin(), iter);
	return First;
}

string Lin::GetData(const string& str, const int count, const char mark)
{
	//���Ե�һ����ʶ��mark
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
	//��ȡ��������С��������ռ䣬��������С���ַ�����  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //����ַ�����β  
	//ɾ��������������ֵ  
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
	//˳�򲥷�����
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