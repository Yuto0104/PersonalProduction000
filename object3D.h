//=============================================================================
//
// 3Dオブジェクトクラス(object3D.h)
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================
#ifndef _OBJECT3D_H_		// このマクロ定義がされてなかったら
#define _OBJECT3D_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"
#include "texture.h"

//=============================================================================
// 3Dオブジェクトクラス
// Author : 唐﨑結斗
// 概要 : 3Dオブジェクト生成を行うクラス
//=============================================================================
class CObject3D : public CObject
{
public:
	// 頂点フォーマット
	const unsigned int	FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	//*****************************************************************************
	// 構造体定義
	//*****************************************************************************
	// 頂点データ
	typedef struct
	{
		D3DXVECTOR3			pos;			// 頂点座標
		D3DXVECTOR3			nor;			// 法線ベクトル
		D3DCOLOR			col;			// 頂点カラー
		D3DXVECTOR2			tex;			// テクスチャ座標
	}VERTEX_3D;

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CObject3D *Create(void);				// 2Dオブジェクトの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CObject3D();
	~CObject3D();

	//--------------------------------------------------------------------
	// オーバーライド関数
	//--------------------------------------------------------------------
	HRESULT Init() override;														// 初期化
	void Uninit() override;															// 終了
	void Update() override;															// 更新
	void Draw() override;															// 描画
	void SetPos(const D3DXVECTOR3 &pos) override;									// 位置のセッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }		// 過去位置のセッター
	void SetRot(const D3DXVECTOR3 &rot) override;									// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size) override;									// 大きさのセッター
	D3DXVECTOR3 GetPos() override { return m_pos; }									// 位置のゲッター
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }							// 過去位置のゲッター
	D3DXVECTOR3 GetRot()  override { return m_rot; }								// 向きのゲッター
	D3DXVECTOR3 GetSize()  override { return m_size; }								// 大きさのゲッター
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }				// ワールドマトリックスのセッター
	D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }									// ワールドマトリックスのゲッター
	void SetBillboard(bool bBillboard) { m_bBillboard = bBillboard; }				// ビルボードの設定
	bool GetBillboard() { return m_bBillboard; }									// ビルボードの取得
	void SetColor(D3DXCOLOR color);													// 色の設定
	D3DXCOLOR GetColor() { return m_color; }										// 色の取得
	virtual void SetVtx();															// 頂点座標などの設定
	void SetCol();																	// 頂点カラーの設定
	void SetTex(const D3DXVECTOR2 &minTex, const D3DXVECTOR2 &maxTex);				// テクスチャ座標の設定
	void LoadTex(const int nNumTex) { m_nNumTex = nNumTex; }						// テクスチャの設定
	void SetZFunc(const _D3DCMPFUNC zFunc) { m_zFunc = zFunc; }						// Zテストの優先度のセッター
	void SetAlphaValue(const int nAlphaValue) { m_nAlphaValue = nAlphaValue; }		// アルファテストの透過率のセッター

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;			// 頂点バッファ
	D3DXMATRIX						m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3						m_pos;				// 位置
	D3DXVECTOR3						m_posOld;			// 過去位置
	D3DXVECTOR3						m_rot;				// 向き
	D3DXVECTOR3						m_size;				// 大きさ
	D3DXCOLOR						m_color;			// カラー
	_D3DCMPFUNC						m_zFunc;			// Zテストの優先度
	int								m_nAlphaValue;		// アルファテストの透過率
	int								m_nNumTex;			// テクスチャの種別
	float							m_fAngle;			// 対角線の角度
	float							m_fLength;			// 対角線の長さ
	bool							m_bBillboard;		// ビルボードかどうか
};

#endif



