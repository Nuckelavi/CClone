//--------------------------------------------------------------------------------------
// File: main.cpp
//
// This application demonstrates animation using matrix transformations
//
// http://msdn.microsoft.com/en-us/library/windows/apps/ff729722.aspx
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#define _XM_NO_INTRINSICS_

#include "main.h"



#include "d_GridTerrain.h"
#include "d_SurfaceDetailEffect.h"

DirectX::XMFLOAT4 g_EyePosition(0.0f, 0, -3, 1.0f);

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT		InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT		InitDevice();
HRESULT		InitMesh();
HRESULT		InitWorld(int width, int height);
void		CleanupDevice();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void		Render();



//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = nullptr;
HWND                    g_hWnd = nullptr;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = nullptr;
ID3D11Device1*          g_pd3dDevice1 = nullptr;
ID3D11DeviceContext*    g_pImmediateContext = nullptr;
ID3D11DeviceContext1*   g_pImmediateContext1 = nullptr;
IDXGISwapChain*         g_pSwapChain = nullptr;
IDXGISwapChain1*        g_pSwapChain1 = nullptr;
ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
ID3D11Texture2D*        g_pDepthStencil = nullptr;
ID3D11DepthStencilView* g_pDepthStencilView = nullptr;
ID3D11VertexShader*     g_pVertexShader = nullptr;

ID3D11PixelShader*      g_pPixelShader = nullptr;
//ID3D11PixelShader*      g_pPixelShaderSolid = nullptr;

ID3D11InputLayout*      g_pVertexLayout = nullptr;
ID3D11Buffer*           g_pVertexBuffer = nullptr;
ID3D11Buffer*           g_pIndexBuffer = nullptr;

ID3D11Buffer*           g_pConstantBuffer = nullptr;
ID3D11Buffer*           g_pMaterialConstantBuffer = nullptr;
ID3D11Buffer*           g_pLightConstantBuffer = nullptr;

ID3D11ShaderResourceView * g_pTextureRV = nullptr;
ID3D11ShaderResourceView * g_pNormalTextureRV = nullptr;

ID3D11SamplerState *	g_pSamplerLinear = nullptr;
ID3D11SamplerState *	g_pSamplerNormal = nullptr;

XMMATRIX                g_World1;
XMMATRIX                g_View;
XMMATRIX                g_Projection;

int						g_viewWidth;
int						g_viewHeight;

DrawableGameObject		g_GameObject;


ID3D11RasterizerState* g_wireframeState;
ID3D11RasterizerState* g_solidState;

CameraManager g_CameraManager;
LightManager g_LightManager = LightManager(g_EyePosition);
TextureManager g_TextureManager;
GUIManager g_GUIManager;
Cube g_CubeTest;
//Camera g_CameraTest;

GraphicsCube g_GraphCubeTest;
Quad* g_QuadTest;
//Grid g_GridTest;
//GridTerrain g_TerrainTest;
GridTerrain g_TerrainHM;
GridTerrain g_TerrainFF;
GridTerrain g_TerrainDS;
GridTerrain g_TerrainCH;

//delete after creating effects ----------------------------
//ID3D11VertexShader* g_pVertexShaderPOM = nullptr;
//ID3D11PixelShader* g_pPixelShaderPOM = nullptr;
//ID3D11InputLayout* g_pVertexLayoutPOM = nullptr;
//ID3D11Buffer* g_pConstantBufferPOM = nullptr;
//ID3D11Buffer* g_pLightConstantBuffer2 = nullptr;
int g_texNum = 10;
ID3D11ShaderResourceView** g_pTextureRVs = new ID3D11ShaderResourceView * [g_texNum];



ID3D11Texture2D* g_renderToTexture = nullptr;
ID3D11RenderTargetView* g_customRenderTargetView = nullptr;
ID3D11ShaderResourceView* g_customShaderResourceView = nullptr;

ID3D11VertexShader* g_pVertexShaderQuad = nullptr;
ID3D11PixelShader* g_pPixelShaderQuad = nullptr;
ID3D11InputLayout* g_pVertexLayoutQuad = nullptr;

ID3D11Buffer* g_pConstantBufferQuad = nullptr;

//---------------------------------------------------------
SurfaceDetailFX* g_pSurfaceShader = new SurfaceDetailFX();

void InitCamera();

//HRESULT SetupPomShader();
//void RenderSurfaceDetailEffect();

HRESULT SetupCustomRenderTargets();
HRESULT SetupQuadShader();
void RenderQuadEffects();

void RenderRegularCube();
void SetupTerrain();


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

    //SetupImgui();
    g_GUIManager.Setup(g_hWnd, g_pd3dDevice, g_pImmediateContext, g_LightManager, g_CameraManager);
    
    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            Render();
        }
    }

    CleanupDevice();

    return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };

	g_viewWidth = 640;
	g_viewHeight = 480;

    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 5",
                           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
                           nullptr );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
