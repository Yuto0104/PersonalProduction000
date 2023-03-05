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

#include "player.h"
#include "game.h"
#include "mesh.h"
#include "motion.h"
#include "renderer.h"
#include "application.h"
#include "camera.h"
#include "keyboard.h"
#include "mouse.h"
#include "calculation.h"
#include "move.h"
#include "debug_proc.h"
#include "orbit.h"
#include "collision_rectangle3D.h"
#include "collision_sphere.h"
#include "parts.h"
#include "model_obj.h"
#include "weapon_obj.h"
#include "motion_enemy.h"
#include "wire.h"
#include "score.h"
#include "score_item.h"
#include "sound.h"
#include "scene_mode.h"
#include "tutorial.h"
#include "joypad.h"

//--------------------------------------------------------------------
// 静的メンバ変数の定義
//--------------------------------------------------------------------
const float CPlayer::fSPEED = 1.0f;
const float CPlayer::fJAMP = 30.0f;

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : モーションキャラクター3Dを生成する
//=============================================================================
CPlayer * CPlayer::Create()
{
	// オブジェクトインスタンス
	CPlayer *pPlayer = nullptr;

	// メモリの解放
	pPlayer = new CPlayer;

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
CPlayer::CPlayer() : m_pMove(nullptr),
m_pOrbit(nullptr),
m_pCollisionRectangle3D(nullptr),
m_pAttack(nullptr),
m_pColliAttack(nullptr),
m_pMyWeapon(nullptr),
m_pWire(nullptr),
m_EAction(NEUTRAL_ACTION),
m_rotDest(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_fSpeed(0.0f),
m_fGravity(0.0f),
m_nNumMotion(0),
m_nNumHandParts(0)
{

}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CPlayer::Init()
{
	// 初期化
	CMotionModel3D::Init();

	// オブジェクトタイプの設定
	SetObjType(OBJETYPE_PLAYER);

	// 移動クラスのメモリ確保
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(fSPEED, 1000.0f, 0.5f, 0.1f);

	// 軌跡の設定
	/*m_pOrbit = COrbit::Create();
	m_pOrbit->SetMtxParent(GetMtxWorld());
	m_pOrbit->SetOffset(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	m_pOrbit->SetDivision(100);
	m_pOrbit->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
	m_pOrbit->SetBlendMode(COrbit::MODE_ADD);*/

	// 3D矩形の当たり判定の設定
	m_pCollisionRectangle3D = CCollision_Rectangle3D::Create();
	m_pCollisionRectangle3D->SetParent(this);
	m_pCollisionRectangle3D->SetPos(D3DXVECTOR3(0.0f, 25.0f, 0.0f));
	m_pCollisionRectangle3D->SetSize(D3DXVECTOR3(30.0f, 50.0f, 20.0f));

	// 手のオブジェクトの設定
	m_pAttack = CModelObj::Create();
	m_pAttack->SetObjType(OBJETYPE_PLAYER);
	m_nNumHandParts = 3;
	m_pColliAttack = CCollision_Rectangle3D::Create();
	m_pColliAttack->SetParent(m_pAttack);
	m_pColliAttack->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pColliAttack->SetSize(D3DXVECTOR3(10.0f, 10.0f, 10.0f));

	// ワイヤー
	m_pWire = CWire::Create();

	return E_NOTIMPL;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CPlayer::Uninit()
{
	// カメラの追従設定(目標 : プレイヤー)
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetFollowTarget(false);
	pCamera->SetTargetPosR(false);

	if (m_pMove != nullptr)
	{// メモリの解放
		delete m_pMove;
		m_pMove = nullptr;
	}

	if (m_pOrbit != nullptr)
	{// 終了処理
		m_pOrbit->Uninit();
		m_pOrbit = nullptr;
	}

	if (m_pCollisionRectangle3D != nullptr)
	{// 終了処理
		m_pCollisionRectangle3D->Uninit();
		m_pCollisionRectangle3D = nullptr;
	}

	if (m_pAttack != nullptr)
	{// 終了処理
		m_pAttack->Uninit();
		m_pAttack = nullptr;
	}

	if (m_pColliAttack != nullptr)
	{// 終了処理
		m_pColliAttack->Uninit();
		m_pColliAttack = nullptr;
	}

	if (m_pWire != nullptr)
	{// 終了処理
		m_pWire->Uninit();
		m_pWire = nullptr;
	}

	// 終了
	CMotionModel3D::Uninit();

	// ゲーム終了
	CGame::SetGame(false);
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CPlayer::Update()
{// キーボードの取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// マウスの取得
	CMouse *pMouse = CApplication::GetMouse();

	// コントローラー
	CJoypad *pJoy = CApplication::GetJoy();

	// モーション情報の取得
	CMotion *pMotion = CMotionModel3D::GetMotion();

	// カメラ情報の取得
	CCamera *pCamera = CApplication::GetCamera();

	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// 位置の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 過去位置の更新
	SetPosOld(pos);
	m_pAttack->SetPosOld(m_pAttack->GetPos());

	if (pMotion != nullptr)
	{// 手のオブジェクトの位置
		CParts *pHand = pMotion->GetParts(m_nNumHandParts);
		D3DXMATRIX mtxParts = pHand->GetMtxWorld();
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &mtxParts);
	}
	
	if (pMouse->GetTrigger(CMouse::MOUSE_KEY_LEFT))
	{// 攻撃
		Attack();
	}

	bool bJump = false;

	if (pJoy->GetUseJoyPad() == 0
		&& pKeyboard->GetTrigger(DIK_SPACE))
	{// ジャンプ
		bJump = true;
	}
	else if (pJoy->GetUseJoyPad() > 0
		&& pJoy->GetTrigger(CJoypad::JOYKEY_A, 0))
	{
		bJump = true;
	}

	if (bJump
		&& m_fGravity == 0.0f)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_JUMP000);
		Jump();
	}

	if (m_pWire->GetTargetObjType() == CObject::OBJTYPE_3DMODEL)
	{
		m_pWire->SetWireMode(CWire::MODE_HANGING);
		m_pWire->SetHanging();
		m_pWire->SetRotVec(D3DXVECTOR3(-1.0f, 0.0f, 0.0f));

		// モーション情報の取得
		CMotion *pMotion = CMotionModel3D::GetMotion();

		if (pMotion != nullptr)
		{
			m_EAction = ATTACK_HANGING;
			pMotion->SetNumMotion(m_EAction);
		}
	}

	if (pKeyboard->GetTrigger(DIK_LSHIFT)
		|| pJoy->GetTrigger(CJoypad::JOYKEY_LEFT_SHOULDER, 0)
		&& (m_pWire->GetWireMode() == CWire::MODE_STOP
		&& m_fGravity != 0.0f))
	{// ワイヤーの射出
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SHOT000);
		D3DXVECTOR3 vec = pCamera->GetPosR() - pCamera->GetPosV();
		m_pWire->SetWireMode(CWire::MODE_FIRING);
		m_pWire->SetMoveVec(vec);
	}

	// 重力の設定
	m_fGravity -= CCalculation::Gravity();

	if (m_pWire->GetWireMode() != CWire::MODE_ATTRACT
		&& m_pWire->GetWireMode() != CWire::MODE_HANGING)
	{// 移動
		pos.y += m_fGravity;
		pos += Move();
	}
	else if (m_pWire->GetWireMode() == CWire::MODE_HANGING)
	{
		// 移動方向の算出
		m_rotDest.y = pCamera->GetRot().y - D3DX_PI;

		// 移動方向の正規化
		m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);

		// 目的の向きの補正
		if (m_rotDest.y - rot.y >= D3DX_PI)
		{// 移動方向の正規化
			m_rotDest.y -= D3DX_PI * 2;
		}
		else if (m_rotDest.y - rot.y <= -D3DX_PI)
		{// 移動方向の正規化
			m_rotDest.y += D3DX_PI * 2;
		}

		m_fGravity = 0.0f;

		D3DXVECTOR3 move = m_pMove->GetMove();
		move.y = 0.0f;
		m_pMove->SetMove(move);

		if (pMotion != nullptr)
		{
			D3DXVECTOR3 startPos = m_pWire->GetStart()->GetPos();
			D3DXVECTOR3 posHand = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// 手のオブジェクトの位置
			CParts *pHand = pMotion->GetParts(m_nNumHandParts);
			D3DXVec3TransformCoord(&posHand, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &pHand->GetMtxWorld());

			// 手との距離の差分を算出
			D3DXVECTOR3 subPos = pos - posHand;

			// 位置の設定
			pos = subPos + startPos;
		}
	}
	else
	{// 移動
		pos += m_pWire->GetMove()->GetMove();
	}

	// 位置の設定
	SetPos(pos);

	// 回転
	Rotate();

	if (pKeyboard->GetRelease(DIK_LSHIFT)
		|| pJoy->GetTrigger(CJoypad::JOYKEY_LEFT_SHOULDER, 0)
		&& m_pWire->GetWireMode() == CWire::MODE_HANGING)
	{
		Dash();
	}

	if (pMotion != nullptr
		&& !pMotion->GetMotion()
		&& m_EAction != ATTACK_JUMP)
	{// ニュートラルモーションの設定
		m_EAction = NEUTRAL_ACTION;
		pMotion->SetNumMotion(m_EAction);
	}

	// 更新
	CMotionModel3D::Update();

	// 位置の設定
	SetPos(pos);

	// メッシュの当たり判定
	if (CMesh3D::CollisonMesh(this))
	{
		Landing();
	}

	// モデルとの当たり判定
	m_pCollisionRectangle3D->Collision(CObject::OBJTYPE_NONE, true);
	m_pCollisionRectangle3D->Collision(CObject::OBJETYPE_ENEMY, true);

	if (m_pCollisionRectangle3D->Collision(CObject::OBJETYPE_SCOREITEM, false))
	{
		CApplication::SCENE_MODE mode = CApplication::GetMode();

		// アイテムの取得
		CScoreItem *pItem = (CScoreItem*)m_pCollisionRectangle3D->GetCollidedObj();
		bool bUse = pItem->GetUse();

		if (mode == CApplication::MODE_GAME)
		{
			if (bUse)
			{// スコアの取得
				CScore *pScore = CGame::GetScore();
				pScore->AddScore(pItem->Acquisition(1800));
				pSound->PlaySound(CSound::SOUND_LABEL_SE_COIN000);
			}
		}
		if (mode == CApplication::MODE_TUTORIAL)
		{
			if (bUse)
			{
				CTutorial *pTutorial = (CTutorial*)CApplication::GetSceneMode();
				pTutorial->SetTutoItem(true);
				pItem->Acquisition(1800);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_COIN000);

			}
		}
	}

	if (m_pCollisionRectangle3D->Collision(CObject::OBJTYPE_3DMODEL, true))
	{
		CCollision_Rectangle3D::EState state = m_pCollisionRectangle3D->GetState();
		bool bPlusMinus = m_pCollisionRectangle3D->GetPlusMinus();

		if (state == CCollision_Rectangle3D::STATE_Y)
		{
			Landing();
		}

		if ((state == CCollision_Rectangle3D::STATE_X
			|| state == CCollision_Rectangle3D::STATE_Y
			|| state == CCollision_Rectangle3D::STATE_Z)
			&& m_pWire->GetWireMode() == CWire::MODE_HANGING)
		{
			// m_pWire->SetWireMode(CWire::MODE_STOP);

			/*D3DXVECTOR3 distance = m_pCollisionRectangle3D->GetCollidedObj()->GetPos() - GetPos();
			D3DXVECTOR3 rotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			rotDiff.y = atan2f(distance.x, distance.z);
			rotDiff.x = D3DX_PI * 0.5f;

			if (state == CCollision_Rectangle3D::STATE_X
				&& bPlusMinus)
			{
				SetRot(rotDiff);
			}
			else if (state == CCollision_Rectangle3D::STATE_X
				&& !bPlusMinus)
			{
				SetRot(rotDiff);
			}
			else if (state == CCollision_Rectangle3D::STATE_Z
				&& bPlusMinus)
			{
				SetRot(rotDiff);
			}
			else if (state == CCollision_Rectangle3D::STATE_Z
				&& !bPlusMinus)
			{
				SetRot(rotDiff);
			}*/
		}
	}

	m_pCollisionRectangle3D->Collision(CObject::OBJTYPE_3DMODEL);

	// 武器との当たり判定
	bool bCollisionWeapon = m_pCollisionRectangle3D->Collision(CObject::OBJETYPE_WEAPON, false);

	if (bCollisionWeapon
		&& pKeyboard->GetTrigger(DIK_E))
	{// 武器の取得
		GetWeapon();
	}

	if (pMotion != nullptr)
	{// 手のオブジェクトの位置
		CParts *pHand = pMotion->GetParts(m_nNumHandParts);
		D3DXMATRIX mtxParts = pHand->GetMtxWorld();
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &mtxParts);

		if (m_pMyWeapon == nullptr)
		{
			m_pAttack->SetPos(pos);
		}

		switch (m_pWire->GetWireMode())
		{
		case CWire::MODE_STOP:
			// ワイヤーの位置の設定
			m_pWire->SetPos(pos);
			break;

		case CWire::MODE_FIRING:
		case CWire::MODE_ATTRACT:
			m_pWire->GetStart()->SetPos(pos);
			break;

		case CWire::MODE_PULL:
			break;

		default:
			break;
		}
	}
	
	if (pMotion != nullptr
		&& m_pMyWeapon != nullptr)
	{
		D3DXMATRIX mtxParts = m_pMyWeapon->GetMtxWorld();
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 25.0f, 0.0f);

		D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(0.0f, 25.0f, 0.0f), &mtxParts);
		m_pAttack->SetPos(pos);
	}

	if (m_EAction == ATTACK_ACTION)
	{
		bool bCollision = m_pColliAttack->Collision(CObject::OBJETYPE_ENEMY, false);

		if (bCollision)
		{// ターゲットの取得
			CMotionEnemy *pTarget = (CMotionEnemy*)m_pColliAttack->GetCollidedObj();

			// 攻撃力の設定
			int nAttack = nFIST_ATTACK;

			if (m_pMyWeapon != nullptr)
			{// 武器を装備している
				nAttack = m_pMyWeapon->GetAttack();
			}

			// 攻撃処理
			pTarget->Hit(nAttack);
		}
	}

	// 位置の取得
	pos = GetPos();
	D3DXVECTOR3 size = m_pCollisionRectangle3D->GetSize();

	if (pos.x - size.x / 2.0f < -5000.0f)
	{
		pos.x = -5000.0f + size.x / 2.0f;
	}
	if (pos.x + size.x / 2.0f > 5000.0f)
	{
		pos.x = 5000.0f - size.x / 2.0f;
	}
	if (pos.z - size.z / 2.0f < -5000.0f)
	{
		pos.z = -5000.0f + size.z / 2.0f;
	}
	if (pos.z + size.z / 2.0f > 5000.0f)
	{
		pos.z = 5000.0f - size.z / 2.0f;
	}

	SetPos(pos);
	
	// 位置の取得
	pos = GetPos();
	D3DXVECTOR3 attackPos = m_pAttack->GetPos();

	// デバック表示
	CDebugProc::Print("プレイヤーの位置 | X : %.3f | Y : %.3f | Z : %.3f |\n", pos.x, pos.y, pos.z);
	CDebugProc::Print("プレイヤーの手の位置 | X : %.3f | Y : %.3f | Z : %.3f |\n", attackPos.x, attackPos.y, attackPos.z);
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void CPlayer::Draw()
{
	// 描画
	CMotionModel3D::Draw();
}

