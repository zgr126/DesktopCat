#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <map>

#define _LIN	Lin::
#define LIN_DEBUG

//所有声明放在此处
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
		//粗略风格		如：2020-9.16
		Careless,
		//天时间内的风格	如：18:02:15
		OneDay,
		//时间标准风格	如：2020-9.16-18:02:15
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