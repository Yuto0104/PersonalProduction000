//=============================================================================
//
// スーパークラス(super.h)
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================
#ifndef _SUPER_H_		// このマクロ定義がされてなかったら
#define _SUPER_H_		// 二重インクルード防止のマクロ定義

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

//=============================================================================
// スーパークラス
// Author : 唐﨑結斗
// 概要 : スーパークラス生成を行う
//=============================================================================
class CSuper
{
public:
	//--------------------------------------------------------------------
	// プライオリティのレベル
	// Author : 唐﨑結斗
	// 概要 : プライオリティのレベルを設定
	//--------------------------------------------------------------------
	enum PRIORITY_LEVEL
	{
		PRIORITY_LEVEL0 = 0,	// レベル0
		PRIORITY_LEVEL1,		// レベル1
		PRIORITY_LEVEL2,		// レベル2
		PRIORITY_LEVEL3,		// レベル3
		PRIORITY_LEVEL4,		// レベル4
		MAX_LEVEL				// 最大レベル
	};

	//--------------------------------------------------------------------
	// 要素の種別の列挙型
	// Author : 唐﨑結斗
	// 概要 : 要素の種別情報を格納
	//--------------------------------------------------------------------
	enum EElementType
	{
		ELEMENT_NONE = 0,			// タイプ無し
		ELEMENT_OBJECT,				// オブジェクト
		ELEMENT_MANAGER,			// マネージャー
		ELEMENT_SCENE,				// シーン
		MAX_ELEMENT,				// 種別の最大数
	};

	//--------------------------------------------------------------------
	// 描画タイプ
	// Author : 唐﨑結斗
	// 概要 : 描画タイプ
	//--------------------------------------------------------------------
	enum DRAW_TYPE
	{
		TYPE_ALL = 0,			// すべて
		TYPE_GAME,				// ゲーム
		TYPE_MAP,				// マップ
		MAX_TYPE				// 最大数
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static void ReleaseAll(bool bPermanent);										// すべてのオブジェクトの解放
	static void UpdateAll();														// すべてのオブジェクトの更新
	static void DrawAll();															// すべてのオブジェクトの描画
	static void DrawAll(DRAW_TYPE EDrawType);										// すべてのオブジェクトの描画
	static void ReleaseListAll();													// すべてのオブジェクトのリスト解除
	static CSuper *GetTop(int nPriority) { return m_pTop[nPriority]; }				// 先頭オブジェクトへのポインタの取得
	static CSuper *GetCurrent(int nPriority) { return m_pCurrent[nPriority]; }		// 現在の(一番後ろ)オブジェクトへのポインタの取得
	static void SetPause(const bool bPause) { m_bPause = bPause; }					// ポーズ状態のセッター
	static bool GetPause() { return m_bPause; }										// ポーズ状態のゲッター

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	explicit CSuper(int nPriority = PRIORITY_LEVEL0);
	virtual ~CSuper();

	//--------------------------------------------------------------------
	// 純粋仮想関数
	//--------------------------------------------------------------------
	virtual HRESULT Init() = 0;													// 初期化
	virtual void Uninit() = 0;													// 終了
	virtual void Update() = 0;													// 更新
	virtual void Draw() = 0;													// 描画
	void SetElement(const EElementType EElement) { m_EElement = EElement; }		// 要素の設定
	void SetDrawType(DRAW_TYPE EDrawType) { m_EDrawType = EDrawType; }			// 描画方法の設定
	void SetPermanent(bool bPermanent) { m_bPermanent = bPermanent; }			// シーン生存フラグの設定
	CSuper *GetPrev() { return m_pPrev; }										// 前のオブジェクト
	CSuper *GetNext() { return m_pNext; }										// 前のオブジェクト
	EElementType GetElement() { return m_EElement; }							// 要素の取得
	DRAW_TYPE GetDrawType() { return m_EDrawType; }								// 描画方法のゲッター
	bool GetPermanent() { return m_bPermanent; }								// シーン生存フラグの取得
	bool GetFlagDeath() { return m_bDeath; }									// 死亡フラグの取得

protected:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Release(void);			// オブジェクトの解放
	void ReleaseList(void);		// オブジェクトのリスト解除

private:
	//--------------------------------------------------------------------
	// 静的メンバ変数
	//--------------------------------------------------------------------
	static CSuper *m_pTop[MAX_LEVEL];			// 先頭オブジェクトへのポインタ
	static CSuper *m_pCurrent[MAX_LEVEL];		// 現在の(一番後ろ)オブジェクトへのポインタ
	static int m_nMax;							// 使用数
	static int m_nPriorityMax[MAX_LEVEL];		// プライオリティごとのオブジェクト数
	static bool m_bPause;						// ポーズを使用しているかどうか

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CSuper *m_pPrev;				// 前のオブジェクトへのポインタ
	CSuper *m_pNext;				// 次のオブジェクトへのポインタ
	EElementType m_EElement;		// 要素
	DRAW_TYPE m_EDrawType;			// 描画方法
	int	 m_nLevPriority;			// プライオリティのレベル
	bool m_bPermanent;				// シーン生存フラグ
	bool m_bDeath;					// 死亡フラグ
};

#endif


