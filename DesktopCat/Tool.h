#pragma once
#include "DesktopCat.h"


namespace Lin
{
	//将一个角度转换成小于360°的角
	double AngleConversion360(const double angle);

	/// <summary>
	/// 把字符串从mark标记符之后的数据删掉。
	///	如：ABCDE@FGH，标记@为mark则删除后变为ABCDE
	/// </summary>
	/// <param name="str">被截短的字符串</param>
	/// <param name="mark">标记字符</param>
	void Fracture(string& str, const char mark);


}