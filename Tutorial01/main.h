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

#include "d_Cube.h"
#include "Camera/d_CameraManager.h"
#include "d_LightManager.h"
#include "d_TextureManager.h"

#include "d_GraphicsCube.h"

using namespace std;

typedef vector<DrawableGameObject*> vecDrawables;
