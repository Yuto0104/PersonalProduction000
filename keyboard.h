//=============================================================================
//
// キーボード入力処理(keyboard.h)
// Author : 唐�ｱ結斗
// 概要 : キーボード生成を行う
//
//=============================================================================
#ifndef _KEYBOARD_H_		// このマクロ定義がされてなかったら
#define _KEYBOARD_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "input.h"

//=============================================================================
// キーボード入力処理
// Author : 唐�ｱ結斗
// 概要 : キーボード生成を行う
//=============================================================================
class CKeyboard : public CInput
{
public:
	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CKeyboard();
	~CKeyboard() override;

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;			// 初期化
	void Uninit(void) override;										// 終了
	void Update(void) override;										// 更新
	bool GetPress(int nKey) override;								// プレス情報の取得
	bool GetTrigger(int nKey) override;								// トリガー情報の取得
	bool GetRelease(int nKey) override;								// リリース情報の取得

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	BYTE m_aKeyState[NUM_KEY_MAX];				// プレス情報
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];		// トリガー情報
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];		// リリース情報
};

#endif

