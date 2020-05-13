#pragma once

#include <Windows.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"


class GUIManager
{
private:
    ImGuiIO io;

    bool show_demo_window = false;
    bool _displayEffects = false;
    bool _showCameraMenu = false;
    bool _showLightMenu = false;

    float _max = 1.0f;
    float _x = 0.0f; 
    float _y = 0.0f;
    float _z = 0.0f;

    int _camera = 0;
    const char* _currentCam = nullptr;

public:
    void Setup(HWND hWnd, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);

    void Render();

    void Shutdown();
};