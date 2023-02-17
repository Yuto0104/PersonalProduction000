//=============================================================================
//
// ワイヤー処理(wire.h)
// Author : 唐﨑結斗
// 概要 : ワイヤー生成を行う
//
//=============================================================================
#ifndef _WIRE_H_		// このマクロ定義がされてなかったら
#define _WIRE_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "model_obj.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CMove;

//=============================================================================
// ワイヤークラス
// Author : 唐﨑結斗
// 概要 : ワイヤー生成を行うクラス
//=============================================================================
class CWire : public CModelObj
{
public:
	//--------------------------------------------------------------------
	// 定数定義
	//--------------------------------------------------------------------
	static const float fDECISION;

	//--------------------------------------------------------------------
	// ワイヤーモードの列挙型
	//--------------------------------------------------------------------
	enum WIRE_MODE
	{
		MODE_STOP = 0,		// 停止
		MODE_FIRING,		// 射出
		MODE_ATTRACT,		// 引きつく
		MODE_HANGING,		// ぶら下がり
		MODE_PULL,			// 引っ張る
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CWire *Create(void);				// オブジェクトの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	explicit CWire();
	~CWire();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;												// 初期化
	void Uninit() override;													// 終了
	void Update() override;													// 更新
	void Draw() override;													// 描画
	void SetPos(const D3DXVECTOR3 &pos) override;							// 位置の設定
	CMove *GetMove() { return m_pMove; }									// 移動情報の取得
	void SetMoveVec(D3DXVECTOR3 moveVec) { m_moveVec = moveVec; }			// 移動方向の設定
	void SetWireMode(WIRE_MODE EWireMode);									// ワイヤーモードの設定
	CModelObj *GetStart() { return m_pStart; }								// スタート地点
	CModelObj *GetGoal() { return m_pGoal; }								// ゴール地点
	WIRE_MODE GetWireMode() { return m_EMode; }								// ワイヤーモードの取得
	CObject::EObjectType GetTargetObjType() { return m_ETargetObjType; }	// 当たったオブジェクトタイプの取得
	void SetHanging();
	void SetRotVec(D3DXVECTOR3 rotVec);
	D3DXVECTOR3 GetMoveing() { return m_move; }
	D3DXVECTOR3 HangingSearch();

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Move();		// 移動
	void Hanging();

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CModelObj *m_pStart;						// スタート地点
	CModelObj *m_pGoal;							// ゴール地点
	CModelObj *m_pDecision;						// 判定用オブジェクト
	CModel3D *m_pWireModel;						// モデル情報
	CMove *m_pMove;								// 移動情報
	CMove *m_pRoll;								// 回転情報
	WIRE_MODE m_EMode;							// ワイヤーモード
	WIRE_MODE m_ENextMode;						// 次のワイヤーモード
	CObject::EObjectType m_ETargetObjType;		// ターゲットのオブジェクトタイプ
	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	D3DXMATRIX m_mtxRot;						// 回転マトリックス
	D3DXQUATERNION m_quat;						// クォータニオン
	D3DXVECTOR3 m_move;							// 移動
	D3DXVECTOR3 m_moveVec;						// 移動ベクトル
	D3DXVECTOR3 m_rot;							// 向き
	D3DXVECTOR3 m_rotDest;						// 目的の向き
	D3DXVECTOR3 m_rotVec;						// 向きベクトル
	D3DXVECTOR3 m_vecAxis;						// 回転軸
	float m_fValueRot;							// 回転角
	float m_fLength;							// 長さ
	float m_fAddRot;							// 加算
	float m_fAccele;							// 角加速度
};

#endif





