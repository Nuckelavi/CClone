#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "DDSTextureLoader.h"
#include "resource.h"
#include <iostream>
#include "structures.h"


class GraphicsCube
{
private:

	DirectX::XMMATRIX _world;
	DirectX::XMFLOAT3 _position;

	static constexpr int NUM_VERTICES = 36;
	//bool rotateX = true;

	ID3D11Buffer* _pVertexBuffer;
	ID3D11Buffer* _pIndexBuffer;


public:
	GraphicsCube();
	~GraphicsCube();

	HRESULT	InitMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
	void Update(float t);
	void Draw(ID3D11DeviceContext* pContext);



	void CalculateTangentBinormal2(POMVertex v0, POMVertex v1, POMVertex v2, 
		DirectX::XMFLOAT3& normal, DirectX::XMFLOAT3& tangent, DirectX::XMFLOAT3& binormal);
	void CalculateTangentBinormal3(POMVertex v0, POMVertex v1, POMVertex v2, 
		DirectX::XMFLOAT3& normal, DirectX::XMFLOAT3& tangent, DirectX::XMFLOAT3& binormal);
	void CalculateModelVectors(POMVertex* vertices, int vertexCount);

	void FlipTangents(const int startPt, POMVertex* vertices, const int vertexCount, const int cluster);


	void SetPosition(DirectX::XMFLOAT3& position);
	void SetVertexBuffer(ID3D11DeviceContext* pContext);
	void SetIndexBuffer(ID3D11DeviceContext* pContext);

	DirectX::XMMATRIX* GetWorld() { return &_world; }
};

