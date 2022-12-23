//=============================================================================
//
// モデルオブジェクトクラス(model_obj.h)
// Author : 唐﨑結斗
// 概要 : モデルオブジェクト生成を行う
//
//=============================================================================
#ifndef _MODEL_OBJ_H_			// このマクロ定義がされてなかったら
#define _MODEL_OBJ_H_			// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel3D;
class CCollision_Rectangle3D;

//=============================================================================
// モデルオブジェクトクラス
// Author : 唐﨑結斗
// 概要 : モデルオブジェクト生成を行うクラス
//=============================================================================
class CModelObj : public CObject
{
public:
	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CModelObj *Create();							// モデルオブジェクトの生成
	static void LoadFile(const char *pFileName);		// ファイルの読み込み

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CModelObj();
	~CModelObj();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;																							// 初期化
	void Uninit() override;																								// 終了
	void Update() override;																								// 更新
	void Draw() override;																								// 描画
	void SetPos(const D3DXVECTOR3 &pos) override { m_pos = pos; };														// 位置のセッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }											// 過去位置のセッター
	void SetRot(const D3DXVECTOR3 &rot) override { m_rot = rot; };														// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size) override { m_size = size; }													// 大きさのセッター
	D3DXVECTOR3 GetPos() override { return m_pos; }																		// 位置のゲッター
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }																// 過去位置のゲッター
	D3DXVECTOR3 GetRot()  override { return m_rot; }																	// 向きのゲッター
	D3DXVECTOR3 GetSize()  override { return m_size; }																	// 大きさのゲッター
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }													// ワールドマトリックスのセッター
	D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }																		// ワールドマトリックスのゲッター
	void SetType(const int nType);																						// タイプの設定
	CModel3D *GetModel() { return m_pModel; }																			// モデル情報の取得
	void *SetCollision(CCollision_Rectangle3D *pColliRectangle3D) { m_pColliRectangle3D = pColliRectangle3D; }			// 当たり判定の設定
	CCollision_Rectangle3D *GetCollision() { return m_pColliRectangle3D; }												// 当たり判定の取得

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CModel3D					*m_pModel;				// モデル情報
	CCollision_Rectangle3D		*m_pColliRectangle3D;	// 当たり判定
	D3DXMATRIX					m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3					m_pos;					// 位置
	D3DXVECTOR3					m_posOld;				// 過去位置
	D3DXVECTOR3					m_rot;					// 向き
	D3DXVECTOR3					m_size;					// 大きさ
	int							m_nType;				// モデルのタイプ
};

#endif
