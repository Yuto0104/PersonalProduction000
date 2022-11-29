//=============================================================================
//
// 軌跡クラス(orbit.cpp)
// Author : 唐﨑結斗
// 概要 : 軌跡生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "orbit.h"
#include "calculation.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 軌跡を生成する
//=============================================================================
COrbit * COrbit::Create(void)
{
	// 軌跡インスタンス
	COrbit *pOrbit = nullptr;

	// メモリの解放
	pOrbit = new COrbit;

	// メモリの確保ができなかった
	assert(pOrbit != nullptr);

	// 数値の初期化
	pOrbit->Init();

	// インスタンスを返す
	return pOrbit;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
COrbit::COrbit()
{
	m_pParentMtx = nullptr;							// 親のマトリックス
	m_offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置の差分
	m_EBlendMode = BLEND_MODE::MODE_NORMAL;			// αブレンドの方法
	m_nDivision = 0;								// 分割数
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
COrbit::~COrbit()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT COrbit::Init()
{
	// 初期化
	CMesh3D::Init();

	// テクスチャの設定
	LoadTex(-1);

	// 位置の差分
	m_offset = D3DXVECTOR3(0.0f, 100.0f, 0.0f);		

	// 分割数
	m_nDivision = 1;				

	// ブロック数の設定
	SetBlock(CMesh3D::DOUBLE_INT(m_nDivision, 1));						

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void COrbit::Uninit()
{// 終了
	CMesh3D::Uninit();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void COrbit::Update()
{// 更新
	CMesh3D::Update();

	// 頂点座標の算出
	SetVtx();
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void COrbit::Draw()
{// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ライトを無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングはしない
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	switch (m_EBlendMode)
	{
	case BLEND_MODE::MODE_NORMAL:
		break;

	case BLEND_MODE::MODE_ADD:
		// αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

	case BLEND_MODE::MODE_SUB:
		// αブレンディングを減算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

	default:
		break;
	}

	// 描画
	CMesh3D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// カリング設定を元に戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ライトを有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 頂点座標などの設定
// Author : 唐﨑結斗
// 概要 : 3D頂点座標、nor、頂点カラーを設定する
//=============================================================================
void COrbit::SetVtx()
{
	// 情報の取得
	DOUBLE_INT block = GetBlock();
	DOUBLE_INT line = GetLine();
	D3DXVECTOR3 offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR col = GetCol();
	int nMaxVtx = GetVtx();
	int nDivision = GetVtx() / 2;

	// 頂点情報の取得
	VERTEX_3D *pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_pParentMtx != nullptr)
	{
		for (int nCntDivision = nDivision - 1; nCntDivision >= 0; nCntDivision--)
		{
			if (nCntDivision <= 0)
			{// 親マトリックスから現在位置を算出
				D3DXVec3TransformCoord(&pVtx[nCntDivision].pos, &offset, m_pParentMtx);
				D3DXVec3TransformCoord(&pVtx[nDivision + nCntDivision].pos, &m_offset, m_pParentMtx);
			}
			else
			{// ひとつ前の頂点位置を代入
				pVtx[nCntDivision].pos = pVtx[nCntDivision - 1].pos;
				pVtx[nDivision + nCntDivision].pos = pVtx[nDivision + nCntDivision - 1].pos;
			}
		}
	}
	else
	{
		for (int nCntVtx = 0; nCntVtx < nMaxVtx; nCntVtx++)
		{// 初期化
			pVtx[nCntVtx].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
void COrbit::SetTex()
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
// 分割数の設定
// Author : 唐﨑結斗
// 概要 : 3Dメッシュの分割数を設定する
//=============================================================================
void COrbit::SetDivision(const int nDivision)
{
	// 分割数の設定
	m_nDivision = nDivision;

	// ブロック数の設定
	SetBlock(CMesh3D::DOUBLE_INT(m_nDivision, 1));
}
