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

//managers
#include "d_GUIManager.h"
#include "Camera/d_CameraManager.h"
#include "d_LightManager.h"
#include "d_TextureManager.h"

//geometry - meshes
#include "Geometry/d_Cube.h"
#include "Geometry/d_GraphicsCube.h"
#include "Geometry/d_Quad.h"

//terrain
#include "d_GridTerrain.h"

//effects
#include "d_SurfaceDetailEffect.h"
#include "d_ScreenSpaceEffects.h"
#include "d_GaussianBlurEffect.h"
#include "d_DepthOfFieldEffect.h"

using namespace std;

typedef vector<DrawableGameObject*> vecDrawables;
