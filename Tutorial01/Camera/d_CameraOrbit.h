#pragma once

//TO DO:
//make orbit camera look at specific object at all times.

#include "d_Camera.h"

class CameraOrbit : public Camera
{
private:
	static constexpr float _originalZoomRate = 0.0025f;
	static constexpr float _originalRotRate = 0.000625f;
	static constexpr float _moveZoomOffset = 0.00125f;//0.001f;
	static constexpr float _rotRateOffset = 0.0003125f;//0.000005f;
	float _maxZoom;
	float _zoomRate;
	float _rotationRate;

public: 
	CameraOrbit(DirectX::XMFLOAT4 position,
		DirectX::XMFLOAT4 forward, DirectX::XMFLOAT4 up,
		int wwidth, int wheight,
		float nearDepth, float farDepth, float maxZoom = 0.0f,
		float moveRate = _originalZoomRate, float rotRate = _originalRotRate);

	void Update();
	void ProcessInput();

	void Rotate(float rotRate, bool isHorizontal);
	void ScaleMovementRateUp();
	void ScaleMovementRateDown();
	void ResetMovementRate();

	void Zoom(bool in);
	void SetZoomThreshold(float newMaxZoom);
};