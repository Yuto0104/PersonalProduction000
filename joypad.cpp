//=============================================================================
//
// ジョイパッドクラス(joypad.cpp)
// Author : 唐﨑結斗
// Author : 磯江寿希亜
// 概要 : ジョイパッドクラスの管理を行う
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include <assert.h>
#include "joypad.h"
#include "debug_proc.h"

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CJoypad::CJoypad()
{
	m_pInput = nullptr;			// DirectInputオブジェクトへのポインタ
	m_pDevJoypad = nullptr;		// 入力デバイス（キーボード（コントローラー用は別に作る））へのポインタ
	m_pJoyPad = nullptr;		// ジョイパッド情報
	m_nMax = 0;					// コントローラーの使用可能数
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CJoypad::~CJoypad()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CJoypad::Init(const int nMax)
{
	// XInputのステートを設定（有効にする）
	XInputEnable(true);

	// コントローラーの最大数
	m_nMax = nMax;

	// メモリの確保
	m_pJoyPad = new JOYPAD[m_nMax];
	assert(m_pJoyPad != nullptr);

	for (int nCnt = 0; nCnt < m_nMax; nCnt++)
	{
		//メモリーのクリア
		memset(&m_pJoyPad[nCnt].press, 0, sizeof(XINPUT_STATE));
		memset(&m_pJoyPad[nCnt].trigger, 0, sizeof(XINPUT_STATE));

		// ジョイパッドの振動制御の0クリア
		memset(&m_pJoyPad[nCnt].vibration, 0, sizeof(XINPUT_VIBRATION));

		// 振動制御用の初期化
		m_pJoyPad[nCnt].wStrength = 0;
		m_pJoyPad[nCnt].nTime = 0;
	}
	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CJoypad::Uninit(void)
{
	//XInputのステートを設定（無効にする）
	XInputEnable(false);

	if (m_pJoyPad != nullptr)
	{
		delete[] m_pJoyPad;
		m_pJoyPad = nullptr;
	}
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CJoypad::Update(void)
{
	//ジョイパッド入力情報
	XINPUT_STATE *pJoyKeyState = new XINPUT_STATE[m_nMax];	

	// 使用数のカウント
	int nCntUse = 0;

	for (int nCnt = 0; nCnt < m_nMax; nCnt++)
	{
		//ジョイパッドの状態を取得
		if (XInputGetState(nCnt, &pJoyKeyState[nCnt]) == ERROR_SUCCESS)
		{
			// トリガー情報を保存
			m_pJoyPad[nCnt].trigger.Gamepad.wButtons
				= ~m_pJoyPad[nCnt].press.Gamepad.wButtons
				& pJoyKeyState[nCnt].Gamepad.wButtons;		

			// プレス処理
			m_pJoyPad[nCnt].press = pJoyKeyState[nCnt];

			// 使用数の増加
			nCntUse++;
		}

		//ジョイパッドの振動
		m_pJoyPad[nCnt].vibration.wLeftMotorSpeed = m_pJoyPad[nCnt].wStrength;
		m_pJoyPad[nCnt].vibration.wRightMotorSpeed = m_pJoyPad[nCnt].wStrength;
		XInputSetState(nCnt, &m_pJoyPad[nCnt].vibration);

		if (m_pJoyPad[nCnt].nTime > 0)
		{
			m_pJoyPad[nCnt].nTime--;
		}
		else
		{
			m_pJoyPad[nCnt].wStrength = 0;
			m_pJoyPad[nCnt].nTime = 0;
		}
	}

	// 使用しているコントローラー数の更新
	m_nUseJoyPad = nCntUse;

	if (pJoyKeyState != nullptr)
	{
		delete[] pJoyKeyState;
		pJoyKeyState = nullptr;
	}
}

//=============================================================================
// プレス処理
// Author : 唐﨑結斗
// 概要 : プレス入力計算を行い結果を返す
//=============================================================================
bool CJoypad::GetPress(JOYKEY Key, int nPlayer)
{
	return (m_pJoyPad[nPlayer].press.Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//=============================================================================
// トリガー処理
// Author : 唐﨑結斗
// 概要 : トリガー入力計算を行い結果を返す
//=============================================================================
bool CJoypad::GetTrigger(JOYKEY Key, int nPlayer)
{
	return (m_pJoyPad[nPlayer].trigger.Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//=============================================================================
// スティック入力処理
// Author : 唐﨑結斗
// 概要 : スティック入力計算を行い結果を返す
//=============================================================================
D3DXVECTOR3 CJoypad::GetStick(JOYKEY Key, int nPlayer)
{
	switch (Key)
	{
	case JOYKEY_LEFT_STICK:
		m_pJoyPad[nPlayer].joyStickPos = D3DXVECTOR3(m_pJoyPad[nPlayer].press.Gamepad.sThumbLX / 32767.0f, - m_pJoyPad[nPlayer].press.Gamepad.sThumbLY / 32767.0f, 0.0f);

		break;
	case JOYKEY_RIGHT_STICK:
		m_pJoyPad[nPlayer].joyStickPos = D3DXVECTOR3(m_pJoyPad[nPlayer].press.Gamepad.sThumbRX / 32767.0f, - m_pJoyPad[nPlayer].press.Gamepad.sThumbRY / 32767.0f, 0.0f);
		break;
	}

	return m_pJoyPad[nPlayer].joyStickPos;
}


//=============================================================================
// スティックプレス処理
// Author : 唐﨑結斗
// 概要 : スティック入力計算を行い結果を返す
//=============================================================================
bool CJoypad::GetStickPress(JOYKEY Key, bool RightLeft, int nPlayer)
{
	JOYKEY stick = JOYKEY_LEFT_STICK;

	if (RightLeft)
	{// 右
		stick = JOYKEY_RIGHT_STICK;
	}
	else
	{// 左
		stick = JOYKEY_LEFT_STICK;
	}


	D3DXVECTOR3 pos = GetStick(stick, nPlayer);	//現在の傾きの取得
	pos.y *= -1.0f;//Yを分かりやすくするために＋－を反転

	if (pos.y > 0.5f
		&& JOYKEY_UP == Key)
	{
		return true;
	}
	else if (pos.x > 0.5f
		&& JOYKEY_RIGHT == Key)
	{
		return true;
	}
	else if (pos.y < -0.5f
		&& JOYKEY_DOWN == Key)
	{
		return true;
	}
	else if (pos.x < -0.5f
		&& JOYKEY_LEFT == Key)
	{
		return true;
	}

	return false;
}

//=============================================================================
// スティック入力処理
// Author : 唐﨑結斗
// 概要 : スティック入力計算を行い結果を返す
//=============================================================================
bool CJoypad::GetStickTrigger(JOYKEY Key, bool RightLeft, int nPlayer)
{
	if (GetStickPress(Key, RightLeft, nPlayer)
		&& Key != m_oldKey[nPlayer][RightLeft])
	{
		m_oldKey[nPlayer][RightLeft] = Key;
		return true;
	}
	else if (GetStickPress(Key, RightLeft, nPlayer)
		&& Key == m_oldKey[nPlayer][RightLeft])
	{
		return false;
	}
	else if (!GetStickPress(Key, RightLeft, nPlayer)
		&& Key != m_oldKey[nPlayer][RightLeft])
	{
		return false;
	}

	m_oldKey[nPlayer][RightLeft] = JOYKEY_MAX;
	return false;
}

//=============================================================================
// ペダル入力処理
// Author : 唐﨑結斗
// 概要 : ペダル入力計算を行い結果を返す
//=============================================================================
int  CJoypad::GetTriggerPedal(JOYKEY Key, int nPlayer)
{
	int nJoypadTriggerPedal = 0;
	switch (Key)
	{
	case JOYKEY_LEFT_TRIGGER:
		nJoypadTriggerPedal = m_pJoyPad[nPlayer].press.Gamepad.bLeftTrigger;
		break;
	case JOYKEY_RIGHT_TRIGGER:
		nJoypadTriggerPedal = m_pJoyPad[nPlayer].press.Gamepad.bRightTrigger;
		break;
	}

	return nJoypadTriggerPedal;
}

//=============================================================================
// 振動出力処理
// Author : 唐﨑結斗
// 概要 : 振動出力計算を行い結果を返す
//=============================================================================
void  CJoypad::Vibration(int nTime, WORD nStrength, int nPlayer)
{
	m_pJoyPad[nPlayer].nTime = nTime;			// 振動持続時間
	m_pJoyPad[nPlayer].wStrength = nStrength;	// 振動の強さ
}

//=============================================================================
// ジョイスティックの傾き取得処理
// Author : 唐﨑結斗
// 概要 : ジョイスティックの傾き取得を返す
//=============================================================================
float CJoypad::GetStickAngle(JOYKEY Key, int nPlayer)
{
	switch (Key)
	{
	case JOYKEY_LEFT_STICK:
		// スティックの角度
		m_pJoyPad[nPlayer].fStickAngle = atan2f(m_pJoyPad[nPlayer].press.Gamepad.sThumbLX / 32767.0f, m_pJoyPad[nPlayer].press.Gamepad.sThumbLY / 32767.0f);

		break;
	case JOYKEY_RIGHT_STICK:
		m_pJoyPad[nPlayer].fStickAngle = atan2f(m_pJoyPad[nPlayer].press.Gamepad.sThumbRX / 32767.0f, m_pJoyPad[nPlayer].press.Gamepad.sThumbRY / 32767.0f);
		break;
	}
	
	return m_pJoyPad[nPlayer].fStickAngle;
}

//=============================================================================
// ジョイスティックの使用状況取得処理
// Author : 唐﨑結斗
// 概要 : ジョイスティックの使用状況取を返す
//=============================================================================
bool CJoypad::Stick(JOYKEY Key, int nPlayer, float fDeadZone)
{
	// 変数宣言
	bool bStick = false;
	D3DXVECTOR2 stickVec;

	switch (Key)
	{
	case JOYKEY_LEFT_STICK:
		// スティックの傾きの大きさの取得
		stickVec = D3DXVECTOR2(-m_pJoyPad[nPlayer].press.Gamepad.sThumbLX / 32767.0f, -m_pJoyPad[nPlayer].press.Gamepad.sThumbLY / 32767.0f);
		break;

	case JOYKEY_RIGHT_STICK:
		// スティックの傾きの大きさの取得
		stickVec = D3DXVECTOR2(-m_pJoyPad[nPlayer].press.Gamepad.sThumbRX / 32767.0f, -m_pJoyPad[nPlayer].press.Gamepad.sThumbRY / 32767.0f);
		break;
	}

	if (D3DXVec2Length(&stickVec) >= fDeadZone)
	{
		bStick = true;
	}

#ifdef _DEBUG
	CDebugProc::Print("傾きの大きさ : %f\n", D3DXVec2Length(&stickVec));
#endif // _DEBUG


	return bStick;
}

//=============================================================================
// すべてのボタンの判定取得
// Author : 唐﨑結斗
// 概要 : どこかのボタンが押されたらtrueを返す
//=============================================================================
bool CJoypad::AnyButton(int nPlayer)
{
	bool bAnyButton = false;

	for (int nCntButton = 0; nCntButton < JOYKEY_MAX; nCntButton++)
	{
		bAnyButton = (m_pJoyPad[nPlayer].press.Gamepad.wButtons & (0x01 << nCntButton)) ? true : false;

		if (bAnyButton)
		{
			break;
		}
	}

	return bAnyButton;
}
