#include "d_DepthOfFieldEffect.h"


//OLD RTT BACKUP

/*
ConstantBufferQuad cbQuad;
cbQuad.vScreenSize = XMFLOAT2((float)g_viewWidth, (float)g_viewHeight);
cbQuad.nBlur = 1;
cbQuad.nEffectID = 0;
g_pImmediateContext->UpdateSubresource(g_pConstantBufferQuad, 0, nullptr, &cbQuad, 0, 0);
*/

/*
case Scene::GAUSSIAN:
    cbQuad.nEffectID = 2;
    g_pImmediateContext->UpdateSubresource(g_pConstantBufferQuad, 0, nullptr, &cbQuad, 0, 0);
    RenderQuadEffects();
    break;*/

    //----------------------------------------------------------
    //THOU SHALLT BE NUKED 
    //----------------------------------------------------------
    /*

    void RenderQuadEffects()
    {
        //ID3D11RenderTargetView* nullRTV = nullptr;
        //g_pImmediateContext->OMSetRenderTargets(1, &nullRTV, nullptr);


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
    */
    //----------------------------------------------------------

    /*
    g_pImmediateContext->OMSetRenderTargets(1, &g_customRenderTargetView, g_pDepthStencilView);

    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, Colors::MidnightBlue);
    g_pImmediateContext->ClearRenderTargetView(g_customRenderTargetView, Colors::SeaGreen);
    g_pImmediateContext->ClearRenderTargetView(g_customRenderTargetView2, Colors::SeaGreen);
    g_pImmediateContext->ClearRenderTargetView(g_customRenderTargetView3, Colors::SeaGreen);
    g_pImmediateContext->ClearRenderTargetView(g_customRenderTargetView4, Colors::SeaGreen);
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    g_pImmediateContext->ClearDepthStencilView(g_depthBlurDsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

    g_View = XMLoadFloat4x4(&currentCam.GetView());
    g_Projection = XMLoadFloat4x4(&currentCam.GetProjection());

    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    UINT cubeStride = sizeof(SimpleVertex);
    UINT offset = 0;
    g_pImmediateContext->IASetVertexBuffers(0, 1, g_GameObject.getPpVertexBuffer(), &cubeStride, &offset);

    g_pImmediateContext->IASetIndexBuffer(g_GameObject.getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

    g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
    g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->PSSetConstantBuffers(1, 1, &g_pMaterialConstantBuffer);
    g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pLightConstantBuffer);

    g_pImmediateContext->PSSetShaderResources(0, g_texNum, &*g_pTextureRVs);
    g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

    g_pImmediateContext->DrawIndexed(36, 0, 0);
    */