//HRESULT CompileShaderFromFile( const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
//{
//    HRESULT hr = S_OK;
//
//    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
//#ifdef _DEBUG
//    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
//    // Setting this flag improves the shader debugging experience, but still allows 
//    // the shaders to be optimized and to run exactly the way they will run in 
//    // the release configuration of this program.
//    dwShaderFlags |= D3DCOMPILE_DEBUG;
//
//    // Disable optimizations to further improve shader debugging
//    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
//#endif
//
//    ID3DBlob* pErrorBlob = nullptr;
//    hr = D3DCompileFromFile( szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, 
//        dwShaderFlags, 0, ppBlobOut, &pErrorBlob );
//    if( FAILED(hr) )
//    {
//        if( pErrorBlob )
//        {
//            OutputDebugStringA( reinterpret_cast<const char*>( pErrorBlob->GetBufferPointer() ) );
//            pErrorBlob->Release();
//        }
//        return hr;
//    }
//    if( pErrorBlob ) pErrorBlob->Release();
//
//    return S_OK;
//}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

	D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDevice( nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );

        if ( hr == E_INVALIDARG )
        {
            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDevice( nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                                    D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
        }

        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = g_pd3dDevice->QueryInterface( __uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice) );
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter(&adapter);
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent( __uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory) );
                adapter->Release();
            }
            dxgiDevice->Release();
        }
    }
    if (FAILED(hr))
        return hr;

    // Create swap chain
    IDXGIFactory2* dxgiFactory2 = nullptr;
    hr = dxgiFactory->QueryInterface( __uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2) );
    if ( dxgiFactory2 )
    {
        // DirectX 11.1 or later
        hr = g_pd3dDevice->QueryInterface( __uuidof(ID3D11Device1), reinterpret_cast<void**>(&g_pd3dDevice1) );
        if (SUCCEEDED(hr))
        {
            (void) g_pImmediateContext->QueryInterface( __uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&g_pImmediateContext1) );
        }

        DXGI_SWAP_CHAIN_DESC1 sd = {};
        sd.Width = width;
        sd.Height = height;
		sd.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;//  DXGI_FORMAT_R16G16B16A16_FLOAT;////DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;

        hr = dxgiFactory2->CreateSwapChainForHwnd( g_pd3dDevice, g_hWnd, &sd, nullptr, nullptr, &g_pSwapChain1 );
        if (SUCCEEDED(hr))
        {
            hr = g_pSwapChain1->QueryInterface( __uuidof(IDXGISwapChain), reinterpret_cast<void**>(&g_pSwapChain) );
        }

        dxgiFactory2->Release();
    }
    else
    {
        // DirectX 11.0 systems
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = g_hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        hr = dxgiFactory->CreateSwapChain( g_pd3dDevice, &sd, &g_pSwapChain );
    }

    // Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
    dxgiFactory->MakeWindowAssociation( g_hWnd, DXGI_MWA_NO_ALT_ENTER );

    dxgiFactory->Release();

    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &pBackBuffer ) );
    if( FAILED( hr ) )
        return hr;

    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &g_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = g_TextureManager.TextureMaxNum();
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = g_pd3dDevice->CreateTexture2D( &descDepth, nullptr, &g_pDepthStencil );
    if( FAILED( hr ) )
        return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
    if( FAILED( hr ) )
        return hr;

    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );


    
    SetupCustomRenderTargets();




    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports( 1, &vp );

	hr = InitMesh();
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"Failed to initialise mesh.", L"Error", MB_OK);
		return hr;
	}

	hr = InitWorld(width, height);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"Failed to initialise world.", L"Error", MB_OK);
		return hr;
	}

	hr = g_GameObject.initMesh(g_pd3dDevice, g_pImmediateContext);
	if (FAILED(hr))
		return hr;

    hr = g_CubeTest.InitMesh(g_pd3dDevice, g_pImmediateContext);
    if (FAILED(hr))
        return hr;

    hr = g_GraphCubeTest.InitMesh(g_pd3dDevice, g_pImmediateContext);
    if (FAILED(hr))
        return hr;

    g_QuadTest = new Quad();
    hr = g_QuadTest->InitMesh(g_pd3dDevice, g_pImmediateContext);
    if (FAILED(hr))
        return hr;


    /*g_GridTest.Setup(5, 5);
    hr = g_GridTest.InitMesh(g_pd3dDevice, g_pImmediateContext, std::vector<float>{0});
    if (FAILED(hr))
        return hr;*/

    SetupTerrain();


    return S_OK;
}

// ***************************************************************************************
// InitMesh
// ***************************************************************************************
HRESULT		InitMesh()
{
	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	HRESULT hr = DX11::CompileShaderFromFile(L"shader.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Set the input layout
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = DX11::CompileShaderFromFile(L"shader.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;


	// Compile the SOLID pixel shader
	//pPSBlob = nullptr;
	//hr = CompileShaderFromFile(L"shader.fx", "PSSolid", "ps_4_0", &pPSBlob);
	//if (FAILED(hr))
	//{
	//	MessageBox(nullptr,
	//		L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
	//	return hr;
	//}

	//// Create the SOLID pixel shader
	//hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShaderSolid);
	//pPSBlob->Release();
	//if (FAILED(hr))
	//	return hr;



    
    g_pSurfaceShader->SetupShader(g_pd3dDevice);
    

    SetupQuadShader();



	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
    D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer);
	if (FAILED(hr))
		return hr;

	// Create the material constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MaterialPropertiesConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pMaterialConstantBuffer);
	if (FAILED(hr))
		return hr;

	// Create the light constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(LightPropertiesConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pLightConstantBuffer);
	if (FAILED(hr))
		return hr;

	// load and setup textures
	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\stone.dds", nullptr, &g_pTextureRV);
	if (FAILED(hr))
		return hr;

    hr = g_TextureManager.InitTextures(g_pd3dDevice);



    D3D11_RASTERIZER_DESC rd = {};
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_solidState);

    rd = {};
    rd.FillMode = D3D11_FILL_WIREFRAME;
    rd.CullMode = D3D11_CULL_NONE;
    hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_wireframeState);



	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);

	return hr;
}

