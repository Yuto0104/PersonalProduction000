//=============================================================================
//
// 3D矩形の衝突判定クラス(collision_rectangle3D.cpp)
// Author : 唐﨑結斗
// 概要 : 3D矩形の衝突判定生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "collision_rectangle3D.h"
#include "application.h"
#include "renderer.h"
#include "object.h"
#include "line.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : モーションキャラクター3Dを生成する
//=============================================================================
CCollision_Rectangle3D * CCollision_Rectangle3D::Create()
{
	// オブジェクトインスタンス
	CCollision_Rectangle3D *pCollisionRectangle3D = nullptr;

	// メモリの解放
	pCollisionRectangle3D = new CCollision_Rectangle3D;

	// メモリの確保ができなかった
	assert(pCollisionRectangle3D != nullptr);

	// 数値の初期化
	pCollisionRectangle3D->Init();

	// インスタンスを返す
	return pCollisionRectangle3D;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CCollision_Rectangle3D::CCollision_Rectangle3D()
{
#ifdef _DEBUG
	m_pLine = nullptr;								// ライン情報
	lineCol = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);		// ラインの色
#endif // _DEBUG

	EState m_state = STATE_NONE;			// 当たった場所
	m_bPlusMinus = true;					// 正負の判定
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CCollision_Rectangle3D::~CCollision_Rectangle3D()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CCollision_Rectangle3D::Init()
{
	// 当たり判定の取得
	SetType(TYPE_RECTANGLE3D);

	// 位置の取得
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// サイズの取得
	SetSize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

#ifdef _DEBUG
	// ライン情報
	m_pLine = new CLine*[12];

	for (int nCntLine = 0; nCntLine < 12; nCntLine++)
	{
		m_pLine[nCntLine] = CLine::Create();
	}
#endif // _DEBUG

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CCollision_Rectangle3D::Uninit()
{
#ifdef _DEBUG
	if (m_pLine != nullptr)
	{
		for (int nCntLine = 0; nCntLine < 12; nCntLine++)
		{
			m_pLine[nCntLine]->Uninit();
			m_pLine[nCntLine] = nullptr;
		}

		delete[] m_pLine;
		m_pLine = nullptr;
	}
#endif // _DEBUG

	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CCollision_Rectangle3D::Update()
{

}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CCollision_Rectangle3D::Draw()
{
	if (GetUseFlag())
	{
#ifdef _DEBUG
		// ラインの更新
		SetLine();
#endif // _DEBUG
	}
}

//=============================================================================
// 当たり判定
// Author : 唐﨑結斗
// 概要 : 当たり判定
//=============================================================================
bool CCollision_Rectangle3D::Collision(CObject::EObjectType objeType, bool bExtrude)
{
#ifdef _DEBUG
	lineCol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
#endif // _DEBUG

	// 先頭インスタンスの取得
	CCollision *pCollision = CCollision::GetTop();
	SetCollidedObj();
	bool bCollision = false;

	if (pCollision != nullptr)
	{
		while (pCollision)
		{// 現在のオブジェクトの次のオブジェクトを保管
			CCollision *pCollisionNext = pCollision->GetNext();
			CObject::EObjectType myObjeType = pCollision->GetParent()->GetObjType();

			if (myObjeType == objeType
				&& pCollision != this
				&& pCollision->GetUseFlag()
				&& !pCollision->GetDeathFlag())
			{
				switch (pCollision->GetType())
				{
				case CCollision::TYPE_RECTANGLE3D:
				case CCollision::TYPE_SPHERE:
					bCollision = ToRectangle(pCollision, bExtrude);
					break;

				default:
					assert(false);
					break;
				}
			}

			if (bCollision)
			{// 当たったオブジェクトの設定
				SetCollidedObj(pCollision->GetParent());
				break;
			}

			// 現在のオブジェクトの次のオブジェクトを更新
			pCollision = pCollisionNext;
		}
	}

	return bCollision;
}

//=============================================================================
// 当たり判定
// Author : 唐﨑結斗
// 概要 : 当たり判定
//=============================================================================
bool CCollision_Rectangle3D::Collision(CObject::EObjectType objeType)
{
#ifdef _DEBUG
	lineCol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
#endif // _DEBUG

	// 先頭インスタンスの取得
	CCollision *pCollision = CCollision::GetTop();
	SetCollidedObj();
	bool bCollision = false;

	if (pCollision != nullptr)
	{
		while (pCollision)
		{// 現在のオブジェクトの次のオブジェクトを保管
			CCollision *pCollisionNext = pCollision->GetNext();
			CObject::EObjectType myObjeType = pCollision->GetParent()->GetObjType();

			if (myObjeType == objeType
				&& pCollision != this
				&& pCollision->GetUseFlag()
				&& !pCollision->GetDeathFlag())
			{
				switch (pCollision->GetType())
				{
				case CCollision::TYPE_RECTANGLE3D:
				case CCollision::TYPE_SPHERE:
					bCollision = ToRectangle(pCollision, true);
					break;

				default:
					assert(false);
					break;
				}
			}

			if (bCollision)
			{// 当たったオブジェクトの設定
				SetCollidedObj(pCollision->GetParent());
			}

			// 現在のオブジェクトの次のオブジェクトを更新
			pCollision = pCollisionNext;
		}
	}

	return bCollision;
}

//=============================================================================
// 矩形との当たり判定
// Author : 唐﨑結斗
// 概要 : 矩形との当たり判定
//=============================================================================
bool CCollision_Rectangle3D::ToRectangle(CCollision *pTarget, bool bExtrude)
{
	// 返り値用の変数
	bool bColision = false;
	m_bPlusMinus = true;

	// 自分の情報を取得する
	D3DXVECTOR3 pos = GetParent()->GetPos() + GetPos();
	D3DXVECTOR3 posOld = GetParent()->GetPosOld() + GetPos();
	D3DXVECTOR3 size = GetSize() / 2.0f;

	// 目標の情報取得
	D3DXVECTOR3 posTarget = pTarget->GetParent()->GetPos() + pTarget->GetPos();
	D3DXVECTOR3 sizeTarget = pTarget->GetSize() / 2.0f;
	m_state = STATE_NONE;

	if ((pos.z - size.z) < (posTarget.z + sizeTarget.z)
		&& (pos.z + size.z) > (posTarget.z - sizeTarget.z)
		&& (pos.x - size.x) < (posTarget.x + sizeTarget.x)
		&& (pos.x + size.x) > (posTarget.x - sizeTarget.x)
		&& (pos.y - size.y) < (posTarget.y + sizeTarget.y)
		&& (pos.y + size.y) > (posTarget.y - sizeTarget.y))
	{
		bColision = true;
	}

	if ((pos.z - size.z) < (posTarget.z + sizeTarget.z)
		&& (pos.z + size.z) > (posTarget.z - sizeTarget.z)
		&& (pos.x - size.x) < (posTarget.x + sizeTarget.x)
		&& (pos.x + size.x) > (posTarget.x - sizeTarget.x))
	{// モデル内にいる(XZ軸)
		if ((posOld.y + size.y) <= (posTarget.y - sizeTarget.y)
			&& (pos.y + size.y) > (posTarget.y - sizeTarget.y))
		{
			m_state = STATE_Y;

			if (bExtrude)
			{
				m_bPlusMinus = false;
				pos.y = posTarget.y - sizeTarget.y - size.y;
			}
		}
		if ((posOld.y - size.y) >= (posTarget.y + sizeTarget.y)
			&& (pos.y - size.y) < (posTarget.y + sizeTarget.y))
		{
			m_state = STATE_Y;

			if (bExtrude)
			{
				pos.y = posTarget.y + sizeTarget.y + size.y;
			}
		}
	}
	if ((pos.y - size.y) < (posTarget.y + sizeTarget.y)
		&& (pos.y + size.y) > (posTarget.y - sizeTarget.y))
	{// モデル内にいる(Y軸)
		if ((pos.z - size.z) < (posTarget.z + sizeTarget.z)
			&& (pos.z + size.z) > (posTarget.z - sizeTarget.z))
		{
			if ((posOld.x + size.x) <= (posTarget.x - sizeTarget.x)
				&& (pos.x + size.x) > (posTarget.x - sizeTarget.x))
			{// モデル内にいる(Z軸)
				m_state = STATE_Z;
				if (bExtrude)
				{
					m_bPlusMinus = false;
					pos.x = posTarget.x - sizeTarget.x - size.x;
				}
			}
			if ((posOld.x - size.x) >= (posTarget.x + sizeTarget.x)
				&& (pos.x - size.x) < (posTarget.x + sizeTarget.x))
			{// モデル内にいる(Z軸)
				m_state = STATE_Z;
				if (bExtrude)
				{
					pos.x = posTarget.x + sizeTarget.x + size.x;
				}
			}
		}
		if ((pos.x - size.x) < (posTarget.x + sizeTarget.x)
			&& (pos.x + size.x) > (posTarget.x - sizeTarget.x))
		{
			if ((posOld.z + size.z) <= (posTarget.z - sizeTarget.z)
				&& (pos.z + size.z) > (posTarget.z - sizeTarget.z))
			{// モデル内にいる(X軸)
				m_state = STATE_X;
				if (bExtrude)
				{
					m_bPlusMinus = false;
					pos.z = posTarget.z - sizeTarget.z - size.z;
				}
			}
			if ((posOld.z - size.z) >= (posTarget.z + sizeTarget.z)
				&& (pos.z - size.z) < (posTarget.z + sizeTarget.z))
			{// モデル内にいる(X軸)
				m_state = STATE_Z;
				if (bExtrude)
				{
					pos.z = posTarget.z + sizeTarget.z + size.z;
				}
			}
		}
	}

	if (bColision)
	{// 位置の設定
		CObject *pParent = GetParent();
		pParent->SetPos(pos - GetPos());
#ifdef _DEBUG
		lineCol = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
#endif // DEBUG
	}
	
	return bColision;
}

//=============================================================================
// ラインの設置
// Author : 唐﨑結斗
// 概要 : ラインを矩形状に設置
//=============================================================================
void CCollision_Rectangle3D::DotCross(CCollision * pTarget)
{
	// 自分の情報を取得する
	D3DXVECTOR3 pos = GetParent()->GetPos() + GetPos();
	D3DXVECTOR3 posOld = GetParent()->GetPosOld() + GetPos();
	D3DXVECTOR3 size = GetSize() / 2.0f;

	// 目標の情報
	D3DXVECTOR3 posTarget = pTarget->GetParent()->GetPos() + GetPos();
	D3DXVECTOR3 posOldTarget = pTarget->GetParent()->GetPosOld() + GetPos();
	D3DXVECTOR3 sizeTarget = pTarget->GetSize() / 2.0f;

	// 頂点の算出
	D3DXVECTOR3 aVec[12] = { D3DXVECTOR3(0.0f,0.0f,0.0f) };
	aVec[0] = D3DXVECTOR3(posTarget.x - sizeTarget.x, posTarget.y, posTarget.z - sizeTarget.z);
	aVec[1] = D3DXVECTOR3(posTarget.x + sizeTarget.x, posTarget.y, posTarget.z - sizeTarget.z);
	aVec[2] = D3DXVECTOR3(posTarget.x - sizeTarget.x, posTarget.y, posTarget.z + sizeTarget.z);
	aVec[3] = D3DXVECTOR3(posTarget.x + sizeTarget.x, posTarget.y, posTarget.z + sizeTarget.z);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{

	}
}

#ifdef _DEBUG
//=============================================================================
// ラインの設置
// Author : 唐﨑結斗
// 概要 : ラインを矩形状に設置
//=============================================================================
void CCollision_Rectangle3D::SetLine()
{
	// 変数宣言
	CObject *pObject = GetParent();
	D3DXVECTOR3 pos = pObject->GetPos() + GetPos();
	const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 size = GetSize() / 2.0f;
	const D3DXCOLOR col = lineCol;

	// ラインの設定
	m_pLine[0]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, size.z), D3DXVECTOR3(size.x, -size.y, size.z), col);
	m_pLine[1]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, -size.z), D3DXVECTOR3(-size.x, -size.y, size.z), col);
	m_pLine[2]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, -size.z), D3DXVECTOR3(size.x, -size.y, -size.z), col);
	m_pLine[3]->SetLine(pos, rot, D3DXVECTOR3(size.x, -size.y, -size.z), D3DXVECTOR3(size.x, -size.y, size.z), col);
	m_pLine[4]->SetLine(pos, rot, D3DXVECTOR3(-size.x, size.y, size.z), D3DXVECTOR3(size.x, size.y, size.z), col);
	m_pLine[5]->SetLine(pos, rot, D3DXVECTOR3(-size.x, size.y, -size.z), D3DXVECTOR3(-size.x, size.y, size.z), col);
	m_pLine[6]->SetLine(pos, rot, D3DXVECTOR3(-size.x, size.y, -size.z), D3DXVECTOR3(size.x, size.y, -size.z), col);
	m_pLine[7]->SetLine(pos, rot, D3DXVECTOR3(size.x, size.y, -size.z), D3DXVECTOR3(size.x, size.y, size.z), col);
	m_pLine[8]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, size.z), D3DXVECTOR3(-size.x, size.y, size.z), col);
	m_pLine[9]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, -size.z), D3DXVECTOR3(-size.x, size.y, -size.z), col);
	m_pLine[10]->SetLine(pos, rot, D3DXVECTOR3(size.x, -size.y, -size.z), D3DXVECTOR3(size.x, size.y, -size.z), col);
	m_pLine[11]->SetLine(pos, rot, D3DXVECTOR3(size.x, -size.y, size.z), D3DXVECTOR3(size.x, size.y, size.z), col);
}
#endif // DEBUG

