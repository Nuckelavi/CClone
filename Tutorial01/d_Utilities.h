#pragma once

#include <Windows.h>
#include <d3d11_1.h>
#include <DirectXMath.h>

struct QuadVertex
{
	DirectX::XMFLOAT4 Pos;
	DirectX::XMFLOAT2 TexCoord;
};

namespace XMF3
{
	DirectX::XMFLOAT3 cross(DirectX::XMFLOAT3& a, DirectX::XMFLOAT3& b);
	DirectX::XMFLOAT3 sum(DirectX::XMFLOAT3& a, DirectX::XMFLOAT3& b);
	DirectX::XMFLOAT3 subtract(DirectX::XMFLOAT3& a, DirectX::XMFLOAT3& b);
	DirectX::XMFLOAT3 scale(DirectX::XMFLOAT3& a, float s);

	float magnitude(DirectX::XMFLOAT3& a);
	void normalize(DirectX::XMFLOAT3& a);
	DirectX::XMFLOAT3 getNormalized(DirectX::XMFLOAT3& a);
}

namespace F
{
	float Max(float a, float b);
	int Round(float a);
}

namespace DX11
{
	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
}