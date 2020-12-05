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

	//����Ϊ�����ñ���
#ifdef LIN_DEBUG
public:
	static unsigned int g_Ctor;		//���ù��캯������
	static unsigned int g_MCtor;	//�����ƶ����캯������
	static unsigned int g_CCtor;	//���ÿ������캯������
	static unsigned int g_Ass;		//���ø�ֵ���������
	static unsigned int g_MAss;		//�����ƶ���ֵ���������
	static unsigned int g_DCtor;	//����������������
#endif
public:
	_LIN LYear* m_pYear;
	_LIN LMonth* m_pMonth;
	_LIN LDay* m_pDay;
	_LIN LHour* m_pHour;
	_LIN LMinute* m_pMinute;
	_LIN LSecond* m_pSecond;
public:
	//���캯��
	LDate();
	//�������캯��
	LDate(const LDate& _date);
	//�ƶ����캯��
	LDate(LDate&& _date)	noexcept;
	~LDate();
	//����=�����
	LDate& operator=(const LDate& _rhs);
	LDate& operator=(const LDate&& _rhs) noexcept;
	//2���������
	LDate operator-(const LDate& _rhs);
	LDate& operator-=(const LDate& _rhs);

	void Copy(const LDate& _date);
	const std::string ToString() const;
private:
	//��ֹ�����ڶ����ɣ�ʹ��new��������ᱨ��
	void* operator new(size_t _sz) { return ::operator new(_sz); }
	void operator delete(void* _ptr) { }
	//����<<�����������ֹ�û����á��û�ֻ��ͨ��������APIʹ��
	LDate& operator<<(std::ostream& _s);
	//2��������Ӽ�û�����壬2��ʱ�������������
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
	//�����ļ��أ��ȴ��ļ����򿪣�
	static std::map<std::string, _LIN LFile*> g_FilePool;
	std::string m_FileName;
	std::auto_ptr<std::ifstream> m_I;
	std::auto_ptr<std::ofstream> m_O;
	/// <summary>
	/// ��¼��ǰ��ȡ���ڼ���
	/// </summary>
	static unsigned int g_len;
private:
	LFile(const std::string& _FileName);
	void* operator new(size_t _sz) { return ::operator new(_sz); }
	void Write(const std::string&, std::ios_base::openmode);
	/// <summary>
	/// ��ȡ�ļ�
	/// </summary>
	/// <param name="">��ȡ��������0�����ȡ������</param>
	/// <param name="">�ļ���ȡ�ķ�ʽ</param>
	/// <returns>�������һ����ȡ�����ַ���</returns>
	std::string Read(unsigned int _len, std::ios_base::openmode _Mode);
	void ReadAll(std::ios_base::openmode _Mode);
public:
	static void BeginRead(LFile*, std::ios_base::openmode = std::ios::in);
	static void BeginWrite(LFile*, std::ios_base::openmode = std::ios::out | std::ios::app);
	static void EndRead(LFile*);
	static void EndWrite(LFile*);
	//����<<������	����ļ�д��
	LFile* operator<<(const char*);
	LFile* operator<<(const int&);
	LFile* operator<<(const float&);
	LFile* operator<<(const double&);
	LFile* operator<<(const long&);
	LFile* operator<<(const std::string&);
	LFile* operator<<(const LDate&);
public:
	~LFile();
	//����ļ��أ����ļ���ӽ������ļ��أ�
	static LFile* AddToFilePool(const std::string& _FileName);
	//�Ƴ��ļ��أ����ļ��Ƴ������ļ��أ�
	static void MoveOutFilePool(const std::string& _FileName);
	friend void Lin::LWriteFile(Lin::LFile*, const std::string&, std::ios_base::openmode);
	friend std::string Lin::LReadFileLine(Lin::LFile*, unsigned int, std::ios_base::openmode);
	friend std::string Lin::LReadFileLinePlus(Lin::LFile*, unsigned int, std::ios_base::openmode);
	friend void Lin::LReadFileAll(Lin::LFile*, std::ios_base::openmode);
};
#pragma endregion