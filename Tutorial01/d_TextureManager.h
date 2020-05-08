#pragma once

#include <d3d11_1.h>

enum class TextureGroup
{
	STONE,
	STONE2,
	BRICK = 4,
	TOYBOX = 7
};

class TextureManager
{
private:
	static constexpr int MAX_TEX_NUM = 3;
	const int _texNum = 10;

	ID3D11ShaderResourceView** _ppTextureRVs;

public:
	TextureManager();
	~TextureManager();

	HRESULT InitTextures(ID3D11Device* _pd3dDevice);

	int TextureMaxNum() const { return MAX_TEX_NUM; }
	ID3D11ShaderResourceView* TexturesAt(TextureGroup texGroup) const;

};