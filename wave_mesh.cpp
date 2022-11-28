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

#include "wave_mesh.h"
#include "calculation.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : スフィアを生成する
//=============================================================================
CWaveMesh * CWaveMesh::Create(void)
{
	// スフィアインスタンス
	CWaveMesh *pWaveMesh = nullptr;

	// メモリの解放
	pWaveMesh = new CWaveMesh;

	// メモリの確保ができなかった
	assert(pWaveMesh != nullptr);

	// 数値の初期化
	pWaveMesh->Init();

	// インスタンスを返す
	return pWaveMesh;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CWaveMesh::CWaveMesh()
{

}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CWaveMesh::~CWaveMesh()
{

}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CWaveMesh::Update()
{// 波
	Wave();

	// 更新
	CMesh3D::Update();
}

//=============================================================================
// 波
// Author : 唐﨑結斗
// 概要 : 波の動きをする
//=============================================================================
void CWaveMesh::Wave()
{
	// 情報の取得
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

			m_fVtxHeight[nCntVtx] += m_fAddWave;

			// 頂点座標の設定
			pVtx[nCntVtx].pos.y = sinf(m_fVtxHeight[nCntVtx]) * m_fAmplitude;
		}
	}

	Normal();

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点座標などの設定
// Author : 唐﨑結斗
// 概要 : 3D頂点座標、rhw、頂点カラーを設定する
//=============================================================================
void CWaveMesh::SetVtx()
{
	// 情報の取得
	DOUBLE_INT line = GetLine();
	D3DXVECTOR3 size = GetSize();
	D3DXVECTOR3 blockSize = GetBlockSize();

	// 頂点情報の取得
	VERTEX_3D *pVtx = NULL;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < line.y; nCntZ++)
	{
		for (int nCntX = 0; nCntX < line.x; nCntX++)
		{// 変数宣言
			int nCntVtx = nCntX + (nCntZ * line.x);

			// 頂点座標の設定
			pVtx[nCntVtx].pos.x = (nCntVtx % line.x * blockSize.x) - size.x / 2.0f;
			pVtx[nCntVtx].pos.y = sinf((m_fVtxHeight[nCntVtx]) * D3DX_PI / 180) * 50.0f;
			pVtx[nCntVtx].pos.z = (nCntVtx / line.x * -blockSize.z) + size.z / 2.0f;
		}
	}

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 波の設定
// Author : 唐﨑結斗
// 概要 : 波の設定
//=============================================================================
void CWaveMesh::SetWave(const float fAddWave, const float fAmplitude)
{
	// 情報の取得
	DOUBLE_INT line = GetLine();

	// 追加する波の大きさの設定
	m_fAddWave = fAddWave * D3DX_PI / 180.0f;

	// 振幅の設定
	m_fAmplitude = fAmplitude;

	for (int nCntZ = 0; nCntZ < line.y; nCntZ++)
	{
		for (int nCntX = 0; nCntX < line.x; nCntX++)
		{// 変数宣言
			int nCntVtx = nCntX + (nCntZ *  line.x);

			m_fVtxHeight[nCntVtx] = nCntZ * m_fAddWave + m_fAddWave;
		}
	}

	// 頂点座標などの設定
	SetVtx();

	// インデックスの設定
	SetIndexBuff();

	// 法線の計算
	Normal();
}