// ***************************************************************************************
// InitWorld
// ***************************************************************************************
HRESULT		InitWorld(int width, int height)
{
	// Initialize the world matrix
	g_World1 = XMMatrixIdentity();

	// Initialize the view matrix
	XMVECTOR Eye = XMLoadFloat4(&g_EyePosition);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);

    //initialise camera
    InitCamera();

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
    if( g_pImmediateContext ) g_pImmediateContext->ClearState();


    delete g_QuadTest;
    g_QuadTest = nullptr;

    if( g_pConstantBuffer ) g_pConstantBuffer->Release();
    if( g_pVertexBuffer ) g_pVertexBuffer->Release();
    if( g_pIndexBuffer ) g_pIndexBuffer->Release();
    if( g_pVertexLayout ) g_pVertexLayout->Release();
    if( g_pVertexShader ) g_pVertexShader->Release();
    if( g_pPixelShader ) g_pPixelShader->Release();
    if( g_pDepthStencil ) g_pDepthStencil->Release();
    if( g_pDepthStencilView ) g_pDepthStencilView->Release();
    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    if( g_pSwapChain1 ) g_pSwapChain1->Release();
    if( g_pSwapChain ) g_pSwapChain->Release();
    if( g_pImmediateContext1 ) g_pImmediateContext1->Release();
    if( g_pImmediateContext ) g_pImmediateContext->Release();
    if( g_pd3dDevice1 ) g_pd3dDevice1->Release();
    if( g_pd3dDevice ) g_pd3dDevice->Release();


    /*if (g_pVertexLayoutPOM && g_pVertexLayoutPOM != nullptr) g_pVertexLayoutPOM->Release();
    if (g_pVertexShaderPOM && g_pVertexShaderPOM != nullptr) g_pVertexShaderPOM->Release();
    if (g_pPixelShaderPOM) g_pPixelShaderPOM->Release();
    if (g_pConstantBufferPOM) g_pConstantBufferPOM->Release();
    if (g_pLightConstantBuffer2) g_pLightConstantBuffer2->Release();*/



    if (g_solidState) g_solidState->Release();
    if (g_wireframeState) g_wireframeState->Release();

    if (g_pVertexLayoutQuad && g_pVertexLayoutQuad != nullptr) g_pVertexLayoutQuad->Release();
    if (g_pVertexShaderQuad && g_pVertexShaderQuad != nullptr) g_pVertexShaderQuad->Release();
    if (g_pPixelShaderQuad) g_pPixelShaderQuad->Release();
    if (g_pConstantBufferQuad) g_pConstantBufferQuad->Release();
    if (g_renderToTexture) g_renderToTexture->Release();
    if (g_customRenderTargetView) g_customRenderTargetView->Release();
    if (g_customShaderResourceView) g_customShaderResourceView->Release();


    if (g_pMaterialConstantBuffer) g_pMaterialConstantBuffer->Release();
    if (g_pLightConstantBuffer) g_pLightConstantBuffer->Release();
    if (g_pTextureRV) g_pTextureRV->Release();
    if (g_pNormalTextureRV) g_pNormalTextureRV->Release();
    if (g_pSamplerLinear) g_pSamplerLinear->Release();
    if (g_pSamplerNormal) g_pSamplerNormal->Release();
    

    if (g_pSurfaceShader != nullptr)
    {
        delete g_pSurfaceShader;
        g_pSurfaceShader = nullptr;
    }

    g_GUIManager.Shutdown();


    
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch( message )
    {
	case WM_LBUTTONDOWN:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		break;
	}
    case WM_PAINT:
        hdc = BeginPaint( hWnd, &ps );
        EndPaint( hWnd, &ps );
        break;

    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;

        // Note that this tutorial does not handle resizing (WM_SIZE) requests,
        // so we created the window without the resize border.

    default:
        return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
    // Update our time
    static float t = 0.0f;
    if( g_driverType == D3D_DRIVER_TYPE_REFERENCE )
    {
        t += ( float )XM_PI * 0.0125f;
    }
    else
    {
        static ULONGLONG timeStart = 0;
        ULONGLONG timeCur = GetTickCount64();
        if( timeStart == 0 )
            timeStart = timeCur;
        t = ( timeCur - timeStart ) / 1000.0f;
    }


    

    // Clear the back buffer
    //g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, Colors::MidnightBlue );

    // Clear the depth buffer to 1.0 (max depth)
   // g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

   


    //CAMERA STUFFFFFFFFFFFFF
    //g_CameraManager.SetCurrentCamera(CameraType::FIRST_PERSON);
    /*g_View = XMLoadFloat4x4(&g_CameraManager.GetCurrentCamera().GetView());
    g_Projection = XMLoadFloat4x4(&g_CameraManager.GetCurrentCamera().GetProjection());*/
    g_CameraManager.UpdateCamera();
    g_View = XMLoadFloat4x4(&g_CameraManager.GetCurrConstCamera()->GetView());
    g_Projection = XMLoadFloat4x4(&g_CameraManager.GetCurrConstCamera()->GetProjection());





    //tie this in with imgui dude
    g_LightManager.Update();


    //toggle global render state
    if (g_GUIManager.GetWireframe())
    {
        g_pImmediateContext->RSSetState(g_wireframeState);
    }
    else
    {
        g_pImmediateContext->RSSetState(g_solidState);
    }


	// Update variables for a cube
	g_GameObject.update(t);
    //g_CubeTest.SetRotation(0, 0.5f, 0);
    g_CubeTest.Update(0.0f);



    // Update variables for the cube
	XMMATRIX* mGO = g_GameObject.getTransform();
    mGO = g_CubeTest.GetWorld();

    ConstantBuffer cb1;
	cb1.mWorld = XMMatrixTranspose( *mGO);
	cb1.mView = XMMatrixTranspose( g_View );
	cb1.mProjection = XMMatrixTranspose( g_Projection );
	cb1.vOutputColor = XMFLOAT4(0, 0, 0, 0);
	g_pImmediateContext->UpdateSubresource( g_pConstantBuffer, 0, nullptr, &cb1, 0, 0 );

	MaterialPropertiesConstantBuffer redPlasticMaterial;
	redPlasticMaterial.Material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	redPlasticMaterial.Material.Specular = XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f);
	redPlasticMaterial.Material.SpecularPower = 32.0f;
	redPlasticMaterial.Material.UseTexture = true;
	g_pImmediateContext->UpdateSubresource(g_pMaterialConstantBuffer, 0, nullptr, &redPlasticMaterial, 0, 0);

	Light light;
	light.Enabled = static_cast<int>(g_LightManager.IsEnabled());
	light.LightType = PointLight;
	light.Color = XMFLOAT4(Colors::White);
	light.SpotAngle = XMConvertToRadians(45.0f);
	light.ConstantAttenuation = 1.0f;
	light.LinearAttenuation = 1;
	light.QuadraticAttenuation = 1;
	

	// set up the light
    XMFLOAT4 LightPosition(g_LightManager.LightVec());
    //XMFLOAT4 LightPosition(g_LightPosition);//g_EyePosition);
	light.Position = LightPosition;
	XMVECTOR LightDirection = XMVectorSet(-LightPosition.x, -LightPosition.y, -LightPosition.z, 0.0f);
	LightDirection = XMVector3Normalize(LightDirection);
	XMStoreFloat4(&light.Direction, LightDirection);

	LightPropertiesConstantBuffer lightProperties;
	lightProperties.EyePosition = LightPosition;
	lightProperties.Lights[0] = light;
	g_pImmediateContext->UpdateSubresource(g_pLightConstantBuffer, 0, nullptr, &lightProperties, 0, 0);



    //int chosenEffect = 2;
    //ConstantBufferPOM cbPOM;
    //mGO = g_GraphCubeTest.GetWorld();
    //cbPOM.mWorld = XMMatrixTranspose(*mGO);
    //cbPOM.mView = XMMatrixTranspose(g_View);
    //cbPOM.mProjection = XMMatrixTranspose(g_Projection);
    //cbPOM.fHeightScale = 0.15f;//0.05f;
    //cbPOM.nMinSamples = 8;
    //cbPOM.nMaxSamples = 32;
    //cbPOM.nEffectID = 0;
    //g_pImmediateContext->UpdateSubresource(g_pConstantBufferPOM, 0, nullptr, &cbPOM, 0, 0);

    //LightPropertiesConstantBuffer2 lightProperties2;
    //lightProperties2.EyePosition = LightPosition;
    //lightProperties2.CameraPosition = g_CameraManager.GetCurrConstCamera()->Position();
    //lightProperties2.Lights[0] = light;
    //g_pImmediateContext->UpdateSubresource(g_pLightConstantBuffer2, 0, nullptr, &lightProperties2, 0, 0);



    //compute inverse WVP. world, view and proj have been already transposed
    XMMATRIX inv = cb1.mWorld;
    inv = XMMatrixMultiply(cb1.mWorld, cb1.mView);
    inv = XMMatrixMultiply(inv, cb1.mProjection);
    XMVECTOR det = XMMatrixDeterminant(inv);
    inv = XMMatrixInverse(&det, inv);

    //compute inverse projection matrix
    XMMATRIX invProj = cb1.mProjection;
    XMVECTOR detProj = XMMatrixDeterminant(invProj);
    invProj = XMMatrixInverse(&detProj, invProj);

    ConstantBufferQuad cbQuad;
    cbQuad.mInvWVP = inv;
    cbQuad.mInvProj = invProj;
    cbQuad.vScreenSize = XMFLOAT2((float)g_viewWidth, (float)g_viewHeight);
    cbQuad.nBlur = 1;
    cbQuad.nEffectID = 0;
    //cbQuad.fDepth = 0.0f;
    //cbQuad.nPassIndex = 0;
    //cbQuad.mPrevVP = XMMatrixMultiply(cb1.mView, cb1.mProjection); 
    g_pImmediateContext->UpdateSubresource(g_pConstantBufferQuad, 0, nullptr, &cbQuad, 0, 0);



    bool doSurfaceDetail = false;
    int effect = 0;

    switch (g_GUIManager.GetScene())
    {
    case Scene::PHYISCS:
        RenderRegularCube();
        break;
    case Scene::NORMAL:
        /*cbPOM.nEffectID = 0;
        g_pImmediateContext->UpdateSubresource(g_pConstantBufferPOM, 0, nullptr, &cbPOM, 0, 0);
        RenderSurfaceDetailEffect();*/
        doSurfaceDetail = true;
        effect = 0;
        break;
    case Scene::PARALLAX:
        doSurfaceDetail = true;
        effect = 1;
        break;
    case Scene::POM:
        doSurfaceDetail = true;
        effect = 2;
        break;
    case Scene::SELFSHADOWING:
        doSurfaceDetail = true;
        effect = 4;
        break;
    case Scene::GRAYSCALE:
        RenderQuadEffects();
        break;
    case Scene::BOXBLUR:
        cbQuad.nEffectID = 1;
        g_pImmediateContext->UpdateSubresource(g_pConstantBufferQuad, 0, nullptr, &cbQuad, 0, 0);
        RenderQuadEffects();
        break;
    case Scene::GAUSSIAN:
        cbQuad.nEffectID = 2;
        g_pImmediateContext->UpdateSubresource(g_pConstantBufferQuad, 0, nullptr, &cbQuad, 0, 0);
        RenderQuadEffects();
        break;
    case Scene::DOFBLUR:
        RenderQuadEffects();
        break;
    case Scene::HEIGHTMAP:
        redPlasticMaterial.Material.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
        g_pImmediateContext->UpdateSubresource(g_pMaterialConstantBuffer, 0, nullptr, &redPlasticMaterial, 0, 0);
        RenderRegularCube();

        g_TerrainHM.GetTerrainGrid()->SetVertexBuffer(g_pImmediateContext);
        g_TerrainHM.GetTerrainGrid()->SetIndexBuffer(g_pImmediateContext);

        mGO = g_TerrainHM.GetTerrainGrid()->GetWorld();
        cb1.mWorld = XMMatrixTranspose(*mGO);
        g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb1, 0, 0);

        g_TerrainHM.Draw(g_pImmediateContext);
        break;
    case Scene::FAULTFORM:
        redPlasticMaterial.Material.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
        g_pImmediateContext->UpdateSubresource(g_pMaterialConstantBuffer, 0, nullptr, &redPlasticMaterial, 0, 0);
        RenderRegularCube();

        g_TerrainFF.GetTerrainGrid()->SetVertexBuffer(g_pImmediateContext);
        g_TerrainFF.GetTerrainGrid()->SetIndexBuffer(g_pImmediateContext);

        mGO = g_TerrainFF.GetTerrainGrid()->GetWorld();
        cb1.mWorld = XMMatrixTranspose(*mGO);
        g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb1, 0, 0);

        g_TerrainFF.Draw(g_pImmediateContext);
        break;
    case Scene::DIAMONDSQUARE:
        redPlasticMaterial.Material.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
        g_pImmediateContext->UpdateSubresource(g_pMaterialConstantBuffer, 0, nullptr, &redPlasticMaterial, 0, 0);
        RenderRegularCube();

        g_TerrainDS.GetTerrainGrid()->SetVertexBuffer(g_pImmediateContext);
        g_TerrainDS.GetTerrainGrid()->SetIndexBuffer(g_pImmediateContext);

        mGO = g_TerrainDS.GetTerrainGrid()->GetWorld();
        cb1.mWorld = XMMatrixTranspose(*mGO);
        g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb1, 0, 0);

        g_TerrainDS.Draw(g_pImmediateContext);
        break;
    case Scene::CIRCLEHILL:
        redPlasticMaterial.Material.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
        g_pImmediateContext->UpdateSubresource(g_pMaterialConstantBuffer, 0, nullptr, &redPlasticMaterial, 0, 0);
        RenderRegularCube();

        g_TerrainCH.GetTerrainGrid()->SetVertexBuffer(g_pImmediateContext);
        g_TerrainCH.GetTerrainGrid()->SetIndexBuffer(g_pImmediateContext);

        mGO = g_TerrainCH.GetTerrainGrid()->GetWorld();
        cb1.mWorld = XMMatrixTranspose(*mGO);
        g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb1, 0, 0);

        g_TerrainCH.Draw(g_pImmediateContext);
        break;
    case Scene::VOXEL:
        redPlasticMaterial.Material.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
        g_pImmediateContext->UpdateSubresource(g_pMaterialConstantBuffer, 0, nullptr, &redPlasticMaterial, 0, 0); 
        RenderRegularCube();
        break;
    }



    if (doSurfaceDetail)
    {
        g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
        g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, Colors::MidnightBlue);
        g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

        g_GraphCubeTest.SetVertexBuffer(g_pImmediateContext);
        g_GraphCubeTest.SetIndexBuffer(g_pImmediateContext);

        mGO = g_GraphCubeTest.GetWorld();
        g_pSurfaceShader->SetConstantBuffers(g_pImmediateContext, mGO, &g_View, &g_Projection, LightPosition,
            g_CameraManager.GetCurrConstCamera()->Position(), light, effect);

        g_pSurfaceShader->Render(g_pImmediateContext);

        g_GraphCubeTest.Draw(g_pImmediateContext);
    }


    //grid render test
   /* g_GridTest.SetVertexBuffer(g_pImmediateContext);
    g_GridTest.SetIndexBuffer(g_pImmediateContext);

    g_GridTest.SetTranslation(0.0f, -3.0f, 0.0f);
    g_GridTest.Update(0.0f);
    mGO = g_GridTest.GetWorld();
    cb1.mWorld = XMMatrixTranspose(*mGO);
    g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb1, 0, 0);

    g_GridTest.Draw(g_pImmediateContext);*/


    //terrain render test
    /*g_TerrainHM.GetTerrainGrid()->SetVertexBuffer(g_pImmediateContext);
    g_TerrainHM.GetTerrainGrid()->SetIndexBuffer(g_pImmediateContext);

    mGO = g_TerrainHM.GetTerrainGrid()->GetWorld();
    cb1.mWorld = XMMatrixTranspose(*mGO);
    g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb1, 0, 0);

    g_TerrainHM.Draw(g_pImmediateContext);*/






    g_GUIManager.Render();


    // Present our back buffer to our front buffer
    g_pSwapChain->Present( 0, 0 );
}


