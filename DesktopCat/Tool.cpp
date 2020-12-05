#include "Tool.h"


double Lin::AngleConversion360(double angle)
{
	while (angle >= 360)
		angle -= 360;
	return angle;
}

void Lin::Fracture(string& str, const char mark)
{
	auto iter = find(str.begin(), str.end(), mark);
	str.erase(iter, str.end());
}