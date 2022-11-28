//=============================================================================
//
// 背景クラス(object.h)
// Author : 唐﨑結斗
// 概要 : 背景生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "bg.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 3Dモデルを生成する
//=============================================================================
CBG * CBG::Create(void)
{
	// オブジェクトインスタンス
	CBG *pBG = nullptr;

	// メモリの解放
	pBG = new CBG;

	// メモリの確保ができなかった
	assert(pBG != nullptr);

	// 数値の初期化
	pBG->Init();

	// インスタンスを返す
	return pBG;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CBG::CBG(int nPriority/* = PRIORITY_LEVEL0*/) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;								// 頂点バッファ
	m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);				// カラー
	m_nNumTex = -1;										// テクスチャの種別
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CBG::~CBG()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CBG::Init()
{// レンダラーのゲット
	CRenderer *pRenderer = CApplication::GetRenderer();

	//頂点バッファの生成
	pRenderer->GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,		// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,														// 頂点ファーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ポリゴン情報の設定
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// カラー
	m_nNumTex = 9;									// テクスチャタイプ

	// 頂点座標の設定
	SetVtx();

	// 頂点カラーの設定
	SetCol(m_col);

	// テクスチャ座標の設定
	SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CBG::Uninit()
{
	//頂点バッファを破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();

		m_pVtxBuff = nullptr;
	}

	// 背景3Dの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CBG::Update()
{

}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void CBG::Draw()
{// レンダラーのゲット
	CRenderer *pRenderer = CApplication::GetRenderer();

	// テクスチャポインタの取得
	CTexture *pTexture = CApplication::GetTexture();

	//テクスチャの設定
	pRenderer->GetDevice()->SetTexture(0, nullptr);

	//テクスチャの設定
	pRenderer->GetDevice()->SetTexture(0, pTexture->GetTexture(m_nNumTex));

	//頂点バッファをデータストリームに設定
	pRenderer->GetDevice()->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pRenderer->GetDevice()->SetFVF(FVF_VERTEX_2D);

	//ポリゴン描画
	pRenderer->GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//テクスチャの設定
	pRenderer->GetDevice()->SetTexture(0, nullptr);
}

//=============================================================================
// 頂点カラーの設定
// Author : 唐﨑結斗
// 概要 : 2Dポリゴンの頂点カラーを設定する
//=============================================================================
void CBG::SetCol(const D3DXCOLOR color)
{
	// カラーの設定
	m_col = color;

	//頂点情報へのポインタを生成
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点座標などの設定
// Author : 唐﨑結斗
// 概要 : 2Dポリゴンの頂点座標、rhw、頂点カラーを設定する
//=============================================================================
void CBG::SetVtx()
{
	//頂点情報へのポインタを生成
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, (float)CRenderer::SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャ座標の設定
// Author : 唐﨑結斗
// 概要 : 2Dオブジェクトのテクスチャ座標を設定する
//=============================================================================
void CBG::SetTex(const D3DXVECTOR2 &minTex, const D3DXVECTOR2 &maxTex)
{
	//頂点情報へのポインタを生成
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(minTex.x, minTex.y);
	pVtx[1].tex = D3DXVECTOR2(maxTex.x, minTex.y);
	pVtx[2].tex = D3DXVECTOR2(minTex.x, maxTex.y);
	pVtx[3].tex = D3DXVECTOR2(maxTex.x, maxTex.y);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}