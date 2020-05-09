#pragma once

#include <vector>

#include "d_Camera.h"
#include "d_CameraFlying.h"

enum class CameraType
{
	FRONT,
	TOPDOWN,
	ORBIT,
	FLYING,
	FIRST_PERSON
};

class CameraManager
{
private:
	static constexpr int CAMERA_NUM = 5;
	CameraType _currentCamera;
	Camera* _pCameras[CAMERA_NUM];

public:
	CameraManager();
	~CameraManager();

	void InitCameras(int windowWidth, int windowHeight);
	void UpdateCamera();
	void SetCurrentCamera(CameraType newCamera);
	const Camera* GetCurrentCamera() const;
	const Camera* GetCamera(CameraType cameraType) const;

};