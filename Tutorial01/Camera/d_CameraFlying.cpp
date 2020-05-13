#include "d_CameraFlying.h"
#include <Windows.h>
//#include <algorithm>

using namespace DirectX;

CameraFlying::CameraFlying(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 forward, 
	DirectX::XMFLOAT4 up, int wwidth, int wheight, float nearDepth, float farDepth) :
	Camera(position, forward, up, wwidth, wheight, nearDepth, farDepth),
	_movementRate(_originalMoveRate),
	_rotationRate(_originalRotRate)
{
}

CameraFlying::CameraFlying(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 forward, 
	DirectX::XMFLOAT4 up, int wwidth, int wheight, float nearDepth, float farDepth, float moveRate, float rotRate) : 
	Camera(position, forward, up, wwidth, wheight, nearDepth, farDepth),
	_movementRate(moveRate),
	_rotationRate(rotRate)
{
}

void CameraFlying::Update()
{
	ProcessInput();
	UpdateViewProj();
}

void CameraFlying::ProcessInput()
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

	//move camera
	if (GetAsyncKeyState(0x53))
	{
		Move(_movementRate, false);
	}
	if (GetAsyncKeyState(0x57))
	{
		Move(-_movementRate, false);
	}
	if (GetAsyncKeyState(0x41))
	{
		Move(_movementRate, true);
	}
	if (GetAsyncKeyState(0x44))
	{
		Move(-_movementRate, true);
	}

	//rotate camera
	if (GetAsyncKeyState(VK_UP))
	{
		Rotate(-_rotationRate, false);
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		Rotate(_rotationRate, false);
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		Rotate(-_rotationRate, true);
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		Rotate(_rotationRate, true);
	}
}

void CameraFlying::Move(float moveRate, bool isSideways)
{
	XMVECTOR distance = XMVectorReplicate(moveRate);
	XMVECTOR direction;
	if (isSideways)
	{
		direction = XMLoadFloat4(&_right);
	}
	else
	{
		direction = XMLoadFloat4(&_forward);
	}
	XMVECTOR position = XMLoadFloat4(&_position);
	XMStoreFloat4(&_position, XMVectorMultiplyAdd(distance, direction, position));
}

void CameraFlying::Rotate(float rotRate, bool isHorizontal)
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

	XMStoreFloat4(&_up, XMVector3TransformNormal(XMLoadFloat4(&_up), Rotation));
	XMStoreFloat4(&_forward, XMVector3TransformNormal(XMLoadFloat4(&_forward), Rotation));
}

void CameraFlying::ScaleMovementRateUp()
{
	_movementRate += _moveRateOffset;
	_rotationRate += _rotRateOffset;
}

void CameraFlying::ScaleMovementRateDown()
{
	/*std::max(0.0f, (_movementRate - _moveRateOffset));
	std::max(0.0f, (_rotationRate - _rotRateOffset));*/
	if (_movementRate >= _moveRateOffset)
	{
		_movementRate -= _moveRateOffset;
	}
	else
	{
		_movementRate = 0.0f;
	}
	if (_rotationRate >= _rotRateOffset)
	{
		_rotationRate -= _rotRateOffset;
	}
	else
	{
		_movementRate = 0.0f;
	}
}

void CameraFlying::ResetMovementRate()
{
	_movementRate = _originalMoveRate;
	_rotationRate = _originalRotRate;
}
