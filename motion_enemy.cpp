//=============================================================================
//
// プレイヤークラス(player.cpp)
// Author : 唐﨑結斗
// 概要 : プレイヤー生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "motion_enemy.h"
#include "game.h"
#include "mesh.h"
#include "motion.h"
#include "renderer.h"
#include "application.h"
#include "calculation.h"
#include "move.h"
#include "debug_proc.h"
#include "collision_rectangle3D.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : モーションキャラクター3Dを生成する
//=============================================================================
CMotionEnemy * CMotionEnemy::Create()
{
	// オブジェクトインスタンス
	CMotionEnemy *pPlayer = nullptr;

	// メモリの解放
	pPlayer = new CMotionEnemy;

	// メモリの確保ができなかった
	assert(pPlayer != nullptr);

	// 数値の初期化
	pPlayer->Init();

	// インスタンスを返す
	return pPlayer;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CMotionEnemy::CMotionEnemy() : m_pMove(nullptr),
m_pCollRectangle3D(nullptr),
m_EAction(NEUTRAL_ACTION),
m_rotDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_fSpeed(0.0f),
m_nNumMotion(0)
{

}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CMotionEnemy::~CMotionEnemy()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CMotionEnemy::Init()
{
	// 初期化
	CMotionModel3D::Init();

	// 移動クラスのメモリ確保
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(1.0f, 5.0f, 0.5f, 0.1f);

	// 3D矩形の当たり判定の設定
	m_pCollRectangle3D = CCollision_Rectangle3D::Create();
	m_pCollRectangle3D->SetParent(this);
	m_pCollRectangle3D->SetPos(D3DXVECTOR3(0.0f, 25.0f, 0.0f));
	m_pCollRectangle3D->SetSize(D3DXVECTOR3(30.0f, 50.0f, 20.0f));

	return E_NOTIMPL;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CMotionEnemy::Uninit()
{
	if (m_pMove != nullptr)
	{// メモリの解放
		delete m_pMove;
		m_pMove = nullptr;
	}

	if (m_pCollRectangle3D != nullptr)
	{// 終了処理
		m_pCollRectangle3D->Uninit();
		m_pCollRectangle3D = nullptr;
	}

	// 終了
	CMotionModel3D::Uninit();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CMotionEnemy::Update()
{
	// モーション情報の取得
	CMotion *pMotion = CMotionModel3D::GetMotion();

	// 位置の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 移動
	pos += Move();

	pos.y -= CCalculation::Gravity();

	// 回転
	Rotate();

	// ニュートラルモーションの設定
	if (pMotion != nullptr
		&& !pMotion->GetMotion())
	{
		pMotion->SetNumMotion(NEUTRAL_ACTION);
		m_EAction = NEUTRAL_ACTION;
	}

	// 位置の設定
	SetPos(pos);

	// メッシュの当たり判定
	CMesh3D *pMesh = CGame::GetMesh();
	pMesh->Collison(this);

	// 更新
	CMotionModel3D::Update();
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void CMotionEnemy::Draw()
{
	// 描画
	CMotionModel3D::Draw();
}

//=============================================================================
// 移動
// Author : 唐﨑結斗
// 概要 : 移動ベクトルを算出する
//=============================================================================
D3DXVECTOR3 CMotionEnemy::Move()
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// 回転
// Author : 唐﨑結斗
// 概要 : 目的の向きまで回転する
//=============================================================================
void CMotionEnemy::Rotate()
{
	// 向きの取得
	D3DXVECTOR3 rot = GetRot();

	// 向きの更新
	rot.y += (m_rotDest.y - rot.y) * 0.5f;

	// 向きの正規化
	rot.y = CCalculation::RotNormalization(rot.y);

	// 向きの設定
	SetRot(rot);
}


