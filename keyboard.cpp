//=============================================================================
//
// 入力処理(input.cpp)
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================

//***************************************************************************
// インクルード
//***************************************************************************
#include <assert.h>
#include <stdio.h>

#include "keyboard.h"

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CKeyboard::CKeyboard()
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		m_aKeyState[nCntKey] = NULL;
		m_aKeyStateTrigger[nCntKey] = NULL;
		m_aKeyStateRelease[nCntKey] = NULL;
	}
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CKeyboard::~CKeyboard()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : キーボードのアクセス権を獲得できた場合、処理を継続する
//=============================================================================
HRESULT CKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 親クラスのメンバ変数を取得
	LPDIRECTINPUTDEVICE8 pDevice = GetDevice();
	LPDIRECTINPUT8 pInput = GetInput();

	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pInput, NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(pInput->CreateDevice(GUID_SysKeyboard, &pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// Deviceへのアクセス権を獲得
	pDevice->Acquire();

	// Deviceの設定
	SetDevice(pDevice);

	// インプットの設定
	SetInput(pInput);

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : キーボードのアクセス権を解放し、デバイスとオブジェクトを解放する
//=============================================================================
void CKeyboard::Uninit(void)
{
	// インプットの終了
	CInput::Uninit();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : キーボードの入力された情報を保存する
//=============================================================================
void CKeyboard::Update(void)
{
	// 親クラスのメンバ変数を取得
	LPDIRECTINPUTDEVICE8 pDevice = GetDevice();

	//変数宣言
	BYTE aKeyState[NUM_KEY_MAX];	//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			// キーボードのトリガー情報を保存
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			// キーボードのリリース情報を保存
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];

			// キーボードのプレス情報を保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		// キーボードへのアクセス権を獲得
		pDevice->Acquire();
	}

	// Deviceの設定
	SetDevice(pDevice);
}

//=============================================================================
//  キーボードプレス情報の取得
// Author : 唐﨑結斗
// 概要 : 引数のキーの情報を返す
//=============================================================================
bool CKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードトリガー情報の取得
// Author : 唐﨑結斗
// 概要 : 引数のキーの情報を返す
//=============================================================================
bool CKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードリリース情報の取得
// Author : 唐﨑結斗
// 概要 : 引数のキーの情報を返す
//=============================================================================
bool CKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

