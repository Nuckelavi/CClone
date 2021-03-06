#pragma once

#include <Windows.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

enum class Scene
{
    PHYISCS,
    NORMAL,
    PARALLAX,
    POM,
    SELFSHADOWING,
    GRAYSCALE,
    BOXBLUR,
    GAUSSIAN,
    DOFBLUR,
    HEIGHTMAP,
    FAULTFORM,
    DIAMONDSQUARE,
    CIRCLEHILL,
    VOXEL,
    LAST_ITEM
};

class CameraManager;
class LightManager;

class GUIManager
{
private:
    ImGuiIO io;

    bool show_demo_window;
    bool _displayEffects;
    bool _showCameraMenu;
    bool _showLightMenu;

    float _max;
    float* _light;

    int _camera;
    Scene _currentScene;
    int _lastScene;
    LightManager* _lightManager;
    CameraManager* _cameraManager;
    const char* _currentCam;
    bool _effectsActive[(int)Scene::LAST_ITEM];
    bool _wireframe;

public:
    GUIManager();
    ~GUIManager();

    void Setup(HWND hWnd, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext,
        LightManager& lm, CameraManager& cm);

    void Render();
    void RenderLightMenu();
    void RenderCameraMenu();
    void ChooseScene();

    void Shutdown();

    Scene GetScene() const { return _currentScene; }
    bool GetWireframe() const { return _wireframe; }
};