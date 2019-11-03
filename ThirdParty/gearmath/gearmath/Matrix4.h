#pragma once
/*
 * 1  0  0  x
 * 0  1  0  y
 * 0  0  1  z
 * 0  0  0  1
*/
class Matrix4
{
public:
	Matrix4()
	{
		m[0][0] = 0.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
		m[1][0] = 0.0; m[1][1] = 0.0; m[1][2] = 0.0; m[1][3] = 0.0;
		m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 0.0; m[2][3] = 0.0;
		m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 0.0;
	}
	Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}

	Matrix4 operator* (const Matrix4& rhs) const
	{
		Matrix4 r;

		r.m[0][0] = m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0] + m[0][2] * rhs.m[2][0] + m[0][3] * rhs.m[3][0];
		r.m[0][1] = m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1] + m[0][2] * rhs.m[2][1] + m[0][3] * rhs.m[3][1];
		r.m[0][2] = m[0][0] * rhs.m[0][2] + m[0][1] * rhs.m[1][2] + m[0][2] * rhs.m[2][2] + m[0][3] * rhs.m[3][2];
		r.m[0][3] = m[0][0] * rhs.m[0][3] + m[0][1] * rhs.m[1][3] + m[0][2] * rhs.m[2][3] + m[0][3] * rhs.m[3][3];

		r.m[1][0] = m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0] + m[1][2] * rhs.m[2][0] + m[1][3] * rhs.m[3][0];
		r.m[1][1] = m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1] + m[1][2] * rhs.m[2][1] + m[1][3] * rhs.m[3][1];
		r.m[1][2] = m[1][0] * rhs.m[0][2] + m[1][1] * rhs.m[1][2] + m[1][2] * rhs.m[2][2] + m[1][3] * rhs.m[3][2];
		r.m[1][3] = m[1][0] * rhs.m[0][3] + m[1][1] * rhs.m[1][3] + m[1][2] * rhs.m[2][3] + m[1][3] * rhs.m[3][3];

		r.m[2][0] = m[2][0] * rhs.m[0][0] + m[2][1] * rhs.m[1][0] + m[2][2] * rhs.m[2][0] + m[2][3] * rhs.m[3][0];
		r.m[2][1] = m[2][0] * rhs.m[0][1] + m[2][1] * rhs.m[1][1] + m[2][2] * rhs.m[2][1] + m[2][3] * rhs.m[3][1];
		r.m[2][2] = m[2][0] * rhs.m[0][2] + m[2][1] * rhs.m[1][2] + m[2][2] * rhs.m[2][2] + m[2][3] * rhs.m[3][2];
		r.m[2][3] = m[2][0] * rhs.m[0][3] + m[2][1] * rhs.m[1][3] + m[2][2] * rhs.m[2][3] + m[2][3] * rhs.m[3][3];

		r.m[3][0] = m[3][0] * rhs.m[0][0] + m[3][1] * rhs.m[1][0] + m[3][2] * rhs.m[2][0] + m[3][3] * rhs.m[3][0];
		r.m[3][1] = m[3][0] * rhs.m[0][1] + m[3][1] * rhs.m[1][1] + m[3][2] * rhs.m[2][1] + m[3][3] * rhs.m[3][1];
		r.m[3][2] = m[3][0] * rhs.m[0][2] + m[3][1] * rhs.m[1][2] + m[3][2] * rhs.m[2][2] + m[3][3] * rhs.m[3][2];
		r.m[3][3] = m[3][0] * rhs.m[0][3] + m[3][1] * rhs.m[1][3] + m[3][2] * rhs.m[2][3] + m[3][3] * rhs.m[3][3];

		return r;
	}

	Matrix4 operator+ (const Matrix4& rhs) const
	{
		Matrix4 r;

		r.m[0][0] = m[0][0] + rhs.m[0][0];
		r.m[0][1] = m[0][1] + rhs.m[0][1];
		r.m[0][2] = m[0][2] + rhs.m[0][2];
		r.m[0][3] = m[0][3] + rhs.m[0][3];

		r.m[1][0] = m[1][0] + rhs.m[1][0];
		r.m[1][1] = m[1][1] + rhs.m[1][1];
		r.m[1][2] = m[1][2] + rhs.m[1][2];
		r.m[1][3] = m[1][3] + rhs.m[1][3];

		r.m[2][0] = m[2][0] + rhs.m[2][0];
		r.m[2][1] = m[2][1] + rhs.m[2][1];
		r.m[2][2] = m[2][2] + rhs.m[2][2];
		r.m[2][3] = m[2][3] + rhs.m[2][3];

		r.m[3][0] = m[3][0] + rhs.m[3][0];
		r.m[3][1] = m[3][1] + rhs.m[3][1];
		r.m[3][2] = m[3][2] + rhs.m[3][2];
		r.m[3][3] = m[3][3] + rhs.m[3][3];

		return r;
	}

	Matrix4 operator- (const Matrix4& rhs) const
	{
		Matrix4 r;
		r.m[0][0] = m[0][0] - rhs.m[0][0];
		r.m[0][1] = m[0][1] - rhs.m[0][1];
		r.m[0][2] = m[0][2] - rhs.m[0][2];
		r.m[0][3] = m[0][3] - rhs.m[0][3];

		r.m[1][0] = m[1][0] - rhs.m[1][0];
		r.m[1][1] = m[1][1] - rhs.m[1][1];
		r.m[1][2] = m[1][2] - rhs.m[1][2];
		r.m[1][3] = m[1][3] - rhs.m[1][3];

		r.m[2][0] = m[2][0] - rhs.m[2][0];
		r.m[2][1] = m[2][1] - rhs.m[2][1];
		r.m[2][2] = m[2][2] - rhs.m[2][2];
		r.m[2][3] = m[2][3] - rhs.m[2][3];

		r.m[3][0] = m[3][0] - rhs.m[3][0];
		r.m[3][1] = m[3][1] - rhs.m[3][1];
		r.m[3][2] = m[3][2] - rhs.m[3][2];
		r.m[3][3] = m[3][3] - rhs.m[3][3];

		return r;
	}

	bool operator== (const Matrix4& rhs) const
	{
		if (m[0][0] != rhs.m[0][0] || m[0][1] != rhs.m[0][1] || m[0][2] != rhs.m[0][2] || m[0][3] != rhs.m[0][3] ||
			m[1][0] != rhs.m[1][0] || m[1][1] != rhs.m[1][1] || m[1][2] != rhs.m[1][2] || m[1][3] != rhs.m[1][3] ||
			m[2][0] != rhs.m[2][0] || m[2][1] != rhs.m[2][1] || m[2][2] != rhs.m[2][2] || m[2][3] != rhs.m[2][3] ||
			m[3][0] != rhs.m[3][0] || m[3][1] != rhs.m[3][1] || m[3][2] != rhs.m[3][2] || m[3][3] != rhs.m[3][3])
		{
			return false;
		}

		return true;
	}

	bool operator!= (const Matrix4& rhs) const
	{
		return !operator==(rhs);
	}

	Matrix4 transpose() const
	{
		return Matrix4(m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	float determinant() const
	{
		// 代数余子式求行列式
		return m[0][0] * minor(1, 2, 3, 1, 2, 3) -
			m[0][1] * minor(1, 2, 3, 0, 2, 3) +
			m[0][2] * minor(1, 2, 3, 0, 1, 3) -
			m[0][3] * minor(1, 2, 3, 0, 1, 2);
	}

	Matrix4 inverse() const
	{
		// 伴随矩阵求逆矩阵
		float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
		float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
		float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
		float m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

		float v0 = m20 * m31 - m21 * m30;
		float v1 = m20 * m32 - m22 * m30;
		float v2 = m20 * m33 - m23 * m30;
		float v3 = m21 * m32 - m22 * m31;
		float v4 = m21 * m33 - m23 * m31;
		float v5 = m22 * m33 - m23 * m32;

		float t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
		float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
		float t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
		float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

		float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

		float d00 = t00 * invDet;
		float d10 = t10 * invDet;
		float d20 = t20 * invDet;
		float d30 = t30 * invDet;

		float d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m10 * m31 - m11 * m30;
		v1 = m10 * m32 - m12 * m30;
		v2 = m10 * m33 - m13 * m30;
		v3 = m11 * m32 - m12 * m31;
		v4 = m11 * m33 - m13 * m31;
		v5 = m12 * m33 - m13 * m32;

		float d02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m21 * m10 - m20 * m11;
		v1 = m22 * m10 - m20 * m12;
		v2 = m23 * m10 - m20 * m13;
		v3 = m22 * m11 - m21 * m12;
		v4 = m23 * m11 - m21 * m13;
		v5 = m23 * m12 - m22 * m13;

		float d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		return Matrix4(
			d00, d01, d02, d03,
			d10, d11, d12, d13,
			d20, d21, d22, d23,
			d30, d31, d32, d33);
	}

private:
	// 余子式
	float minor(int r0, int r1, int r2,
				int c0, int c1, int c2) const
	{
		return m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r2][c1] * m[r1][c2]) -
			m[r0][c1] * (m[r1][c0] * m[r2][c2] - m[r2][c0] * m[r1][c2]) +
			m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r2][c0] * m[r1][c1]);
	}
public:
	float m[4][4];
};