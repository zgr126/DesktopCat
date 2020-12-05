#pragma once
#include "DesktopCat.h"


namespace Lin
{
	//��һ���Ƕ�ת����С��360��Ľ�
	double AngleConversion360(const double angle);

	/// <summary>
	/// ���ַ�����mark��Ƿ�֮�������ɾ����
	///	�磺ABCDE@FGH�����@Ϊmark��ɾ�����ΪABCDE
	/// </summary>
	/// <param name="str">���ض̵��ַ���</param>
	/// <param name="mark">����ַ�</param>
	void Fracture(string& str, const char mark);


}