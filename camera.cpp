//**************************************************************************************************
//
// カメラ処理(camera.h)
// Auther：唐﨑結斗
// 概要 : カメラの設定処理
//
//**************************************************************************************************

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "camera.h"
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "mouse.h"
#include "keyboard.h"
#include "move.h"
#include "calculation.h"
#include "object.h"
#include "debug_proc.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
const float CCamera::CAMERA_NEAR = 10.0f;				// ニア
const float CCamera::CAMERA_FUR = 100000.0f;			// ファー

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CCamera::CCamera()
{
	m_pMove = nullptr;								// 移動クラスのインスタンス
	m_pRoll = nullptr;								// 移動クラスのインスタンス(角度)
	m_pTarget = nullptr;							// ターゲット
	m_pTargetPosR = nullptr;						// ターゲット(注視点)
	m_mtxWorld = {};								// ワールドマトリックス
	m_mtxProj = {};									// プロジェクションマトリックス
	m_mtxView = {};									// ビューマトリックス
	m_viewport = {};								// ビューポート
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 注視点
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 上方向ベクトル
	m_posVOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 視点差分
	m_posROffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 注視点差分
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動方向
	m_posRDiff = D3DXVECTOR2(0.0f, 0.0f);			// 注視点ターゲット時の差分
	m_aspect = D3DXVECTOR2(0.0f, 0.0f);				// アスペクト比の値
	m_viewType = TYPE_CLAIRVOYANCE;					// 投影の種別
	m_fViewing = 0.0f;								// 視野角
	m_fRotMove = 0.0f;								// 移動方向
	m_fCoeffFllow = 0.0f;							// 追従の減衰係数
	m_fShake = 0.0f;								// 揺れの量
	m_nCntShake = 0;								// 揺れのカウント
	m_bUseRollX = true;								// X軸の回転の使用状況
	m_bUseRollY = true;								// Y軸の回転の使用状況
	m_bFllow = false;								// 追従を行うか
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 視点と注視点の間の距離を算出する
//=============================================================================
HRESULT CCamera::Init()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 固定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 差分
	m_posVOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posROffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 注視点ターゲット時の差分
	m_posRDiff = D3DXVECTOR2(0.0f, 100.0f);			

	// 視野角の初期値の代入
	m_fViewing = 45.0f;

	// 移動クラスのメモリ確保
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(10.0f, 100.0f, 0.0f, 0.3f);

	// 移動クラス(角度)のメモリ確保
	m_pRoll = new CMove;
	assert(m_pRoll != nullptr);
	m_pRoll->SetMoving(0.05f, 5.0f, 0.0f, 0.5f);

	// アスペクト比の設定
	m_aspect = D3DXVECTOR2((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT);

	// ビューポートの大きさ設定
	SetViewSize(0, 0, CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : 終了
//=============================================================================
void CCamera::Uninit(void)
{
	if (m_pMove != nullptr)
	{// 終了処理
	 // メモリの解放
		delete m_pMove;
		m_pMove = nullptr;
	}

	if (m_pRoll != nullptr)
	{// 終了処理
	 // メモリの解放
		delete m_pRoll;
		m_pRoll = nullptr;
	}
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新
//=============================================================================
void CCamera::Update(void)
{
	if (m_pTarget == nullptr)
	{
		Move();
	}
	if (m_pTargetPosR == nullptr)
	{
		Rotate();
	}
	
	CalMatrix();

#ifdef _DEBUG
	CDebugProc::Print("カメラの視点 | X : %.3f | Y : %.3f | Z : %.3f |\n", m_posV.x, m_posV.y, m_posV.z);
	CDebugProc::Print("カメラの注視点 | X : %.3f | Y : %.3f | Z : %.3f |\n", m_posR.x, m_posR.y, m_posR.z);
	CDebugProc::Print("カメラの向き | X : %.3f | Y : %.3f | Z : %.3f |\n", m_rot.x, m_rot.y, m_rot.z);
	CDebugProc::Print("視点のオフセット | X : %.3f | Y : %.3f | Z : %.3f |\n", m_posVOffset.x, m_posVOffset.y, m_posVOffset.z);
	CDebugProc::Print("注視点のオフセット | X : %.3f | Y : %.3f | Z : %.3f |\n", m_posROffset.x, m_posROffset.y, m_posROffset.z);
#endif // _DEBUG
}

//=============================================================================
// カメラの設定
// Author : 唐﨑結斗
// 概要 : ビューマトリックスの設定
//=============================================================================
void CCamera::Set()
{// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);			// 行列初期化関数

	//// 上方向ベクトルの算出
	//D3DXVECTOR3 rotAdd = m_rot + D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, D3DX_PI * 0.5f);
	//rotAdd.x = CCalculation::RotNormalization(rotAdd.x);
	//rotAdd.y = CCalculation::RotNormalization(rotAdd.y);
	//rotAdd.z = CCalculation::RotNormalization(rotAdd.z);

	D3DXVECTOR3 adjust = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (m_nCntShake > 0)
	{
		m_nCntShake--;

		adjust.x = (float)(rand() % (int)(m_fShake * 200) / 100) - m_fShake;
		adjust.y = (float)(rand() % (int)(m_fShake * 200) / 100) - m_fShake;
		adjust.z = (float)(rand() % (int)(m_fShake * 200) / 100) - m_fShake;

		if (m_nCntShake <= 0)
		{
			m_nCntShake = 0;
		}
	}

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&(m_posV + adjust),
		&(m_posR + adjust),
		&m_vecU);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// ビューポートの適応
	pDevice->SetViewport(&m_viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProj);			// 行列初期化関数

	switch (m_viewType)
	{
	case TYPE_CLAIRVOYANCE:
		// プロジェクションマトリックスの作成(透視投影)
		D3DXMatrixPerspectiveFovLH(&m_mtxProj,											// プロジェクションマトリックス
			D3DXToRadian(m_fViewing),													// 視野角
			(float)CRenderer::SCREEN_WIDTH / (float)CRenderer::SCREEN_HEIGHT,			// アスペクト比
			CAMERA_NEAR,																// ニア
			CAMERA_FUR);																// ファー
		break;

	case TYPE_PARALLEL:
		// プロジェクションマトリックスの作成(平行投影)
		D3DXMatrixOrthoLH(&m_mtxProj,		// プロジェクションマトリックス
			m_aspect.x,						// 幅
			m_aspect.y,						// 高さ
			CAMERA_NEAR,					// ニア
			CAMERA_FUR);					// ファー
		break;

	default:
		assert(false);
		break;
	}

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProj);
}

//=============================================================================
// カメラの投影方法の設定
// Author : 唐﨑結斗
// 概要 : 
//=============================================================================
void CCamera::SetViewType(VIEW_TYPE type)
{
	m_viewType = type;
}

//=============================================================================
// カメラの視点の設定
// Author : 唐﨑結斗
// 概要 : 引数から視点を設定する
//=============================================================================
void CCamera::SetPosVOffset(const D3DXVECTOR3 posVOffset)
{
	// 視点の設定
	m_posVOffset = posVOffset;
}

//=============================================================================
// カメラの注視点の設定
// Author : 唐﨑結斗
// 概要 : 引数から注視点を設定する
//=============================================================================
void CCamera::SetPosROffset(const D3DXVECTOR3 posROffset)
{
	// 注視点の設定
	m_posROffset = posROffset;
}

//=============================================================================
// カメラの視点から注視点までの距離を設定する
// Author : 唐﨑結斗
// 概要 : 視点から注視点までの距離を算出する
//=============================================================================
void CCamera::SetRot(const D3DXVECTOR3 rot)
{
	// 向きの算出
	m_rot = rot;
}

//=============================================================================
// 追従相手の設定
// Author : 唐﨑結斗
// 概要 : 追従相手の設定
//=============================================================================
void CCamera::SetFollowTarget(CObject *pTarget, float fCoeffFllow)
{
	m_pTarget = pTarget;
	m_fCoeffFllow = fCoeffFllow;
	m_bFllow = true;
}

//=============================================================================
// 追従相手の設定
// Author : 唐﨑結斗
// 概要 : 追従相手の設定
//=============================================================================
void CCamera::SetFollowTarget(bool bUse)
{
	if (!bUse)
	{// 追従終了
		m_pTarget = nullptr;
		m_bFllow = bUse;
	}
}

//=============================================================================
// ターゲット(注視点)の設定
// Author : 唐﨑結斗
// 概要 : ターゲット(注視点)の設定
//=============================================================================
void CCamera::SetTargetPosR(CObject *pTargetPosR)
{
	m_pTargetPosR = pTargetPosR;
}

//=============================================================================
// ターゲット(注視点)の設定
// Author : 唐﨑結斗
// 概要 : ターゲット(注視点)の設定
//=============================================================================
void CCamera::SetTargetPosR(bool bUse)
{
	if (!bUse)
	{// 追従終了
		m_pTargetPosR = nullptr;
	}
}

//=============================================================================
// 回転の使用状況の設定
// Author : 唐﨑結斗
// 概要 : 回転の使用状況の設定
//=============================================================================
void CCamera::SetUseRoll(bool X, bool Y)
{
	m_bUseRollX = X;	// X軸の回転の使用状況
	m_bUseRollY = Y;	// Y軸の回転の使用状況
}

//=============================================================================
// ビューポートサイズの設定
// Author : 唐﨑結斗
// 概要 : 画面左上を0.0に画面サイズを設定
//=============================================================================
void CCamera::SetViewSize(DWORD X, DWORD Y, int fWidth, int fHeight)
{
	//引数を代入
	m_viewport.X = X;					//ビューポートの左上X座標
	m_viewport.Y = Y;					//ビューポートの左上Y座標
	m_viewport.Width = fWidth;			//ビューポートの幅
	m_viewport.Height = fHeight;		//ビューポートの高さ
}

//=============================================================================
// マトリックス計算を行う
// Author : 唐﨑結斗
// 概要 : 
//=============================================================================
void CCamera::Shake(const int nTime, const float fShake)
{
	m_nCntShake = nTime;
	m_fShake = fShake;
}

//=============================================================================
// マトリックス計算を行う
// Author : 唐﨑結斗
// 概要 : 
//=============================================================================
void CCamera::CalMatrix()
{
	if (m_pTarget != nullptr)
	{
		m_pos = m_pTarget->GetPos();
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);											// 行列初期化関数

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);			// 行列回転関数
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);						// 行列掛け算関数 

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);				// 行列移動関数
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);					// 行列掛け算関数

	// ワールドマトリックスをローカル変換
	if (m_pTargetPosR != nullptr)
	{// 注視点の設定
		m_posR = m_pTargetPosR->GetPos();

		// 方向ベクトルの計算
		D3DXVECTOR3 diff = m_pTargetPosR->GetPos() - m_pos;

		// 視点の設定
		D3DXVec3TransformCoord(&m_posV, &m_posVOffset, &m_mtxWorld);
		D3DXVECTOR3 vec = m_posR - m_posV;
		D3DXVec3Normalize(&vec, &vec);
		D3DXVECTOR3 rightAngle = D3DXVECTOR3(vec.z, vec.y, vec.x * -1);

		// 視点の算出
		m_posV += -vec * m_posRDiff.y;
		m_posV += -rightAngle * m_posRDiff.x;

		// 向きの算出
		//m_rot.x = atan2f(diff.y, fDiffLength);
		m_rot.y = atan2f(diff.x, diff.z);
	}
	else
	{
		D3DXVec3TransformCoord(&m_posV, &m_posVOffset, &m_mtxWorld);
		D3DXVec3TransformCoord(&m_posR, &m_posROffset, &m_mtxWorld);
	}

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=============================================================================
// カメラの回転
// Author : 唐﨑結斗
// 概要 : 
//=============================================================================
void CCamera::Rotate(void)
{
	// 入力情報の取得
	const float MIN_MOUSE_MOVED = 1.0f;

	CMouse *pMouse = CApplication::GetMouse();
	D3DXVECTOR3 rollDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// マウスの移動量の取得
	D3DXVECTOR3 mouseMove = D3DXVECTOR3(pMouse->GetMouseMove().y, pMouse->GetMouseMove().x, pMouse->GetMouseMove().z);

	if (D3DXVec3Length(&mouseMove) > MIN_MOUSE_MOVED || D3DXVec3Length(&mouseMove) < -MIN_MOUSE_MOVED)
	{// マウスの移動方向のノーマライズ
		D3DXVec3Normalize(&mouseMove, &mouseMove);

		// 移動方向の算出
		rollDir = mouseMove * (D3DX_PI / 180.0f);

		if (!m_bUseRollX)
		{
			rollDir.x = 0.0f;
		}
		if (!m_bUseRollY)
		{
			rollDir.y = 0.0f;
		}
	}

	// 摩擦係数の計算
	m_pRoll->Moving(rollDir);
	rollDir = m_pRoll->GetMove();

	// カメラの向きの正規化
	rollDir.x = CCalculation::RotNormalization(rollDir.x);
	rollDir.y = CCalculation::RotNormalization(rollDir.y);
	rollDir.z = CCalculation::RotNormalization(rollDir.z);

	// 回転
	m_rot += rollDir;

	if (m_rot.y < -D3DX_PI)
	{// 向きが-D3DX_PI未満の時
		m_rot.y += D3DX_PI * 2;
	}
	if (m_rot.y > D3DX_PI)
	{// 向きがD3DX_PI以上の時
		m_rot.y -= D3DX_PI * 2;
	}
	if (m_rot.x < -D3DX_PI * 0.5f + 0.1f)
	{// 向きが-D3DX_PI未満の時
		m_rot.x = -D3DX_PI * 0.5f + 0.1f;
	}
	if (m_rot.x > D3DX_PI * 0.5f - 0.1f)
	{// 向きがD3DX_PI以上の時
		m_rot.x = D3DX_PI * 0.5f - 0.1f;
	}
}

