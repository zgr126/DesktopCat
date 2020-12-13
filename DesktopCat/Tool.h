#pragma once
#include "DesktopCat.h"
#include <tuple>


#define Range(Min,Max)	(rand() % (Max - Min) + Min)

namespace Lin
{
	//��һ���Ƕ�ת����С��360��Ľ�
	double AngleConversion360(const double angle);
	
	//����2����ֵ
	double Minus(double left, double right);

	//����2�����
	double Division(double top, double bottom);

	//����str���м�����Ч���ݣ�(1,2,3,4)����4����Ч����
	int GetValueCount(const string& str);

	/// <summary>
	/// ���ַ�����mark��Ƿ�֮�������ɾ����
	///	�磺ABCDE@FGH�����@Ϊmark��ɾ�����ΪABCDE
	/// </summary>
	/// <param name="str">���ض̵��ַ���</param>
	/// <param name="mark">����ַ�</param>
	void FractureBack(string& str, const char mark);		//�Ӻ�ɾ��
	void FractureFront(string& str, const char mark);		//��ǰɾ��

	/// <summary>
	/// ��ȡstr�е�һ�����ݻ����ݰ����ض̵�һ�����ݣ����ݰ��Ǳ���Ƿ����ŵġ�
	///	�����Ƿ���#����һ�����ݰ�����#(1,2,3)#��#data#
	///	ԭstr��#(1,2,3)#data#���ض̺�str��#data#		���أ�(1,2,3)
	/// </summary>
	/// <param name="str">����</param>
	///	<param name="mark">��Ƿ�</param>
	/// <returns>����str��һ�����ݻ����ݰ�����</returns>
	string GetFirst(string& str, const char mark);

	/// <summary>
	/// ��ȡ�ڼ������ݻ����ݰ������ض�str
	/// </summary>
	/// <param name="str">����</param>
	/// <param name="count">��Ƿ�</param>
	/// <param name="mark">��count�����ݰ�����0��ʼ��</param>
	/// <returns>��count�����ݻ����ݰ�</returns>
	string GetData(const string& str, const int count, const char mark);

	/// <summary>
	/// ���Ե�str��һЩ�ַ�������str��ɾ������
	///	�÷���Neglect(str,mark_tuple(' ', ',');	//ɾ���ո�Ͷ���
	/// </summary>
	/// <typeparam name="...Args">���������mark</typeparam>
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
	/// �ļ��е�һ������
	/// </summary>
	/// <typeparam name="T">һ��Ϊint, uint, float, ufloat����������</typeparam>
	template<typename T>
	struct LFileData
	{
		T m_Val;
		LFileData(const T& val) :m_Val(val) { }
	};

	/// <summary>
	/// ��������ת��	����LFileData<T>��ʽ
	/// </summary>
	/// <typeparam name="T">int, uint, float, ufloat	�����ַ������Ͷ�֧��</typeparam>
	template<typename T>
	struct LDataConversion
	{
		static LFileData<T> Conversion(string& str)
		{
			//atof�������Զ����Կո�
			return LFileData<T>((T)atof(str.c_str()));
		}
	};
	/// <summary>
	/// ���ݰ���ת��	����vector<LFileData<T>>��ʽ
	/// </summary>
	/// <typeparam name="T">int, uint, float, ufloat...	�����ַ������Ͷ�֧��</typeparam>
	template<typename T>
	struct LDataPackConversion
	{
		static vector<LFileData<T>> Conversion(string& str)
		{
			//�Ⱥ��Ե�()��ո�
			Neglect(str, make_tuple(' ', '(', ')'));	//����������ɣ�1,2,3,0.1
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

	//stringתwstring
	std::wstring string2wstring(const std::string& str);

#pragma region vector��������ɾ��
	//���ӻ�ɾ��vectorԪ��	count>0��������Ԫ�أ�count<0ɾ������Ԫ��
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
	//����	������������һ��Ԫ��
	template<typename T>
	void PatchValue(vector<T>& vec, int count)
	{
		for (int i = 0; i < count; ++i)
		{
			vec.push_back(vec.back());
		}
	}
	//ɾ��
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

	//�������һ��vec�е�Ԫ�أ�ÿ��Ԫ�ض�����ͬ�Ļ��ᱻ����
	template<typename T>
	T GetRange(const vector<T>& vec)
	{
		int index = rand() % vec.size();
		return vec[index];
	}
	//�������һ��vec�е�Ԫ�أ�ÿ��Ԫ�ر����еĸ���Ϊ��Ӧ��probabilityԪ�ص�ֵ���ٷֱȣ���ע�⣺probabilityÿ��Ԫ����Ӻ��ֵ�������100
	template<typename T>
	T GetRange(const vector<T>& vec, const vector<UINT>& probability)
	{
		//��[1,100]�������һ������
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


	//������Ӳ˵�
	void WindowAddMenu(HMENU& menu, UINT_PTR ID, LPCWSTR str);
	//�������ͼƬ�˵�
	void WindowAddBitmapMenu(HMENU& menu, UINT_PTR ID, LPCWSTR str, HBITMAP& hbmp);

	//������������				�ļ���·��
	void PlayMusic(vector<wstring>& filePath);
	//���������߳�
	DWORD WINAPI PlayMusicThread(void* param);
}