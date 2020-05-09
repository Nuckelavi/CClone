#pragma once

#include <Windows.h>
#include <DirectXMath.h>

class LightManager
{
private:
	DirectX::XMFLOAT4 _lightPosition;
	const DirectX::XMFLOAT4 _originalPosition;

	static constexpr float MOVEMENT_RATE = 0.01f;
	bool _enabled;

public:
	LightManager() = default;
	LightManager(DirectX::XMFLOAT4& light);

	void Update();
	void SetLight(DirectX::XMFLOAT4& lightPos);
	void SetEnabled(bool enabled) { _enabled = enabled; }
	const DirectX::XMFLOAT4& LightVec() const { return _lightPosition; }
	bool IsEnabled() const { return _enabled; }
};