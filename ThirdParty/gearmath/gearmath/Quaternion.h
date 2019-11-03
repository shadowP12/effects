#pragma once
#include "MathPrerequisites.h"
#include "Vector3.h"
#include "Matrix4.h"

class Quaternion
{
public:
	Quaternion()
		:x(0.0),y(0.0),z(0.0),w(1.0)
	{
	}
	Quaternion(float x, float y, float z, float w)
		:x(x), y(y), z(z), w(w)
	{
	}

	Quaternion operator+ (const Quaternion& rhs) const
	{
		return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	Quaternion operator- (const Quaternion& rhs) const
	{
		return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	Quaternion operator* (const Quaternion& rhs) const
	{
		return Quaternion
		(
			w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
			w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
			w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
			w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z
		);
	}

	void fromAxisAngle(const Vector3& axis, float angle)
	{
		float half_sin = std::sin(0.5f * angle);
		float half_cos = std::cos(0.5f * angle);
		
		w = half_cos;
		x = half_sin * axis.x;
		y = half_sin * axis.y;
		z = half_sin * axis.z;
	}

	void fromEulerAngles(float yaw, float pitch, float roll)
	{
		pitch *= 0.5f;
		yaw *= 0.5f;
		roll *= 0.5f;

		float sinp = std::sin(pitch);
		float siny = std::sin(yaw);
		float sinr = std::sin(roll);
		float cosp = std::cos(pitch);
		float cosy = std::cos(yaw);
		float cosr = std::cos(roll);

		w = cosp * cosy * cosr + sinp * siny * sinr;
		x = sinp * cosy * cosr - cosp * siny * sinr;
		y = cosp * siny * cosr + sinp * cosy * sinr;
		z = cosp * cosy * sinr - sinp * siny * cosr;
	}

	Matrix4 toMatrix()
	{
		return Matrix4(
			1.0f - 2.0f * y * y - 2.0f * z * z, 2.0f * x * y - 2.0f * z * w, 2.0f * x * z + 2.0f * y * w, 0.0f,
			2.0f * x * y + 2.0f * z * w, 1.0f - 2.0f * x * x - 2.0f * z * z, 2.0f * y * z - 2.0f * x * w, 0.0f,
			2.0f * x * z - 2.0f * y * w, 2.0f * y * z + 2.0f * x * w, 1.0f - 2.0f * x * x - 2.0f * y * y, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

public:
	float x, y, z, w;
};