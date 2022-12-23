//**************************************************************************************************
//
// モーション処理(motion.h)
// Auther：唐﨑結斗
// 概要 : モーションクラスの設定
//
//**************************************************************************************************
#ifndef _MOTION_H_			// このマクロ定義がされてなかったら
#define _MOTION_H_			// 二重インクルード防止のマクロ定義

//***************************************************************************
// インクルード
//***************************************************************************
#include "model3D.h"
#include "renderer.h"
#include <vector>

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CParts;
class CModel3D;

//=============================================================================
// モーションクラス
// Author : 唐﨑結斗
// 概要 : モーション設定を行うクラス
//=============================================================================
class CMotion
{
public:
	//***************************************************************************
	// 定数定義
	//***************************************************************************
	static const unsigned int MAX_MOTION = (128);					// モーション数の最大数
	static const unsigned int MAX_MODEL_PARTS = (128);				// モデル数の最大数
	static const unsigned int MAX_KEY = (64);						// キーの最大数
	static const unsigned int MAX_KEYSET = (64);					// キー設定の最大数
	static const unsigned int MOTION_BLEND_FRAM = (12);				// モーションブレンドのフレーム数	

	//***************************************************************
	// キー構造体を定義
	//***************************************************************
	typedef struct
	{
		D3DXVECTOR3		pos;	// 現在位置
		D3DXVECTOR3		rot;	// 現在の向き
	}MyKey;

	//***************************************************************
	// キー設定構造体を定義
	//***************************************************************
	typedef struct
	{
		int					nFrame;			// フレーム数
		std::vector<MyKey>	pKey;			// キー情報
	}MyKeySet;

	//***************************************************************
	// モーション設定構造体を定義
	//***************************************************************
	typedef struct
	{
		int						nNumKey;				// キー数
		int						nCntKeySet;				// キーセットカウント
		int						nCntFrame;				// フレームカウント
		bool					bLoop;					// ループ使用状況
		bool					bMotion;				// モーションを行っているか
		std::vector<MyKeySet>	pKeySet;				// キー設定情報
	}MyMotion;

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CMotion(const char *pFileName);
	~CMotion();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	// 更新
	void Update();

	// 終了
	void Uninit(void);

	// モーションストップ
	void StopMotion(bool bStop) { m_bStop = bStop; }

	// パーツをもとの場所に配置する
	void SetPartsOrigin();

	// モーションの初期設定
	void SetMotion(const int nCntMotionSet);

	// モーションの初期設定
	void SetMotion(const int nCntMotionSet, const int nNumKeySet);

	// パーツの設定
	void SetParts(D3DXMATRIX mtxWorld);

	// モーション番号の設定
	void SetNumMotion(const int nNumMotion);

	// モーションを行っているか設定
	void SetUseMotion(bool bMotion) { m_bMotion = bMotion; }

	// モーションブレンドを行っているか設定
	void SetMotionBlend(bool bMotionBlend) { m_bMotionBlend = bMotionBlend; }

	// パーツの取得
	CParts *GetParts(const int nNumParts) { return m_pParts[nNumParts]; }

	// パーツの最大数の取得
	int GetMaxParts() { return m_nMaxParts; }

	// パーツの最大数の取得
	int GetMaxKeySet(const int nNumMotion) { return m_motion[nNumMotion].nNumKey; }

	// 扱うモーションの番号の取得
	int GetNumMotion() { return m_nNumMotion; }

	// モーションを行っているか取得
	bool GetMotion() { return m_bMotion; }

	// モーションブレンドを行っているか取得
	bool GetMotionBlend() { return m_bMotionBlend; }

	// モーションの停止フラグの取得
	bool GetStopFlag() { return m_bStop; }

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	// 初期化
	void Init();

	// モーションの再生
	void PlayMotion();

	// モーションブレンド
	void MotionBlend();

	// モーション読み込み
	void LoodSetMotion(const char *pFileName);

	// カウントリセット
	void CntReset(const int nNumMotionOld);

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CModel3D					*m_pParent;									// 親
	std::vector<MyMotion>		m_motion;									// モーション
	std::vector<CParts*>		m_pParts;									// パーツ
	char						m_partsFile[MAX_MODEL_PARTS][0xff];			// パーツのXファイル名
	int							m_nMaxParts;								// パーツ数
	int							m_nNumMotion;								// 扱うモーション
	bool						m_bMotion;									// モーションを行うか
	bool						m_bMotionBlend;								// モーションブレンド
	bool						m_bStop;									// モーションの停止フラグ
};

#endif