//=============================================================================
// ダッシュ
// Author : 唐﨑結斗
// 概要 : ダッシュ
//=============================================================================
void CPlayer::Dash()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// モーション情報の取得
	CMotion *pMotion = CMotionModel3D::GetMotion();

	// 位置の取得
	D3DXVECTOR3 pos = GetPos();

	pSound->PlaySound(CSound::SOUND_LABEL_SE_JUMP001);
	m_pWire->SetWireMode(CWire::MODE_STOP);
	D3DXVECTOR3 vecMove = pos - GetPosOld();
	D3DXVECTOR3 rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = GetRot();
	rot.y = CCalculation::RotNormalization((rot.y - D3DX_PI));

	float fLength = sqrtf((vecMove.x * vecMove.x) + (vecMove.y * vecMove.y) + (vecMove.z * vecMove.z));
	rotMove.x = atan2f(sqrtf((vecMove.x * vecMove.x) + (vecMove.z * vecMove.z)), vecMove.y);

	// 位置の設定
	vecMove.z = sinf(rotMove.x) * cosf(rot.y) * fLength;
	vecMove.x = sinf(rotMove.x) * sinf(rot.y) * fLength;
	vecMove.y = cosf(rotMove.x) * fLength;

	m_pMove->SetSpeed(100.0f);
	m_pMove->Moving(vecMove);
	m_pMove->SetSpeed(fSPEED);

	if (pMotion != nullptr)
	{
		m_EAction = ATTACK_JUMP;
		pMotion->SetNumMotion(m_EAction);
	}
}

