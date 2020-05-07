#pragma once

#include <vector>

#include "d_Camera.h"

enum class CameraType
{
	FRONT,
	TOPDOWN,
	FLYING,
	FIRST_PERSON
};

class CameraManager
{
private:
	static constexpr int CAMERA_NUM = 4;
	CameraType _currentCamera;
	Camera _cameras[CAMERA_NUM];

public:
	CameraManager();
	~CameraManager();

	void InitCameras(int windowWidth, int windowHeight);
	void UpdateCamera();
	void SetCurrentCamera(CameraType newCamera);
	const Camera GetCurrentCamera() const;
	const Camera GetCamera(CameraType cameraType) const;

};