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

public:
    void Setup(HWND hWnd, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext);

    void Render();
};