//=============================================================================
//
// ライン描画処理(line.h)
// Author : 唐﨑結斗
// 概要 : ライン生成を行う
//
//=============================================================================
#ifndef _LINE_H_		// このマクロ定義がされてなかったら
#define _LINE_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//=============================================================================
// ラインクラス
// Author : 唐﨑結斗
// 概要 : ライン生成を行うクラス
//=============================================================================
class CLine : public CObject
{
public:
	// 頂点フォーマット
	const unsigned int	FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE);

	//*****************************************************************************
	// 構造体定義
	//*****************************************************************************
	// 頂点データ
	typedef struct
	{
		D3DXVECTOR3			pos;			// 頂点座標
		D3DXVECTOR3			nor;			// 法線ベクトル
		D3DCOLOR			col;			// 頂点カラー
	}VERTEX_3D;

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CLine *Create(void);				// オブジェクトの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	explicit CLine();
	~CLine();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;																				// 初期化
	void Uninit() override;																					// 終了
	void Update() override;																					// 更新
	void Draw() override;																					// 描画
	void SetPos(const D3DXVECTOR3 &pos) override;															// 位置のセッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }								// 過去位置のセッター
	void SetRot(const D3DXVECTOR3 &rot) override;															// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size) override;															// 大きさのセッター
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }										// ワールドマトリックスのセッター
	D3DXVECTOR3 GetPos() override { return m_pos; }															// 位置のゲッター
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }													// 過去位置のゲッター
	D3DXVECTOR3 GetRot()  override { return m_rot; }														// 向きのゲッター
	D3DXVECTOR3 GetSize()  override { return m_size; }														// 大きさのゲッター
	D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }															// ワールドマトリックスのゲッター
	void SetVtx();																							// 頂点座標などの設定
	void SetCol(const D3DXCOLOR color);																		// 頂点カラーの設定
	void SetLine(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 start, D3DXVECTOR3 goal, D3DXCOLOR col);		// ラインの設置

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
	D3DXVECTOR3						m_start;			// 始点
	D3DXVECTOR3						m_goal;				// 終点
};

#endif




