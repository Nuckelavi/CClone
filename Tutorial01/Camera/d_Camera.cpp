#include "d_Camera.h"

using namespace DirectX;

Camera::Camera() : Camera(nullvec, nullvec, nullvec, 
	640, 480, 1.0f, 100.0f)
{
	ComputeRightVector();
}

Camera::Camera(DirectX::XMFLOAT4 position, 
	DirectX::XMFLOAT4 forward, DirectX::XMFLOAT4 up) : 
	Camera(position, forward, up, 640, 480, 1.0f, 100.0f)
{
	ComputeRightVector();
}

Camera::Camera(DirectX::XMFLOAT4 position,
	DirectX::XMFLOAT4 forward, DirectX::XMFLOAT4 up,
	int wwidth, int wheight,
	float nearDepth, float farDepth) : 
	_position(position),
	_forward(forward),
	_up(up),
	_wwidth(wwidth),
	_wheight(wheight),
	_nearDepth(nearDepth),
	_farDepth(farDepth)
{
	ComputeRightVector();
}

Camera::~Camera()
{
}

void Camera::ComputeRightVector()
{
	XMVECTOR Forward = XMLoadFloat4(&_forward);
	XMVector4Normalize(Forward);
	XMVECTOR Up = XMLoadFloat4(&_up);
	XMVector4Normalize(Up);
	XMVECTOR Right = XMVector3Cross(Forward, Up);
	XMStoreFloat4(&_right, Right);
}

void Camera::CalculateViewProj()
{
	//init view matrix
	XMVECTOR Eye = XMLoadFloat4(&_position);
	XMVECTOR Forward = XMLoadFloat4(&_forward);
	XMVECTOR Up = XMLoadFloat4(&_up);

	XMStoreFloat4x4(&_view, XMMatrixLookAtLH(Eye, Forward, Up));

	//init projection matrix
	XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2,
		(float)_wwidth / (float)_wheight, _nearDepth, _farDepth));
}

void Camera::UpdateViewProj()
{
	XMVECTOR Forward = XMLoadFloat4(&_forward);
	XMVECTOR Up = XMLoadFloat4(&_up);
	XMVECTOR Right = XMLoadFloat4(&_right);
	XMVECTOR Position = XMLoadFloat4(&_position);

	XMVector4Normalize(Forward);
	XMStoreFloat4(&_forward, Forward);
	Up = XMVector4Normalize(XMVector3Cross(Forward, Right));
	XMStoreFloat4(&_up, Up);

	Right = XMVector3Cross(Up, Forward);
	XMStoreFloat4(&_right, Right);

	float x = XMVectorGetX(XMVector3Dot(Position, Right));
	float y = XMVectorGetX(XMVector3Dot(Position, Up));
	float z = XMVectorGetX(XMVector3Dot(Position, Forward));

	//set view
	_view(0, 0) = _right.x;
	_view(1, 0) = _right.y;
	_view(2, 0) = _right.z;
	_view(3, 0) = x;

	_view(0, 1) = _up.x;
	_view(1, 1) = _up.y;
	_view(2, 1) = _up.z;
	_view(3, 1) = y;

	_view(0, 2) = _forward.x;
	_view(1, 2) = _forward.y;
	_view(2, 2) = _forward.z;
	_view(3, 2) = z;

	_view(0, 3) = 0.0f;
	_view(1, 3) = 0.0f;
	_view(2, 3) = 0.0f;
	_view(3, 3) = 1.0f;

	//set projection
	XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2,
		(float)_wwidth / (float)_wheight, _nearDepth, _farDepth));
}

void Camera::Update()
{
	UpdateViewProj();
}

void Camera::Reshape(int wwidth, int wheight, float nearDepth, float farDepth)
{
	SetWindow(wwidth, wheight);
	SetNearFarDepth(nearDepth, farDepth);
}

void Camera::ResetCamera(DirectX::XMFLOAT4 newPos)
{
	//NEEEEEEDS ADJUSTING !!!!!!!!!!!!!!!!!!!
	_position = newPos;
}

void Camera::SetPosition(DirectX::XMFLOAT4& position)
{
	_position = position;
}

void Camera::SetForward(DirectX::XMFLOAT4& forward)
{
	_forward = forward;
	ComputeRightVector();
}

void Camera::SetUp(DirectX::XMFLOAT4& up)
{
	_up = up;
	ComputeRightVector();
}

void Camera::SetNearFarDepth(float nearDepth, float farDepth)
{
	_nearDepth = nearDepth;
	_farDepth = farDepth;
}

void Camera::SetWindow(int wwidth, int wheight)
{
	_wwidth = wwidth;
	_wheight = wheight;
}

const DirectX::XMFLOAT4X4& Camera::GetViewProjection() const
{
	XMMATRIX view = XMLoadFloat4x4(&_view);
	XMMATRIX projection = XMLoadFloat4x4(&_projection);

	XMFLOAT4X4 viewProj;
	XMStoreFloat4x4(&viewProj, view * projection);

	return viewProj;
}
