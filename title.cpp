//=============================================================================
//
// タイトルクラス(title.cpp)
// Author : 唐﨑結斗
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
#include "map_manager.h"
#include "camera.h"
#include "sphere.h"
#include "model_obj.h"

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CTitle::CTitle() : m_nextMode(MODE_GAME),	// 次に設定するモード
m_pTitleLogo(nullptr),						// タイトルロゴオブジェクト
m_pNewGame(nullptr),						// ニューゲームオブジェクト
m_pTutorial(nullptr),						// チュートリアルオブジェクト
m_pExit(nullptr),							// 終了オブジェクト
m_fAddAlpha(0.0f),							// フレーム数のカウント
m_nCntFrame(0),								// フレームカウント
m_bPressEnter(true)							// エンターキーを押せるか
{

}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CTitle::Init()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM000);

	// マップの設定
	CMapManager *pMap = CMapManager::Create();
	pMap->SetMap("data/FILE/map000.txt");

	// スカイボックスの設定
	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pSphere->SetRadius(10000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
	pSphere->LoadTex(1);

	CModelObj *pModel = CModelObj::Create();
	pModel->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pTitleLogo = CObject2D::Create();
	m_pTitleLogo->SetPos(D3DXVECTOR3(640.0f, 280.0f, 0.0f));
	m_pTitleLogo->SetSize(D3DXVECTOR3(640.0f, 200.0f, 0.0f));
	m_pTitleLogo->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pTitleLogo->LoadTex(13);

	m_pNewGame = CObject2D::Create();
	m_pNewGame->SetPos(D3DXVECTOR3(640.0f, 550.0f, 0.0f));
	m_pNewGame->SetSize(D3DXVECTOR3(300.0f, 60.0f, 0.0f));
	m_pNewGame->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pNewGame->LoadTex(15);

	m_pTutorial = CObject2D::Create();
	m_pTutorial->SetPos(D3DXVECTOR3(640.0f, 610.0f, 0.0f));
	m_pTutorial->SetSize(D3DXVECTOR3(280.0f, 50.0f, 0.0f));
	m_pTutorial->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pTutorial->LoadTex(20);

	m_pExit = CObject2D::Create();
	m_pExit->SetPos(D3DXVECTOR3(640.0f, 670.0f, 0.0f));
	m_pExit->SetSize(D3DXVECTOR3(170.0f, 50.0f, 0.0f));
	m_pExit->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pExit->LoadTex(21);

	// カメラの追従設定(目標 : プレイヤー)
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetUseRoll(false, false);
	pCamera->SetFollowTarget(pModel, 1.0);
	pCamera->SetPosVOffset(D3DXVECTOR3(0.0f, 50.0f, -9000.0f));
	pCamera->SetPosROffset(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
	pCamera->SetRot(D3DXVECTOR3(0.35f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CTitle::Uninit()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// サウンド終了
	pSound->StopSound();

	// カメラの追従設定
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetFollowTarget(false);
	pCamera->SetTargetPosR(false);

	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CTitle::Update()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	CCamera *pCamera = CApplication::GetCamera();
	D3DXVECTOR3 rot = pCamera->GetRot();
	rot.y += D3DX_PI / 180.0f * 0.1f;
	pCamera->SetRot(rot);

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
		switch (m_nextMode)
		{
		case MODE_GAME:
			CApplication::SetNextMode(CApplication::MODE_GAME);
			break;

		case MODE_TUTORIAL:
			CApplication::SetNextMode(CApplication::MODE_TUTORIAL);
			break;

		case MODE_EXIT:
			// ウィンドウを破棄
			DestroyWindow(CApplication::GetWnd());
			break;

		default:
			assert(false);
			break;
		}
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CTitle::Draw()
{

}

//=============================================================================
// オブジェクトの点滅
// Author : 唐﨑結斗
// 概要 : 指定のオブジェクトを点滅させる
//=============================================================================
void CTitle::FlashObj()
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
	case MODE_GAME:
		pObj = m_pNewGame;
		m_pTutorial->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pExit->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case MODE_TUTORIAL:
		pObj = m_pTutorial;
		m_pNewGame->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pExit->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		break;

	case MODE_EXIT:
		pObj = m_pExit;
		m_pTutorial->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pNewGame->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
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
void CTitle::SelectMode()
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
