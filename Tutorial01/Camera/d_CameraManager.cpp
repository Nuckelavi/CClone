#include "d_CameraManager.h"

using namespace DirectX;

CameraManager::CameraManager() :
	_currentCamera(CameraType::FRONT)
{   
}

CameraManager::~CameraManager()
{
}

void CameraManager::InitCameras(int windowWidth, int windowHeight)
{
    _cameras[(int)CameraType::FRONT] =
        Camera(XMFLOAT4(0.0f, 0.0f, 4.0f, 0.0f),
            XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),
            XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f),
            windowWidth, windowHeight, 1.0f, 100.0f);
    _cameras[(int)CameraType::FRONT].UpdateViewProj();

    _cameras[(int)CameraType::TOPDOWN] =
        Camera(XMFLOAT4(0.0f, 4.0f, 0.0f, 0.0f),
            XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),
            XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),
            windowWidth, windowHeight, 1.0f, 100.0f);
    _cameras[(int)CameraType::TOPDOWN].UpdateViewProj();

    _cameras[(int)CameraType::FLYING] = _cameras[0];
    _cameras[(int)CameraType::FIRST_PERSON] = _cameras[1];

    for (Camera& cam : _cameras)
    {
        cam.UpdateViewProj();
    }
}

void CameraManager::UpdateCamera()
{
    _cameras[(int)_currentCamera].Update();
}

void CameraManager::SetCurrentCamera(CameraType newCamera)
{
	_currentCamera = newCamera;
    UpdateCamera();
}

const Camera CameraManager::GetCurrentCamera() const
{
	return _cameras[(int)_currentCamera];
}

const Camera CameraManager::GetCamera(CameraType cameraType) const
{
	return _cameras[(int)cameraType];
}

