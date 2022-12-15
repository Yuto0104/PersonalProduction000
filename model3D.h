//=============================================================================
//
// 3Dモデルクラス(model3D.h)
// Author : 唐﨑結斗
// 概要 : 3Dモデル生成を行う
//
//=============================================================================
#ifndef _MODEL_H_			// このマクロ定義がされてなかったら
#define _MODEL_H_			// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"
#include "main.h"

//=============================================================================
// 3Dモデルクラス
// Author : 唐﨑結斗
// 概要 : 3Dモデル生成を行うクラス
//=============================================================================
class CModel3D
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
	// モデルのマテリアル情報
	// Author : 唐﨑結斗
	// 概要 : モデルのマテリアル情報の設定
	//--------------------------------------------------------------------
	struct MODEL_MATERIAL
	{
		LPD3DXMESH		pMesh;				// メッシュ情報へのポインタ
		LPD3DXBUFFER	pBuffer;			// マテリアル情報へのポインタ
		DWORD			nNumMat;			// マテリアル情報の数
		D3DXVECTOR3		size;				// モデルの大きさ
		int				*pNumTex;			// テクスチャタイプ
		char			aFileName[0xff];	// Xファイルのパス
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CModel3D *Create();										// 3Dモデルの生成
	static void InitModel();										// モデルの初期化
	static void UninitModel();										// モデルの終了
	static void LoadModel(const char *pFileName);					// モデルの読み込み
	static MODEL_MATERIAL *GetMaterial() { return m_material; }		// マテリアル情報の取得
	static int GetMaxModel() { return m_nMaxModel; }				// モデル数 

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CModel3D();
	~CModel3D();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init();															// 初期化
	void Uninit();															// 終了
	void Update();															// 更新
	void Draw();															// 描画
	void Draw(D3DXMATRIX mtxParent);										// 描画(オーバーロード)
	void DrawMaterial();													// マテリアル描画
	void SetPos(const D3DXVECTOR3 &pos);									// 位置のセッター
	void SetRot(const D3DXVECTOR3 &rot);									// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size);									// 大きさのセッター
	void SetMtxWorld(D3DXMATRIX mtxWorld) { m_mtxWorld = mtxWorld; }		// ワールドマトリックスの設定
	void SetParent(CModel3D *pParent) { m_pParent = pParent; }				// 親モデルのセッター
	void SetModelID(const int nModelID) { m_nModelID = nModelID; }			// モデルID
	void SetColor(const D3DXCOLOR color);									// カラーの設定
	void SetColor(bool bColor) { m_bColor = bColor; }						// カラーの設定(オーバーロード)
	void SetShadow(bool bShadow) { m_bShadow = bShadow; }					// 影の使用状況の設定
	void SetLighting(bool bLighting) { m_bLighting = bLighting; }			// ライトを使用状況の設定
	D3DXVECTOR3 GetPos() { return m_pos; }									// 位置のゲッター
	D3DXVECTOR3 GetRot() { return m_rot; }									// 向きのゲッター
	D3DXVECTOR3 GetSize() { return m_size; }								// 大きさのゲッター
	MODEL_MATERIAL GetMyMaterial() { return m_material[m_nModelID]; }		// マテリアル情報の取得
	D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }							// ワールドマトリックスの取得
	CModel3D *GetParent() { return m_pParent; }								// 親モデルのゲッター

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Shadow();		// 影の描画

	//--------------------------------------------------------------------
	// 静的メンバ変数
	//--------------------------------------------------------------------
	static MODEL_MATERIAL	*m_material;		// マテリアル情報
	static int				m_nMaxModel;		// モデル数			

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CModel3D			*m_pParent;			// 親モデルの情報
	D3DXMATRIX			m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3			m_pos;				// 位置
	D3DXVECTOR3			m_rot;				// 向き
	D3DXVECTOR3			m_size;				// 大きさ
	D3DXCOLOR			m_color;			// カラー
	int					m_nModelID;			// モデルID
	bool				m_bColor;			// カラーを使用する
	bool				m_bShadow;			// 影の使用状況
	bool				m_bLighting;		// ライトの使用状況
};

#endif