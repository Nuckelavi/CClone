#include "d_TextureManager.h"

#include "DDSTextureLoader.h"

TextureManager::TextureManager() :
	_texNum(10)
{
	_ppTextureRVs = new ID3D11ShaderResourceView* [_texNum];
}

TextureManager::~TextureManager()
{
	for (int i = 0; i < _texNum; ++i)
	{
		if (_ppTextureRVs[i] && _ppTextureRVs[i] != nullptr)
		{
			_ppTextureRVs[i]->Release();
		}
	}

	delete _ppTextureRVs;
	_ppTextureRVs = nullptr;
}

HRESULT TextureManager::InitTextures(ID3D11Device* _pd3dDevice)
{
	const wchar_t* texFilePaths[] =
	{
		L"Resources\\stone.dds", //0
		//L"Resources\\conenormal.dds",
		L"Resources\\stone_texture.dds", //1
		L"Resources\\stone_normalMap.dds", 
		L"Resources\\stone_heightMap.dds", 
		L"Resources\\bricks_TEX.dds", //4
		L"Resources\\bricks_NORM.dds", 
		L"Resources\\bricks_DISP.dds", 
		L"Resources\\toybox_TEX.dds", //7
		L"Resources\\toybox_NORM.dds", 
		L"Resources\\toybox_DISP.dds"
	};

	HRESULT hr;
	for (int i = 0; i < _texNum; ++i)
	{
		hr = DirectX::CreateDDSTextureFromFile(_pd3dDevice, texFilePaths[i],
			nullptr, &_ppTextureRVs[i]);
		if (FAILED(hr))
			return hr;
	}
}

ID3D11ShaderResourceView* TextureManager::TexturesAt(TextureGroup texGroup) const
{
	return _ppTextureRVs[(int)texGroup];
}
