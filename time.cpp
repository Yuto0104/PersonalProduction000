//=============================================================================
//
// タイムクラス(time.h)
// Author : 唐﨑結斗
// 概要 : タイム生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <math.h>

#include "time.h"
#include "number.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : タイムを生成する
//=============================================================================
CTime *CTime::Create(int nMaxDigit)
{
	// オブジェクトインスタンス
	CTime *pTime = nullptr;

	pTime = new CTime;

	// メモリの確保ができなかった
	assert(pTime != nullptr);

	// 数値の初期化
	pTime->Init(nMaxDigit);

	// インスタンスを返す
	return pTime;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CTime::CTime(int nPriority /*= CObject::PRIORITY_LEVEL3*/) : CObject(nPriority)
{
	m_pNumber = nullptr;								// ナンバー
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 過去位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさ
	m_wholeSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 全体の大きさ
	m_nDigit = 0;										// 桁数
	m_nTime = 0;										// タイム
	m_nCntFrame = 0;									// フレームカウント
	m_bAdd = true;										// 加算
	m_bTimeEnd = false;									// 時間切れ
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CTime::~CTime()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CTime::Init()
{
	return S_OK;
}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CTime::Init(int nDigit)
{
	// 桁数の設定
	m_nMaxDigit = nDigit;
	m_nDigit = 1;

	// タイムの設定
	m_nTime = 0;

	// 位置の設定
	m_pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);

	// 向きの設定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 大きさの設定
	m_size = D3DXVECTOR3(30.0f, 50.0f, 0.0f);

	// 全体の大きさの設定
	m_wholeSize = D3DXVECTOR3(300.0f, 50.0f, 0.0f);

	// ナンバーの生成
	SetDigitNumbers();

	// ナンバーの設定
	SetNumber();

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CTime::Uninit()
{
	if (m_pNumber != nullptr)
	{// 終了処理
		for (int nCntDigit = 0; nCntDigit < m_nDigit; nCntDigit++)
		{
			m_pNumber[nCntDigit]->Uninit();
		}

		// メモリの解放
		delete[] m_pNumber;
		m_pNumber = nullptr;
	}

	// タイムの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CTime::Update()
{
	CalTime();
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CTime::Draw()
{

}

//=============================================================================
// 位置のセッター
// Author : 唐﨑結斗
// 概要 : 位置のメンバ変数に引数を代入
//=============================================================================
void CTime::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;

	// ナンバーの設定
	SetNumber();
}

//=============================================================================
// 向きのセッター
// Author : 唐﨑結斗
// 概要 : 向きのメンバ変数に引数を代入
//=============================================================================
void CTime::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;

	// ナンバーの設定
	SetNumber();
}

//=============================================================================
// 大きさのセッター
// Author : 唐﨑結斗
// 概要 : 大きさのメンバ変数に引数を代入
//=============================================================================
void CTime::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;

	// ナンバーの設定
	SetNumber();
}

//=============================================================================
// 全体の大きさのセッター
// Author : 唐﨑結斗
// 概要 : 全体の大きさのメンバ変数に引数を代入
//=============================================================================
void CTime::SetWholeSize(const D3DXVECTOR3 & wholeSize)
{
	m_wholeSize = wholeSize;

	// ナンバーの設定
	SetNumber();
}

//=============================================================================
// 色のセッター
// Author : 唐﨑結斗
// 概要 : 色の設定を行う
//=============================================================================
void CTime::SetColor(const D3DXCOLOR & col)
{
	for (int nCntDigit = 0; nCntDigit < m_nDigit; nCntDigit++)
	{
		m_pNumber[nCntDigit]->SetCol(col);
	}
}

//=============================================================================
// タイムのセッター
// Author : 唐﨑結斗
// 概要 : タイムに数値を代入して、ナンバーの設定
//=============================================================================
void CTime::SetTime(int nTime)
{
	m_nTime = nTime;

	AddDigit();

	for (int nCntDigit = 0; nCntDigit < m_nDigit; nCntDigit++)
	{
		int nNum0 = (int)pow(10, nCntDigit + 1);
		int nNum1 = (int)pow(10, nCntDigit);

		m_pNumber[nCntDigit]->SetNumber(m_nTime % nNum0 / nNum1);
	}
}

//=============================================================================
// ナンバーの設定
// Author : 唐﨑結斗
// 概要 : ナンバーの設定を行う
//=============================================================================
void CTime::SetNumber()
{
	// サイズの算出結果を代入
	D3DXVECTOR3 size = m_size;
	float fFullSize = m_size.x * m_nDigit;

	if (m_wholeSize.x <= (m_size.x * m_nDigit))
	{
		size = D3DXVECTOR3(m_wholeSize.x / m_nDigit, m_size.y, 0.0f);
		fFullSize = m_wholeSize.x;
	}

	float fSpX = m_pos.x - (m_nDigit - 1) * 0.5f * size.x;

	for (int nCntDigit = 0; nCntDigit < m_nDigit; nCntDigit++)
	{
		int nNumDigit = (m_nDigit - 1) - nCntDigit;

		// 位置の設定
		m_pNumber[nNumDigit]->SetPos(D3DXVECTOR3(fSpX + size.x * nCntDigit, m_pos.y, 0.0f));

		// 向きの設定
		m_pNumber[nNumDigit]->SetRot(m_rot);

		// 大きさの設定
		m_pNumber[nNumDigit]->SetSize(size);
	}
}

//=============================================================================
// 時間の設定
// Author : 唐﨑結斗
// 概要 : 時間の設定を行う
//=============================================================================
void CTime::CalTime()
{
	m_nCntFrame++;

	if (m_nCntFrame % 60 == 0)
	{
		if (m_bAdd)
		{
			m_nTime++;
		}
		else
		{
			m_nTime--;

			if (m_nTime < 0)
			{
				m_nTime = 0;
				m_bTimeEnd = true;
			}
		}

		SetTime(m_nTime);
	}
}

//=============================================================================
// 数字の生成
// Author : 唐﨑結斗
// 概要 : 数字を生成する
//=============================================================================
void CTime::SetDigitNumbers()
{
	if (m_pNumber == nullptr)
	{// ナンバーのメモリ確保
		m_pNumber = new CNumber*[m_nDigit];
		assert(m_pNumber != nullptr);

		for (int nCntDigit = 0; nCntDigit < m_nDigit; nCntDigit++)
		{// ナンバーの設定
			m_pNumber[nCntDigit] = CNumber::Create();
		}
	}
}

//=============================================================================
// 桁数を増やす
// Author : 唐﨑結斗
// 概要 : 桁数を増やす
//=============================================================================
void CTime::AddDigit()
{
	while (1)
	{
		if (m_nTime >= (int)pow(10, m_nDigit))
		{
			if (m_pNumber != nullptr)
			{// 終了処理
				for (int nCntDigit = 0; nCntDigit < m_nDigit; nCntDigit++)
				{
					m_pNumber[nCntDigit]->Uninit();
				}

				// メモリの解放
				delete[] m_pNumber;
				m_pNumber = nullptr;
			}

			// 桁数の加算
			m_nDigit += 1;

			if (m_nMaxDigit <= m_nDigit)
			{
				m_nDigit = m_nMaxDigit;
			}

			// ナンバーの生成
			SetDigitNumbers();

			// ナンバーの設定
			SetNumber();
		}
		else if (m_nTime < (int)pow(10, m_nDigit - 1))
		{
			bool bBreak = false;

			if (m_pNumber != nullptr)
			{// 終了処理
				for (int nCntDigit = 0; nCntDigit < m_nDigit; nCntDigit++)
				{
					m_pNumber[nCntDigit]->Uninit();
				}

				// メモリの解放
				delete[] m_pNumber;
				m_pNumber = nullptr;
			}

			// 桁数の加算
			m_nDigit -= 1;

			if (1 >= m_nDigit)
			{
				m_nDigit = 1;
				bBreak = true;
			}

			// ナンバーの生成
			SetDigitNumbers();

			// ナンバーの設定
			SetNumber();

			if (bBreak)
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
}
