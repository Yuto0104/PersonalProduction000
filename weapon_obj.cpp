//=============================================================================
//
// モデルオブジェクトクラス(model_obj.cpp)
// Author : 唐﨑結斗
// 概要 : モデルオブジェクト生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <stdio.h>

#include "weapon_obj.h"
#include "application.h"
#include "renderer.h"
#include "model3D.h"
#include "collision_rectangle3D.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : インスタンスを生成する
//=============================================================================
CWeaponObj * CWeaponObj::Create()
{
	// オブジェクトインスタンス
	CWeaponObj *pWeaponObj = nullptr;

	// メモリの解放
	pWeaponObj = new CWeaponObj;

	// メモリの確保ができなかった
	assert(pWeaponObj != nullptr);

	// 数値の初期化
	pWeaponObj->Init();

	// インスタンスを返す
	return pWeaponObj;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CWeaponObj::CWeaponObj() : m_pParent(nullptr),		// 親オブジェクトの情報
m_weaponType(WEAPONTYPE_NONE),						// 武器の種別
m_posOffset(D3DXVECTOR3(0.0f,0.0f,0.0f)),			// 位置の差分
m_rotOffset(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),			// 向きの差分
m_colliSize(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),			// 当たり判定の大きさ
m_nAttack(0)										// 攻撃力
{

}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CWeaponObj::~CWeaponObj()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CWeaponObj::Init()
{
	// モデルオブジェクトの初期化
	CModelObj::Init();

	// オブジェクトのタイプ設定
	SetObjType(OBJETYPE_WEAPON);

	return E_NOTIMPL;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CWeaponObj::Uninit()
{
	// モデルオブジェクトの終了
	CModelObj::Uninit();

	// オブジェクトの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CWeaponObj::Update()
{
	// モデルオブジェクトの更新
	CModelObj::Update();
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CWeaponObj::Draw()
{
	// 情報の取得
	D3DXMATRIX mtxWorld = GetMtxWorld();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	CModel3D *pModel = GetModel();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);											// 行列初期化関数

	if (m_pParent != nullptr)
	{// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rotOffset.y, m_rotOffset.x, m_rotOffset.z);			// 行列回転関数
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);												// 行列掛け算関数 

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_posOffset.x, m_posOffset.y, m_posOffset.z);					// 行列移動関数
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);											// 行列掛け算関数

		// 親のワールドマットリックス
		D3DXMATRIX mtxParent = m_pParent->GetMtxWorld();

		// 行列掛け算関数
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxParent);

		// 位置の設定
		D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &mtxWorld);
		SetPos(pos);
	}
	else
	{// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);			// 行列回転関数
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);						// 行列掛け算関数 

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);					// 行列移動関数
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);					// 行列掛け算関数
	}

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// モデルの描画
	pModel->Draw(mtxWorld);
	SetMtxWorld(mtxWorld);
}
