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


	//控制台输出
#pragma region Console
	//使LDate支持<<操作符
	std::ostream& operator<<(std::ostream&, const Lin::LDate&);
	template<int IDX, int MAX, typename... Args>
	struct LPrintConsole_Tuple;
	/// <summary>
	///	将任意类型、数量的变量输出到控制台
	/// </summary>
	/// <typeparam name="...Args">参数包</typeparam>
	/// <param name="_s">传入std::cout</param>
	/// <param name="_tuple">使用LPackage宏将参数打包成tuple传入</param>
	/// <returns>std::cout</returns>
	///	<example>	<!--
	///	注意：该类型变量必须支持<<操作符
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
	/// 当只需简单写入字符串且数据量少时，建议用LWriteFile。
	///	注意：此函数不能与模板函数同时使用
	/// </summary>
	/// <param name="">写入的数据</param>
	/// <param name="">打开方式，默认out | app</param>
	///	<example>
		///	LWriteFile(pFile, "test1");
		///	LWriteFile(pFile, "test2");
	///	</example>
	void LWriteFile(Lin::LFile* _pFile, const std::string& _Info, std::ios_base::openmode _Mode = std::ios::out | std::ios::app);

	//模板写入方法
	template<int IDX, int MAX, typename... Args>
	struct LPutFile_Tuple;
	/// <summary>
	/// 当需同一时间大量写入且不同类型，建议使用以下模板，针对大量写入的速度优化。
	///	注意：此模板不能与LWriteFile一起使用
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
	/// 文件读操作（读取指定行）不能与LReadFileLinePlus搭配使用，读取小数量数据时，建议使用此方法
	/// </summary>
	/// <param name="">文件指针	LFile*</param>
	/// <param name="">要读取的行数</param>
	/// <param name="">读取方式</param>
	/// <returns>成功返回指定行的内容，失败返回空字符串</returns>
	///	<example>
	///		string str1 = LReadFileLine(pFile, 4);	//读取第4行
	///		string str2 = LReadFileLine(pFile, 2);	//读取第2行
	///	</example>
	std::string LReadFileLine(Lin::LFile*, unsigned int = 1, std::ios_base::openmode = std::ios::in);
	/// <summary>
	/// 文件读操作Plus（读取指定行）不能与LReadFileLine搭配使用，当需大量读取文件内容时，建议使用此方法。此方法对大量读取文件内容有速度优化
	/// </summary>
	/// <param name="">文件指针	LFile*</param>
	/// <param name="">需要读取的行数</param>
	/// <param name="">读取方式</param>
	/// <returns>成功返回指定行的内容，失败返回空字符串</returns>
	///	<example>
	///		LFile::BeginRead(pFile, _Mode);
	///		string str1 = LReadFileLinePlus(pFile, 2);	//读取第2行
	///		string str2 = LReadFileLinePlus(pFile, 1);	//读取第1行
	///		LFile::EndRead(pFile);
	///	可以这样做，但不建议。建议读取行数按小到大传入。
	///	</example>
	std::string LReadFileLinePlus(Lin::LFile*, unsigned int = 1, std::ios_base::openmode = std::ios::in);
	/// <summary>
	/// 读取全部内容，并且在控制台上打印出来
	/// </summary>
	/// <param name="">文件指针</param>
	/// <param name="">选择读取方式</param>
	///	<example>
	///		LReadFileAll(pFile);
	///	</example>
	void LReadFileAll(Lin::LFile*, std::ios_base::openmode = std::ios::in);

	//从字符串中判断 有多少个同类型的数据
	int GetVarNum(std::string&, char _Delim);
	/*	使用方法:
		$表示float*数据、#表示string数据
		std::string _str("$2.5$1.56$0.32$");
		int VarNum = GetVarNum(_str, '$');	//判断_str中有几个float数据
		结果 VarNum = 3;
	*/

	//读取并截短数据
	std::string Truncated(std::string& _str1, char _Delim, void** _ch);
	//	_str1	需要操作的字符串		_Delim	标识符号如#,$以判断需要读取的类型
	//	_ch		指向截短的数据
	/*使用方法
		std::string _str1("$2.1$2.2$");
		char* ch = NULL;
		std::string _str2;
		std::string _str2(Truncated(_str1, '$', ch, _str2));
		结果:
		_str1:	$2.1$2.2$
		ch:		"2.1"
		_str2:	$2.2$
	*/

	//将数据转换成相应的string*、float数据
	void Conversion(std::string&, char, void**);
}