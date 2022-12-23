//=============================================================================
//
// 衝突判定クラス(collision.h)
// Author : 唐﨑結斗
// 概要 : 衝突判定生成を行う
//
//=============================================================================
#ifndef _COLLISION_H_		// このマクロ定義がされてなかったら
#define _COLLISION_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// ライブラリーリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")			// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")		// [d3d9.lib]の拡張ライブラリ

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "d3dx9.h"							// 描画処理に必要
#include "object.h"

//=============================================================================
// 衝突判定クラス
// Author : 唐﨑結斗
// 概要 : 衝突判定クラス生成を行う
//=============================================================================
class CCollision
{
public:
	//--------------------------------------------------------------------
	// 当たり判定の種別
	// Author : 唐﨑結斗
	// 概要 : 当たり判定の種別
	//--------------------------------------------------------------------
	enum COLLISION_TYPE
	{
		TYPE_RECTANGLE3D = 0,	// 3D矩形
		TYPE_SPHERE,			// 球
		MAX_LEVEL				// 最大レベル
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static void ReleaseAll();									// すべてのオブジェクトの解放
	static void UpdateAll();									// すべてのオブジェクトの更新
	static void DrawAll();										// すべてのオブジェクトの描画
	static void ReleaseListAll();								// すべてのオブジェクトのリスト解除
	static CCollision *GetTop() { return m_pTop; }				// 先頭オブジェクトへのポインタのゲッター
	static CCollision *GetCurrent() { return m_pCurrent; }		// 現在の(一番後ろ)オブジェクトへのポインタのゲッター

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	explicit CCollision();
	virtual ~CCollision();

	//--------------------------------------------------------------------
	// 純粋仮想関数
	//--------------------------------------------------------------------
	virtual HRESULT Init() = 0;														// 初期化
	virtual void Uninit() = 0;														// 終了
	virtual void Update() = 0;														// 更新
	virtual void Draw() = 0;														// 描画
	virtual bool Collision(CObject::EObjectType objeType, bool bExtrude) = 0;		// 当たり判定	

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Release(void);																	// オブジェクトの解放
	void ReleaseList(void);																// オブジェクトのリスト解除
	void SetParent(CObject *pParent) { m_pParent = pParent; }							// 親オブジェクトの設定
	void SetCollidedObj(CObject *pCollidedObj) { m_pCollidedObj = pCollidedObj; }		// 当たったオブジェクトのポインタ取得
	void SetCollidedObj() { m_pCollidedObj = nullptr; }									// 当たったオブジェクトのポインタ取得
	void SetType(COLLISION_TYPE type) { m_type = type; }								// 当たり判定の設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }										// 位置の設定
	void SetSize(D3DXVECTOR3 size) { m_size = size; }									// サイズの設定
	void SetUseFlag(bool bUse) { m_bUse = bUse; }										// 使用状況の設定
	CCollision *GetPrev() { return m_pPrev; }											// 前のオブジェクトへのポインタ取得
	CCollision *GetNext() { return m_pNext; }											// 次のオブジェクトへのポインタ取得
	CObject *GetParent() { return m_pParent; }											// 親オブジェクトのポインタ取得
	CObject *GetCollidedObj() { return m_pCollidedObj; }								// 当たったオブジェクトのポインタ取得
	COLLISION_TYPE GetType() { return m_type; }											// 当たり判定の取得
	D3DXVECTOR3 GetPos() { return m_pos; }												// 位置の取得
	D3DXVECTOR3 GetSize() { return m_size; }											// サイズの取得
	bool GetUseFlag() { return m_bUse; }												// 使用状況の取得
	bool GetDeathFlag() { return m_bDeath; }											// 死亡フラグの取得

private:
	//--------------------------------------------------------------------
	// 静的メンバ変数
	//--------------------------------------------------------------------
	static CCollision *m_pTop;			// 先頭オブジェクトへのポインタ
	static CCollision *m_pCurrent;		// 現在の(一番後ろ)オブジェクトへのポインタ
	static int m_nMax;					// 使用数

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CCollision *m_pPrev;			// 前のオブジェクトへのポインタ
	CCollision *m_pNext;			// 次のオブジェクトへのポインタ
	CObject *m_pParent;				// 親のオブジェクトへのポインタ
	CObject *m_pCollidedObj;		// 衝突したオブジェクトへのポインタ
	COLLISION_TYPE m_type;			// 当たり判定の種別
	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_size;				// 大きさ
	bool m_bUse;					// 使用状況
	bool m_bDeath;					// 死亡フラグ
};

#endif


