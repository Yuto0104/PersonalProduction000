//**************************************************************************************************
//
// カメラ処理(camera.h)
// Auther：唐﨑結斗
// 概要 : カメラの設定処理
//
//**************************************************************************************************
#ifndef _CAMERA_H_			// このマクロ定義がされてなかったら
#define _CAMERA_H_			// 二重インクルード防止のマクロ定義

//***************************************************************************
// インクルード
//***************************************************************************
#include"main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CMove;
class CObject;

//=============================================================================
// カメラクラス
// Author : 唐﨑結斗
// 概要 : カメラ設定を行うクラス
//=============================================================================
class CCamera
{
private:
	//--------------------------------------------------------------------
	// 定数定義
	//--------------------------------------------------------------------
	static const float CAMERA_NEAR;			// ニアの最大値
	static const float CAMERA_FUR;			// ファーの最大値

public:
	//=============================================================================
	// 投影方法のタイプ列挙型
	// Author : 唐﨑結斗
	// 概要 : 投影方法の種別
	//=============================================================================
	enum VIEW_TYPE
	{
		TYPE_CLAIRVOYANCE = 0,		// 透視投影
		TYPE_PARALLEL,				// 平行投影
		MAX_VIEW_TYPE,				// タイプの最大数
	};

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CCamera();
	~CCamera();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init(void);														// 初期化
	void Uninit(void);														// 終了
	void Update(void);														// 更新
	void Set(void);															// 設定
	void SetViewType(VIEW_TYPE type);										// タイプの設定
	void Zoom();															// カメラの拡縮
	D3DXVECTOR3 GetPosV() { return m_posV; }
	D3DXVECTOR3 GetPosR() { return m_posR; }
	D3DXVECTOR3 GetRot() { return m_rot; }
	D3DXMATRIX GetMtxView() { return m_mtxView; }
	D3DXMATRIX GetMtxProj() { return m_mtxProj; }
	CObject	*GetTarget() { return m_pTarget; }
	void SetPosVOffset(const D3DXVECTOR3 posVOffset);
	void SetPosROffset(const D3DXVECTOR3 posROffset);
	void SetRot(const D3DXVECTOR3 rot);
	void SetFollowTarget(CObject *pTarget, float fCoeffFllow);
	void SetFollowTarget(bool bUse);
	void SetTargetPosR(CObject *pTargetPosR);
	void SetTargetPosR(bool bUse);
	void SetViewing(const float fViewing) { m_fViewing = fViewing; }
	void SetPosRDiff(const D3DXVECTOR2 posRDiff) { m_posRDiff = posRDiff; }
	void SetAspect(D3DXVECTOR2 aspect) { m_aspect = aspect; }
	void SetUseRoll(bool X, bool Y);
	void SetViewSize(DWORD X, DWORD Y, int fWidth, int fHeight);
	CObject *GetTargetPosR() { return m_pTargetPosR; }
	void Shake(const int nTime, const float fShake);

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void CalMatrix();				// マトリックス計算
	void Rotate(void);				// 回転
	void Move(void);				// 移動

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CMove				*m_pMove;			// 移動クラスのインスタンス
	CMove				*m_pRoll;			// 移動クラスのインスタンス(角度)
	CObject				*m_pTarget;			// ターゲット(位置)
	CObject				*m_pTargetPosR;		// ターゲット(注視点)
	D3DXMATRIX			m_mtxWorld;			// ワールドマトリックス
	D3DXMATRIX			m_mtxProj;			// プロジェクションマトリックス
	D3DXMATRIX			m_mtxView;			// ビューマトリックス
	D3DVIEWPORT9        m_viewport;			// ビューポート
	D3DXVECTOR3			m_pos;				// 位置
	D3DXVECTOR3			m_posV;				// 視点
	D3DXVECTOR3			m_posR;				// 注視点
	D3DXVECTOR3			m_vecU;				// 上方向ベクトル
	D3DXVECTOR3			m_posVOffset;		// 視点差分
	D3DXVECTOR3			m_posROffset;		// 注視点差分
	D3DXVECTOR3			m_rot;				// 向き
	D3DXVECTOR3			m_rotMove;			// 移動方向
	D3DXVECTOR2			m_posRDiff;			// 注視点ターゲット時の差分
	D3DXVECTOR2			m_aspect;			// アスペクト比の値
	VIEW_TYPE			m_viewType;			// 投影の種別
	float				m_fViewing;			// 視野角
	float				m_fRotMove;			// 移動方向
	float				m_fCoeffFllow;		// 追従の減衰係数
	float				m_fShake;			// 揺れの量
	int					m_nCntShake;		// 揺れのカウント
	bool				m_bUseRollX;		// X軸の回転の使用状況
	bool				m_bUseRollY;		// Y軸の回転の使用状況
	bool				m_bFllow;			// 追従を行うか
};

#endif
