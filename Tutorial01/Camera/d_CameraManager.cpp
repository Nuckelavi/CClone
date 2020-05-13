#include "d_CameraManager.h"

using namespace DirectX;

CameraManager::CameraManager() :
	_currentCamera(CameraType::FRONT)
{   
    for (int i = 0; i < CAMERA_NUM; ++i)
    {
        _pCameras[i] = nullptr;
    }
}

CameraManager::~CameraManager()
{
    for (int i = 0; i < CAMERA_NUM; ++i)
    {
        delete _pCameras[i];
    }
}

void CameraManager::InitCameras(int windowWidth, int windowHeight)
{
    _pCameras[(int)CameraType::FRONT] =
        new Camera(XMFLOAT4(0.0f, 0.0f, 4.0f, 0.0f),
            XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),
            XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f),
            windowWidth, windowHeight, 1.0f, 100.0f);

    _pCameras[(int)CameraType::TOPDOWN] =
        new Camera(XMFLOAT4(0.0f, 4.0f, 0.0f, 0.0f),
            XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),
            XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),
            windowWidth, windowHeight, 1.0f, 100.0f);

    _pCameras[(int)CameraType::FLYING] = 
        new CameraFlying(XMFLOAT4(0.0f, 0.0f, 4.0f, 0.0f),
            XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),
            XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f),
            windowWidth, windowHeight, 1.0f, 100.0f);

    _pCameras[(int)CameraType::ORBIT] = 
        new CameraOrbit(XMFLOAT4(0.0f, 0.0f, 4.0f, 0.0f),
            XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),
            XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f),
            windowWidth, windowHeight, 1.0f, 100.0f, 4.5f);

    _pCameras[(int)CameraType::FIRST_PERSON] =
        new CameraFlying(XMFLOAT4(0.0f, 0.0f, 4.0f, 0.0f),
            XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f),
            XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f),
            windowWidth, windowHeight, 1.0f, 100.0f);

    for (int i = 0; i < CAMERA_NUM; ++i)
    {
        _pCameras[i]->UpdateViewProj();
    }
}

void CameraManager::UpdateCamera()
{
    _pCameras[(int)_currentCamera]->Update();
}

void CameraManager::SetCurrentCamera(CameraType newCamera)
{
	_currentCamera = newCamera;
    UpdateCamera();
}

void CameraManager::SetCurrentCameraInt(int newCameraIndex)
{
    for (int i = 0; i <= (int)CameraType::FIRST_PERSON; ++i)
    {
        if (newCameraIndex == 2)
        {
            int yoink;
        }
        if (i == newCameraIndex)
        {
            _currentCamera = (CameraType)i;
            break;
        }
    }
    UpdateCamera();
}

const Camera* CameraManager::GetCurrConstCamera() const
{
    return  _pCameras[(int)_currentCamera];
}

Camera* CameraManager::GetCurrentCamera() const
{
    return _pCameras[(int)_currentCamera];
}

const Camera* CameraManager::GetCamera(CameraType cameraType) const
{
	return _pCameras[(int)cameraType];
}

