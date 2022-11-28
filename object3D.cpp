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

#include "object3D.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 2Dオブジェクトを生成する
//=============================================================================
CObject3D * CObject3D::Create(void)
{
	// オブジェクトインスタンス
	CObject3D *pObject3D = nullptr;

	// メモリの解放
	pObject3D = new CObject3D;

	// メモリの確保ができなかった
	assert(pObject3D != nullptr);

	// 数値の初期化
	pObject3D->Init();

	// インスタンスを返す
	return pObject3D;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CObject3D::CObject3D()
{
	m_pVtxBuff = nullptr;								// 頂点バッファ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさ
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// カラー
	m_fAngle = 0.0f;									// 対角線の角度
	m_fLength = 0.0f;									// 対角線の長さ
	m_nNumTex = -1;										// テクスチャタイプ
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CObject3D::~CObject3D()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CObject3D::Init()
{// レンダラーのゲット
	CRenderer *pRenderer = CApplication::GetRenderer();

	//頂点バッファの生成
	pRenderer->GetDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,		// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,														// 頂点ファーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ポリゴン情報の設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 大きさ
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// カラー
	m_zFunc = D3DCMP_LESS;							// Zテストの優先度
	m_nAlphaValue = 100;							// アルファテストの透過率
	m_nNumTex = -1;									// テクスチャタイプ
	m_bBillboard = false;							// ビルボードかどうか

	// 頂点座標などの設定
	SetVtx();

	// 頂点カラーを設定
	SetCol();

	// テクスチャ座標の設定
	SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CObject3D::Uninit()
{
	//頂点バッファを破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();

		m_pVtxBuff = nullptr;
	}

	// オブジェクト3Dの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CObject3D::Update()
{

}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void CObject3D::Draw()
{// レンダラーのゲット
	CRenderer *pRenderer = CApplication::GetRenderer();

	// テクスチャポインタの取得
	CTexture *pTexture = CApplication::GetTexture();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxView;

	// ワールドマトリックスの初期化
	// 行列初期化関数(第一引数の[行列]を[単位行列]に初期化)
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_bBillboard)
	{// ビルボードである
		// ライトを無効
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// ビューマトリックスの設定
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// カメラ逆行列を設定
		D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
		m_mtxWorld._41 = 0.0f;
		m_mtxWorld._42 = 0.0f;
		m_mtxWorld._43 = 0.0f;
	}
	else
	{// 向きの反映
		// 行列回転関数 (第一引数に[ヨー(y)ピッチ(x)ロール(z)]方向の回転行列を作成)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

		// 行列掛け算関数 (第二引数 * 第三引数を第一引数に格納)
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}

	// 位置を反映
	// 行列移動関数 (第一引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);						// 行列掛け算関数

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_nNumTex));

	// Zテストを使用する
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZFUNC, m_zFunc);

	// αテストを使用する
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, m_nAlphaValue);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ライトを有効	
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zテストの終了
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// αテストの終了
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// テクスチャの解除
	pDevice->SetTexture(0, nullptr);
}

//=============================================================================
// 位置のセッター
// Author : 唐﨑結斗
// 概要 : 位置のメンバ変数に引数を代入
//=============================================================================
void CObject3D::SetPos(const D3DXVECTOR3 &pos)
{
	// 位置の設定
	m_pos = pos;

	// 頂点座標などの設定
	SetVtx();
}

//=============================================================================
// 向きのセッター
// Author : 唐﨑結斗
// 概要 : 向きのメンバ変数に引数を代入
//=============================================================================
void CObject3D::SetRot(const D3DXVECTOR3 &rot)
{
	// 位置の設定
	m_rot = rot;

	// 頂点座標などの設定
	SetVtx();
}

//=============================================================================
// 大きさのセッター
// Author : 唐﨑結斗
// 概要 : 大きさのメンバ変数に引数を代入
//=============================================================================
void CObject3D::SetSize(const D3DXVECTOR3 & size)
{
	// 大きさの設定
	m_size = size;

	// 頂点座標などの設定
	SetVtx();
}

//=============================================================================
// 色の設定
// Author : 唐﨑結斗
// 概要 : 色を設定する
//=============================================================================
void CObject3D::SetColor(D3DXCOLOR color)
{
	// 色の設定
	m_color = color;

	// 頂点カラーの設定
	SetCol();
}

//=============================================================================
// 頂点座標などの設定
// Author : 唐﨑結斗
// 概要 : 3D頂点座標、rhw、頂点カラーを設定する
//=============================================================================
void CObject3D::SetVtx()
{
	//頂点情報へのポインタを生成
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);

	// 各頂点の法線の設定(*ベクトルの大きさは1にする必要がある)
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点カラーの設定
// Author : 唐﨑結斗
// 概要 : 頂点カラーを設定する
//=============================================================================
void CObject3D::SetCol()
{
	//頂点情報へのポインタを生成
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = m_color;
	pVtx[1].col = m_color;
	pVtx[2].col = m_color;
	pVtx[3].col = m_color;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャ座標の設定
// Author : 唐﨑結斗
// 概要 : 3Dオブジェクトのテクスチャ座標を設定する
//=============================================================================
void CObject3D::SetTex(const D3DXVECTOR2 &minTex, const D3DXVECTOR2 &maxTex)
{
	//頂点情報へのポインタを生成
	VERTEX_3D *pVtx;

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
