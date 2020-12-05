//#include "NoMemberFunction.h"
#include "LFile.h"
#include <fstream>

using namespace std;
using namespace Lin;

#pragma region LInt
ostream& LInt::operator<<(ostream& _s)	const
{
	_s << IntToString(m_val);
	return _s;
}
LInt::~LInt() { }
#pragma endregion

#pragma region LYear
LYear::LYear(int _year)	noexcept
{
	m_val = _year;
}
bool LYear::isValue(int) { return true; }
ostream& LYear::operator<<(ostream& _s)	const
{
	return LInt::operator<<(_s);
}
#pragma endregion

#pragma region LMonth
LMonth::LMonth(int _month)noexcept
{
	try { isValue(_month); }
	catch (const wchar_t* _wch) { OutputDebugString(_wch); }
}
bool LMonth::isValue(int _month)
{
	if (Lin::isNumber(_month, static_cast<int>(LMonthArea::Min), static_cast<int>(LMonthArea::Max)))
	{
		m_val = _month;
		return true;
	}
	m_val = _month;
	throw L"Month Val Erro\n";
}
ostream& LMonth::operator<<(ostream& _s)	const
{ 
	return LInt::operator<<(_s);
}
#pragma endregion
#pragma region LDay
LDay::LDay(int _day)noexcept
{
	try { isValue(_day); }
	catch (const wchar_t* _wch) { OutputDebugString(_wch); }
}
bool LDay::isValue(int _day)
{
	if (Lin::isNumber(_day, static_cast<int>(LDayArea::Min), static_cast<int>(LDayArea::Max)))
	{
		m_val = _day;
		return true;
	}
	m_val = _day;
	throw L"Day Val Erro\n";
}
ostream& LDay::operator<<(ostream& _s)	const
{
	return LInt::operator<<(_s);
}
#pragma endregion
#pragma region LHour
LHour::LHour(int _hour)noexcept
{
	try { isValue(_hour); }
	catch (const wchar_t* _wch) { OutputDebugString(_wch); }
}
bool LHour::isValue(int _hour)
{
	if (Lin::isNumber(_hour, static_cast<int>(LHourArea::Min), static_cast<int>(LHourArea::Max)))
	{
		m_val = _hour;
		return true;
	}
	m_val = _hour;
	throw L"Hour Val Erro\n";
}
ostream& LHour::operator<<(ostream& _s)	const
{ 
	return LInt::operator<<(_s); 
}
#pragma endregion
#pragma region Minute
LMinute::LMinute(int _minute)noexcept
{
	try { isValue(_minute); }
	catch (const wchar_t* _wch) { OutputDebugString(_wch); }
}
bool LMinute::isValue(int _minute)
{
	if (Lin::isNumber(_minute, static_cast<int>(LMinuteArea::Min), static_cast<int>(LMinuteArea::Max)))
	{
		m_val = _minute;
		return true;
	}
	m_val = _minute;
	throw L"Minute Val Erro\n";
}
ostream& LMinute::operator<<(ostream& _s)	const
{
	return LInt::operator<<(_s);
}
#pragma endregion
#pragma region LSecond
LSecond::LSecond(int _second)noexcept
{
	try { isValue(_second); }
	catch (const wchar_t* _wch) { OutputDebugString(_wch); }
}
bool LSecond::isValue(int _second)
{
	if (Lin::isNumber(_second, static_cast<int>(LSecondArea::Min), static_cast<int>(LSecondArea::Max)))
	{
		m_val = _second;
		return true;
	}
	m_val = _second;
	throw L"Second Val Erro\n";
}
ostream& LSecond::operator<<(ostream& _s)	const
{
	return LInt::operator<<(_s);
}
#pragma endregion





//全局变量初始化
LDateStyle LDate::g_style = LDateStyle::Norm;

#pragma region LDate类实现

//LDate测试变量赋值
#ifdef LIN_DEBUG
unsigned int LDate::g_Ctor = 0;
unsigned int LDate::g_MCtor = 0;
unsigned int LDate::g_CCtor = 0;
unsigned int LDate::g_Ass = 0;
unsigned int LDate::g_MAss = 0;
unsigned int LDate::g_DCtor = 0;
#endif

LDate::LDate() : m_pYear(nullptr), m_pMonth(nullptr), m_pDay(nullptr), m_pHour(nullptr), m_pMinute(nullptr), m_pSecond(nullptr)
{
	SYSTEMTIME localSysTime;
	GetLocalTime(&localSysTime);
	m_pYear = new LYear(static_cast<int>(localSysTime.wYear)),
		m_pMonth = new LMonth(static_cast<int>(localSysTime.wMonth)),
		m_pDay = new LDay(static_cast<int>(localSysTime.wDay)),
		m_pHour = new LHour(static_cast<int>(localSysTime.wHour)),
		m_pMinute = new LMinute(static_cast<int>(localSysTime.wMinute)),
		m_pSecond = new LSecond(static_cast<int>(localSysTime.wSecond));
#ifdef LIN_DEBUG
	++g_Ctor;
#endif
}

