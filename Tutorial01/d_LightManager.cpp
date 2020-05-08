#include "d_LightManager.h"

LightManager::LightManager(DirectX::XMFLOAT4& light) :
	_lightPosition(light),
	_originalPosition(light)
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
    if (GetAsyncKeyState(0x41))
    {
        _lightPosition.x -= MOVEMENT_RATE;
    }
    if (GetAsyncKeyState(0x44))
    {
        _lightPosition.x += MOVEMENT_RATE;
    }
    if (GetAsyncKeyState(0x57))
    {
        _lightPosition.y += MOVEMENT_RATE;
    }
    if (GetAsyncKeyState(0x53))
    {
        _lightPosition.y -= MOVEMENT_RATE;
    }
    if (GetAsyncKeyState(0x45))
    {
        _lightPosition.z += MOVEMENT_RATE;
    }
    if (GetAsyncKeyState(0x51))
    {
        _lightPosition.z -= MOVEMENT_RATE;
    }
    if (GetAsyncKeyState(VK_NUMPAD0))
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
    }
}

void LightManager::SetLight(DirectX::XMFLOAT4& lightPos)
{
    _lightPosition = lightPos;
}
