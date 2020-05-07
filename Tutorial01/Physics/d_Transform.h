#pragma once

#include <DirectXMath.h>
#include "d_Vec3.h"

using namespace std;

class Transform
{
private:
	Vec3 _position;
	Vec3 _rotation;
	Vec3 _scale;

	DirectX::XMFLOAT4X4 _world;

public:
	Transform();
	Transform(Vec3 pos, Vec3 rot, Vec3 scale);
	~Transform();

	//set
	void SetPosition(Vec3 position); 
	void SetPosition(float x, float y, float z);
	
	void SetRotation(Vec3 rotation);
	void SetRotation(float x, float y, float z);
	
	void SetScale(Vec3 scale);
	void SetScale(float x, float y, float z);

	//get
	Vec3 GetPosition() const { return _position; }
	Vec3 GetRotation() const { return _rotation; }
	Vec3 GetScale() const { return _scale; }

	DirectX::XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }
	DirectX::XMFLOAT4X4& GetWorld() { return _world; }

};