LDate::LDate(const LDate& _date) : m_pYear(nullptr), m_pMonth(nullptr), m_pDay(nullptr), m_pHour(nullptr), m_pMinute(nullptr), m_pSecond(nullptr)
{
	//通过调用operator=来实现拷贝构造
	Copy(_date);
#ifdef LIN_DEBUG
	++g_CCtor;
#endif
}

LDate::LDate(LDate&& _date)	noexcept
{
	move(std::forward<LDate>(_date));
#ifdef LIN_DEBUG
	++g_MCtor;
#endif
}

LDate& LDate::operator=(const _LIN LDate& _rhs)
{
	Copy(_rhs);
#ifdef LIN_DEBUG
	++g_Ass;
#endif
	return *this;
}
LDate& LDate::operator=(const LDate&& _rhs)	noexcept
{
	try { release(); }
	catch (...) { OutputDebugString(L"LDate释放资源失败\n"); }
	move(std::forward<LDate>(const_cast<LDate&>(_rhs)));
#ifdef LIN_DEBUG
	++g_MAss;
#endif
	return *this;
}
LDate LDate::operator-(const LDate& _rhs)
{
	LDate lhs(*this);
	lhs -= _rhs;
	return lhs;
}
LDate& LDate::operator-=(const LDate& _rhs)
{
	(m_pSecond->m_val -= _rhs.m_pSecond->m_val) < 0 ? (m_pMinute->m_val -= 1, m_pSecond->m_val *= -1) : NULL;
	(m_pMinute->m_val -= _rhs.m_pMinute->m_val) < 0 ? (m_pHour->m_val -= 1, m_pMinute->m_val *= -1) : NULL;
	(m_pHour->m_val -= _rhs.m_pHour->m_val) < 0 ? (m_pDay->m_val -= 1, m_pHour->m_val *= -1) : NULL;
	(m_pDay->m_val -= _rhs.m_pDay->m_val) < 0 ? (m_pMonth->m_val -= 1, m_pDay->m_val *= -1) : NULL;
	(m_pMonth->m_val -= _rhs.m_pMonth->m_val) < 0 ? (m_pYear->m_val -= 1, m_pMonth->m_val *= -1) : NULL;
	(m_pYear->m_val -= _rhs.m_pYear->m_val);
	return *this;
}

LDate& LDate::operator<<(ostream& _s)
{
	//如：2020-9.16-18:02:15
	string str = ToString();
	_s << str;
	return *this;
}

void LDate::Copy(const LDate& _date)
{
	try
	{
		release();
		m_pYear = new LYear(_date.m_pYear->m_val);
		m_pMonth = new LMonth(_date.m_pMonth->m_val);
		m_pDay = new LDay(_date.m_pDay->m_val);
		m_pHour = new LHour(_date.m_pHour->m_val);
		m_pMinute = new LMinute(_date.m_pMinute->m_val);
		m_pSecond = new LSecond(_date.m_pSecond->m_val);
	}
	catch (bad_alloc&) { OutputDebugString(L"LDate内存分配失败\n"); }
	catch (...) { OutputDebugString(L"LDate释放资源失败\n"); }
}
const string LDate::ToString() const
{
	switch (g_style)
	{
		case LDateStyle::Careless:
			return ToCarelessString();
		case LDateStyle::OneDay:
			return ToOneDayString();
		case LDateStyle::Norm:
			return ToNormString();
	}
	return "Null";
}

void LDate::release()
{
	delete m_pYear;
	delete m_pMonth;
	delete m_pDay;
	delete m_pHour;
	delete m_pMinute;
	delete m_pSecond;
}
void LDate::move(LDate&& _rhs)	noexcept
{
	m_pYear = _rhs.m_pYear;
	m_pMonth = _rhs.m_pMonth;
	m_pDay = _rhs.m_pDay;
	m_pHour = _rhs.m_pHour;
	m_pMinute = _rhs.m_pMinute;
	m_pSecond = _rhs.m_pSecond;
	_rhs.m_pYear = nullptr;
	_rhs.m_pMonth = nullptr;
	_rhs.m_pDay = nullptr;
	_rhs.m_pHour = nullptr;
	_rhs.m_pMinute = nullptr;
	_rhs.m_pSecond = nullptr;
}
LDate::~LDate()
{
	try { release(); }
	catch (...)
	{
		OutputDebugString(L"LDate析构函数异常\n");
	}
#ifdef LIN_DEBUG
	++g_DCtor;
#endif
}

