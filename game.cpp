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
#include "wire.h"
#include "map_manager.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CPlayer *CGame::m_pPlayer = nullptr;					// プレイヤークラス
CMesh3D *CGame::m_pMesh3D;								// メッシュクラス
CMotionEnemy *CGame::m_pMotionModel3D;					// モーションモデルクラス
CWire *CGame::m_pWire;
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
	pSphere->SetRadius(50000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));

	// プレイヤーの設定
	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetMotion("data/MOTION/motion.txt");
	m_pPlayer->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 敵の設置
	m_pMotionModel3D = CMotionEnemy::Create();
	m_pMotionModel3D->SetMotion("data/MOTION/motion.txt");
	m_pMotionModel3D->SetPos(D3DXVECTOR3(100.0f, 0.0f, 0.0f));
	CMotionEnemy *pMotionModel3D = CMotionEnemy::Create();
	pMotionModel3D->SetMotion("data/MOTION/motion.txt");
	pMotionModel3D->SetPos(D3DXVECTOR3(-100.0f, 0.0f, 0.0f));

	// カメラの追従設定(目標 : プレイヤー)
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetFollowTarget(m_pPlayer, 1.0);
	pCamera->SetPosVOffset(D3DXVECTOR3(0.0f, 0.0f, -500.0f));
	pCamera->SetPosROffset(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
	pCamera->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// カメラの追従設定(目標 : プレイヤー)
	pCamera = CApplication::GetMapCamera();
	pCamera->SetFollowTarget(m_pPlayer, 1.0);
	pCamera->SetPosVOffset(D3DXVECTOR3(0.0f, 1000.0f, 0.0f));
	pCamera->SetPosROffset(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pCamera->SetViewSize(0, 0, CRenderer::SCREEN_WIDTH / 4, CRenderer::SCREEN_HEIGHT / 4);

	// 武器のの設置
	CWeaponObj *pWeapon = CWeaponObj::Create();
	pWeapon->SetPos(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
	pWeapon->SetType(11);
	pWeapon->SetWeaponType(CWeaponObj::WEAPONTYPE_KNIFE);
	pWeapon->SetAttack(5);
	CCollision_Rectangle3D *pCollision = pWeapon->GetCollision();
	pCollision->SetSize(D3DXVECTOR3(10.0f, 36.0f, 10.0f));
	pCollision->SetPos(D3DXVECTOR3(0.0f, 18.0f, 0.0f));

	// ワイヤー
	m_pWire = CWire::Create();
	m_pWire->SetPos(D3DXVECTOR3(0.0f, 10.0f, 200.0f));

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

	// フォグの有効設定
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// マウスカーソルを出す
	pMouse->SetShowCursor(true);

	// カメラの追従設定
	CCamera *pCamera = CApplication::GetCamera();
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

	if (m_pWire != nullptr)
	{
		m_pWire->Uninit();
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
#ifdef _DEBUG
	CDebugProc::Print("敵の体力 : %d\n", m_pMotionModel3D->GetLife());

	// キーボードの取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// カメラの追従設定
	CCamera *pCamera = CApplication::GetCamera();

	if (pKeyboard->GetTrigger(DIK_F3))
	{
		CApplication::SetNextMode(CApplication::MODE_RESULT);
	}
	if (pKeyboard->GetTrigger(DIK_F4))
	{
		CCamera *pCamera = CApplication::GetCamera();

		if (!pCamera->GetTargetPosR())
		{
			pCamera->SetTargetPosR(m_pMotionModel3D);
			pCamera->SetPosVOffset(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
			pCamera->SetPosRDiff(D3DXVECTOR2(100.0f, 200.0f));
		}
		else
		{
			pCamera->SetTargetPosR(false);
		}
	}
	if (pKeyboard->GetTrigger(DIK_F5))
	{
		D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		switch (m_pWire->GetWireMode())
		{
		case CWire::MODE_STOP:
			m_pWire->SetWireMode(CWire::MODE_FIRING);
			vec = pCamera->GetPosV() - pCamera->GetPosR();
			break;

		case CWire::MODE_FIRING:
			m_pWire->SetWireMode(CWire::MODE_ATTRACT);
			pCamera->SetFollowTarget(m_pWire->GetStart(), 1.0);
			vec = m_pWire->GetStart()->GetPos() - m_pWire->GetGoal()->GetPos();
			break;

		case CWire::MODE_ATTRACT:
			m_pWire->SetWireMode(CWire::MODE_FIRING);
			vec = pCamera->GetPosV() - pCamera->GetPosR();
			break;

		default:
			break;
		}
		
		D3DXVec3Normalize(&vec, &vec);
		m_pWire->SetMoveVec(-vec);
	}

	if (pKeyboard->GetPress(DIK_LSHIFT))
	{
		pCamera->Zoom();
	}

#endif // _DEBUG

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