void InitCamera()
{
    g_CameraManager.InitCameras(g_viewWidth, g_viewHeight);
    g_CameraManager.SetCurrentCamera(CameraType::FRONT);

}
/*
HRESULT SetupPomShader()
{
    // Compile the vertex shader
    ID3DBlob* pVSBlob2 = nullptr;
    HRESULT hr = DX11::CompileShaderFromFile(L"d_POMShader.fx", "VS", "vs_4_0", &pVSBlob2);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The POM Vertex Shader FX file cannot be compiled.", L"Error", MB_OK);
        return hr;
    }

    // Create the vertex shader
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob2->GetBufferPointer(), pVSBlob2->GetBufferSize(), nullptr, &g_pVertexShaderPOM);
    if (FAILED(hr))
    {
        pVSBlob2->Release();
        return hr;
    }

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
    hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob2->GetBufferPointer(),
        pVSBlob2->GetBufferSize(), &g_pVertexLayoutPOM);
    pVSBlob2->Release();
    if (FAILED(hr))
        return hr;

    // Set the input layout
    //g_pImmediateContext->IASetInputLayout(g_pVertexLayoutPOM);

    // Compile the pixel shader
    ID3DBlob* pPSBlob2 = nullptr;
    hr = DX11::CompileShaderFromFile(L"d_POMShader.fx", "PS", "ps_4_0", &pPSBlob2);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The POM Pixel Shader FX file cannot be compiled.", L"Error", MB_OK);
        return hr;
    }

    // Create the pixel shader
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob2->GetBufferPointer(), pPSBlob2->GetBufferSize(), nullptr, &g_pPixelShaderPOM);
    pPSBlob2->Release();
    if (FAILED(hr))
        return hr;


    // Create the constant buffer
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBufferPOM);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pConstantBufferPOM);
    if (FAILED(hr))
        return hr;


    // Create the light constant buffer
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(LightPropertiesConstantBuffer2);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pLightConstantBuffer2);
    if (FAILED(hr))
        return hr;

    int texCount = 0;
    const wchar_t* texFilePaths[] =
    {
        //L"Resources\\stone.dds", //0
        ////L"Resources\\conenormal.dds",
        //L"Resources\\stone_texture.dds", //1
        //L"Resources\\stone_normalMap.dds",
        //L"Resources\\stone_heightMap.dds",
        L"Resources\\bricks_TEX.dds", //4
        L"Resources\\bricks_NORM.dds",
        L"Resources\\bricks_DISP.dds",
        //L"Resources\\toybox_TEX.dds", //7
        //L"Resources\\toybox_NORM.dds",
        //L"Resources\\toybox_DISP.dds"
    };
    g_texNum = 3;
    for (int i = 0; i < g_texNum; ++i)
    {
        hr = CreateDDSTextureFromFile(g_pd3dDevice, texFilePaths[i], nullptr, &g_pTextureRVs[texCount]);//&g_pTextureRV);
        if (FAILED(hr))
            return hr;
        ++texCount;
    }



    return hr;
}


void RenderSurfaceDetailEffect()
{
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

    // Clear the back buffer
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, Colors::MidnightBlue);

    // Clear the depth buffer to 1.0 (max depth)
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


    //g_pImmediateContext->UpdateSubresource(g_pConstantBufferPOM, 0, nullptr, &cbPOM, 0, 0);

    g_GraphCubeTest.SetVertexBuffer(g_pImmediateContext);
    g_GraphCubeTest.SetIndexBuffer(g_pImmediateContext);

    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBufferPOM);
    g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBufferPOM);
    g_pImmediateContext->PSSetConstantBuffers(1, 1, &g_pMaterialConstantBuffer);
    g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pLightConstantBuffer2);

    g_pImmediateContext->IASetInputLayout(g_pVertexLayoutPOM);
    g_pImmediateContext->VSSetShader(g_pVertexShaderPOM, nullptr, 0);
    g_pImmediateContext->PSSetShader(g_pPixelShaderPOM, nullptr, 0);
    g_pImmediateContext->PSSetShaderResources(0, 3, g_pTextureRVs);
    g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

    g_GraphCubeTest.Draw(g_pImmediateContext);
}*/

