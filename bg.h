//=============================================================================
//
// 背景クラス(bg.h)
// Author : 唐﨑結斗
// 概要 : 背景生成を行う
//
//=============================================================================
#ifndef _BG_H_		// このマクロ定義がされてなかったら
#define _BG_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"
#include "texture.h"

//=============================================================================
// 背景クラス
// Author : 唐﨑結斗
// 概要 : 背景生成を行うクラス
//=============================================================================
class CBG : public CObject
{
public:
	// 頂点フォーマット
	const unsigned int	FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	//*****************************************************************************
	// 構造体定義
	//*****************************************************************************
	// 頂点データ
	struct VERTEX_2D
	{
		D3DXVECTOR3		pos;		// 位置
		float			rhw;		// rhw
		D3DCOLOR		col;		// カラー
		D3DXVECTOR2		tex;		// テクスチャ座標
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CBG *Create(void);				// 背景の生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CBG(int nPriority = PRIORITY_LEVEL0);
	~CBG() override;

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;														// 初期化
	void Uninit() override;															// 終了
	void Update() override;															// 更新
	void Draw() override;															// 描画
	void SetPos(const D3DXVECTOR3 &pos) override { pos; }							// 位置のセッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { posOld; }					// 過去位置のセッター
	void SetRot(const D3DXVECTOR3 &rot) override { rot; }							// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size) override { size; }						// 大きさのセッター
	D3DXVECTOR3 GetPos() override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			// 位置のゲッター
	D3DXVECTOR3 GetPosOld()  override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 過去位置のゲッター
	D3DXVECTOR3 GetRot()  override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 向きのゲッター
	D3DXVECTOR3 GetSize()  override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 大きさのゲッター
	void SetCol(const D3DXCOLOR color);												// 頂点カラーの設定
	void LoadTex(const int nNumTex) { m_nNumTex = nNumTex; }						// テクスチャの設定

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void SetVtx();															// 頂点座標などの設定
	void SetTex(const D3DXVECTOR2 &minTex, const D3DXVECTOR2 &maxTex);		// テクスチャ座標の設定

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	LPDIRECT3DVERTEXBUFFER9		 m_pVtxBuff;		// 頂点バッファ
	D3DCOLOR					 m_col;				// カラー
	int							 m_nNumTex;			// テクスチャの種別
};

#endif
