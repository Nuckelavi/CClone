#pragma once

#include <Windows.h>
#include <DirectXMath.h>

class LightManager
{
private:
	DirectX::XMFLOAT4 _lightPosition;
	DirectX::XMFLOAT4 _originalPosition;

	float MOVEMENT_RATE;
	bool _enabled;

public:
	LightManager();
	LightManager(DirectX::XMFLOAT4& light);

	void Update();
	void SetLight(DirectX::XMFLOAT4& lightPos);
	void SetLight(float x, float y, float z);
	void SetEnabled(bool enabled) { _enabled = enabled; }
	const DirectX::XMFLOAT4& LightVec() const { return _lightPosition; }
	bool IsEnabled() const { return _enabled; }
	const DirectX::XMFLOAT4& GetLight() const { return _lightPosition; }
};