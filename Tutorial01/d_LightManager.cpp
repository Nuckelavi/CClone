#include "d_LightManager.h"

LightManager::LightManager() :
    _lightPosition(DirectX::XMFLOAT4(0.0f, 0.75f, -1.0f, 1.0f)),
    _originalPosition(_lightPosition),
    _enabled(true),
    MOVEMENT_RATE(0.01f)
{
}

LightManager::LightManager(DirectX::XMFLOAT4& light) :
	_lightPosition(light),
	_originalPosition(light),
    _enabled(true),
    MOVEMENT_RATE(0.01f)
{
}

void LightManager::Update()
{
    //-------------------------------------------------------------------
   //INPUTS
   //-------------------------------------------------------------------
   /*A KEY - move light -X axis
    *D KEY - move light +X axis
    *W KEY - move light +Y axis
    *S KEY - move light -Y axis
    *E KEY - move light +Z axis
    *Q KEY - move light -Z axis
    *NUMPAD 0 - reset light position (light in front of object)
    *NUMPAD 1 - set light to the left of object
    *NUMPAD 2 - set light behind the object
    *NUMPAD 3 - set light to the right of object
    */
    //------------------------------------------------------------------
    if (GetAsyncKeyState(VK_NUMPAD4))
    {
        _lightPosition.x -= MOVEMENT_RATE;
    }
    if (GetAsyncKeyState(VK_NUMPAD6))
    {
        _lightPosition.x += MOVEMENT_RATE;
    }
    if (GetAsyncKeyState(VK_NUMPAD8))
    {
        _lightPosition.y += MOVEMENT_RATE;
    }
    if (GetAsyncKeyState(VK_NUMPAD2))
    {
        _lightPosition.y -= MOVEMENT_RATE;
    }
    if (GetAsyncKeyState(VK_NUMPAD9))
    {
        _lightPosition.z += MOVEMENT_RATE;
    }
    if (GetAsyncKeyState(VK_NUMPAD7))
    {
        _lightPosition.z -= MOVEMENT_RATE;
    }
    /*if (GetAsyncKeyState(VK_NUMPAD0))
    {
        _lightPosition = _originalPosition;
    }
    if (GetAsyncKeyState(VK_NUMPAD2))
    {
        _lightPosition.x = _originalPosition.x;
        _lightPosition.y = _originalPosition.y;
        _lightPosition.z = -_originalPosition.z;
    }
    if (GetAsyncKeyState(VK_NUMPAD1))
    {
        _lightPosition.x = _originalPosition.z;
        _lightPosition.y = _originalPosition.y;
        _lightPosition.z = _originalPosition.x;
    }
    if (GetAsyncKeyState(VK_NUMPAD3))
    {
        _lightPosition.x = -_originalPosition.z;
        _lightPosition.y = _originalPosition.y;
        _lightPosition.z = _originalPosition.x;
    }*/

    if (GetAsyncKeyState(0x4C))
    {
        _enabled = (!_enabled);
    }
}

void LightManager::SetLight(DirectX::XMFLOAT4& lightPos)
{
    _lightPosition = lightPos;
}

void LightManager::SetLight(float x, float y, float z)
{
    _lightPosition = { x, y, z, _lightPosition.w };
}
