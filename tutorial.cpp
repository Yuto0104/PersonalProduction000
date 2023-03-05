//=============================================================================
//
// チュートリアルクラス(tutorial.cpp)
// Author : 唐﨑結斗
// 概要 : チュートリアルクラスの管理を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "tutorial.h"
#include "calculation.h"
#include "keyboard.h"
#include "mouse.h"
#include "application.h"
#include "camera.h"
#include "renderer.h"
#include "object.h"
#include "object3D.h"
#include "player.h"
#include "motion_model3D.h"
#include "motion_enemy.h"
#include "model3D.h"
#include "mesh.h"
#include "sphere.h"
#include "bg.h"
#include "model_obj.h"
#include "weapon_obj.h"
#include "collision_rectangle3D.h"
#include "debug_proc.h"
#include "map_manager.h"
#include "score_item.h"
#include "score.h"
#include "time.h"
#include "sound.h"
#include "object2D.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CPlayer *CTutorial::m_pPlayer = nullptr;					// プレイヤークラス
CScore *CTutorial::m_pScore = nullptr;						// スコアインスタンス
D3DXCOLOR CTutorial::fogColor;								// フォグカラー
float CTutorial::fFogStartPos;								// フォグの開始点
float CTutorial::fFogEndPos;								// フォグの終了点
float CTutorial::fFogDensity;								// フォグの密度

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CTutorial::CTutorial() : m_pTutorial(nullptr),
m_nCntFrame(0)
{

}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CTutorial::Init()
{// マウスの取得
	CMouse *pMouse = CApplication::GetMouse();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM003);

	// 重力の値を設定
	CCalculation::SetGravity(0.2f);

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

	// プレイヤーの設定
	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetMotion("data/MOTION/motion001.txt");
	m_pPlayer->SetPos(D3DXVECTOR3(724.0f, 0.0f, 2700.0f));

	// スコア
	m_pScore = CScore::Create(10, false);
	m_pScore->SetScore(0);
	m_pScore->SetPos(D3DXVECTOR3(1280.0f, m_pScore->GetSize().y / 2.0f, 0.0f));

	// カメラの追従設定(目標 : プレイヤー)
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetFollowTarget(m_pPlayer, 1.0);
	pCamera->SetPosVOffset(D3DXVECTOR3(0.0f, 50.0f, -200.0f));
	pCamera->SetPosROffset(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
	pCamera->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	pCamera->SetUseRoll(true, true);

	// マウスカーソルを消す
	pMouse->SetShowCursor(false);

	// フォグの数値設定
	fogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// フォグカラー
	fFogStartPos = 1.0f;								// フォグの開始点
	fFogEndPos = 100000.0f;								// フォグの終了点
	fFogDensity = 0.00001f;								// フォグの密度

	// フォグの有効設定
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	// フォグカラーの設定
	pDevice->SetRenderState(D3DRS_FOGCOLOR, fogColor);

	// フォグの範囲設定
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fFogStartPos));
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fFogEndPos));

	// フォグの密度の設定
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&fFogDensity));

	m_pTutorial = CObject2D::Create();
	m_pTutorial->SetPos(D3DXVECTOR3(1050.0f, 320.0f, 0.0f));
	m_pTutorial->SetSize(D3DXVECTOR3(500.0f, 500.0f, 0.0f));
	m_pTutorial->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pTutorial->LoadTex(22);

	m_pTutorialEnd = CObject2D::Create();
	m_pTutorialEnd->SetPos(D3DXVECTOR3(1050.0f, 600.0f, 0.0f));
	m_pTutorialEnd->SetSize(D3DXVECTOR3(500.0f, 50.0f, 0.0f));
	m_pTutorialEnd->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pTutorialEnd->LoadTex(24);

	m_bTutoItem = false;

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CTutorial::Uninit()
{// マウスの取得
	CMouse *pMouse = CApplication::GetMouse();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// サウンド終了
	pSound->StopSound();

	// フォグの有効設定
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// マウスカーソルを出す
	pMouse->SetShowCursor(true);

	// カメラの追従設定
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetFollowTarget(false);
	pCamera->SetTargetPosR(false);

	// カメラの追従設定
	pCamera = CApplication::GetMapCamera();
	pCamera->SetFollowTarget(false);
	pCamera->SetTargetPosR(false);

	// 静的変数の初期化
	if (m_pPlayer != nullptr)
	{
		m_pPlayer = nullptr;
	}

	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CTutorial::Update()
{
	// カメラの追従設定
	CCamera *pCamera = nullptr;

	// キーボードの取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

#ifdef _DEBUG

	// カメラの追従設定
	pCamera = CApplication::GetCamera();

	if (pKeyboard->GetTrigger(DIK_F10))
	{
		pCamera->Shake(60, 50.0f);
	}

	if (pKeyboard->GetPress(DIK_LSHIFT))
	{
		pCamera->Zoom();
	}

#endif // _DEBUG

	if (m_bTutoItem)
	{
		m_nCntFrame--;
		m_pTutorial->SetSize(D3DXVECTOR3(500.0f, 300.0f, 0.0f));
		m_pTutorial->LoadTex(23);

		if (m_nCntFrame <= 0)
		{
			m_nCntFrame = 0;
			m_bTutoItem = false;
		}
	}
	else
	{
		m_pTutorial->SetSize(D3DXVECTOR3(500.0f, 500.0f, 0.0f));
		m_pTutorial->LoadTex(22);
	}

	if (pKeyboard->GetTrigger(DIK_F3))
	{
		CApplication::SetNextMode(CApplication::MODE_TITLE);
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CTutorial::Draw()
{

}

//=============================================================================
// アイテムのチュートリアル
// Author : 唐﨑結斗
// 概要 : アイテムのチュートリアルを使用する
//=============================================================================
void CTutorial::SetTutoItem(bool bTutoItem)
{
	m_bTutoItem = bTutoItem;
	m_nCntFrame = 300;
}