void RenderRegularCube()
{
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
    // Clear the back buffer
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, Colors::MidnightBlue);

    // Clear the depth buffer to 1.0 (max depth)
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


    g_CubeTest.SetVertexBuffer(g_pImmediateContext);
    g_CubeTest.SetIndexBuffer(g_pImmediateContext);

    //set constant buffers
    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->PSSetConstantBuffers(1, 1, &g_pMaterialConstantBuffer);
    g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pLightConstantBuffer);

    //set effect's vertex and pixel shaders
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
    g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
    g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);

    //set textures and sampler
    ID3D11ShaderResourceView* tempsrv = (g_TextureManager.TexturesAt(TextureGroup::STONE));
    g_pImmediateContext->PSSetShaderResources(0, 1, &tempsrv);
    g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

    g_CubeTest.Draw(g_pImmediateContext);


}



HRESULT SetupCustomRenderTargets()
{
    HRESULT hr;

    //Create render to target
    D3D11_TEXTURE2D_DESC renderToTexDesc = {};
    ZeroMemory(&renderToTexDesc, sizeof(renderToTexDesc));
    renderToTexDesc.Width = 640; //g_viewWidth;
    renderToTexDesc.Height = 480;//g_viewHeight;
    renderToTexDesc.MipLevels = 1;
    renderToTexDesc.ArraySize = 1;
    renderToTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    renderToTexDesc.SampleDesc.Count = 1;
    renderToTexDesc.SampleDesc.Quality = 0;
    renderToTexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    renderToTexDesc.Usage = D3D11_USAGE_DEFAULT;
    renderToTexDesc.CPUAccessFlags = 0;
    renderToTexDesc.MiscFlags = 0;
    hr = g_pd3dDevice->CreateTexture2D(&renderToTexDesc, nullptr, &g_renderToTexture);
    if (FAILED(hr))
        return hr;

    D3D11_RENDER_TARGET_VIEW_DESC customRtvDesc;
    customRtvDesc.Format = renderToTexDesc.Format;
    customRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    customRtvDesc.Texture2D.MipSlice = 0;
    hr = g_pd3dDevice->CreateRenderTargetView(g_renderToTexture, &customRtvDesc, &g_customRenderTargetView);
    if (FAILED(hr))
        return hr;

    D3D11_SHADER_RESOURCE_VIEW_DESC customSrvDesc;
    customSrvDesc.Format = renderToTexDesc.Format;
    customSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    customSrvDesc.Texture2D.MostDetailedMip = 0;
    customSrvDesc.Texture2D.MipLevels = 1;
    hr = g_pd3dDevice->CreateShaderResourceView(g_renderToTexture, &customSrvDesc, &g_customShaderResourceView);
    if (FAILED(hr))
        return hr;

    g_pImmediateContext->OMSetRenderTargets(1, &g_customRenderTargetView, g_pDepthStencilView);

    return hr;
}

