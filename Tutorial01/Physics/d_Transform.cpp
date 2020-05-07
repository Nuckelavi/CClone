#include "d_Transform.h"

using namespace DirectX;

Transform::Transform() : 
	_position(Vec3()),
	_rotation(Vec3()),
	_scale(Vec3(1.0f, 1.0f, 1.0f)),
	_world(XMFLOAT4X4())
{
}

Transform::Transform(Vec3 pos, Vec3 rot, Vec3 scale) :
	_position(pos),
	_rotation(rot),
	_scale(scale),
	_world(XMFLOAT4X4())
{
}

Transform::~Transform()
{

}

void Transform::SetPosition(Vec3 position)
{
	_position = position;
}

void Transform::SetPosition(float x, float y, float z)
{
	SetPosition(Vec3(x, y, z));
}

void Transform::SetRotation(Vec3 rotation)
{
	_rotation = rotation;
}

void Transform::SetRotation(float x, float y, float z)
{
	SetRotation(Vec3(x, y, z));
}

void Transform::SetScale(Vec3 scale)
{
	_scale = scale;
}

void Transform::SetScale(float x, float y, float z)
{
	SetScale(Vec3(x, y, z));
}
