#include "LFile.h"

using namespace std;
using namespace Lin;


std::ostream& Lin::operator<<(std::ostream& _s, const Lin::LDate& _date)
{
	return _s << "[" << _date.ToString() << "]";
}

string Lin::IntToString(const int _number)
{
	if (_number < 10)
		return std::to_string(0) += std::to_string(_number);
	else
		return std::to_string(_number);
}

inline LDateStyle Lin::GetDateStyle()	noexcept
{
	return LDate::g_style;
}

inline void Lin::SetDateStyle(LDateStyle _style)	noexcept
{
	LDate::g_style = _style;
}

void Lin::LWriteFile(Lin::LFile* _pFile, const string& _Info, ios_base::openmode _Mode)
{
	_pFile->Write(_Info, _Mode);
}

string Lin::LReadFileLine(Lin::LFile* _pFile, unsigned int _len, ios_base::openmode _Mode)
{
	LFile::BeginRead(_pFile, _Mode);
	string str = _pFile->Read(_len, _Mode);
	LFile::EndRead(_pFile);
	return str;
}

string Lin::LReadFileLinePlus(Lin::LFile* _pFile, unsigned int _len, ios_base::openmode _Mode)
{
	return _pFile->Read(_len, _Mode);
}

void Lin::LReadFileAll(Lin::LFile* _pFile, std::ios_base::openmode _Mode)
{
	_pFile->ReadAll(_Mode);
}

int Lin::GetVarNum(std::string& _str, char _Delim)
{
	int VarNum = 0, _strLenget = _str.length();
	for (int i = 0; i < _strLenget; ++i)
	{
		if (_str[i] == _Delim)
			++VarNum;
	}
	//返回VarNum-1的原因是因为: $2$2.1$3.1$	其中有4个$，但是数据只有3个
	return VarNum - 1;
}

std::string Lin::Truncated(std::string& _str, char _Delim, void** _param)
{
	std::string str(_str);
	std::string::iterator iter = str.begin();
	for (++iter; iter != str.end(); ++iter)
	{
		if ((*iter) == _Delim)
		{
			std::string str(str.begin() + 1, iter);
			Conversion(str, _Delim, _param);
			break;
		}
	}
	//返回截短之后的副本
	str.erase(str.begin(), iter);
	return str;
}	

void Lin::Conversion(std::string& _str, char _Delim, void** _param)
{
	switch (_Delim)
	{
		case '#': 			//string*
		{
			(*_param) = new std::string(_str);
		}
		break;
		case '$':			//float*
		{
			(*_param) = new float(atof(_str.c_str()));
		}
		break;
	}
}