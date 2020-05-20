#include "d_GUIManager.h"

#include "Camera/d_CameraManager.h"
#include "d_LightManager.h"

#include "d_Utilities.h"

GUIManager::GUIManager() :
    show_demo_window(false),
    _displayEffects(false),
    _showCameraMenu(false),
    _showLightMenu(false),
    _max(1.0f),
    //_light{0.0f, 0.0f, 0.0f},
    _camera(0),
    _currentScene(Scene::NORMAL),
    _currentCam(nullptr),
    _lastScene(0),
    _wireframe(false)
{
    for (int i = 0; i < (int)Scene::LAST_ITEM; ++i)
    {
        _effectsActive[i] = false;
    }

    _light = new float[3];
}

GUIManager::~GUIManager()
{
    delete[] _light;
}

void GUIManager::Setup(HWND hWnd, ID3D11Device* pd3dDevice,
    ID3D11DeviceContext* pContext, LightManager& lm, CameraManager& cm)
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(pd3dDevice, pContext);

    _cameraManager = &cm;
    _lightManager = &lm;

    DirectX::XMFLOAT4 light = _lightManager->GetLight();
    _max = F::Max(F::Max(std::abs(light.x), std::abs(light.y)), std::abs(light.z));
}

void GUIManager::Render()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();


    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoBackground;
    //window_flags |= ImGuiWindowFlags_NoScrollbar;
    //window_flags |= ImGuiWindowFlags_NoCollapse;
    //window_flags |= ImGuiWindowFlags_NoNav;
    //window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    bool _pOpen = true;

    ImGui::Begin(" ", NULL, window_flags);

    if (ImGui::BeginMenuBar())
    {
        ImGui::SetWindowSize(ImVec2(640, 10));
        ImGui::SetWindowPos(ImVec2(0, 0));
        if (ImGui::BeginMenu("Scenes"))//, NULL, &_displayEffects))
        {
            ChooseScene();

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Other Menus"))//, NULL, &_displayEffects))
        {
            ImGui::MenuItem("Camera Control", NULL, &_showCameraMenu);
            ImGui::MenuItem("Lights Control", NULL, &_showLightMenu);
            ImGui::MenuItem("Wireframe", NULL, &_wireframe);

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();


    if (_showCameraMenu)
    {
        RenderCameraMenu();
    }

    if (_showLightMenu)
    {
        RenderLightMenu();
    }

    
    //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Rendering
    ImGui::Render();
    //g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
    //g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, (float*)&clear_color);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void GUIManager::RenderLightMenu()
{
    DirectX::XMFLOAT4 light = _lightManager->GetLight();

    //_max = F::Max(F::Max(std::abs(light.x), std::abs(light.y)), std::abs(light.z));

    if (_max != 0.0f)
    {
        _light[0] = light.x / _max;
        _light[1] = light.y / _max;
        _light[2] = light.z / _max;
    }
    else
    {
        _light[0] = 0.0f;
        _light[1] = 0.0f;
        _light[2] = 0.0f;
    }

    //render ---------------------------------------
    ImGui::Begin("Lights Menu", &_showLightMenu);

    ImGui::Text("Position");
    ImGui::SameLine();
    ImGui::SliderFloat3("", _light, -1.0f, 1.0f);

    ImGui::Text("Max offset");
    ImGui::SameLine();
    ImGui::InputFloat("", &_max, 0.25f, 0.5f, 3);

    if (_max < 0.0f)
    {
        _max = 0.0f;
    }

    _lightManager->SetLight(_light[0] * _max, _light[1] * _max, _light[2] * _max);


    ImGui::Text("    ");

    bool lightsEnabled = _lightManager->IsEnabled();
    const char* toggleButton;
    if (lightsEnabled)
    {
        toggleButton = "Turn Lights Off";
    }
    else
    {
        toggleButton = "Turn Lights On";
    }

    if (ImGui::Button(toggleButton))
    {
        _lightManager->SetEnabled(!lightsEnabled);
    }

    ImGui::End();
}

void GUIManager::RenderCameraMenu()
{
    ImGui::Begin("Camera Menu", &_showCameraMenu);

    
    const char* Cameras[] = {
        "Front",
        "Top-Down",
        "Orbit",
        "Flying",
        "First Person"
    };

    int camIndex = (int)_cameraManager->GetCameraType();
    if (camIndex < IM_ARRAYSIZE(Cameras))
    {
        _currentCam = Cameras[camIndex];
    }
    
    if (ImGui::BeginCombo("Camera     ", _currentCam))
    {
        for (int i = 0; i < IM_ARRAYSIZE(Cameras); ++i)
        {
            bool selected = (_currentCam == Cameras[i]);
            if (ImGui::Selectable(Cameras[i], selected))
            {
                _currentCam = Cameras[i];
            }
            if (selected)
            {
                ImGui::SetItemDefaultFocus();
                _currentCam = Cameras[i];
            }

            if (ImGui::IsItemClicked())
            {
                _currentCam = Cameras[i];
                _cameraManager->SetCurrentCameraInt(i);
                break;
            }
        }

        ImGui::EndCombo();
    }

    if (_cameraManager->GetCameraType() == CameraType::ORBIT ||
        _cameraManager->GetCameraType() == CameraType::FLYING)
    {
        if (ImGui::Button("Speed Up"))
        {
            //change these to virtual
            _cameraManager->GetCurrentCamera()->ScaleMovementRateUp();
        }
        ImGui::SameLine();
        if (ImGui::Button("Slow Down"))
        {
            _cameraManager->GetCurrentCamera()->ScaleMovementRateDown();
        }
    }


    ImGui::End();
}

void GUIManager::ChooseScene()
{
    const char* SceneNames[] = {
        //"Physics Demo",
        "Blank",
        "Normal Mapping",
        "Simple Parallax",
        "Parallax Occlusion",
        "POM + Self Shadowing",
        "Grayscale",
        "Box Blur",
        "Gaussian Blur",
        "Depth of Field Blur",
        "Heightmap Terrain",
        "Fault Formation",
        "Diamond Square",
        "Circle/Hill"
        //"Voxel"
    };
    
    for (int i = 0; i < IM_ARRAYSIZE(SceneNames); ++i)
    {
        ImGui::MenuItem(SceneNames[i], NULL, &_effectsActive[i]);
        if (ImGui::IsItemClicked())
        {
            _effectsActive[_lastScene] = false;
            _currentScene = (Scene)i;
            _lastScene = i;
            ImGui::SetItemDefaultFocus();
        }
    }

}

void GUIManager::Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
