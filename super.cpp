//=============================================================================
//
// オブジェクトクラス(Super.h)
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "super.h"
#include "debug_proc.h"
#include "renderer.h"
#include "application.h"
#include "camera.h"
#include "object.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CSuper *CSuper::m_pTop[MAX_LEVEL] = { nullptr };		// 先頭オブジェクトへのポインタ
CSuper *CSuper::m_pCurrent[MAX_LEVEL] = { nullptr };	// 現在の(一番後ろ)オブジェクトへのポインタ
int CSuper::m_nMax = 0;									// 使用数
int CSuper::m_nPriorityMax[MAX_LEVEL] = { 0 };			// プライオリティごとのオブジェクト数
bool CSuper::m_bPause = false;							// ポーズを使用しているかどうか

//=============================================================================
// インスタンスの解放
// Author : 唐﨑結斗
// 概要 : すべてのインスタンスを解放する処理
//=============================================================================
void CSuper::ReleaseAll(bool bPermanent)
{
	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{// 変数宣言
			CSuper *pSuper = m_pTop[nCntPriority];

			while (pSuper)
			{// 現在のオブジェクトの次のオブジェクトを保管
				CSuper *pSuperNext = pSuper->m_pNext;
				if (bPermanent)
				{
					if (!pSuper->m_bDeath)
					{// 終了処理
						pSuper->Uninit();

						// オブジェクトの解放
						pSuper->Release();
					}
				}
				else
				{
					if (!pSuper->m_bDeath
						&& !pSuper->m_bPermanent)
					{// 終了処理
						pSuper->Uninit();

						// オブジェクトの解放
						pSuper->Release();
					}
				}

				// 現在のオブジェクトの次のオブジェクトを更新
				pSuper = pSuperNext;
			}
		}
	}

	// すべてのリスト解除
	ReleaseListAll();
}

//=============================================================================
// すべてのインスタンスの更新
// Author : 唐﨑結斗
// 概要 : 使用されてるインスタンスの更新処理を呼び出す
//=============================================================================
void CSuper::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{// 変数宣言
			CSuper *pSuper = m_pTop[nCntPriority];

			while (pSuper)
			{// 現在のオブジェクトの次のオブジェクトを保管
				CSuper *pSuperNext = pSuper->m_pNext;

				if (!pSuper->m_bDeath)
				{// オブジェクトの更新
					if (pSuper->GetElement() != ELEMENT_OBJECT)
					{
						pSuper->Update();
					}
					if (pSuper->GetElement() == ELEMENT_OBJECT)
					{
						CObject *pObject = (CObject*)pSuper;
					
						if (!m_bPause)
						{
							pSuper->Update();
						}
						else if (m_bPause
							&& pObject->GetObjType() == CObject::OBJTYPE_PAUSE)
						{
							pSuper->Update();
						}
					}
				}
				
				// 現在のオブジェクトの次のオブジェクトを更新
				pSuper = pSuperNext;
			}
		}
	}
}

//=============================================================================
// すべてのインスタンスの描画
// Author : 唐﨑結斗
// 概要 : 使用されてるインスタンスの描画処理を呼び出す
//=============================================================================
void CSuper::DrawAll()
{
	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{// 変数宣言
			CSuper *pSuper = m_pTop[nCntPriority];

			while (pSuper)
			{// 現在のオブジェクトの次のオブジェクトを保管
				CSuper *pSuperNext = pSuper->m_pNext;

				if (!pSuper->m_bDeath)
				{// オブジェクトの描画
					pSuper->Draw();
				}

				// 現在のオブジェクトの次のオブジェクトを更新
				pSuper = pSuperNext;
			}
		}
	}

	// すべてのリスト解除
	ReleaseListAll();
}

