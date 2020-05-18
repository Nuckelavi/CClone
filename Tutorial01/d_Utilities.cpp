#include "d_Utilities.h"

namespace XMF3
{
	using DirectX::XMFLOAT3;

	XMFLOAT3 cross(XMFLOAT3& a, XMFLOAT3& b)
	{
		return XMFLOAT3(a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}

	XMFLOAT3 sum(XMFLOAT3& a, XMFLOAT3& b)
	{
		return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	XMFLOAT3 subtract(XMFLOAT3& a, XMFLOAT3& b)
	{
		return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	DirectX::XMFLOAT3 scale(DirectX::XMFLOAT3& a, float s)
	{
		return DirectX::XMFLOAT3(a.x * s, a.y * s, a.z * s);
	}

	float magnitude(XMFLOAT3& a)
	{
		return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
	}

	void normalize(XMFLOAT3& a)
	{
		float mag = magnitude(a);
		a.x /= mag;
		a.y /= mag;
		a.z /= mag;
	}

	XMFLOAT3 getNormalized(XMFLOAT3& a)
	{
		float mag = magnitude(a);
		return XMFLOAT3(a.x / mag, a.y / mag, a.z / mag);
	}
}

float F::Max(float a, float b)
{
	return a >= b ? a : b;
}

int F::Round(float a)
{
	float remainder = a - (int)a;
	int roundedRemainder = 0;
	if (remainder >= 0.5)
	{
		roundedRemainder = 1;
	}

	return ((int)a + roundedRemainder);
}
