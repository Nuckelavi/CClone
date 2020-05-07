#include "d_Vec3.h"

Vec3::Vec3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vec3::Vec3(float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;
}

Vec3::~Vec3()
{

}

float Vec3::Magnitude()
{
	return sqrt(x * x + y * y + z * z);
}

//because on some machines, calling sqrt is slow
float Vec3::SqMagnitude()
{
	return (x * x + y * y + z * z);
}

Vec3 Vec3::Normalized()
{
	float mag = Magnitude();
	if (mag != 0)
	{
		float normX = x / mag;
		float normY = y / mag;
		float normZ = z / mag;

		return Vec3(normX, normY, normZ);
	}
	else { return Vec3(); }
}

void Vec3::Normalize()
{
	float mag = Magnitude();
	if (mag != 0)
	{
		x /= mag;
		y /= mag;
		z /= mag;
	}
}

float Vec3::Dot(Vec3 a, Vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 Vec3::Cross(Vec3 a, Vec3 b)
{
	float crossX = a.y * b.z - a.z * b.y;
	float crossY = a.z * b.x - a.x * b.z;
	float crossZ = a.x * b.y - a.y * b.x;

	return Vec3(crossX, crossY, crossZ);
}

void Vec3::AddScaledVector(const Vec3& a, float s)
{
	x += a.x * s;
	y += a.y * s;
	z += a.z * s;
}

Vec3 Vec3::MatrixMultiply(DirectX::XMFLOAT3X3 matrix)
{
	using DirectX::XMFLOAT3X3;

	Vec3 result;
	result.x = x * matrix._11 + y * matrix._21 + z * matrix._31;
	result.y = x * matrix._12 + y * matrix._22 + z * matrix._32;
	result.z = x * matrix._13 + y * matrix._23 + z * matrix._33;

	return result;
}