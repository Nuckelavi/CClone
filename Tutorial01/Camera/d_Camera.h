#pragma once

#include <DirectXMath.h>

class Camera
{
protected:
	static constexpr DirectX::XMFLOAT4 nullvec = { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 _position;
	DirectX::XMFLOAT4 _forward;
	DirectX::XMFLOAT4 _up;
	DirectX::XMFLOAT4 _right;

	int _wwidth;
	int _wheight;
	float _nearDepth;
	float _farDepth;

	DirectX::XMFLOAT4X4 _world;
	DirectX::XMFLOAT4X4 _view;
	DirectX::XMFLOAT4X4 _projection;

public:
	Camera();
	Camera(DirectX::XMFLOAT4 position,
		DirectX::XMFLOAT4 forward, DirectX::XMFLOAT4 up);
	Camera(DirectX::XMFLOAT4 position, 
		DirectX::XMFLOAT4 forward, DirectX::XMFLOAT4 up,
		int wwidth, int wheight,
		float nearDepth, float farDepth);
	~Camera();

	void ComputeRightVector();
	void CalculateViewProj();
	void UpdateViewProj();
	void Update();

	void Reshape(int wwidth, int wheight,
		float nearDepth, float farDepth);
	void ResetCamera(DirectX::XMFLOAT4 newPos);

	//set
	void SetPosition(DirectX::XMFLOAT4& position);
	void SetForward(DirectX::XMFLOAT4& forward);
	void SetUp(DirectX::XMFLOAT4& up);

	void SetNearFarDepth(float nearDepth, float farDepth);
	void SetWindow(int wwidth, int wheight);

	//get - all passed by reference, STORE IN VALUE VARIABLE,
	//NOT REFERENCE VARIABLE !!!
	const DirectX::XMFLOAT4X4& GetView() const { return _view; }
	const DirectX::XMFLOAT4X4& GetProjection() const { return _projection; }
	const DirectX::XMFLOAT4X4& GetViewProjection() const;

	const DirectX::XMFLOAT4& Position() const { return _position; }
	const DirectX::XMFLOAT4& Forward() const { return _forward; }
	const DirectX::XMFLOAT4& Up() const { return _up; }

};