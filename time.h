//=============================================================================
//
// タイムクラス(time.h)
// Author : 唐﨑結斗
// 概要 : タイム生成を行う
//
//=============================================================================
#ifndef _TIME_H_		// このマクロ定義がされてなかったら
#define _TIME_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;

//=============================================================================
// タイムクラス
// Author : 唐﨑結斗
// 概要 : タイム生成を行うクラス
//=============================================================================
class CTime : public CObject
{
public:
	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CTime *Create(int nMaxDigit);		// タイムの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CTime(int nPriority = CObject::PRIORITY_LEVEL3);
	~CTime() override;

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;														// 初期化
	HRESULT Init(int nDigit);														// 初期化(オーバーロード)
	void Uninit() override;															// 終了
	void Update() override;															// 更新
	void Draw() override;															// 描画
	void SetPos(const D3DXVECTOR3 &pos) override;									// 位置のセッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }		// 過去位置のセッター
	void SetRot(const D3DXVECTOR3 &rot) override;									// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size) override;									// 大きさのセッター
	void SetWholeSize(const D3DXVECTOR3 &wholeSize);								// 全体の大きさのセッター
	void SetColor(const D3DXCOLOR &col);										
	D3DXVECTOR3 GetPos() override { return m_pos; }									// 位置のゲッター
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }							// 過去位置のゲッター
	D3DXVECTOR3 GetRot()  override { return m_rot; }								// 向きのゲッター
	D3DXVECTOR3 GetSize()  override { return m_size; }								// 大きさのゲッター
	D3DXVECTOR3 GetWholeSize() { return m_wholeSize; }								// 全体の大きさのゲッター
	void SetTime(int nTime);														// タイムのセッター
	void SetNumber();																// ナンバーの設定
	void CalTime();																	// タイムの算出
	void SetTimeAdd(bool bAdd) { m_bAdd = bAdd; }									// 加算
	bool GetTimeEnd() { return m_bTimeEnd; }										// 時間切れ

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void SetDigitNumbers();
	void AddDigit();

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CNumber				**m_pNumber;		// ナンバー
	D3DXVECTOR3			m_pos;				// 位置
	D3DXVECTOR3			m_posOld;			// 過去の位置
	D3DXVECTOR3			m_rot;				// 向き
	D3DXVECTOR3			m_size;				// 大きさ
	D3DXVECTOR3			m_wholeSize;		// 全体の大きさ
	int					m_nMaxDigit;		// 最大桁数
	int					m_nDigit;			// 桁数
	int					m_nTime;			// タイム
	int					m_nCntFrame;		// フレームカウント
	bool				m_bAdd;				// 加算
	bool				m_bTimeEnd;			// 時間切れ
};

#endif


