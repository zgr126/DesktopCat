#pragma once
#include "DesktopCat.h"
#include <tuple>


#define Range(Min,Max)	(rand() % (Max - Min) + Min)

namespace Lin
{
	//将一个角度转换成小于360°的角
	double AngleConversion360(const double angle);
	
	//计算2数差值
	double Minus(double left, double right);

	//计算2数相除
	double Division(double top, double bottom);

	//计算str中有几个有效数据，(1,2,3,4)中有4个有效数据
	int GetValueCount(const string& str);

	/// <summary>
	/// 把字符串从mark标记符之后的数据删掉。
	///	如：ABCDE@FGH，标记@为mark则删除后变为ABCDE
	/// </summary>
	/// <param name="str">被截短的字符串</param>
	/// <param name="mark">标记字符</param>
	void FractureBack(string& str, const char mark);		//从后删除
	void FractureFront(string& str, const char mark);		//从前删除

	/// <summary>
	/// 获取str中第一个数据或数据包并截短第一个数据，数据包是被标记符夹着的。
	///	假设标记符是#。则一个数据包就是#(1,2,3)#或#data#
	///	原str：#(1,2,3)#data#，截短后str：#data#		返回：(1,2,3)
	/// </summary>
	/// <param name="str">数据</param>
	///	<param name="mark">标记符</param>
	/// <returns>返回str第一个数据或数据包内容</returns>
	string GetFirst(string& str, const char mark);

	/// <summary>
	/// 截取第几个数据或数据包。不截短str
	/// </summary>
	/// <param name="str">数据</param>
	/// <param name="count">标记符</param>
	/// <param name="mark">第count个数据包。从0开始计</param>
	/// <returns>第count个数据或数据包</returns>
	string GetData(const string& str, const int count, const char mark);

	/// <summary>
	/// 忽略掉str的一些字符（将从str中删除掉）
	///	用法：Neglect(str,mark_tuple(' ', ',');	//删除空格和逗号
	/// </summary>
	/// <typeparam name="...Args">任意个数的mark</typeparam>
	/// <param name="str"></param>
	/// <param name="tuple"></param>
	template<typename... Args>
	void Neglect(string& str, const std::tuple<Args...>& tuple)
	{
		Neglect_Struct<0, sizeof...(Args), Args...>::NeglectMark(str, tuple);
	}
	template<int IDX, int MAX, typename... Args>
	struct Neglect_Struct
	{
		static void NeglectMark(string& str, const std::tuple<Args...>& tuple)
		{
			auto iter = str.begin();
			do
			{
				iter = find(iter, str.end(), std::get<IDX>(tuple));
				if (iter == str.end())	break;
				str.erase(iter, iter + 1);
			} while (iter < str.end());
			Neglect_Struct<IDX + 1, MAX, Args...>::NeglectMark(str, tuple);
		}
	};
	template<int MAX, typename... Args>
	struct Neglect_Struct<MAX, MAX, Args...>
	{
		static void NeglectMark(string& str, const std::tuple<Args...>& tuple)
		{

		}
	};



	/// <summary>
	/// 文件中的一个数据
	/// </summary>
	/// <typeparam name="T">一般为int, uint, float, ufloat等数字类型</typeparam>
	template<typename T>
	struct LFileData
	{
		T m_Val;
		LFileData(const T& val) :m_Val(val) { }
	};

	/// <summary>
	/// 单个数据转换	返回LFileData<T>形式
	/// </summary>
	/// <typeparam name="T">int, uint, float, ufloat	除了字符串类型都支持</typeparam>
	template<typename T>
	struct LDataConversion
	{
		static LFileData<T> Conversion(string& str)
		{
			//atof函数会自动忽略空格。
			return LFileData<T>((T)atof(str.c_str()));
		}
	};
	/// <summary>
	/// 数据包的转换	返回vector<LFileData<T>>形式
	/// </summary>
	/// <typeparam name="T">int, uint, float, ufloat...	除了字符串类型都支持</typeparam>
	template<typename T>
	struct LDataPackConversion
	{
		static vector<LFileData<T>> Conversion(string& str)
		{
			//先忽略掉()与空格
			Neglect(str, make_tuple(' ', '(', ')'));	//经过处理后变成：1,2,3,0.1
			vector<LFileData<T>> Data;
			int ValueCount = GetValueCount(str);
			for (int i = 0; i < ValueCount; ++i)
			{
				Data.push_back(LDataConversion<T>::Conversion(str));
				FractureFront(str, ',');
			}
			return Data;
		}
	};

	//string转wstring
	std::wstring string2wstring(const std::string& str);

#pragma region vector的增参与删参
	//增加或删除vector元素	count>0增加若干元素，count<0删除若干元素
	template<typename T>
	void AdditionOrDeleteValue(vector<T>& vec, int count)
	{
		if (count > 0)
		{
			PatchValue(vec, count);
		}
		else if(count < 0)
		{
			LeaveValue(vec, count);
		}
	}
	//补参	拷贝并添加最后一个元素
	template<typename T>
	void PatchValue(vector<T>& vec, int count)
	{
		for (int i = 0; i < count; ++i)
		{
			vec.push_back(vec.back());
		}
	}
	//删参
	template<typename T>
	void LeaveValue(vector<T>& vec, int count)
	{
		if (count > 0)	count *= -1;
		for (int i = 0; i > count; --i)
		{
			vec.pop_back();
		}
	}
#pragma endregion

	//随机返回一个vec中的元素，每个元素都有相同的机会被抽中
	template<typename T>
	T GetRange(const vector<T>& vec)
	{
		int index = rand() % vec.size();
		return vec[index];
	}
	//随机返回一个vec中的元素，每个元素被抽中的概率为对应的probability元素的值（百分比）。注意：probability每个元素相加后的值必须等于100
	template<typename T>
	T GetRange(const vector<T>& vec, const vector<UINT>& probability)
	{
		//从[1,100]中随机抽一个整数
		int Number = rand() % 100 + 1;
		UINT Area = 0, index = 0;
		for (auto item : probability)
		{
			Area += item;
			if (Number <= Area)	break;
			++index;
		}
		return vec[index];
	}


	//托盘添加菜单
	void WindowAddMenu(HMENU& menu, UINT_PTR ID, LPCWSTR str);
	//托盘添加图片菜单
	void WindowAddBitmapMenu(HMENU& menu, UINT_PTR ID, LPCWSTR str, HBITMAP& hbmp);

	//连续播放声音				文件名路径
	void PlayMusic(vector<wstring>& filePath);
	//播放音乐线程
	DWORD WINAPI PlayMusicThread(void* param);
}