//=============================================================================
// すべてのインスタンスの描画
// Author : 唐﨑結斗
// 概要 : 使用されてるインスタンスの描画処理を呼び出す
//=============================================================================
void CSuper::DrawAll(DRAW_TYPE EDrawType)
{
	// カメラポインタ
	CCamera *pCamera = nullptr;

	switch (EDrawType)
	{
	case CSuper::TYPE_GAME:
		pCamera = CApplication::GetCamera();
		break;

	case CSuper::TYPE_MAP:
		pCamera = CApplication::GetMapCamera();
		break;

	default:
		assert(false);
		break;
	}

	// カメラの描画
	pCamera->Set();

	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{// 変数宣言
			CSuper *pSuper = m_pTop[nCntPriority];

			while (pSuper)
			{// 現在のオブジェクトの次のオブジェクトを保管
				CSuper *pSuperNext = pSuper->m_pNext;
				DRAW_TYPE EMyDrawType = pSuper->GetDrawType();

				if (!pSuper->m_bDeath
					&& (EMyDrawType == TYPE_ALL
					|| EMyDrawType == EDrawType))
				{// オブジェクトの描画
					pSuper->Draw();
				}

				// 現在のオブジェクトの次のオブジェクトを更新
				pSuper = pSuperNext;
			}
		}
	}

	// すべてのリスト解除
	ReleaseListAll();
}

//=============================================================================
// すべてのオブジェクトのリスト解除
// Author : 唐﨑結斗
// 概要 : すべてのオブジェクトのリスト解除を呼び出す
//=============================================================================
void CSuper::ReleaseListAll()
{
	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{// 変数宣言
			CSuper *pSuper = m_pTop[nCntPriority];

			while (pSuper)
			{// 現在のオブジェクトの次のオブジェクトを保管
				CSuper *pSuperNext = pSuper->m_pNext;

				if (pSuper->m_bDeath)
				{// オブジェクトのリスト解除
					pSuper->ReleaseList();
				}

				// 現在のオブジェクトの次のオブジェクトを更新
				pSuper = pSuperNext;
			}
		}
	}
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CSuper::CSuper(int nPriority/* = PRIORITY_LEVEL0*/) : m_pPrev(nullptr),			// 前のオブジェクトへのポインタ
m_pNext(nullptr),																// 次のオブジェクトへのポインタ
m_EElement(ELEMENT_NONE),														// 要素
m_EDrawType(TYPE_ALL),															// 描画方法
m_nLevPriority(nPriority),														// プライオリティのレベル
m_bDeath(false)																	// 死亡フラグ
{
	// 使用数のインクリメント
	m_nMax++;
	m_nPriorityMax[m_nLevPriority]++;

	if (m_pTop[m_nLevPriority] == nullptr)
	{// 先頭ポインターの設定
		m_pTop[m_nLevPriority] = this;
	}
	else
	{// 過去の最後尾オブジェクトの次のオブジェクトを設定
		m_pCurrent[m_nLevPriority]->m_pNext = this;

		// 自分の前のオブジェクトの設定
		this->m_pPrev = m_pCurrent[m_nLevPriority];
	}
	
	// 自分を現在のオブジェクトに設定
	m_pCurrent[m_nLevPriority] = this;
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CSuper::~CSuper()
{
	
}

//=============================================================================
// インスタンスの解放
// Author : 唐﨑結斗
// 概要 : すべてのインスタンスを解放する処理
//=============================================================================
void CSuper::Release(void)
{
	if (!m_bDeath)
	{// 使用数のデクリメント
		m_nMax--;
		m_nPriorityMax[m_nLevPriority]--;

		// 死亡フラグを立てる
		m_bDeath = true;
	}
}

//=============================================================================
// リストの解除と破棄
// Author : 唐﨑結斗
// 概要 : リストの解除と破棄をする処理
//=============================================================================
void CSuper::ReleaseList(void)
{
	if (m_pTop[m_nLevPriority] == this)
	{// 先頭オブジェクトを自分の次のオブジェクトに設定
		m_pTop[m_nLevPriority] = m_pNext;
	}
	if (m_pCurrent[m_nLevPriority] == this)
	{// 最後尾オブジェクトを自分の前のオブジェクトに設定
		m_pCurrent[m_nLevPriority] = m_pPrev;
	}

	if (m_pPrev != nullptr)
	{// 前のオブジェクトの前のオブジェクトに自分の次のオブジェクトを設定
		m_pPrev->m_pNext = m_pNext;
	}
	if (m_pNext != nullptr)
	{// 自分の次のオブジェクトに自分の前のオブジェクトを設定
		m_pNext->m_pPrev = m_pPrev;
	}

	// オブジェクトの解放
	delete this;
}