HRESULT SetupQuadShader()
{
    HRESULT hr; 

    //create quad VS shader
    ID3DBlob* pVSBlob = nullptr;
    hr = DX11::CompileShaderFromFile(L"d_GaussianBlur.fx", "VS", "vs_4_0", &pVSBlob);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The d_quadShader.fx file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShaderQuad);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return hr;
    }

    D3D11_INPUT_ELEMENT_DESC layout2[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        //{ "DEPTH", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    UINT numElements2 = ARRAYSIZE(layout2);

    hr = g_pd3dDevice->CreateInputLayout(layout2, numElements2, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &g_pVertexLayoutQuad);
    if (FAILED(hr))
        return hr;


    //set up quad pixel shader
    ID3DBlob* pPSBlob = nullptr;
    hr = DX11::CompileShaderFromFile(L"d_GaussianBlur.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(nullptr, L"The d_quadShader.fx file cannot be compiled. Pixel shader failed.", L"Error", MB_OK);
        return hr;
    }

    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShaderQuad);
    pPSBlob->Release();
    if (FAILED(hr))
        return hr;

    //set constant buffers
    D3D11_BUFFER_DESC bd = {};
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBufferQuad);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pConstantBufferQuad);
    if (FAILED(hr))
        hr;

    return hr;
}

