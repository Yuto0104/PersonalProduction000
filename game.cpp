//=============================================================================
//
// ゲームクラス(game.cpp)
// Author : 唐﨑結斗
// 概要 : ゲームクラスの管理を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "game.h"
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

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CPlayer *CGame::m_pPlayer = nullptr;					// プレイヤークラス
CMesh3D *CGame::m_pMesh3D;								// メッシュクラス
CScore *CGame::m_pScore = nullptr;						// スコアインスタンス
CTime *CGame::m_pTime = nullptr;						// タイム
D3DXCOLOR CGame::fogColor;								// フォグカラー
float CGame::fFogStartPos;								// フォグの開始点
float CGame::fFogEndPos;								// フォグの終了点
float CGame::fFogDensity;								// フォグの密度
bool CGame::m_bGame = false;							// ゲームの状況

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CGame::Init()
{// マウスの取得
	CMouse *pMouse = CApplication::GetMouse();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM001);

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

	// タイム
	m_pTime = CTime::Create(3);
	m_pTime->SetTime(120);
	m_pTime->SetTimeAdd(false);
	m_pTime->SetPos(D3DXVECTOR3(640.0f, m_pTime->GetSize().y / 2.0f, 0.0f));

	// カメラの追従設定(目標 : プレイヤー)
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetFollowTarget(m_pPlayer, 1.0);
	pCamera->SetPosVOffset(D3DXVECTOR3(0.0f, 50.0f, -200.0f));
	pCamera->SetPosROffset(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
	pCamera->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	pCamera->SetUseRoll(true, true);

	// カメラの追従設定(目標 : プレイヤー)
	pCamera = CApplication::GetMapCamera();
	pCamera->SetFollowTarget(m_pPlayer, 1.0);
	pCamera->SetPosVOffset(D3DXVECTOR3(0.0f, 5000.0f, -1.0f));
	pCamera->SetPosROffset(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pCamera->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pCamera->SetViewSize(0, 0, 250, 250);
	pCamera->SetUseRoll(false, true);
	pCamera->SetAspect(D3DXVECTOR2(10000.0f, 10000.0f));

	//// 武器のの設置
	//CWeaponObj *pWeapon = CWeaponObj::Create();
	//pWeapon->SetPos(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
	//pWeapon->SetType(11);
	//pWeapon->SetWeaponType(CWeaponObj::WEAPONTYPE_KNIFE);
	//pWeapon->SetAttack(5);
	//CCollision_Rectangle3D *pCollision = pWeapon->GetCollision();
	//pCollision->SetSize(D3DXVECTOR3(10.0f, 36.0f, 10.0f));
	//pCollision->SetPos(D3DXVECTOR3(0.0f, 18.0f, 0.0f));

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

	m_bGame = true;

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CGame::Uninit()
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

	if (m_pMesh3D != nullptr)
	{
		m_pMesh3D = nullptr;
	}

	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CGame::Update()
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

	if (pKeyboard->GetTrigger(DIK_F3))
	{
		m_pTime->SetTime(0);
	}

	if (m_pTime->GetTimeEnd()
		&& m_bGame)
	{
		m_bGame = false;

		// カメラの追従設定
		pCamera = CApplication::GetCamera();
		pCamera->SetFollowTarget(false);
		pCamera = CApplication::GetMapCamera();
		pCamera->SetFollowTarget(false);

		// スコアの設定
		CApplication::SetScore(m_pScore->GetScore());
	}

	if (!m_bGame)
	{
		CApplication::SetNextMode(CApplication::MODE_RESULT);
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CGame::Draw()
{

}
