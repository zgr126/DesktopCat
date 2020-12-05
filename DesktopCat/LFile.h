#pragma once
#include "Statement.h"
#include "NoMemberFunction.h"

struct _LIN LInt
{
	std::ostream& operator<<(std::ostream& _s)	const;
	virtual bool isValue(int _value) = 0;
	int m_val;
	virtual ~LInt();
};

#pragma region LYear,LMonth,LDay,LHour,LMinute,LSecond
struct _LIN LYear final: public _LIN LInt
{
	explicit LYear(int _year = 1970) noexcept;
	bool isValue(int);
	std::ostream& operator<<(std::ostream& _s)	const;
};
struct _LIN LMonth final : public _LIN LInt
{
	enum LMonthArea { Min = 1, Max = 12 };
	explicit LMonth(int _month = 1) noexcept;
	bool isValue(int _month);
	std::ostream& operator<<(std::ostream& _s)	const;
};
struct _LIN LDay final : public _LIN LInt
{
	enum LDayArea { Min = 1, Max = 31 };
	explicit LDay(int _day = 1) noexcept;
	bool isValue(int);
	std::ostream& operator<<(std::ostream& _s)	const;
};
struct _LIN LHour final: public _LIN LInt
{
	enum LHourArea { Min = 0, Max = 23 };
	explicit LHour(int _hour = 0) noexcept;
	bool isValue(int);
	std::ostream& operator<<(std::ostream& _s)	const;
};
struct _LIN LMinute final : public _LIN LInt
{
	enum LMinuteArea { Min = 0, Max = 59 };
	explicit LMinute(int _minute = 0) noexcept;
	bool isValue(int);
	std::ostream& operator<<(std::ostream& _s)	const;
};
struct _LIN LSecond final : public _LIN LInt
{
	enum LSecondArea { Min = 0, Max = 59 };
	explicit LSecond(int _second = 0) noexcept;
	bool isValue(int);
	std::ostream& operator<<(std::ostream& _s)	const;
};
#pragma endregion

#pragma region LDate
class _LIN LDate final
{
private:
	static _LIN LDateStyle g_style;

	//以下为测试用变量
#ifdef LIN_DEBUG
public:
	static unsigned int g_Ctor;		//调用构造函数次数
	static unsigned int g_MCtor;	//调用移动构造函数次数
	static unsigned int g_CCtor;	//调用拷贝构造函数次数
	static unsigned int g_Ass;		//调用赋值运算符次数
	static unsigned int g_MAss;		//调用移动赋值运算符次数
	static unsigned int g_DCtor;	//调用析构函数次数
#endif
public:
	_LIN LYear* m_pYear;
	_LIN LMonth* m_pMonth;
	_LIN LDay* m_pDay;
	_LIN LHour* m_pHour;
	_LIN LMinute* m_pMinute;
	_LIN LSecond* m_pSecond;
public:
	//构造函数
	LDate();
	//拷贝构造函数
	LDate(const LDate& _date);
	//移动构造函数
	LDate(LDate&& _date)	noexcept;
	~LDate();
	//重载=运算符
	LDate& operator=(const LDate& _rhs);
	LDate& operator=(const LDate&& _rhs) noexcept;
	//2个日期相减
	LDate operator-(const LDate& _rhs);
	LDate& operator-=(const LDate& _rhs);

	void Copy(const LDate& _date);
	const std::string ToString() const;
private:
	//禁止对象在堆生成，使用new创建对象会报错
	void* operator new(size_t _sz) { return ::operator new(_sz); }
	void operator delete(void* _ptr) { }
	//重载<<运算符，并禁止用户调用。用户只能通过给定的API使用
	LDate& operator<<(std::ostream& _s);
	//2个日期相加减没有意义，2个时间相减才有意义
	LDate operator+(const LDate& _rhs) { return *this; }
	LDate operator+=(const LDate& _rhs) { return *this; }

	void release();
	void move(LDate&& _rhs)	noexcept;

	const std::string ToCarelessString() const;
	const std::string ToOneDayString() const;
	const std::string ToNormString() const;
public:
	friend _LIN LDateStyle Lin::GetDateStyle()	noexcept;
	friend void Lin::SetDateStyle(LDateStyle _style)	noexcept;
};
#pragma endregion

#pragma region LFile
class _LIN LFile final :public _LIN LUnCopyable
{
private:
	//待开文件池（等待文件被打开）
	static std::map<std::string, _LIN LFile*> g_FilePool;
	std::string m_FileName;
	std::auto_ptr<std::ifstream> m_I;
	std::auto_ptr<std::ofstream> m_O;
	/// <summary>
	/// 记录当前读取到第几行
	/// </summary>
	static unsigned int g_len;
private:
	LFile(const std::string& _FileName);
	void* operator new(size_t _sz) { return ::operator new(_sz); }
	void Write(const std::string&, std::ios_base::openmode);
	/// <summary>
	/// 读取文件
	/// </summary>
	/// <param name="">读取的行数，0代表读取所有行</param>
	/// <param name="">文件读取的方式</param>
	/// <returns>返回最后一个读取到的字符串</returns>
	std::string Read(unsigned int _len, std::ios_base::openmode _Mode);
	void ReadAll(std::ios_base::openmode _Mode);
public:
	static void BeginRead(LFile*, std::ios_base::openmode = std::ios::in);
	static void BeginWrite(LFile*, std::ios_base::openmode = std::ios::out | std::ios::app);
	static void EndRead(LFile*);
	static void EndWrite(LFile*);
	//重载<<操作符	针对文件写入
	LFile* operator<<(const char*);
	LFile* operator<<(const int&);
	LFile* operator<<(const float&);
	LFile* operator<<(const double&);
	LFile* operator<<(const long&);
	LFile* operator<<(const std::string&);
	LFile* operator<<(const LDate&);
public:
	~LFile();
	//添加文件池（将文件添加进待开文件池）
	static LFile* AddToFilePool(const std::string& _FileName);
	//移出文件池（将文件移出待开文件池）
	static void MoveOutFilePool(const std::string& _FileName);
	friend void Lin::LWriteFile(Lin::LFile*, const std::string&, std::ios_base::openmode);
	friend std::string Lin::LReadFileLine(Lin::LFile*, unsigned int, std::ios_base::openmode);
	friend std::string Lin::LReadFileLinePlus(Lin::LFile*, unsigned int, std::ios_base::openmode);
	friend void Lin::LReadFileAll(Lin::LFile*, std::ios_base::openmode);
};
#pragma endregion