#pragma once

#include <cmath>
#include <DirectXMath.h>

class Vec3
{
private:
public:
	float x, y, z;

	Vec3();
	Vec3(float X, float Y, float Z);
	~Vec3();

	float Magnitude();
	float SqMagnitude();
	Vec3 Normalized();
	void Normalize();

	float Dot(Vec3 a, Vec3 b);
	Vec3 Cross(Vec3 a, Vec3 b);

	void AddScaledVector(const Vec3& a, float s);

	Vec3 MatrixMultiply(DirectX::XMFLOAT3X3 matrix);

	Vec3 operator+(const Vec3& a)
	{
		Vec3 result;
		result.x = x + a.x;
		result.y = y + a.y;
		result.z = z + a.z;
		return result;
	}

	void operator+=(const Vec3& a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
	}

	Vec3 operator-(const Vec3& a)
	{
		Vec3 result;
		result.x = x + a.x;
		result.y = y + a.y;
		result.z = z + a.z;
		return result;
	}

	void operator-=(const Vec3& a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
	}

	Vec3 operator*(const float& s)
	{
		Vec3 result;
		result.x = x * s;
		result.y = y * s;
		result.z = z * s;
		return result;
	}

	void operator*=(const float& s)
	{
		x *= s;
		y *= s;
		z *= s;
	}

	void operator=(const Vec3& oldVec)
	{
		x = oldVec.x;
		y = oldVec.y;
		z = oldVec.z;
	}
};