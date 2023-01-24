//=============================================================================
//
// 移動処理(move.h)
// Author : 唐﨑結斗
// 概要 : 移動に関する計算を行う
//
//=============================================================================
#ifndef _MOVE_H_		// このマクロ定義がされてなかったら
#define _MOVE_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// ライブラリーリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")			// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")		// [d3d9.lib]の拡張ライブラリ

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "d3dx9.h"							// 描画処理に必要

//=============================================================================
// 移動クラス
// Author : 唐﨑結斗
// 概要 : 移動設定を行うクラス
//=============================================================================
class CMove
{
public:
	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CMove();
	virtual ~CMove();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void SetMoving(float fSpeed, float fMaxSpeed, float fMinSpeed, float fFriction);	// 移動情報の初期設定
	void SetMove(D3DXVECTOR3 move) { m_move = move; }									// 移動量の設定
	void SetSpeed(const float fSpeed) { m_fSpeed = fSpeed; }							// 速度の設定
	void SetMaxSpeed(const float fMaxSpeed) { m_fMaxSpeed = fMaxSpeed; }				// 最高速度の設定
	void SetMinSpeed(const float fMinSpeed) { m_fMinSpeed = fMinSpeed; }				// 最低速度の設定
	void SetFriction(const float fFriction) { m_fFriction = fFriction; }				// 摩擦係数の設定
	void Moving(const D3DXVECTOR3 &moveDir);											// 慣性を伴う移動
	void SetMoveVec(const D3DXVECTOR3 vec);												// 移動方向ベクトルの設定
	D3DXVECTOR3 GetMove() { return m_move; }											// 移動ベクトルのゲッター
	float GetSpeed() { return m_fSpeed; }												// 速度の取得
	float GetMaxSpeed() { return m_fMaxSpeed; }											// 最高速度の取得
	float GetMinSpeed() { return m_fMinSpeed; }											// 最低速度の取得
	float GetFriction() { return m_fFriction; }											// 摩擦係数の取得
	float GetMoveLength() { return D3DXVec3Length(&m_move); }							// 移動距離のゲッター

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	D3DXVECTOR3			m_move;					// 移動量
	float				m_fSpeed;				// 移動速度
	float				m_fMaxSpeed;			// 最高速度
	float				m_fMinSpeed;			// 最低速度
	float				m_fFriction;			// 摩擦係数
};

#endif