//=============================================================================
// カメラの移動
// Author : 唐﨑結斗
// 概要 : カメラの移動
//=============================================================================
void CCamera::Move(void)
{
	const float CAMERA_MOVE_SPEED = 5.0f;
	CKeyboard *pKeyboard = CApplication::GetKeyboard();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (pKeyboard->GetPress(DIK_W) == true
		|| pKeyboard->GetPress(DIK_A) == true
		|| pKeyboard->GetPress(DIK_D) == true
		|| pKeyboard->GetPress(DIK_S) == true)
	{// 移動キーが押された
	 // 前後左右移動
		if (pKeyboard->GetPress(DIK_W) == true)
		{// [W]キーが押された時
			if (pKeyboard->GetPress(DIK_A) == true)
			{// [A]キーが押された時
			 // 移動方向の更新
				m_rotMove.y = D3DX_PI * -0.25f;
				m_rotMove.x = D3DX_PI * -0.25f;
			}
			else if (pKeyboard->GetPress(DIK_D) == true)
			{// [D]キーが押された時
			 // 移動方向の更新
				m_rotMove.y = D3DX_PI * 0.25f;
				m_rotMove.x = D3DX_PI * 0.25f;
			}
			else
			{// 移動方向の更新
				m_rotMove.y = D3DX_PI * 0.0f;
				m_rotMove.x = D3DX_PI * 0.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// [S]キーが押された時
			if (pKeyboard->GetPress(DIK_A) == true)
			{// [A]キーが押された時
			 // 移動方向の更新
				m_rotMove.y = D3DX_PI * -0.75f;
				m_rotMove.x = D3DX_PI * -0.75f;
			}
			else if (pKeyboard->GetPress(DIK_D) == true)
			{// [D]キーが押された時
			 // 移動方向の更新
				m_rotMove.y = D3DX_PI * 0.75f;
				m_rotMove.x = D3DX_PI * 0.75f;
			}
			else
			{// 移動方向の更新
				m_rotMove.y = 0.0f;
				m_rotMove.x = D3DX_PI;
			}
		}
		else if (pKeyboard->GetPress(DIK_A) == true)
		{// [A]キーが押された時
		 // 移動方向の更新
			m_rotMove.y = D3DX_PI * -0.5f;
			m_rotMove.x = D3DX_PI * 0.0f;
		}
		else if (pKeyboard->GetPress(DIK_D) == true)
		{// [D]キーが押された時
		 // 移動方向の更新
			m_rotMove.y = D3DX_PI * 0.5f;
			m_rotMove.x = D3DX_PI * 0.0f;
		}

		// 移動方向の算出
		m_rotMove.x += m_rot.x;

		if (m_rotMove.x > D3DX_PI)
		{// 移動方向の正規化
			m_rotMove.x -= D3DX_PI * 2;
		}
		else if (m_rotMove.x < -D3DX_PI)
		{// 移動方向の正規化
			m_rotMove.x += D3DX_PI * 2;
		}

		m_rotMove.y += m_rot.y;

		if (m_rotMove.y > D3DX_PI)
		{// 移動方向の正規化
			m_rotMove.y -= D3DX_PI * 2;
		}
		else if (m_rotMove.y < -D3DX_PI)
		{// 移動方向の正規化
			m_rotMove.y += D3DX_PI * 2;
		}

		// 視点位置の更新
		move.z = sinf(m_rotMove.x) * cosf(m_rotMove.y);
		move.x = sinf(m_rotMove.x) * sinf(m_rotMove.y);
		move.y = cosf(m_rotMove.x);
	}

	// 摩擦係数の計算
	m_pMove->Moving(move);
	m_pos += m_pMove->GetMove();
}

//=============================================================================
// カメラの拡縮処理
// Author : 唐﨑結斗
// 概要 : マウスホイールでカメラの拡縮を行う
//=============================================================================
void CCamera::Zoom()
{// マウス情報の取得
	CMouse *pMouse = CApplication::GetMouse();

	m_posVOffset.z += (float)pMouse->GetMouseWheel() * 1.0f;
}


