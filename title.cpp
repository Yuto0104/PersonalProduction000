//=============================================================================
//
// タイトルクラス(title.cpp)
// Author : 唐�ｱ結斗
// 概要 : タイトルクラスの管理を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "title.h"

#include "application.h"
#include "keyboard.h"
#include "object2D.h"
#include "sound.h"

//=============================================================================
// コンストラクタ
// Author : 唐�ｱ結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CTitle::CTitle() : m_pTitleLogo(nullptr),	// タイトルロゴオブジェクト
m_pPressEnter(nullptr),						// プレスエンターオブジェクト
m_fAddAlpha(0.0f),							// フレーム数のカウント
m_nCntFrame(0),								// フレームカウント
m_bPressEnter(true)							// エンターキーを押せるか
{

}

//=============================================================================
// デストラクタ
// Author : 唐�ｱ結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// 初期化
// Author : 唐�ｱ結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CTitle::Init()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM000);

	m_pTitleLogo = CObject2D::Create();
	m_pTitleLogo->SetPos(D3DXVECTOR3(640.0f, 280.0f, 0.0f));
	m_pTitleLogo->SetSize(D3DXVECTOR3(640.0f, 200.0f, 0.0f));
	m_pTitleLogo->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pTitleLogo->LoadTex(13);

	m_pPressEnter = CObject2D::Create();
	m_pPressEnter->SetPos(D3DXVECTOR3(640.0f, 500.0f, 0.0f));
	m_pPressEnter->SetSize(D3DXVECTOR3(400.0f, 120.0f, 0.0f));
	m_pPressEnter->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pPressEnter->LoadTex(14);

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐�ｱ結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CTitle::Uninit()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// サウンド終了
	pSound->StopSound();

	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐�ｱ結斗
// 概要 : 更新を行う
//=============================================================================
void CTitle::Update()
{
	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

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
		CApplication::SetNextMode(CApplication::MODE_GAME);
	}
}

//=============================================================================
// 描画
// Author : 唐�ｱ結斗
// 概要 : 描画を行う
//=============================================================================
void CTitle::Draw()
{

}

//=============================================================================
// オブジェクトの点滅
// Author : 唐�ｱ結斗
// 概要 : 指定のオブジェクトを点滅させる
//=============================================================================
void CTitle::FlashObj()
{
	if (m_bPressEnter)
	{
		m_fAddAlpha += 0.07f;
	}
	else if (!m_bPressEnter)
	{
		m_fAddAlpha += 0.5f;
		m_nCntFrame++;
	}

	m_pPressEnter->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, sinf(m_fAddAlpha) * 3.0f));
}