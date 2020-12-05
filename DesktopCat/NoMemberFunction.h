#pragma once
#include "Statement.h"
#include "LFile.h"

#define LPackage	make_tuple

namespace Lin
{
	std::string IntToString(const int _number);
	inline LDateStyle GetDateStyle()noexcept;
	inline void SetDateStyle(LDateStyle _style)noexcept;


	template<typename T>
	inline bool isNumber(T _number, T _min, T _max)noexcept
	{
		return (_number >= _min && _number <= _max);
	}

	inline bool isDataNullptr() { return false; }
	template<typename pData, typename... pDatas>
	inline bool isDataNullptr(pData* _pdata, pDatas*... _pdatas)
	{
		if (_pdata == nullptr)
			return true;
		Lin::isDataNullptr(_pdatas...);
	}


	//����̨���
#pragma region Console
	//ʹLDate֧��<<������
	std::ostream& operator<<(std::ostream&, const Lin::LDate&);
	template<int IDX, int MAX, typename... Args>
	struct LPrintConsole_Tuple;
	/// <summary>
	///	���������͡������ı������������̨
	/// </summary>
	/// <typeparam name="...Args">������</typeparam>
	/// <param name="_s">����std::cout</param>
	/// <param name="_tuple">ʹ��LPackage�꽫���������tuple����</param>
	/// <returns>std::cout</returns>
	///	<example>	<!--
	///	ע�⣺�����ͱ�������֧��<<������
	///	std::cout << LPackage(ldate, "Char", 5.5);-->
	///	</example>
	template<typename... Args>
	std::ostream& operator<<(std::ostream& _s, const std::tuple<Args...>& _tuple)
	{
		LPrintConsole_Tuple<0, sizeof...(Args), Args...>::LPut(_s, _tuple);
		return _s;
	}
	template<int IDX, int MAX, typename... Args>
	struct LPrintConsole_Tuple
	{
		static void LPut(std::ostream& _s, const std::tuple<Args...>& _tuple)
		{
			_s << std::get<IDX>(_tuple) << (IDX + 1 == MAX ? "\n" : " ");
			LPrintConsole_Tuple<IDX + 1, MAX, Args...>::LPut(_s, _tuple);
		}
	};
	template<int MAX, typename... Args>
	struct LPrintConsole_Tuple<MAX, MAX, Args...>
	{
		static void LPut(std::ostream&, const std::tuple<Args...>&)
		{
		}
	};
#pragma endregion


#pragma region LFile
	/// <summary>
	/// ��ֻ���д���ַ�������������ʱ��������LWriteFile��
	///	ע�⣺�˺���������ģ�庯��ͬʱʹ��
	/// </summary>
	/// <param name="">д�������</param>
	/// <param name="">�򿪷�ʽ��Ĭ��out | app</param>
	///	<example>
		///	LWriteFile(pFile, "test1");
		///	LWriteFile(pFile, "test2");
	///	</example>
	void LWriteFile(Lin::LFile* _pFile, const std::string& _Info, std::ios_base::openmode _Mode = std::ios::out | std::ios::app);

	//ģ��д�뷽��
	template<int IDX, int MAX, typename... Args>
	struct LPutFile_Tuple;
	/// <summary>
	/// ����ͬһʱ�����д���Ҳ�ͬ���ͣ�����ʹ������ģ�壬��Դ���д����ٶ��Ż���
	///	ע�⣺��ģ�岻����LWriteFileһ��ʹ��
	/// </summary>
	///	<example>
	///		LFile::BeginWrite(pFile);<!--
	///		pFile << LPackage(type1,type2,type3);
	///		pFile << LPackage(data1, data2);	-->
	///		LFile::BeginWrite(pFile);
	///	</example>
	template<typename... Args>
	Lin::LFile* operator<<(Lin::LFile* _pFile, const std::tuple<Args...>& _tuple)
	{
		LPutFile_Tuple<0, sizeof...(Args), Args...>::LPut(_pFile, _tuple);
		return _pFile;
	}
	template<int IDX, int MAX, typename... Args>
	struct LPutFile_Tuple
	{
		static void LPut(Lin::LFile* _pFile, const std::tuple<Args...>& _tuple)
		{
			_pFile->operator<<(std::get<IDX>(_tuple));
			_pFile->operator<<((IDX + 1 == MAX ? "\n" : " "));
			LPutFile_Tuple<IDX + 1, MAX, Args...>::LPut(_pFile, _tuple);
		}
	};
	template<int MAX, typename... Args>
	struct LPutFile_Tuple<MAX, MAX, Args...>
	{
		static void LPut(Lin::LFile* _pFile, const std::tuple<Args...>&)
		{
		}
	};


