//=============================================================================
//
// オブジェクトクラス(object.h)
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "object2D.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 2Dオブジェクトを生成する
//=============================================================================
CObject2D * CObject2D::Create(void)
{
	// オブジェクトインスタンス
	CObject2D *pObject2D = nullptr;

	// メモリの解放
	pObject2D = new CObject2D;

	if (pObject2D != nullptr)
	{// 数値の初期化
		pObject2D->Init();
	}
	else
	{// メモリの確保ができなかった
		assert(false);
	}

	// インスタンスを返す
	return pObject2D;
}

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 2Dオブジェクトを生成する
//=============================================================================
CObject2D * CObject2D::Create(int nPriority)
{
	// オブジェクトインスタンス
	CObject2D *pObject2D = nullptr;

	// メモリの解放
	pObject2D = new CObject2D(nPriority);

	if (pObject2D != nullptr)
	{// 数値の初期化
		pObject2D->Init();
	}
	else
	{// メモリの確保ができなかった
		assert(false);
	}

	// インスタンスを返す
	return pObject2D;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CObject2D::CObject2D(int nPriority/* = PRIORITY_LEVEL0*/) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;								// 頂点バッファ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさ
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);			// カラー
	m_fAngle = 0.0f;									// 対角線の角度
	m_fLength = 0.0f;									// 対角線の長さ
	m_nNumTex = -1;										// テクスチャタイプ
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CObject2D::~CObject2D()
{
	
}

//=============================================================================
// ポリゴンの初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CObject2D::Init()
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
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_size = D3DXVECTOR3(100.0f, 100.0f, 0.0f);		// 大きさ
	m_nNumTex = -1;									// テクスチャタイプ
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色の設定

	// 色の設定
	SetCol(m_col);

	// テクスチャ座標の設定
	SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// ポリゴンの終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CObject2D::Uninit()
{ 
	//頂点バッファを破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();

		m_pVtxBuff = nullptr;
	}

	// オブジェクト2Dの解放
	Release();
}

//=============================================================================
// ポリゴンの更新
// Author : 唐﨑結斗
// 概要 : 2Dポリゴンの更新を行う
//=============================================================================
void CObject2D::Update()
{
	
}

//=============================================================================
// ポリゴンの描画
// Author : 唐﨑結斗
// 概要 : 2Dポリゴンの描画を行う
//=============================================================================
void CObject2D::Draw()
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
// 位置のセッター
// Author : 唐﨑結斗
// 概要 : 位置のメンバ変数に引数を代入
//=============================================================================
void CObject2D::SetPos(const D3DXVECTOR3 &pos)
{
	// 位置の設定
	m_pos = pos;

	// 頂点座標などの設定
	SetVtx();

	// 色の設定
	SetCol(m_col);
}

//=============================================================================
// 向きのセッター
// Author : 唐﨑結斗
// 概要 : 向きのメンバ変数に引数を代入
//=============================================================================
void CObject2D::SetRot(const D3DXVECTOR3 &rot)
{
	// 向きの設定
	m_rot = rot;

	// 頂点座標などの設定
	SetVtx();

	// 色の設定
	SetCol(m_col);
}

//=============================================================================
// 大きさのセッター
// Author : 唐﨑結斗
// 概要 : 大きさのメンバ変数に引数を代入
//=============================================================================
void CObject2D::SetSize(const D3DXVECTOR3 & size)
{
	// 大きさの設定
	m_size = size;

	// 頂点座標などの設定
	SetVtx();

	// 色の設定
	SetCol(m_col);
}

//=============================================================================
// 頂点座標などの設定
// Author : 唐﨑結斗
// 概要 : 2Dポリゴンの頂点座標、rhw、頂点カラーを設定する
//=============================================================================
void CObject2D::SetVtx()
{
	//頂点情報へのポインタを生成
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//対角線の長さを算出する
	m_fLength = sqrtf((m_size.x * m_size.x) + (m_size.y * m_size.y)) / 2.0f;

	//対角線の角度を算出
	m_fAngle = atan2f(m_size.x, m_size.y);

	// 頂点情報を設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) *  m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) *  m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - (0 + m_fAngle)) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - (0 + m_fAngle)) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z - (0 - m_fAngle)) *  m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z - (0 - m_fAngle)) *  m_fLength;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 色の設定
// Author : 唐﨑結斗
// 概要 : 頂点カラーを設定する
//=============================================================================
void CObject2D::SetCol(const D3DCOLOR &col)
{
	m_col = col;

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
// テクスチャ座標の設定
// Author : 唐﨑結斗
// 概要 : 2Dオブジェクトのテクスチャ座標を設定する
//=============================================================================
void CObject2D::SetTex(const D3DXVECTOR2 &minTex, const D3DXVECTOR2 &maxTex)
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
