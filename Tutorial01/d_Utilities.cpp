#include "d_Utilities.h"
#include <d3dcompiler.h>

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


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT DX11::CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}