void RenderQuadEffects()
{
    /*ID3D11RenderTargetView* nullRTV = nullptr;
    g_pImmediateContext->OMSetRenderTargets(1, &nullRTV, nullptr);*/


    g_pImmediateContext->OMSetRenderTargets(1, &g_customRenderTargetView, g_pDepthStencilView);

    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, Colors::Coral);
    g_pImmediateContext->ClearRenderTargetView(g_customRenderTargetView, Colors::SeaGreen);
    //g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    //g_pImmediateContext->ClearDepthStencilView(g_depthBlurDsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    
    g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);


    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    g_CubeTest.SetVertexBuffer(g_pImmediateContext);
    g_CubeTest.SetIndexBuffer(g_pImmediateContext);

    g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
    g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);

    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->PSSetConstantBuffers(1, 1, &g_pMaterialConstantBuffer);
    g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pLightConstantBuffer);
    

    //for cube
    ID3D11ShaderResourceView* tempsrv = (g_TextureManager.TexturesAt(TextureGroup::STONE));
    g_pImmediateContext->PSSetShaderResources(0, 1, &tempsrv);
    //if(tempsrv) tempsrv->Release();
    //for graphics cube
    //g_pImmediateContext->PSSetShaderResources(0, 3, g_pTextureRVs);


    

    g_CubeTest.Draw(g_pImmediateContext);

    

    //RTT------------------
    g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);//nullptr);

    


    //bind shader
    g_pImmediateContext->IASetInputLayout(g_pVertexLayoutQuad);
    g_pImmediateContext->VSSetShader(g_pVertexShaderQuad, nullptr, 0);
    g_pImmediateContext->PSSetShader(g_pPixelShaderQuad, nullptr, 0);

    g_QuadTest->SetVertexBuffer(g_pImmediateContext);
    g_QuadTest->SetIndexBuffer(g_pImmediateContext);

    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBufferQuad);
    g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBufferQuad);

    g_pd3dDevice->CreateShaderResourceView(g_renderToTexture, nullptr, &g_customShaderResourceView);
    g_pImmediateContext->PSSetShaderResources(0, 1, &g_customShaderResourceView);

    g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
    
    g_QuadTest->Draw(g_pImmediateContext);

    ID3D11ShaderResourceView* const pSRV[1] = { NULL };
    g_pImmediateContext->PSSetShaderResources(0, 1, pSRV);
}

