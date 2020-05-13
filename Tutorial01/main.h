#pragma once

#include <windows.h>
#include <windowsx.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "DDSTextureLoader.h"
#include "resource.h"
#include <iostream>

#include "DrawableGameObject.h"
#include "structures.h"

#include <vector>

//imgui headers
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

//managers
#include "Camera/d_CameraManager.h"
#include "d_LightManager.h"
#include "d_TextureManager.h"

//geometry - meshes
#include "d_Cube.h"
#include "d_GraphicsCube.h"

using namespace std;

typedef vector<DrawableGameObject*> vecDrawables;
