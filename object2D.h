//=============================================================================
//
// 2Dオブジェクトクラス(object2D.h)
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================
#ifndef _OBJECT2D_H_		// このマクロ定義がされてなかったら
#define _OBJECT2D_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"
#include "texture.h"

//=============================================================================
// 2Dオブジェクトクラス
// Author : 唐﨑結斗
// 概要 : 2Dオブジェクト生成を行うクラス
//=============================================================================
class CObject2D : public CObject
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
	static CObject2D *Create(void);				// 2Dオブジェクトの生成
	static CObject2D *Create(int nPriority);	// 2Dオブジェクトの生成(オーバーロード)

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	explicit CObject2D(int nPriority = PRIORITY_LEVEL0);
	~CObject2D() override;

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
	void LoadTex(const int nNumTex) { m_nNumTex = nNumTex; }						// テクスチャの設定
	void SetCol(const D3DCOLOR &col);												// 色のセッター

protected:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void SetVtx();															// 頂点座標などの設定
	void SetTex(const D3DXVECTOR2 &minTex, const D3DXVECTOR2 &maxTex);		// テクスチャ座標の設定

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;			// 頂点バッファ
	D3DXVECTOR3						m_pos;				// 位置
	D3DXVECTOR3						m_posOld;			// 過去の位置
	D3DXVECTOR3						m_rot;				// 向き
	D3DXVECTOR3						m_size;				// 大きさ
	D3DCOLOR						m_col;				// カラー
	int								m_nNumTex;			// テクスチャの種別番号
	float							m_fAngle;			// 対角線の角度
	float							m_fLength;			// 対角線の長さ
};

#endif