	/// <summary>
	/// �ļ�����������ȡָ���У�������LReadFileLinePlus����ʹ�ã���ȡС��������ʱ������ʹ�ô˷���
	/// </summary>
	/// <param name="">�ļ�ָ��	LFile*</param>
	/// <param name="">Ҫ��ȡ������</param>
	/// <param name="">��ȡ��ʽ</param>
	/// <returns>�ɹ�����ָ���е����ݣ�ʧ�ܷ��ؿ��ַ���</returns>
	///	<example>
	///		string str1 = LReadFileLine(pFile, 4);	//��ȡ��4��
	///		string str2 = LReadFileLine(pFile, 2);	//��ȡ��2��
	///	</example>
	std::string LReadFileLine(Lin::LFile*, unsigned int = 1, std::ios_base::openmode = std::ios::in);
	/// <summary>
	/// �ļ�������Plus����ȡָ���У�������LReadFileLine����ʹ�ã����������ȡ�ļ�����ʱ������ʹ�ô˷������˷����Դ�����ȡ�ļ��������ٶ��Ż�
	/// </summary>
	/// <param name="">�ļ�ָ��	LFile*</param>
	/// <param name="">��Ҫ��ȡ������</param>
	/// <param name="">��ȡ��ʽ</param>
	/// <returns>�ɹ�����ָ���е����ݣ�ʧ�ܷ��ؿ��ַ���</returns>
	///	<example>
	///		LFile::BeginRead(pFile, _Mode);
	///		string str1 = LReadFileLinePlus(pFile, 2);	//��ȡ��2��
	///		string str2 = LReadFileLinePlus(pFile, 1);	//��ȡ��1��
	///		LFile::EndRead(pFile);
	///	�������������������顣�����ȡ������С�����롣
	///	</example>
	std::string LReadFileLinePlus(Lin::LFile*, unsigned int = 1, std::ios_base::openmode = std::ios::in);
	/// <summary>
	/// ��ȡȫ�����ݣ������ڿ���̨�ϴ�ӡ����
	/// </summary>
	/// <param name="">�ļ�ָ��</param>
	/// <param name="">ѡ���ȡ��ʽ</param>
	///	<example>
	///		LReadFileAll(pFile);
	///	</example>
	void LReadFileAll(Lin::LFile*, std::ios_base::openmode = std::ios::in);

	//���ַ������ж� �ж��ٸ�ͬ���͵�����
	int GetVarNum(std::string&, char _Delim);
	/*	ʹ�÷���:
		$��ʾfloat*���ݡ�#��ʾstring����
		std::string _str("$2.5$1.56$0.32$");
		int VarNum = GetVarNum(_str, '$');	//�ж�_str���м���float����
		��� VarNum = 3;
	*/

	//��ȡ���ض�����
	std::string Truncated(std::string& _str1, char _Delim, void** _ch);
	//	_str1	��Ҫ�������ַ���		_Delim	��ʶ������#,$���ж���Ҫ��ȡ������
	//	_ch		ָ��ض̵�����
	/*ʹ�÷���
		std::string _str1("$2.1$2.2$");
		char* ch = NULL;
		std::string _str2;
		std::string _str2(Truncated(_str1, '$', ch, _str2));
		���:
		_str1:	$2.1$2.2$
		ch:		"2.1"
		_str2:	$2.2$
	*/

	//������ת������Ӧ��string*��float����
	void Conversion(std::string&, char, void**);
}