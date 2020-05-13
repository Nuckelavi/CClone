#include "d_CameraOrbit.h"
#include <Windows.h>

using namespace DirectX;

CameraOrbit::CameraOrbit(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 forward,
	DirectX::XMFLOAT4 up, int wwidth, int wheight, float nearDepth,
	float farDepth, float maxZoom, float moveRate, float rotRate) :
	Camera(position, forward, up, wwidth, wheight, nearDepth, farDepth),
	_maxZoom(maxZoom),
	_zoomRate(moveRate),
	_rotationRate(rotRate)
{
}

void CameraOrbit::Update()
{
	ProcessInput();
	UpdateViewProj();
}

void CameraOrbit::ProcessInput()
{
	if (GetAsyncKeyState(0x52))
	{
		ResetMovementRate();
	}

	//adjust movement and rotation rates
	if (GetAsyncKeyState('6'))//VK_ADD))
	{
		ScaleMovementRateUp();
	}
	if (GetAsyncKeyState('7'))//VK_SUBTRACT))
	{
		ScaleMovementRateDown();
	}

	//rotate camera
	if (GetAsyncKeyState(VK_UP))
	{
		Rotate(_rotationRate, false);
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		Rotate(-_rotationRate, false);
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		Rotate(_rotationRate, true);
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		Rotate(-_rotationRate, true);
	}

	if (GetAsyncKeyState(VK_ADD))
	{
		Zoom(true);
	}
	if (GetAsyncKeyState(VK_SUBTRACT))
	{
		Zoom(false);
	}
}

void CameraOrbit::Rotate(float rotRate, bool isHorizontal)
{
	XMMATRIX Rotation;
	if (isHorizontal)
	{
		Rotation = XMMatrixRotationY(rotRate);
	}
	else
	{
		Rotation = XMMatrixRotationAxis(XMLoadFloat4(&_right), rotRate);
		XMStoreFloat4(&_right, XMVector3TransformNormal(XMLoadFloat4(&_right), Rotation));
	}
	
	XMStoreFloat4(&_position, XMVector3TransformNormal(XMLoadFloat4(&_position), Rotation));
	XMStoreFloat4(&_forward, XMVector3TransformNormal(XMLoadFloat4(&_forward), Rotation));
	XMStoreFloat4(&_up, XMVector3TransformNormal(XMLoadFloat4(&_up), Rotation));
}

void CameraOrbit::ScaleMovementRateUp()
{
	_zoomRate += _moveZoomOffset;
	_rotationRate += _rotRateOffset;
}

void CameraOrbit::ScaleMovementRateDown()
{
	if (_zoomRate >= _moveZoomOffset)
	{
		_zoomRate -= _moveZoomOffset;
	}
	else
	{
		_zoomRate = 0.0f;
	}
	if (_rotationRate >= _rotRateOffset)
	{
		_rotationRate -= _rotRateOffset;
	}
	else
	{
		_rotationRate = 0.0f;
	}
}

void CameraOrbit::ResetMovementRate()
{
	_zoomRate = _originalZoomRate;
	_rotationRate = _originalRotRate;
}

void CameraOrbit::Zoom(bool in)
{
	XMVECTOR distance = XMVectorReplicate(_zoomRate);
	XMVECTOR direction = XMLoadFloat4(&_forward);
	XMVECTOR position = XMLoadFloat4(&_position);

	if (in)
	{
		if ((_position.x * _position.x + _position.y * _position.y + _position.z * _position.z) <= _maxZoom)
		{
			return;
		}
		distance *= -1.0f;
	}

	
	XMStoreFloat4(&_position, XMVectorMultiplyAdd(distance, direction, position));

}
