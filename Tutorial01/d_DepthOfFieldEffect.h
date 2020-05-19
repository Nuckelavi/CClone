#pragma once

//backup for motion blur
//----------------------------------------------------------
//THOU SHALLT BE NUKED 
//----------------------------------------------------------

    //compute inverse WVP. world, view and proj have been already transposed
    /*XMMATRIX inv = cb1.mWorld;
    inv = XMMatrixMultiply(cb1.mWorld, cb1.mView);
    inv = XMMatrixMultiply(inv, cb1.mProjection);
    XMVECTOR det = XMMatrixDeterminant(inv);
    inv = XMMatrixInverse(&det, inv);*/

    //compute inverse projection matrix
    /*XMMATRIX invProj = cb1.mProjection;
    XMVECTOR detProj = XMMatrixDeterminant(invProj);
    invProj = XMMatrixInverse(&detProj, invProj);*/
    /*
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
    */
//----------------------------------------------------------