//=============================================================================
// 移動
// Author : 唐﨑結斗
// 概要 : キー入力で方向を決めて、移動ベクトルを算出する
//=============================================================================
D3DXVECTOR3 CPlayer::Move()
{
	// 変数宣言
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// キーボードの取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// モーション情報の取得
	CMotion *pMotion = CMotionModel3D::GetMotion();

	// コントローラー
	CJoypad *pJoy = CApplication::GetJoy();

	if (pJoy->GetUseJoyPad() == 0
		&& (pKeyboard->GetPress(DIK_W)
		|| pKeyboard->GetPress(DIK_A)
		|| pKeyboard->GetPress(DIK_D)
		|| pKeyboard->GetPress(DIK_S)))
	{// 移動キーが押された
		if (pKeyboard->GetPress(DIK_W))
		{// [W]キーが押された時
			if (pKeyboard->GetPress(DIK_A))
			{// [A]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * -0.25f;
			}
			else if (pKeyboard->GetPress(DIK_D))
			{// [D]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * 0.25f;
			}
			else
			{// 移動方向の更新
				m_rotDest.y = D3DX_PI * 0.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_S))
		{// [S]キーが押された時
			if (pKeyboard->GetPress(DIK_A))
			{// [A]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * -0.75f;
			}
			else if (pKeyboard->GetPress(DIK_D))
			{// [D]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * 0.75f;
			}
			else
			{// 移動方向の更新q
				m_rotDest.y = D3DX_PI;
			}
		}
		else if (pKeyboard->GetPress(DIK_A))
		{// [A]キーが押された時
		 // 移動方向の更新
			m_rotDest.y = D3DX_PI * -0.5f;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{// [D]キーが押された時
		 // 移動方向の更新
			m_rotDest.y = D3DX_PI * 0.5f;
		}

		// カメラ情報の取得
		CCamera *pCamera = CApplication::GetCamera();

		// 移動方向の算出
		m_rotDest.y += pCamera->GetRot().y;

		// 移動方向の正規化
		m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);

		// 移動量の計算
		move = D3DXVECTOR3(sinf(m_rotDest.y), 0.0f, cosf(m_rotDest.y));

		// 角度の正規化
		m_rotDest.y -= D3DX_PI;

		// 移動方向の正規化
		m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);

		if (m_EAction == NEUTRAL_ACTION)
		{// 移動
			m_EAction = MOVE_ACTION;
		
			if (pMotion != nullptr)
			{
				pMotion->SetNumMotion(m_EAction);
			}
		}
	}
	else if (pJoy->GetUseJoyPad() > 0
		&& pJoy->Stick(CJoypad::JOYKEY_LEFT_STICK, 0, 0.5f))
	{
		// 向きの計算
		m_rotDest.y = pJoy->GetStickAngle(CJoypad::JOYKEY_LEFT_STICK, 0);

		// カメラ情報の取得
		CCamera *pCamera = CApplication::GetCamera();

		// 移動方向の算出
		m_rotDest.y += pCamera->GetRot().y;

		// 移動方向の正規化
		m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);

		// 移動量の計算
		move = D3DXVECTOR3(sinf(m_rotDest.y), 0.0f, cosf(m_rotDest.y));

		// 角度の正規化
		m_rotDest.y -= D3DX_PI;

		// 移動方向の正規化
		m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);

		if (m_EAction == NEUTRAL_ACTION)
		{// 移動
			m_EAction = MOVE_ACTION;

			if (pMotion != nullptr)
			{
				pMotion->SetNumMotion(m_EAction);
			}
		}
	}

	// 移動情報の計算
 	m_pMove->Moving(move);

	// 移動情報の取得
	D3DXVECTOR3 moveing = m_pMove->GetMove();

	if (sqrtf((moveing.x * moveing.x) + (moveing.z * moveing.z)) <= 0.0f
		&& pMotion != nullptr
		&& m_EAction == MOVE_ACTION)
	{
		m_EAction = NEUTRAL_ACTION;
		pMotion->SetNumMotion(m_EAction);
	}

	// デバック表示
	CDebugProc::Print("移動ベクトル : %.3f\n", m_pMove->GetMoveLength());

	// 向きの取得
	D3DXVECTOR3 rot = GetRot();

	// 目的の向きの補正
	if (m_rotDest.y - rot.y >= D3DX_PI)
	{// 移動方向の正規化
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - rot.y <= -D3DX_PI)
	{// 移動方向の正規化
		m_rotDest.y += D3DX_PI * 2;
	}

	return moveing;
}

