#include "d_CameraOrbit.h"
#include <Windows.h>

using namespace DirectX;

CameraOrbit::CameraOrbit(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 forward, 
	DirectX::XMFLOAT4 up, int wwidth, int wheight, float nearDepth, 
	float farDepth, float moveRate, float rotRate) :
	Camera(position, forward, up, wwidth, wheight, nearDepth, farDepth),
	_movementRate(moveRate),
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
	_movementRate += _moveRateOffset;
	_rotationRate += _rotRateOffset;
}

void CameraOrbit::ScaleMovementRateDown()
{
	if (_movementRate >= _moveRateOffset)
	{
		_movementRate -= _moveRateOffset;
	}
	if (_rotationRate >= _rotRateOffset)
	{
		_rotationRate -= _rotRateOffset;
	}
}

void CameraOrbit::ResetMovementRate()
{
	_movementRate = _originalMoveRate;
	_rotationRate = _originalRotRate;
}
