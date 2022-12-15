//=============================================================================
//
// 球の衝突判定クラス(collision_sphere.cpp)
// Author : 唐﨑結斗
// 概要 : 球の衝突判定生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "collision_sphere.h"
#include "application.h"
#include "renderer.h"
#include "object.h"
#include "model3D.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : インスタンスを生成する
//=============================================================================
CCollision_Sphere * CCollision_Sphere::Create()
{
	// オブジェクトインスタンス
	CCollision_Sphere *pCollisionSphere = nullptr;

	// メモリの解放
	pCollisionSphere = new CCollision_Sphere;

	// メモリの確保ができなかった
	assert(pCollisionSphere != nullptr);

	// 数値の初期化
	pCollisionSphere->Init();

	// インスタンスを返す
	return pCollisionSphere;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CCollision_Sphere::CCollision_Sphere()
{
#ifdef _DEBUG
	m_pModel = nullptr;		// モデル情報
	m_mtxWorld = {};
#endif // _DEBUG
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CCollision_Sphere::~CCollision_Sphere()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CCollision_Sphere::Init()
{
	// 当たり判定の取得
	SetType(TYPE_SPHERE);

	// 位置の取得
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// サイズの取得
	SetSize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

#ifdef _DEBUG
	// モデルクラスのメモリ確保
	m_pModel = CModel3D::Create();
	assert(m_pModel != nullptr);
	m_pModel->Init();
	m_pModel->SetModelID(0);
	m_pModel->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
	m_pModel->SetShadow(false);
#endif // _DEBUG

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CCollision_Sphere::Uninit()
{
#ifdef _DEBUG
	if (m_pModel != nullptr)
	{// 終了処理
		m_pModel->Uninit();

		// メモリの解放
		delete m_pModel;
		m_pModel = nullptr;
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
void CCollision_Sphere::Update()
{
#ifdef _DEBUG
	// モデルの更新
	m_pModel->Update();
#endif // _DEBUG
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CCollision_Sphere::Draw()
{
#ifdef _DEBUG
	// 位置の取得
	D3DXVECTOR3 pos = GetParent()->GetPos() + GetPos();

	// モデルのサイズの設定
	m_pModel->SetSize(GetSize());

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);									// 行列初期化関数

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);				// 行列移動関数
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);			// 行列掛け算関数

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	m_pModel->Draw(m_mtxWorld);
#endif // _DEBUG
}

//=============================================================================
// 当たり判定
// Author : 唐﨑結斗
// 概要 : 当たり判定
//=============================================================================
bool CCollision_Sphere::Collision(CObject::EObjectType objeType, bool bExtrude)
{
	// 先頭インスタンスの取得
	CCollision *pCollision = CCollision::GetTop();
	bool bCollision = false;

	if (pCollision != nullptr)
	{
		while (pCollision)
		{// 現在のオブジェクトの次のオブジェクトを保管
			CCollision *pCollisionNext = pCollision->GetNext();
			CObject::EObjectType myObjeType = pCollision->GetParent()->GetObjType();

			if (myObjeType == objeType
				&& pCollision != this
				&& !pCollision->GetDeathFlag())
			{
				switch (pCollision->GetType())
				{
				case CCollision::TYPE_RECTANGLE3D:
				case CCollision::TYPE_SPHERE:
					bCollision = ToSphere(pCollision, bExtrude);
					break;

				default:
					assert(false);
					break;
				}
			}

			// 現在のオブジェクトの次のオブジェクトを更新
			pCollision = pCollisionNext;
		}
	}

	return bCollision;
}

//=============================================================================
// 球との当たり判定
// Author : 唐﨑結斗
// 概要 : 球との当たり判定
//=============================================================================
bool CCollision_Sphere::ToSphere(CCollision * pTarget, bool bExtrude)
{
	// 変数宣言
	bool bCollision = false;

	// 自分の情報を取得する
	D3DXVECTOR3 pos = GetParent()->GetPos() + GetPos();
	D3DXVECTOR3 size = GetSize() / 2.0f;

	// 目標の情報取得
	D3DXVECTOR3 posTarget = pTarget->GetParent()->GetPos() + pTarget->GetPos();
	D3DXVECTOR3 sizeTarget = pTarget->GetSize() / 2.0f;

	// 判定を行う距離を算出
	float fJudgeDistance = sqrtf((size.x * size.x) + (size.y * size.y) + (size.z * size.z));
	fJudgeDistance += sqrtf((sizeTarget.x * sizeTarget.x) + (sizeTarget.y * sizeTarget.y) + (sizeTarget.z * sizeTarget.z));

	// お互いの位置の差を算出
	D3DXVECTOR3 distance = posTarget - pos;
	float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z));

	if (fDistance <= fJudgeDistance)
	{// 位置の差が判定を行う距離より短い場合
		bCollision = true;

		if (bExtrude)
		{
			// 角度の算出
			D3DXVECTOR3 rotDiff;
			rotDiff.y = atan2f(distance.x, distance.z);
			rotDiff.x = atan2f(sqrtf((distance.x * distance.x) + (distance.z * distance.z)), distance.y);
			rotDiff.z = 0.0f;

			// 位置の算出
			pos.z = posTarget.z - sinf(rotDiff.x) * cosf(rotDiff.y) * fJudgeDistance;
			pos.x = posTarget.x - sinf(rotDiff.x) * sinf(rotDiff.y) * fJudgeDistance;
			pos.y = posTarget.y - cosf(rotDiff.x) * fJudgeDistance;
		}
	}

	if (bCollision)
	{// 位置の設定
		CObject *pParent = GetParent();
		pParent->SetPos(pos - GetPos());
	}

	return bCollision;
}

