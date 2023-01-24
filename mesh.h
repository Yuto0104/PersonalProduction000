//=============================================================================
//
// 3Dメッシュクラス(mesh.h)
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================
#ifndef _MESH3D_H_		// このマクロ定義がされてなかったら
#define _MESH3D_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"
#include "texture.h"

//=============================================================================
// 3Dメッシュクラス
// Author : 唐﨑結斗
// 概要 : 3Dオブジェクト生成を行うクラス
//=============================================================================
class CMesh3D : public CObject
{
public:
	//--------------------------------------------------------------------
	// DOUBLE_INT変数の追加
	// Author : 唐﨑結斗
	// 概要 : テクスチャアニメーションのパターン数設定に使用
	//--------------------------------------------------------------------
	struct DOUBLE_INT
	{
		// 変数宣言
		int   x, y;

		constexpr DOUBLE_INT() : x(0), y(0) { }
		constexpr DOUBLE_INT(int _x, int _y) : x(_x), y(_y) { }

		// 加算
		DOUBLE_INT operator +(const DOUBLE_INT &texPattern) {
			DOUBLE_INT pattern;
			pattern.x = x + texPattern.x;
			pattern.y = y + texPattern.y;
			return pattern;
		}

		// 加算(+=)
		DOUBLE_INT &operator +=(const DOUBLE_INT &texPattern) {
			x += texPattern.x;
			y += texPattern.y;
			return *this;
		}

		// 減算
		DOUBLE_INT operator -(const DOUBLE_INT &texPattern) {
			DOUBLE_INT pattern;
			pattern.x = x - texPattern.x;
			pattern.y = y - texPattern.y;
			return pattern;
		}

		// 減算(-=)
		DOUBLE_INT &operator -=(const DOUBLE_INT &texPattern) {
			x -= texPattern.x;
			y -= texPattern.y;
			return *this;
		}

		// 乗算
		DOUBLE_INT operator *(const DOUBLE_INT &texPattern) {
			DOUBLE_INT pattern;
			pattern.x = x * texPattern.x;
			pattern.y = y * texPattern.y;
			return pattern;
		}

		// 乗算(*=)
		DOUBLE_INT &operator *=(const DOUBLE_INT &texPattern) {
			x *= texPattern.x;
			y *= texPattern.y;
			return *this;
		}

		// 代入
		DOUBLE_INT &operator =(const DOUBLE_INT &texPattern) {
			x = texPattern.x;
			y = texPattern.y;
			return *this;
		}
	};

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
	static CMesh3D *Create(void);					// 2Dオブジェクトの生成
	static bool CollisonMesh(CObject *pTarget);		// 当たり判定

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CMesh3D();
	~CMesh3D();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;														// 初期化
	void Uninit() override;															// 終了
	void Update() override;															// 更新
	void Draw() override;															// 描画
	void SetPos(const D3DXVECTOR3 &pos) override;									// 位置のセッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }		// 過去位置のセッター
	void SetRot(const D3DXVECTOR3 &rot) override;									// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size) override;									// 大きさのセッター
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }				// ワールドマトリックスのセッター
	void SetBlock(DOUBLE_INT block);												// ブロック数のセッター
	D3DXVECTOR3 GetPos() override { return m_pos; }									// 位置のゲッター
	D3DXVECTOR3 GetPosOld() override { return m_posOld; }							// 過去位置のゲッター
	D3DXVECTOR3 GetRot() override { return m_rot; }									// 向きのゲッター
	D3DXVECTOR3 GetSize() override { return m_size; }								// 大きさのゲッター
	D3DXVECTOR3 GetBlockSize() { return m_blockSize; }								// ブロックサイズのゲッター
	D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }									// ワールドマトリックスのゲッター
	DOUBLE_INT GetBlock() { return m_block; }										// ブロック数のゲッター
	DOUBLE_INT GetLine() { return m_line; }											// ライン数のゲッター
	int GetVtx() { return m_nVtx; }													// 頂点数のゲッター
	void SetSplitTex(bool bSplitTex);												// テクスチャ分割するかのセッター
	bool GetSplitTex() { return m_bSplitTex; }										// テクスチャ分割するかのゲッター
	void SetScrollTex(D3DXVECTOR2 addTex, bool bScrollTex);							// テクスチャスクロールの設定
	void LoadTex(const int nNumTex) { m_nNumTex = nNumTex; }						// テクスチャの設定
	void SetCol(const D3DXCOLOR &col);												// 色の設定
	D3DXCOLOR GetCol() { return m_col; }											// 色の取得
	D3DXVECTOR2 GetTex() { return m_tex; }											// テクスチャのゲッター
	bool Collison(CObject *pTarget);												// 当たり判定
	void SetUseCollison(bool bUseCollison) { m_bUseCollison = bUseCollison; }		// 当たり判定の使用状況の設定
	bool GetUseCollison() { return m_bUseCollison; }								// 当たり判定の使用状況の取得

protected:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	virtual void SetVtx();									// 頂点座標などの設定
	virtual void SetTex();									// テクスチャ座標の設定
	void SetIndexBuff();									// インデックスバッファの設定
	void SetMeshInfo();										// メッシュの数値の計算
	void TexScroll();										// テクスチャスクロール
	void Normal();											// 法線

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;			 // 頂点バッファ

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	LPDIRECT3DINDEXBUFFER9			m_pIdxBuff;							// インデックスバッファへのポインタ
	D3DXMATRIX						m_mtxWorld;							// ワールドマトリックス
	D3DXVECTOR3						m_pos;								// 位置
	D3DXVECTOR3						m_posOld;							// 過去位置
	D3DXVECTOR3						m_rot;								// 向き
	D3DXVECTOR3						m_size;								// 大きさ
	D3DXVECTOR3						m_blockSize;						// ブロックサイズ
	D3DXVECTOR2						m_tex;								// テクスチャ座標の基準値
	D3DXVECTOR2						m_addTex;							// テクスチャ座標の加算値
	D3DXCOLOR						m_col;								// カラー
	DOUBLE_INT						m_block;							// ブロック数
	DOUBLE_INT						m_line;								// 列数
	int 							m_nNumTex;							// テクスチャの種別
	int								m_nLineVtxX;						// 列頂点数(X)
	int								m_nVtx;								// 頂点数
	int								m_nPolygon;							// ポリゴン数
	int								m_nIndex;							// インデックス数
	bool							m_bSplitTex;						// テクスチャの分割するか
	bool							m_bScrollTex;						// テクスチャをスクロールするかどうか
	bool							m_bUseCollison;						// 当たり判定を使うか否か
};

#endif



