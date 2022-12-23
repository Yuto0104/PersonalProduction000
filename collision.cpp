//=============================================================================
//
// 衝突判定クラス(collision.cpp)
// Author : 唐﨑結斗
// 概要 : 衝突判定生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "collision.h"
#include "renderer.h"
#include "application.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CCollision *CCollision::m_pTop = { nullptr };			// 先頭オブジェクトへのポインタ
CCollision *CCollision::m_pCurrent = { nullptr };		// 現在の(一番後ろ)オブジェクトへのポインタ
int CCollision::m_nMax = 0;								// 使用数

//=============================================================================
// インスタンスの解放
// Author : 唐﨑結斗
// 概要 : すべてのインスタンスを解放する処理
//=============================================================================
void CCollision::ReleaseAll()
{
	if (m_pTop != nullptr)
	{// 変数宣言
		CCollision *pCollision = m_pTop;

		while (pCollision)
		{// 現在のオブジェクトの次のオブジェクトを保管
			CCollision *pCollisionNext = pCollision->m_pNext;

			if (!pCollision->m_bDeath)
			{// 終了処理
				pCollision->Uninit();

				// オブジェクトの解放
				pCollision->Release();
			}

			// 現在のオブジェクトの次のオブジェクトを更新
			pCollision = pCollisionNext;
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
void CCollision::UpdateAll(void)
{
	if (m_pTop != nullptr)
	{// 変数宣言
		CCollision *pCollision = m_pTop;

		while (pCollision)
		{// 現在のオブジェクトの次のオブジェクトを保管
			CCollision *pCollisionNext = pCollision->m_pNext;

			if (!pCollision->m_bDeath)
			{// オブジェクトの更新
				pCollision->Update();
			}

			// 現在のオブジェクトの次のオブジェクトを更新
			pCollision = pCollisionNext;
		}
	}
}

//=============================================================================
// すべてのインスタンスの描画
// Author : 唐﨑結斗
// 概要 : 使用されてるインスタンスの描画処理を呼び出す
//=============================================================================
void CCollision::DrawAll()
{
	if (m_pTop != nullptr)
	{// 変数宣言
		CCollision *pCollision = m_pTop;

		while (pCollision)
		{// 現在のオブジェクトの次のオブジェクトを保管
			CCollision *pCollisionNext = pCollision->m_pNext;

			if (!pCollision->m_bDeath)
			{// オブジェクトの描画
				pCollision->Draw();
			}

			// 現在のオブジェクトの次のオブジェクトを更新
			pCollision = pCollisionNext;
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
void CCollision::ReleaseListAll()
{
	if (m_pTop != nullptr)
	{// 変数宣言
		CCollision *pCollision = m_pTop;

		while (pCollision)
		{// 現在のオブジェクトの次のオブジェクトを保管
			CCollision *pCollisionNext = pCollision->m_pNext;

			if (pCollision->m_bDeath)
			{// オブジェクトのリスト解除
				pCollision->ReleaseList();
			}

			// 現在のオブジェクトの次のオブジェクトを更新
			pCollision = pCollisionNext;
		}
	}
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CCollision::CCollision() : m_pPrev(nullptr),			// 前のオブジェクトへのポインタ
m_pNext(nullptr),										// 次のオブジェクトへのポインタ
m_pCollidedObj(nullptr),								// 衝突したオブジェクトへのポインタ
m_pParent(nullptr),										// 親のオブジェクト
m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),						// 位置
m_bUse(true),											// 使用状況
m_bDeath(false)											// 死亡フラグ
{
	// 使用数のインクリメント
	m_nMax++;

	if (m_pTop == nullptr)
	{// 先頭ポインターの設定
		m_pTop = this;
	}
	else
	{// 過去の最後尾オブジェクトの次のオブジェクトを設定
		m_pCurrent->m_pNext = this;

		// 自分の前のオブジェクトの設定
		this->m_pPrev = m_pCurrent;
	}
	
	// 自分を現在のオブジェクトに設定
	m_pCurrent = this;
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CCollision::~CCollision()
{
	
}

//=============================================================================
// インスタンスの解放
// Author : 唐﨑結斗
// 概要 : すべてのインスタンスを解放する処理
//=============================================================================
void CCollision::Release(void)
{
	if (!m_bDeath)
	{// 使用数のデクリメント
		m_nMax--;

		// 死亡フラグを立てる
		m_bDeath = true;
	}
}

//=============================================================================
// リストの解除と破棄
// Author : 唐﨑結斗
// 概要 : リストの解除と破棄をする処理
//=============================================================================
void CCollision::ReleaseList(void)
{
	if (m_pTop == this)
	{// 先頭オブジェクトを自分の次のオブジェクトに設定
		m_pTop = m_pNext;
	}
	if (m_pCurrent == this)
	{// 最後尾オブジェクトを自分の前のオブジェクトに設定
		m_pCurrent = m_pPrev;
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

