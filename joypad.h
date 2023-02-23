//=============================================================================
//
// ジョイパッドクラス(joypad.h)
// Author : 唐﨑結斗
// Author : 磯江寿希亜
// 概要 : ジョイパッドクラスの管理を行う
//
//=============================================================================
#ifndef _JOYPAD_H_		//このマクロ定義がされなかったら
#define _JOYPAD_H_		//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
//ライブラリーファイルのリンク
//----------------------------------------------------------------------------
#pragma comment(lib,"dinput8.lib")		// 入力処理に必要(キーボード)
#pragma comment(lib,"xinput.lib")		// 入力処理に必要（ジョイパット）
#pragma comment(lib,"dxguid.lib")		// DirectXコンポーネントに使用

//----------------------------------------------------------------------------
//インクルード
//----------------------------------------------------------------------------
#include <Windows.h>
#pragma push_macro("new")
#undef new
#include <d3dx9.h>
#pragma pop_macro("new")
#define	 DIRECTINPUT_VERSION (0x0800)	// ビルド時の警告対策用マクロ(この位置から動かさない)
#include <Xinput.h>						// ジョイパット処理
#include <dinput.h>						// 入力処理に必要

//=============================================================================
// ジョイパッドクラス
// Author : 唐﨑結斗
// Author : 磯江寿希亜
// 概要 : ジョイパッドクラス生成を行うクラス
//=============================================================================
class CJoypad
{
public:
	//--------------------------------------------------------------------
	// キー種類の列挙型
	//--------------------------------------------------------------------
	typedef enum
	{
		JOYKEY_UP = 0,				// 十字ボタン上
		JOYKEY_DOWN,				// 十字ボタン下
		JOYKEY_LEFT,				// 十字ボタン左
		JOYKEY_RIGHT,				// 十字ボタン右
		JOYKEY_START,				// スタートボタン
		JOYKEY_BACK,				// バックボタン
		JOYKEY_LEFT_THUMB,			// 左スティック押込み
		JOYKEY_RIGHT_THUMB,			// 右スティック押込み
		JOYKEY_LEFT_SHOULDER,		// L１ボタン
		JOYKEY_RIGHT_SHOULDER,		// R１ボタン
		JOYKEY_LEFT_TRIGGER,		// L２トリガー
		JOYKEY_RIGHT_TRIGGER,		// R２トリガー
		JOYKEY_A,					// Aボタン
		JOYKEY_B,					// Bボタン
		JOYKEY_X,					// Xボタン
		JOYKEY_Y,					// Yボタン
		JOYKEY_LEFT_STICK,			// 左スティック
		JOYKEY_RIGHT_STICK,			// 右スティック
		JOYKEY_MAX
	}JOYKEY;

	//--------------------------------------------------------------------
	// ジョイパッド情報の構造体
	//--------------------------------------------------------------------
	struct JOYPAD
	{
		XINPUT_STATE		press;				// ジョイパットのプレス情報
		XINPUT_STATE		trigger;			// ジョイパットのトリガー情報
		XINPUT_VIBRATION	vibration;			// ジョイパッドのバイブレーション
		D3DXVECTOR3			joyStickPos;		// ジョイスティックの傾き
		WORD				wStrength;			// 振動の強さ (0 - 65535)
		int					nTime;				// 振動持続時間
		float				fStickAngle;		// スティックの向き
	};

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CJoypad();
	~CJoypad();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init(const int nMax);										// 初期化
	void Uninit(void);													// 終了
	void Update(void);													// 更新
	bool GetPress(JOYKEY Key, int nPlayer);								// プレス
	bool GetTrigger(JOYKEY Key, int nPlayer);							// トリガー
	D3DXVECTOR3 GetStick(JOYKEY Key, int nPlayer);						// スティック
	bool GetStickPress(JOYKEY Key, bool RightLeft, int nPlayer);		// スティックプレス
	bool GetStickTrigger(JOYKEY Key, bool RightLeft, int nPlayer);		// スティックトリガー
	int GetTriggerPedal(JOYKEY Key, int nPlayer);						// トリガーペダル
	void Vibration(int nTime, WORD nStrength, int nPlayer);				// 振動制御
	int GetUseJoyPad() { return m_nUseJoyPad; }							// 使用数の取得
	float GetStickAngle(JOYKEY Key, int nPlayer);						// スティックの傾きの取得
	bool Stick(JOYKEY Key, int nPlayer, float fDeadZone);				// スティックの使用状況の取得
	bool AnyButton(int nPlayer);										// すべてのボタンの判定取得

private:	
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	LPDIRECTINPUT8			m_pInput;					// DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8	m_pDevJoypad;				// 入力デバイス（キーボード（コントローラー用は別に作る））へのポインタ
	JOYPAD					*m_pJoyPad;					// ジョイパッド情報
	JOYKEY					m_oldKey[4][2];				//ジョイスティックの前回の傾き
	int						m_nMax;						// コントローラーの使用可能数
	int						m_nUseJoyPad;				// 使用しているコントローラー数
};

#endif