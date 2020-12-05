#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <map>

#define _LIN	Lin::
#define LIN_DEBUG

//�����������ڴ˴�
namespace Lin
{
	class LUnCopyable;
	class LLock;
	struct LInt;
	struct LYear;
	struct LMonth;
	struct LDay;
	struct LHour;
	struct LMinute;
	struct LSecond;
	class LDate;
	enum class LDateStyle
	{
		//���Է��		�磺2020-9.16
		Careless,
		//��ʱ���ڵķ��	�磺18:02:15
		OneDay,
		//ʱ���׼���	�磺2020-9.16-18:02:15
		Norm,
	};

	class LFile;
}

class _LIN LUnCopyable
{
protected:
	LUnCopyable()noexcept { }
	virtual ~LUnCopyable()noexcept { }
private:
	LUnCopyable(const LUnCopyable& _obj);
	LUnCopyable& operator=(const LUnCopyable& _obj);
};