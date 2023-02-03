//=============================================================================
//
// ランキングクラス(ranking.h)
// Author : 唐﨑結斗
// 概要 : ランキング生成を行う
//
//=============================================================================
#ifndef _RANKING_H_		// このマクロ定義がされてなかったら
#define _RANKING_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScore;
class CObject2D;

//=============================================================================
// ランキングクラス
// Author : 唐﨑結斗
// 概要 : ランキング生成を行うクラス
//=============================================================================
class CRanking : public CObject
{
public:
	//--------------------------------------------------------------------
	// コマンドタイプの設定
	// Author : 唐﨑結斗
	// 概要 : 
	//--------------------------------------------------------------------
	enum COMMAND_TYPE
	{
		COMMAND_TYPE_NONE = 0,		// タイプ無し
		COMMAND_TYPE_SET,			// セッターコマンド
		COMMAND_TYPE_GET,			// ゲッターコマンド
		COMMAND_TYPE_MAX			// コマンドの最大数
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CRanking *Create(int nMaxRanking, int nNewScore);		// ランキングの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CRanking(int nPriority = CObject::PRIORITY_LEVEL3);
	~CRanking() override;

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;														// 初期化
	HRESULT Init(int nMaxRanking, int nNewScore);									// 初期化(オーバーロード)
	void Uninit() override;															// 終了
	void Update() override;															// 更新
	void Draw() override;															// 描画
	void SetPos(const D3DXVECTOR3 &pos) override;									// 位置のセッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }		// 過去位置のセッター
	void SetRot(const D3DXVECTOR3 &rot) override;									// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size) override;									// 大きさのセッター
	void SetWholeSize(const D3DXVECTOR3 &wholeSize);								// 全体の大きさのセッター
	D3DXVECTOR3 GetPos() override { return m_pos; }									// 位置のゲッター
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }							// 過去位置のゲッター
	D3DXVECTOR3 GetRot()  override { return m_rot; }								// 向きのゲッター
	D3DXVECTOR3 GetSize()  override { return m_size; }								// 大きさのゲッター
	D3DXVECTOR3 GetWholeSize() { return m_wholeSize; }								// 全体の大きさのゲッター
	void GetRanking();																// ランキングの取得
	int SetRanking(const int nTime);												// ランキングの設定

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void CreateScore();
	void SetScore();
	void LoadRanking();
	void LoadFile();
	void SetScoreNomber();
	void FlashScore();
	void SetFailObj();

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CScore				**m_pScore;								// スコア
	CObject2D			*pFailObj;								// 失敗時のアナウンス用のオブジェクト
	D3DXVECTOR3			m_pos;									// 位置
	D3DXVECTOR3			m_posOld;								// 過去の位置
	D3DXVECTOR3			m_rot;									// 向き
	D3DXVECTOR3			m_size;									// 大きさ
	D3DXVECTOR3			m_wholeSize;							// 全体の大きさ
	int					m_nMaxRanking;							// ランキング数
	int					m_nDigit;								// 桁数
	int					*m_aRankingData;						// ランキングデータ
	int					m_nPortNum;								// ポート番号
	int					m_nNewScore;							// 新しいスコア
	int					m_UpdateRank;							// 更新されたランク
	float				m_fAddAlpha;							// アルファの加算値
	char				m_nIpAddress[128];						// IPアドレス
	bool				m_bUpdatingRanking;						// 通信してるか否か
	bool				m_bRankingUpdatedSuccess;				// 成功してるか否か
};

#endif

