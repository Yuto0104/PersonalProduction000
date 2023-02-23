//=============================================================================
//
// ポーズクラス(pause.h)
// Author : 唐﨑結斗
// 概要 : ポーズ生成を行う
//
//=============================================================================
#ifndef _PAUSE_H_		// このマクロ定義がされてなかったら
#define _PAUSE_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObject2D;

//=============================================================================
// ポーズクラス
// Author : 唐﨑結斗
// 概要 : ポーズ生成を行うクラス
//=============================================================================
class CPause : public CObject
{
public:
	//--------------------------------------------------------------------
	// モードの列挙型
	// Author : 唐﨑結斗
	// 概要 : モードを識別する
	//--------------------------------------------------------------------
	enum NEXT_MODE
	{
		MODE_RETURN = 0,		// リターン
		MODE_GAME,				// ゲーム
		MODE_TITLE,				// タイトル
		MAX_MODE,				// シーンの最大数
		MODE_NONE,				// シーン無し
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CPause *Create();		// ポーズの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CPause(int nPriority = CObject::PRIORITY_LEVEL3);
	~CPause() override;

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
	void SetColor(const D3DXCOLOR &col);											// 色の設定
	void SetPause(const bool bPause);												// ポーズの使用状況
	D3DXVECTOR3 GetPos() override { return m_pos; }									// 位置のゲッター
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }							// 過去位置のゲッター
	D3DXVECTOR3 GetRot()  override { return m_rot; }								// 向きのゲッター
	D3DXVECTOR3 GetSize()  override { return m_size; }								// 大きさのゲッター
	bool GetPause() { return m_bPause; }											// ポーズの使用状況のゲッター

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void FlashObj();
	void SelectMode();

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	NEXT_MODE		m_nextMode;			// 次のモード
	CObject2D		*m_pPauseBGObj;		// ポーズ背景オブジェクト
	CObject2D		*m_pReturnObj;		// リターンオブジェクト
	CObject2D		*m_pNewGameObj;		// ニューゲームオブジェクト
	CObject2D		*m_pTitleObj;		// タイトルオブジェクト
	D3DXVECTOR3		m_pos;				// 位置
	D3DXVECTOR3		m_posOld;			// 過去の位置
	D3DXVECTOR3		m_rot;				// 向き
	D3DXVECTOR3		m_size;				// 大きさ
	float			m_fAddAlpha;		// フレーム数のカウント
	int				m_nCntFrame;		// フレームカウント
	bool			m_bPressEnter;		// エンターキーを押せるか
	bool			m_bPause;			// ポーズしているか
};

#endif


