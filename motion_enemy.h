//=============================================================================
//
// モーションエネミークラス(motion_enemy.h)
// Author : 唐�ｱ結斗
// 概要 : モーションエネミー生成を行う
//
//=============================================================================
#ifndef _MOTION_ENEMY_H_			// このマクロ定義がされてなかったら
#define _MOTION_ENEMY_H_			// 二重インクルード防止のマクロ定義

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
#include "motion_model3D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CMove;
class CCollision_Rectangle3D;

//=============================================================================
// モーションエネミークラス
// Author : 唐�ｱ結斗
// 概要 : モーションエネミー生成を行うクラス
//=============================================================================
class CMotionEnemy : public CMotionModel3D
{
public:
	//--------------------------------------------------------------------
	// モーションエネミーのアクションの列挙型
	//--------------------------------------------------------------------
	enum ACTION_TYPE
	{
		NEUTRAL_ACTION = 0,		// ニュートラル
		MOVE_ACTION,			// 移動
		ATTACK_ACTION,			// 攻撃
		MAX_ACTION,				// 最大数
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CMotionEnemy *Create();			// モーションエネミーの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CMotionEnemy();
	~CMotionEnemy();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;										// 初期化
	void Uninit() override;											// 終了
	void Update() override;											// 更新
	void Draw() override;											// 描画
	void SetSpeed(const float fSpeed) { m_fSpeed = fSpeed; }		// 移動速度の設定
	
private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	D3DXVECTOR3 Move();		// 移動
	void Rotate();			// 回転
		
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CMove						*m_pMove;				// 移動情報
	CCollision_Rectangle3D		*m_pCollRectangle3D;	// 3D矩形の当たり判定
	ACTION_TYPE					m_EAction;				// アクションタイプ
	D3DXVECTOR3					m_move;					// 移動ベクトル
	D3DXVECTOR3					m_rotDest;				// 目的の向き
	float						m_fSpeed;				// 移動速度	
	int							m_nNumMotion;			// 現在のモーション番号
};

#endif