//=============================================================================
// 回転
// Author : 唐﨑結斗
// 概要 : 目的の向きまで回転する
//=============================================================================
void CPlayer::Rotate()
{
	// 向きの取得
	D3DXVECTOR3 rot = GetRot();
	float fFriction = 0.5f;

	if (m_pWire->GetWireMode() == CWire::MODE_HANGING)
	{
		fFriction = 0.1f;
	}

	// 向きの更新
	rot.y += (m_rotDest.y - rot.y) * fFriction;

	// 向きの正規化
	rot.y = CCalculation::RotNormalization(rot.y);

	// 向きの設定
	SetRot(rot);
}

//=============================================================================
// ジャンプ
// Author : 唐﨑結斗
// 概要 : ジャンプする
//=============================================================================
void CPlayer::Jump()
{
	// 移動情報の計算
	D3DXVECTOR3 move = m_pMove->GetMove();
	move.y = 0.0f;
	m_fGravity = 0.0f;
	m_pMove->SetMove(move);
	m_pMove->SetSpeed(fJAMP);
	m_pMove->Moving(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pMove->SetSpeed(fSPEED);

	// モーション情報の取得
	CMotion *pMotion = CMotionModel3D::GetMotion();

	if (pMotion != nullptr)
	{
		m_EAction = ATTACK_JUMP;
		pMotion->SetNumMotion(m_EAction);
	}
}

//=============================================================================
// 攻撃
// Author : 唐﨑結斗
// 概要 : 攻撃する
//=============================================================================
void CPlayer::Attack()
{
	// モーション情報の取得
	CMotion *pMotion = CMotionModel3D::GetMotion();

	if (pMotion != nullptr)
	{
		m_EAction = ATTACK_ACTION;
		pMotion->SetNumMotion(m_EAction);
	}
}

//=============================================================================
// 武器の入手
// Author : 唐﨑結斗
// 概要 : 現在の武器を捨てて、新しい武器を取得する
//=============================================================================
void CPlayer::GetWeapon()
{
	// モーション情報の取得
	CMotion *pMotion = CMotionModel3D::GetMotion();

	// 手のパーツの取得
	CParts *pHand = pMotion->GetParts(m_nNumHandParts);
	CCollision_Rectangle3D *pWeaponCollision = nullptr;

	if (m_pMyWeapon != nullptr)
	{// 持っていた武器の装備解除
		m_pMyWeapon->SetParent();
		pWeaponCollision = m_pMyWeapon->GetCollision();
		pWeaponCollision->SetUseFlag(true);
	}

	// 武器の装備
	CWeaponObj *pWeapon = (CWeaponObj*)m_pCollisionRectangle3D->GetCollidedObj();
	pWeapon->SetParent(pHand);
	m_pMyWeapon = pWeapon;
	m_pMyWeapon->SetPosOffset(D3DXVECTOR3(3.0f, 0.0f, 0.0f));
	m_pMyWeapon->SetRotOffset(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	pWeaponCollision = m_pMyWeapon->GetCollision();
	pWeaponCollision->SetUseFlag(false);

	if (m_EAction == NEUTRAL_ACTION)
	{
		pMotion->SetNumMotion(m_EAction);
	}
}

//=============================================================================
// 着地
// Author : 唐﨑結斗
// 概要 : 着地モーションをはじめ、重力を0にする
//=============================================================================
void CPlayer::Landing()
{
	// モーション情報の取得
	CMotion *pMotion = CMotionModel3D::GetMotion();

	m_fGravity = 0.0f;

	D3DXVECTOR3 move = m_pMove->GetMove();
	move.y = 0.0f;
	m_pMove->SetMove(move);

	if (m_pWire->GetWireMode() == CWire::MODE_FIRING)
	{
		m_pWire->SetWireMode(CWire::MODE_STOP);
	}

	if (m_EAction == ATTACK_JUMP)
	{
		m_EAction = ATTACK_LANDING;
		pMotion->SetNumMotion(m_EAction);
	}
}


