//=============================================================================
//
// ポーズクラス(pause.cpp)
// Author : 唐﨑結斗
// 概要 : ポーズ生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "pause.h"
#include "object2D.h"
#include "renderer.h"
#include "application.h"
#include "keyboard.h"
#include "sound.h"
#include "fade.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : スコアを生成する
//=============================================================================
CPause *CPause::Create()
{
	// オブジェクトインスタンス
	CPause *pPause = nullptr;

	pPause = new CPause;

	// メモリの確保ができなかった
	assert(pPause != nullptr);

	// 数値の初期化
	pPause->Init();

	// インスタンスを返す
	return pPause;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CPause::CPause(int nPriority /*= CObject::PRIORITY_LEVEL3*/) : CObject(nPriority)
{
	m_nextMode = MODE_RETURN;								// 次のモード
	m_pPauseBGObj = nullptr;							// ポーズ背景オブジェクト
	m_pReturnObj = nullptr;								// リターンオブジェクト
	m_pNewGameObj = nullptr;							// ニューゲームオブジェクト
	m_pTitleObj = nullptr;								// タイトルオブジェクト
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 過去の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大き
	m_fAddAlpha = 0.0f;									// フレーム数のカウント
	m_nCntFrame = 0;									// フレームカウント
	m_bPressEnter = true;								// エンターキーを押せるかさ
	m_bPause = false;									// ポーズしているか
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CPause::Init()
{
	m_pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				
	m_size = D3DXVECTOR3(400.0f, 400.0f, 0.0f);
	SetObjType(CObject::OBJTYPE_PAUSE);

	// 生存フラグ
	SetPermanent(true);

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CPause::Uninit()
{
	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CPause::Update()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	if (CApplication::GetMode() == CApplication::MODE_GAME
		&& !CApplication::GetFade()->GetFadeSituation())
	{
		if (m_bPause)
		{
			if (m_bPressEnter)
			{
				SelectMode();
			}

			FlashObj();

			if (m_bPressEnter
				&& pKeyboard->GetTrigger(DIK_RETURN))
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
				m_bPressEnter = false;
			}

			if (!m_bPressEnter
				&& m_nCntFrame >= 40)
			{
				m_bPressEnter = true;
				m_nCntFrame = 0;

				switch (m_nextMode)
				{
				case MODE_RETURN:
					SetPause(false);
					break;

				case MODE_GAME:
					SetPause(false);
					CApplication::SetNextMode(CApplication::MODE_GAME);
					break;

				case MODE_TITLE:
					SetPause(false);
					CApplication::SetNextMode(CApplication::MODE_TITLE);
					break;

				default:
					assert(false);
					break;
				}
			}

			if (pKeyboard->GetTrigger(DIK_P))
			{
				SetPause(false);
			}

		}
		else if (!m_bPause
			&& pKeyboard->GetTrigger(DIK_P))
		{
			SetPause(true);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE);
		}
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CPause::Draw()
{

}

//=============================================================================
// 位置のセッター
// Author : 唐﨑結斗
// 概要 : 位置のメンバ変数に引数を代入
//=============================================================================
void CPause::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}

//=============================================================================
// 向きのセッター
// Author : 唐﨑結斗
// 概要 : 向きのメンバ変数に引数を代入
//=============================================================================
void CPause::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;
}

//=============================================================================
// 大きさのセッター
// Author : 唐﨑結斗
// 概要 : 大きさのメンバ変数に引数を代入
//=============================================================================
void CPause::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;
}

//=============================================================================
// 色のセッター
// Author : 唐﨑結斗
// 概要 : 色の設定を行う
//=============================================================================
void CPause::SetColor(const D3DXCOLOR & col)
{
	
}

//=============================================================================
// ポーズのセッター
// Author : 唐﨑結斗
// 概要 : ポーズの設定を行う
//=============================================================================
void CPause::SetPause(const bool bPause)
{
	m_bPause = bPause;

	if (m_bPause)
	{
		// ポーズ背景オブジェクト
		m_pPauseBGObj = CObject2D::Create();
		m_pPauseBGObj->SetSize(m_size);
		m_pPauseBGObj->SetPos(m_pos);
		m_pPauseBGObj->LoadTex(19);
		m_pPauseBGObj->SetCol(D3DXCOLOR(0.7f, 0.7f, 0.2f, 0.5f));
		m_pPauseBGObj->SetObjType(CObject::OBJTYPE_PAUSE);

		// リターンオブジェクト
		m_pReturnObj = CObject2D::Create();					
		m_pReturnObj->SetSize(D3DXVECTOR3(300.0f, 100.0f, 0.0f));
		m_pReturnObj->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y - m_pReturnObj->GetSize().y - 20.0f, 0.0f));
		m_pReturnObj->LoadTex(18);
		m_pReturnObj->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pReturnObj->SetObjType(CObject::OBJTYPE_PAUSE);

		// ニューゲームオブジェクト
		m_pNewGameObj = CObject2D::Create();
		m_pNewGameObj->SetPos(m_pos);
		m_pNewGameObj->SetSize(D3DXVECTOR3(400.0f, 100.0f, 0.0f));
		m_pNewGameObj->LoadTex(15);
		m_pNewGameObj->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pNewGameObj->SetObjType(CObject::OBJTYPE_PAUSE);

		// タイトルオブジェクト
		m_pTitleObj = CObject2D::Create();
		m_pTitleObj->SetSize(D3DXVECTOR3(300.0f, 100.0f, 0.0f));
		m_pTitleObj->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + m_pTitleObj->GetSize().y + 20.0f, 0.0f));
		m_pTitleObj->LoadTex(16);
		m_pTitleObj->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pTitleObj->SetObjType(CObject::OBJTYPE_PAUSE);
	}
	else
	{
		// ポーズ背景オブジェクト
		m_pPauseBGObj->Uninit();

		// リターンオブジェクト
		m_pReturnObj->Uninit();

		// ニューゲームオブジェクト
		m_pNewGameObj->Uninit();

		// タイトルオブジェクト
		m_pTitleObj->Uninit();
	}

	CSuper::SetPause(m_bPause);
}

//=============================================================================
// オブジェクトの点滅
// Author : 唐﨑結斗
// 概要 : 指定のオブジェクトを点滅させる
//=============================================================================
void CPause::FlashObj()
{
	CObject2D *pObj = nullptr;

	if (m_bPressEnter)
	{
		m_fAddAlpha += 0.07f;
	}
	else if (!m_bPressEnter)
	{
		m_fAddAlpha += 0.5f;
		m_nCntFrame++;
	}

	switch (m_nextMode)
	{
	case MODE_RETURN:
		pObj = m_pReturnObj;
		m_pNewGameObj->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pTitleObj->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case MODE_GAME:
		pObj = m_pNewGameObj;
		m_pReturnObj->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pTitleObj->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case MODE_TITLE:
		pObj = m_pTitleObj;
		m_pReturnObj->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pNewGameObj->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	default:
		assert(false);
		break;
	}

	pObj->SetCol(D3DXCOLOR(0.7f, 0.7f, 0.7f, sinf(m_fAddAlpha) * 3.0f));
}

//=============================================================================
// モードの選択
// Author : 唐﨑結斗
// 概要 : モードの選択する
//=============================================================================
void CPause::SelectMode()
{
	int nMode = (int)m_nextMode;

	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_W))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
		nMode--;

		if (nMode < 0)
		{
			nMode = 2;
		}
	}
	else if (pKeyboard->GetTrigger(DIK_S))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
		nMode++;

		if (nMode > 2)
		{
			nMode = 0;
		}
	}

	m_nextMode = (NEXT_MODE)nMode;
}

