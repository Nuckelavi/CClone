#pragma once

#include <vector>

#include "d_Camera.h"
#include "d_CameraFlying.h"
#include "d_CameraOrbit.h"

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
	void SetCurrentCameraInt(int newCameraIndex);
	const Camera* GetCurrConstCamera() const;
	Camera* GetCurrentCamera() const;
	const Camera* GetCamera(CameraType cameraType) const;
	CameraType GetCameraType() const { return _currentCamera; }

};