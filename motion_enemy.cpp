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
#include "camera.h"
#include "parts.h"

//--------------------------------------------------------------------
// 静的メンバ変数の宣言
//--------------------------------------------------------------------
const float CMotionEnemy::fFRICTION = 0.1f;			// 減衰係数

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
m_fGravity(0.0f),
m_fSpeed(0.0f),
m_nNumMotion(0),
m_nLife(0),
m_nCntState(0),
m_nCntAction(0),
m_bDamage(false)
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
	m_pMove->SetMoving(1.0f, 100.0f, 0.5f, fFRICTION);

	// 3D矩形の当たり判定の設定
	m_pCollRectangle3D = CCollision_Rectangle3D::Create();
	m_pCollRectangle3D->SetParent(this);
	m_pCollRectangle3D->SetPos(D3DXVECTOR3(0.0f, 25.0f, 0.0f));
	m_pCollRectangle3D->SetSize(D3DXVECTOR3(30.0f, 50.0f, 20.0f));

	// 体力の初期値
	m_nLife = 10;

	// オブジェクトタイプの設定
	SetObjType(OBJETYPE_ENEMY);

	return E_NOTIMPL;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CMotionEnemy::Uninit()
{
	// カメラの取得
	CCamera *pCamera = CApplication::GetCamera();

	if (pCamera->GetTargetPosR() == this)
	{// カメラのターゲットを終了
		pCamera->SetTargetPosR(false);
		pCamera->SetPosVOffset(D3DXVECTOR3(0.0f, 0.0f, -500.0f));
	}

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

	if (m_bDamage)
	{// 状態のカウントを減少
		m_nCntState--;

		if (m_nCntState <= 0)
		{// カウントリセット
			m_nCntState = 0;
			m_bDamage = false;

			// パーツ数の取得
			int nMaxParts = pMotion->GetMaxParts();

			for (int nCntParts = 0; nCntParts < nMaxParts; nCntParts++)
			{// パーツの取得
				CParts *pParts = pMotion->GetParts(nCntParts);
				pParts->SetColor(false);
			}
		}
	}

	m_nCntAction--;

	if (m_nCntAction <= 0)
	{
		m_nCntAction = 0;

		int nAction = rand() % MAX_ACTION;

		m_EAction = (ACTION_TYPE)nAction;


	}

	// 位置の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 重力の設定
	m_fGravity -= CCalculation::Gravity();

	// 移動
	pos.y += m_fGravity;
	pos += Move();

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
	if (CMesh3D::CollisonMesh(this))
	{
		m_fGravity = 0.0f;

		D3DXVECTOR3 move = m_pMove->GetMove();
		move.y = 0.0f;
		m_pMove->SetMove(move);
	}

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
// 攻撃を受ける
// Author : 唐﨑結斗
// 概要 : 攻撃を受け、ダメージ状態にする
//=============================================================================
void CMotionEnemy::Hit(const int nAttack)
{
	if (!m_bDamage
		&& m_nLife > 0)
	{// 状態カウンターの設定
		m_nCntState = 30;

		// ダメージフラグ
		m_bDamage = true;

		// 体力の減少
		m_nLife -= nAttack;

		if (m_nLife <= 0)
		{// 体力が0以下
			Uninit();
			return;
		}

		// モーション情報の取得
		CMotion *pMotion = GetMotion();
		int nMaxParts = pMotion->GetMaxParts();

		for (int nCntParts = 0; nCntParts < nMaxParts; nCntParts++)
		{// パーツの取得
			CParts *pParts = pMotion->GetParts(nCntParts);
			pParts->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}				
}

//=============================================================================
// 行動の設定
// Author : 唐﨑結斗
// 概要 : 行動を設定する
//=============================================================================
void CMotionEnemy::SetAction()
{
	// アクションを行う時間
	int nTimeAction = 0;

	switch (m_EAction)
	{
	case CMotionEnemy::NEUTRAL_ACTION:
		nTimeAction = rand() % (60 * 5);
		break;

	case CMotionEnemy::MOVE_ACTION:
		nTimeAction = rand() % (60 * 3);
		break;

	default:
		assert(false);
		break;
	}

	// カウントの更新
	m_nCntAction = nTimeAction;


}

//=============================================================================
// 移動
// Author : 唐﨑結斗
// 概要 : 移動ベクトルを算出する
//=============================================================================
D3DXVECTOR3 CMotionEnemy::Move()
{
	// 移動方向
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 移動情報の計算
	m_pMove->Moving(vec);

	return m_pMove->GetMove();
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


