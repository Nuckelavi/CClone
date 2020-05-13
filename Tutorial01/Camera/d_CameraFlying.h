#pragma once

#include "d_Camera.h"

class CameraFlying : public Camera
{
private:
	static constexpr float _originalMoveRate = 0.01f;
	static constexpr float _originalRotRate = 0.005f;
	static constexpr float _moveRateOffset = 0.0025f;
	static constexpr float _rotRateOffset = 0.00125f;
	float _movementRate;
	float _rotationRate;

public:
	CameraFlying(DirectX::XMFLOAT4 position,
		DirectX::XMFLOAT4 forward, DirectX::XMFLOAT4 up,
		int wwidth, int wheight,
		float nearDepth, float farDepth);
	CameraFlying(DirectX::XMFLOAT4 position,
		DirectX::XMFLOAT4 forward, DirectX::XMFLOAT4 up,
		int wwidth, int wheight,
		float nearDepth, float farDepth,
		float moveRate, float rotRate);

	void Update();
	void ProcessInput();

	void Move(float moveRate, bool isSideways);
	void Rotate(float rotRate, bool isHorizontal);
	void ScaleMovementRateUp();
	void ScaleMovementRateDown();
	void ResetMovementRate();

};