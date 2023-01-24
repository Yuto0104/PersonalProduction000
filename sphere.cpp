//=============================================================================
//
// スフィアクラス(sphere.cpp)
// Author : 唐﨑結斗
// 概要 : スフィア生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "sphere.h"
#include "calculation.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : スフィアを生成する
//=============================================================================
CSphere * CSphere::Create(void)
{
	// スフィアインスタンス
	CSphere *pSphere = nullptr;

	// メモリの解放
	pSphere = new CSphere;

	// メモリの確保ができなかった
	assert(pSphere != nullptr);

	// 数値の初期化
	pSphere->Init();

	// インスタンスを返す
	return pSphere;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CSphere::CSphere()
{
	m_sphereRange = D3DXVECTOR2(0.0f,0.0f);
	m_fRadius = 0.0f;
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CSphere::~CSphere()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CSphere::Init()
{
	// 初期化
	CMesh3D::Init();

	// 初期値の設定
	m_sphereRange = D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * 0.5f);
	m_fRadius = 50.0f;

	// テクスチャの設定
	LoadTex(-1);

	// オブジェクトタイプの設定
	SetObjType(CObject::OBJETYPE_MESH);

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CSphere::Uninit()
{// 終了
	CMesh3D::Uninit();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CSphere::Update()
{// 更新
	CMesh3D::Update();
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void CSphere::Draw()
{// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ライトを無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 描画
	CMesh3D::Draw();

	// ライトを有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 頂点座標などの設定
// Author : 唐﨑結斗
// 概要 : 3D頂点座標、nor、頂点カラーを設定する
//=============================================================================
void CSphere::SetVtx()
{
	// 情報の取得
	DOUBLE_INT block = GetBlock();
	DOUBLE_INT line = GetLine();

	// 頂点情報の取得
	VERTEX_3D *pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < line.y; nCntZ++)
	{
		for (int nCntX = 0; nCntX < line.x; nCntX++)
		{// 変数宣言
			int nCntVtx = nCntX + (nCntZ *  line.x);
			float fRot = (m_sphereRange.x / block.x) * nCntX;					// Y軸の角度の設定
			float fHalfRot = (m_sphereRange.y / block.y) * nCntZ;				// 半球のZ軸の角度の半分

			// 高さと半径の設定
			D3DXVECTOR2 radius = D3DXVECTOR2(sinf(fHalfRot) * m_fRadius, cosf(fHalfRot) * m_fRadius);

			// 頂点座標の設定
			pVtx[nCntVtx].pos.x = sinf(fRot) * radius.y;
			pVtx[nCntVtx].pos.z = cosf(fRot) * radius.y;
			pVtx[nCntVtx].pos.y = radius.x;

			/*pVtx[nCntVtx].pos.z = sinf(fHalfRot) * cosf(fRot) * m_fRadius;
			pVtx[nCntVtx].pos.x = sinf(fHalfRot) * sinf(fRot) * m_fRadius;
			pVtx[nCntVtx].pos.y = cosf(fHalfRot) * m_fRadius;*/

			// ワールド座標にキャスト
			CCalculation::WorldCastVtx(pVtx[nCntVtx].pos, GetPos(), GetRot());

			// 各頂点の法線の設定(*ベクトルの大きさは1にする必要がある)
			pVtx[nCntVtx].nor.x = pVtx[nCntVtx].pos.x;
			pVtx[nCntVtx].nor.z = pVtx[nCntVtx].pos.z;
			pVtx[nCntVtx].nor.y = 0.0f;
			D3DXVec3Normalize(&pVtx[nCntVtx].nor, &pVtx[nCntVtx].nor);

			// 頂点カラーの設定
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャ座標の設定
// Author : 唐﨑結斗
// 概要 : 3Dメッシュのテクスチャ座標を設定する
//=============================================================================
void CSphere::SetTex()
{
	// 情報の取得
	DOUBLE_INT block = GetBlock();
	DOUBLE_INT line = GetLine();
	D3DXVECTOR2 tex = GetTex();

	// 頂点情報の取得
	VERTEX_3D *pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < line.y; nCntZ++)
	{
		for (int nCntX = 0; nCntX < line.x; nCntX++)
		{// 変数宣言
			int nCntVtx = nCntX + (nCntZ *  line.x);

			if (GetSplitTex())
			{// テクスチャ座標の設定
				pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f * nCntX + tex.x, 1.0f * (line.y - nCntZ) + tex.y);
			}
			else
			{// テクスチャ座標の設定
				pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f / block.x * nCntX + tex.x, 1.0f / block.y * (line.y - nCntZ) + tex.y);
			}
		}
	}

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 球の半径の設定
// Author : 唐﨑結斗
// 概要 : 球の半径を設定する
//=============================================================================
void CSphere::SetRadius(float fRadius)
{
	m_fRadius = fRadius;

	// 頂点座標の設定
	SetVtx();
}

//=============================================================================
// 球の描画範囲の設定
// Author : 唐﨑結斗
// 概要 : 球の描画範囲を設定する
//=============================================================================
void CSphere::SetSphereRange(D3DXVECTOR2 sphereRange)
{
	m_sphereRange = sphereRange;

	// 頂点座標の設定
	SetVtx();
}