const string LDate::ToCarelessString() const
{
	//若指针为nullptr，就打印信息，使用nullptr数据时编译器会自动中断
	if (Lin::isDataNullptr<LYear,LMonth, LDay>(m_pYear, m_pMonth, m_pDay))
		OutputDebugString(L"ToCarelessString方法失败，LDate内数据是nullptr\n");
	string str = IntToString(m_pYear->m_val) + "-";
	str += IntToString(m_pMonth->m_val) + ".";
	str += IntToString(m_pDay->m_val);
	return str;
}
const string LDate::ToOneDayString() const
{
	if (Lin::isDataNullptr<LHour, LMinute, LSecond>(m_pHour, m_pMinute, m_pSecond))
		OutputDebugString(L"ToOneDayString方法失败，LDate内数据是nullptr\n");
	string str = IntToString(m_pHour->m_val) + ":";
	str += IntToString(m_pMinute->m_val) + ":";
	str += IntToString(m_pSecond->m_val);
	return str;
}
const string LDate::ToNormString() const
{
	if (Lin::isDataNullptr<LYear, LMonth, LDay, LHour, LMinute, LSecond>(m_pYear, m_pMonth, m_pDay, m_pHour, m_pMinute, m_pSecond))
		OutputDebugString(L"ToNormString方法失败，LDate内数据是nullptr\n");
	string str = IntToString(m_pYear->m_val) + "-";
	str += IntToString(m_pMonth->m_val) + ".";
	str += IntToString(m_pDay->m_val) + "-";
	str += IntToString(m_pHour->m_val) + ":";
	str += IntToString(m_pMinute->m_val) + ":";
	str += IntToString(m_pSecond->m_val);
	return str;
}
#pragma endregion

#pragma region LFile类实现
map<string, LFile*> LFile::g_FilePool;
unsigned int LFile::g_len = 1;

LFile::LFile(const string& _FileName) :m_FileName(_FileName) { }
LFile::~LFile() { }

void LFile::BeginRead(LFile* _pFile, ios_base::openmode _Mode)
{
	try
	{
		if (_pFile->m_I.get() == nullptr)
			_pFile->m_I = auto_ptr<ifstream>(new ifstream(_pFile->m_FileName, _Mode));
		if (!*_pFile->m_I.get())
			throw L"ErrorOpen!\n";
		LFile::g_len = 1;
	}
	catch (const wchar_t* _tch) { OutputDebugString(_tch); }
}
void LFile::BeginWrite(LFile* _pFile, ios_base::openmode _Mode)
{
	try
	{
		if (_pFile->m_O.get() == nullptr)
			_pFile->m_O = auto_ptr<ofstream>(new ofstream(_pFile->m_FileName, _Mode));
		if (!*_pFile->m_O.get())
			throw L"ErrorOpen!\n";
	}
	catch (const wchar_t* _tch) { OutputDebugString(_tch); }
}
void LFile::EndRead(LFile* _pFile) { _pFile->m_I->close(); _pFile->m_I.reset(); LFile::g_len = 1; }
void LFile::EndWrite(LFile* _pFile) { _pFile->m_O->close(); _pFile->m_O.reset(); }

void LFile::Write(const string& _Info, ios_base::openmode _Mode)
{
	LFile::BeginWrite(this, _Mode);
	string Info(_Info);
	if (Info.rfind('\n') == Info.npos)
		Info += "\n";
	m_O->write(Info.c_str(), Info.size());
	LFile::EndWrite(this);
}
string LFile::Read(unsigned int _len, ios_base::openmode _Mode)
{
	if (_len == 0)	ReadAll(_Mode);
	if (_len < g_len)
	{
		LFile::EndRead(this);
		LFile::BeginRead(this, _Mode);
	}
	char str[256];
	while (m_I->getline(str, 256, '\n'))
	{
		if (g_len == _len)
		{
			++g_len;
			break;
		}
		++g_len;
	}
	return str;
}

void LFile::ReadAll(std::ios_base::openmode _Mode)
{
	BeginRead(this, _Mode);
	char str[256];
	while (m_I->getline(str, 256, '\n'))
		cout << str << endl;
	EndRead(this);
}

#pragma region 重载<<操作符
LFile* LFile::operator<<(const char* _value)
{
	*m_O << _value;
	return this;
}
LFile* LFile::operator<<(const int& _value)
{
	*m_O << _value;
	return this;
}
LFile* LFile::operator<<(const float& _value)
{
	*m_O << _value;
	return this;
}
LFile* LFile::operator<<(const double& _value)
{
	*m_O << _value;
	return this;
}
LFile* LFile::operator<<(const long& _value)
{
	*m_O << _value;
	return this;
}
LFile* LFile::operator<<(const string& _value)
{
	return operator<<(_value.c_str());
}
LFile* LFile::operator<<(const LDate& _date)
{
	return operator<<(_date.ToString());
}
#pragma endregion

LFile* LFile::AddToFilePool(const string& _FileName)
{
	LFile* file = g_FilePool[_FileName.c_str()];
	if (file != nullptr)
		return file;
	try
	{
		file = new LFile(_FileName);
	}
	catch (bad_alloc&) { OutputDebugString(L"创建LFile对象失败"); return file = nullptr; }
	g_FilePool[_FileName] = file;
	return file;
}
void LFile::MoveOutFilePool(const string& _FileName)
{
	for (auto value : g_FilePool)
		if (value.first == _FileName)
		{
			if (value.second->m_I.get() || value.second->m_O.get())
				throw L"IO资源未释放\n";
			delete value.second;
			g_FilePool.erase(value.first);
			break;
		}
}
#pragma endregion