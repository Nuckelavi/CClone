#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "DDSTextureLoader.h"
#include "resource.h"
#include <iostream>
#include "structures.h"


using namespace DirectX;



class DrawableGameObject
{
public:
	DrawableGameObject();
	~DrawableGameObject();

	HRESULT								initMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);
	void								update(float t);
	void								draw(ID3D11DeviceContext* pContext);
	ID3D11Buffer*						getVertexBuffer() { return m_pVertexBuffer; }
	ID3D11Buffer*						getIndexBuffer() { return m_pIndexBuffer; }
	ID3D11ShaderResourceView**			getTextureResourceView() { return &m_pTextureResourceView; 	}
	XMMATRIX*							getTransform() { return &m_World; }
	ID3D11SamplerState**				getTextureSamplerState() { return &m_pSamplerLinear; }
	MaterialPropertiesConstantBuffer	getMaterial() { return m_material;}
	void								setPosition(XMFLOAT3 position);

	void setVertexBuffer(ID3D11DeviceContext* pContext);
	void setIndexBuffer(ID3D11DeviceContext* pContext);

private:
	
	XMMATRIX							m_World;

	ID3D11Buffer*						m_pVertexBuffer;
	ID3D11Buffer*						m_pIndexBuffer;
	ID3D11ShaderResourceView*			m_pTextureResourceView;
	ID3D11SamplerState *				m_pSamplerLinear;
	MaterialPropertiesConstantBuffer	m_material;
	XMFLOAT3							m_position;

	
};

