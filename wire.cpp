//=============================================================================
//
// ワイヤークラス(wire.cpp)
// Author : 唐﨑結斗
// 概要 : ワイヤー生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <stdio.h>

#include "wire.h"
#include "application.h"
#include "renderer.h"
#include "move.h"
#include "line.h"
#include "collision_rectangle3D.h"
#include "motion_enemy.h"
#include "calculation.h"
#include "camera.h"
#include "debug_proc.h"

//--------------------------------------------------------------------
// 定数定義
//--------------------------------------------------------------------
const float CWire::fDECISION = 500.0f;

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : インスタンスを生成する
//=============================================================================
CWire *CWire::Create()
{
	// オブジェクトインスタンス
	CWire *pWire = nullptr;

	// メモリの解放
	pWire = new CWire;

	// メモリの確保ができなかった
	assert(pWire != nullptr);

	// 数値の初期化
	pWire->Init();

	// インスタンスを返す
	return pWire;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CWire::CWire() : m_pStart(nullptr),			// スタート地点
m_pGoal(nullptr),							// ゴール地点
m_pDecision(nullptr),						// 判定用オブジェクト
m_pMove(nullptr),							// 移動情報
m_pRoll(nullptr),							// 回転情報
m_pLine(nullptr),							// ワイヤー
m_EMode(MODE_STOP),							// ワイヤーモード
m_ENextMode(MODE_STOP),						// 次のワイヤーモード
m_quat(),									// クォータニオン
m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),		// 移動
m_moveVec(D3DXVECTOR3(0.0f,0.0f,0.0f)),		// 移動ベクトル
m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),		// 向き
m_rotDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	// 目的の向き
m_rotVec(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	// 向きベクトル
m_fLength(0.0f)								// 長さ
{// ワールドマトリックス
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転マトリックス
	D3DXMatrixIdentity(&m_mtxRot);							
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CWire::~CWire()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CWire::Init()
{
	// ワイヤーの初期化
	CModelObj::Init();

	// オブジェクトのタイプ設定
	SetObjType(CObject::OBJETYPE_WIRE);

	// 当たり判定
	CCollision_Rectangle3D *m_pCollision = nullptr;

	// スタート地点
	m_pStart = CModelObj::Create();
	m_pStart->SetObjType(CObject::OBJETYPE_WIRE);
	m_pCollision = m_pStart->GetCollision();
	m_pCollision->SetSize(D3DXVECTOR3(10.0f, 10.0f, 10.0f));

	// ゴール地点
	m_pGoal = CModelObj::Create();
	m_pGoal->SetObjType(CObject::OBJETYPE_WIRE);
	m_pCollision = m_pGoal->GetCollision();
	m_pCollision->SetSize(D3DXVECTOR3(10.0f, 10.0f, 10.0f));

	// 判定用オブジェクト
	m_pDecision = CModelObj::Create();
	m_pDecision->SetObjType(CObject::OBJETYPE_WIRE);
	m_pCollision = m_pDecision->GetCollision();
	m_pCollision->SetSize(D3DXVECTOR3(600.0f, 10.0f, 600.0f));

	// ライン
	m_pLine = CLine::Create();

	// 移動クラスのメモリ確保
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(40.0f, 1000.0f, 0.0f, 0.01f);

	// 回転クラスのメモリ確保
	m_pRoll = new CMove;
	assert(m_pRoll != nullptr);
	m_pRoll->SetMoving(10.0f, 1000.0f, 0.0f, 0.01f);

	return E_NOTIMPL;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CWire::Uninit()
{
	if (m_pStart != nullptr)
	{// 終了
		m_pStart->Uninit();
		m_pStart = nullptr;
	}

	if (m_pGoal != nullptr)
	{// 終了
		m_pGoal->Uninit();
		m_pGoal = nullptr;
	}

	if (m_pDecision != nullptr)
	{// 終了
		m_pDecision->Uninit();
		m_pDecision = nullptr;
	}

	if (m_pLine != nullptr)
	{// 終了
		m_pLine->Uninit();
		m_pLine = nullptr;
	}

	if (m_pMove != nullptr)
	{// メモリの解放
		delete m_pMove;
		m_pMove = nullptr;
	}

	if (m_pRoll != nullptr)
	{// メモリの解放
		delete m_pRoll;
		m_pRoll = nullptr;
	}

	// ワイヤーの終了
	CModelObj::Uninit();

	// オブジェクトの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CWire::Update()
{
	if (m_EMode == MODE_HANGING)
	{
		Hanging();
	}
	else
	{// 移動
		Move();
	}

	// 当たり判定
	CCollision_Rectangle3D *pCollision = nullptr;
	m_ETargetObjType = CObject::OBJTYPE_NONE;

	switch (m_EMode)
	{
	case CWire::MODE_STOP:
		break;

	case CWire::MODE_FIRING:
		pCollision = m_pGoal->GetCollision();

		if (pCollision->Collision(CObject::OBJETYPE_ENEMY, false))
		{
			m_EMode = MODE_PULL;
			m_pMove->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			D3DXVECTOR3 vec = GetStart()->GetPos() - GetGoal()->GetPos();
			D3DXVec3Normalize(&vec, &vec);
			SetMoveVec(vec);

			// 移動
			Move();

			CMotionEnemy *pEnemy = (CMotionEnemy*)pCollision->GetCollidedObj();
			CMove *pMoveEnemy = pEnemy->GetMove();
			pMoveEnemy->SetMove(m_pMove->GetMove());
		}
		if (pCollision->Collision(CObject::OBJTYPE_3DMODEL, false))
		{
			m_EMode = MODE_STOP;
			m_ETargetObjType = OBJTYPE_3DMODEL;
		}

		break;

	case CWire::MODE_ATTRACT:
	case CWire::MODE_PULL:
		pCollision = m_pGoal->GetCollision();

		if (pCollision->Collision(CObject::OBJETYPE_WIRE, false))
		{
			m_EMode = MODE_STOP;
			SetPos(m_pStart->GetPos());
		}

		break;

	case CWire::MODE_HANGING:
		break;

	default:
		assert(false);
		break;
	}

	// ワイヤーの更新
	CModelObj::Update();
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CWire::Draw()
{
	// ワイヤーの描画
	CModelObj::Draw();

	if (m_EMode == MODE_HANGING)
	{
		// 位置の取得
		D3DXVECTOR3 pos = m_pGoal->GetPos();

		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

		// 計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);									// 行列初期化関数

		// クォータニオンの作成
		D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, m_fValueRot);

		// 向きの反映
		D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);
		D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);				// 行列移動関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);			// 行列掛け算関数

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	}
	else
	{
		// ワールドマトリックス
		D3DXMatrixIdentity(&m_mtxWorld);

		// 回転マトリックス
		D3DXMatrixIdentity(&m_mtxRot);
	}

	if (m_pLine != nullptr)
	{// ラインの設定
		m_pLine->SetLine(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pStart->GetPos(), m_pGoal->GetPos(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//=============================================================================
// 位置の設定
// Author : 唐﨑結斗
// 概要 : 位置を設定する
//=============================================================================
void CWire::SetPos(const D3DXVECTOR3 & pos)
{
	m_pStart->SetPos(pos);
	m_pDecision->SetPos(D3DXVECTOR3(pos.x, pos.y + fDECISION, pos.z));
	m_pGoal->SetPos(pos);
	CModelObj::SetPos(pos);
}

//=============================================================================
// 移動
// Author : 唐﨑結斗
// 概要 : 移動方向を決定し
//=============================================================================
void CWire::Move()
{
	// 位置
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	switch (m_EMode)
	{
	case CWire::MODE_STOP:
		break;

	case CWire::MODE_FIRING:
	case CWire::MODE_PULL:
		pos = m_pGoal->GetPos();
		break;

	case CWire::MODE_ATTRACT:
		pos = m_pStart->GetPos();
		break;

	default:
		assert(false);
		break;
	}

	// 移動情報
	m_pMove->Moving(m_moveVec);
	pos += m_pMove->GetMove();

	switch (m_EMode)
	{
	case CWire::MODE_STOP:
		m_pMove->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;

	case CWire::MODE_FIRING:
	case CWire::MODE_PULL:
		m_pGoal->SetPos(pos);
		break;

	case CWire::MODE_ATTRACT:
		m_pStart->SetPos(pos);
		m_pDecision->SetPos(D3DXVECTOR3(pos.x, pos.y + fDECISION, pos.z));
		break;

	default:
		break;
	}

	// 移動ベクトルのリセット
	m_moveVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// ぶら下がり処理
// Author : 唐﨑結斗
// 概要 : 
//=============================================================================
void CWire::Hanging()
{
	// 位置
	D3DXVECTOR3 pos = m_pStart->GetPos();
	D3DXVECTOR3 posGoal = m_pGoal->GetPos();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// カメラ情報の取得
	CCamera *pCamera = CApplication::GetCamera();

	// 移動方向の算出
	m_rotDest.y = pCamera->GetRot().y + D3DX_PI;

	// 移動方向の正規化
	m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y - m_rot.y);

	// 向きの更新
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.01f;

	// 向きの正規化
	m_rot.y = CCalculation::RotNormalization(m_rot.y);

	// 摩擦係数の計算
	m_pRoll->SetMoveVec(D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y)));
	m_pRoll->Moving(m_rotVec);
	D3DXVECTOR3 moveing = m_pRoll->GetMove();

	// 移動
	D3DXVECTOR3 vec = GetGoal()->GetPos() - GetStart()->GetPos();
	D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(0.0f, -m_fLength, 0.0f), &m_mtxWorld);
	m_move = pos - m_pStart->GetPos();
	m_pStart->SetPos(pos);
	m_pDecision->SetPos(D3DXVECTOR3(pos.x, pos.y + fDECISION, pos.z));

	m_rotVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 回転軸の設定
	m_vecAxis = D3DXVECTOR3(moveing.z, moveing.y, moveing.x * -1);

	// 回転角の設定
	m_fValueRot = D3DXVec3Length(&moveing) / m_fLength;

	float fLimit = 0.5f;

	if ((m_rot.x <= D3DX_PI * fLimit  && m_rot.x >= D3DX_PI * 0.0f)
		|| (m_rot.x >= D3DX_PI * -fLimit  && m_rot.x <= D3DX_PI * 0.0f))
	{
		//m_EMode = MODE_STOP;
	}

	CDebugProc::Print("ワイヤーの向き : %.3f\n", m_rot.x);
}

//=============================================================================
// モードの設定
// Author : 唐﨑結斗
// 概要 : 
//=============================================================================
void CWire::SetWireMode(WIRE_MODE EWireMode)
{
	m_EMode = EWireMode;
	m_pMove->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
// 向きの設定
// Author : 唐﨑結斗
// 概要 : 
//=============================================================================
void CWire::SetHanging()
{
	// 方向ベクトル
	D3DXVECTOR3 vec = GetGoal()->GetPos() - GetStart()->GetPos();

	// 向きの設定
	m_rot.z = sqrtf((vec.x * vec.x) + (vec.z * vec.z));
	m_rot.x = atan2f(m_rot.z, vec.y);
	m_rot.y = atan2f(vec.x, vec.z);
	m_rot.z = 0.0f;
	
	// 移動方向の設定
	m_rotVec = D3DXVECTOR3(-vec.x, vec.y, -vec.z);

	// 長さの設定
	m_fLength = sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

//=============================================================================
// 方向ベクトルの設定
// Author : 唐﨑結斗
// 概要 : 
//=============================================================================
void CWire::SetRotVec(D3DXVECTOR3 rotVec)
{
	m_rotVec = rotVec;
	m_pRoll->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
// ぶら下がる場所の設定
// Author : 唐﨑結斗
// 概要 : 
//=============================================================================
D3DXVECTOR3 CWire::HangingSearch()
{
	// 返り値の定義
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 当たり判定
	CCollision_Rectangle3D *pCollision = m_pDecision->GetCollision();

	if (pCollision->Collision(CObject::OBJTYPE_3DMODEL, false))
	{
		CModelObj *pModel = (CModelObj*)pCollision->GetCollidedObj();

		// 情報の取得
		D3DXVECTOR3 posTarget = pModel->GetPos();
		posTarget.y = m_pDecision->GetPos().y;
		D3DXVECTOR3 pos = m_pStart->GetPos();

		// 方向ベクトルの算出
		vec = posTarget - pos;
	}

	// 正規化
	D3DXVec3Normalize(&vec, &vec);

	return vec;
}
