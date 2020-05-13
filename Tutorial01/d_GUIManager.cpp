#include "d_GUIManager.h"

void GUIManager::Setup(HWND hWnd, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(pd3dDevice, pContext);
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
    //if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    //if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    //if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
    //if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
    //if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
    //if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    //if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
    //if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
    //if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    //if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

    bool _pOpen = true;

    ImGui::Begin("Hello, world!", NULL, window_flags);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Scenes"))//, NULL, &_displayEffects))
        {
            ImGui::MenuItem("Physics Demo", NULL, &_displayEffects);
            ImGui::MenuItem("Normap Mapping", NULL, &_displayEffects);
            ImGui::MenuItem("Simple Parallax", NULL, &_displayEffects);
            ImGui::MenuItem("Parallax Occlusion", NULL, &_displayEffects);
            ImGui::MenuItem("POM + Self Shadowing", NULL, &_displayEffects);
            ImGui::MenuItem("Grayscale", NULL, &_displayEffects);
            ImGui::MenuItem("Box Blur", NULL, &_displayEffects);
            ImGui::MenuItem("Gaussian Blur", NULL, &_displayEffects);
            ImGui::MenuItem("Depth of Field Blur", NULL, &_displayEffects);
            ImGui::MenuItem("Heightmap Terrain", NULL, &_displayEffects);
            ImGui::MenuItem("Fault Formation Terrain", NULL, &_displayEffects);
            ImGui::MenuItem("Diamond Square Terrain", NULL, &_displayEffects);
            ImGui::MenuItem("Circle/Hill Terrain", NULL, &_displayEffects);
            ImGui::MenuItem("Voxel Terrain", NULL, &_displayEffects);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Other Menus"))//, NULL, &_displayEffects))
        {
            ImGui::MenuItem("Camera Control", NULL, &_showCameraMenu);
            ImGui::MenuItem("Lights Control", NULL, &_showLightMenu);

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Text("This is some useful text.");
    ImGui::Checkbox("Demo Window", &show_demo_window);
    ImGui::End();


    if (_showCameraMenu)
    {
        ImGui::Begin("Camera Menu");

        if (ImGui::BeginMenu("Camera Type"))
        {
            
            ImGui::EndMenu();
        }

        const char* Cameras[] = {
            "Front",
            "Top-Down",
            "Orbit",
            "Flying",
            "First Person"
        };

        if (ImGui::BeginCombo("Camera Type", "yeet"))
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
                }
            }
            ImGui::EndCombo();
        }

        ImGui::End();
    }

    /*
    const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO", "PPPP", "QQQQQQQQQQ", "RRR", "SSSS" };
static const char* current_item = NULL;

if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
{
    for (int n = 0; n < IM_ARRAYSIZE(items); n++)
    {
        bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
        if (ImGui::Selectable(items[n], is_selected)
            current_item = items[n];
        if (is_selected)
            ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
    }
    ImGui::EndCombo();
}
    */


    if (_showLightMenu)
    {
        ImGui::Begin("Lights Menu");

        ImGui::Text("Position");
        ImGui::SameLine();
        ImGui::SliderFloat3("", &_x, -1.0f, 1.0f);

        ImGui::Text("Max offset");
        ImGui::SameLine();
        ImGui::InputFloat("", &_max, 0.25f, 0.5f, 3);
        if (_max < 0.0f)
        {
            _max = 0.0f;
        }

        ImGui::End();
    }




    //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Rendering
    ImGui::Render();
    //g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
    //g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, (float*)&clear_color);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void GUIManager::Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
