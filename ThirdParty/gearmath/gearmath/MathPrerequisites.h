#pragma once
#include <cmath>
// std::sin,std::cos都以弧度为单位
#define PI 3.1415926
// 角度转弧度
inline float radians(float degrees)
{
	return (PI / 180.0f) * degrees;
}

// 弧度转角度
inline float degrees(float radians)
{
	return (radians * 180.0) / PI;
}