void SetupTerrain()
{
    //FROM HEIGHTMAP
    g_TerrainHM.SetHeightmap(513, 513, "Resources\\coastMountain513.raw");
    g_TerrainHM.GetHmGen().LoadHeightmap(g_TerrainHM.GetHeightValues(),
        g_TerrainHM.GetHMStruct());
    g_TerrainHM.SetGridRatio(0.3f);
    g_TerrainHM.SetupTerrain(g_pd3dDevice, g_pImmediateContext, 40.0f, false, true, 1);

    g_TerrainHM.GetTerrainGrid()->SetTranslation(0.0f, -g_TerrainHM.GetHMScale() / 2.0f, 0.0f);
    g_TerrainHM.Update(0.0f);


    //FAULT FORMATION
    g_TerrainFF.SetHeightmap(154, 154, "");
    g_TerrainFF.GetHmGen().FaultFormation(g_TerrainFF.GetHeightValues(), 
        g_TerrainFF.GetHMStruct().width, 200, true);
    g_TerrainFF.SetGridRatio(1.0f);
    g_TerrainFF.SetupTerrain(g_pd3dDevice, g_pImmediateContext, 40.0f);

    g_TerrainFF.GetTerrainGrid()->SetTranslation(0.0f, -g_TerrainFF.GetHMScale() * 0.75f, 0.0f);
    g_TerrainFF.Update(0.0f);


    //DIAMOND SQUARE
    int texpow = 7;
    int texDim = (int)std::pow(2, 7) + 1;
    g_TerrainDS.SetHeightmap(texDim, texDim, "");
    g_TerrainDS.GetHmGen().DiamondSquare(g_TerrainDS.GetHeightValues(), texpow, g_TerrainDS.GetHMStruct().width);
    g_TerrainDS.SetGridRatio(1.0f);
    g_TerrainDS.SetupTerrain(g_pd3dDevice, g_pImmediateContext, 40.0f, false, true, 4);

    g_TerrainDS.GetTerrainGrid()->SetTranslation(0.0f, -g_TerrainDS.GetHMScale() * 0.5f, 0.0f);
    g_TerrainDS.Update(0.0f);


    //CIRCLE HILL
    g_TerrainCH.SetHeightmap(154, 154, "");
    g_TerrainCH.GetHmGen().CircleHill(g_TerrainCH.GetHeightValues(), g_TerrainCH.GetHMStruct().width,
        g_TerrainCH.GetHMStruct().height, 200, 2.0f, 20.0f);
    g_TerrainCH.SetGridRatio(1.0f);
    g_TerrainCH.SetupTerrain(g_pd3dDevice, g_pImmediateContext, 20.0f);

    g_TerrainCH.GetTerrainGrid()->SetTranslation(0.0f, -g_TerrainCH.GetHMScale() * 0.5f, 0.0f);
    g_TerrainCH.Update(0.0f);

}