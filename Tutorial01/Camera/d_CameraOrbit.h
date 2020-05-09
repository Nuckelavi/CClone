#pragma once

#include "d_Camera.h"

class CameraOrbit : public Camera
{
private:
	static constexpr float _originalMoveRate = 0.01f;
	static constexpr float _originalRotRate = 0.0005f;
	static constexpr float _moveRateOffset = 0.001f;
	static constexpr float _rotRateOffset = 0.000005f;
	float _movementRate;
	float _rotationRate;

public: 
	CameraOrbit(DirectX::XMFLOAT4 position,
		DirectX::XMFLOAT4 forward, DirectX::XMFLOAT4 up,
		int wwidth, int wheight,
		float nearDepth, float farDepth,
		float moveRate = _originalMoveRate, float rotRate = _originalRotRate);

	void Update();
	void ProcessInput();

	void Rotate(float rotRate, bool isHorizontal);
	void ScaleMovementRateUp();
	void ScaleMovementRateDown();
	void ResetMovementRate();
};