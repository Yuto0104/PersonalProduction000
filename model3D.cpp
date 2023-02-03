//=============================================================================
//
// 3Dモデルクラス(model3D.h)
// Author : 唐﨑結斗
// 概要 : 3Dモデル生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <stdio.h>
#include <assert.h>

#include "model3D.h"
#include "renderer.h"
#include "application.h"
#include "texture.h"
#include "light.h"

//--------------------------------------------------------------------
// 静的メンバ変数定義
//--------------------------------------------------------------------
CModel3D::MODEL_MATERIAL *CModel3D::m_material = nullptr;		// マテリアル情報
int CModel3D::m_nMaxModel = 0;									// モデル数		

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 3Dモデルを生成する
//=============================================================================
CModel3D * CModel3D::Create()
{
	// オブジェクトインスタンス
	CModel3D *pModel3D = nullptr;

	// メモリの解放
	pModel3D = new CModel3D;

	// メモリの確保ができなかった
	assert(pModel3D != nullptr);

	// 数値の初期化
	pModel3D->Init();

	// インスタンスを返す
	return pModel3D;
}

//=============================================================================
// モデル情報の初期化
// Author : 唐﨑結斗
// 概要 : 読み込んだモデル情報を元に3Dモデルを生成する
//=============================================================================
void CModel3D::InitModel()
{
	// レンダラーのゲット
	CRenderer *pRenderer = CApplication::GetRenderer();

	// テクスチャポインタの取得
	CTexture *pTexture = CApplication::GetTexture();

	// テクスチャ情報の取得
	CTexture::TEXTURE *pTextureData = pTexture->GetTextureData();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ファイル読み込み
	LoadModel("data/FILE/data.txt");

	// テクスチャの使用数のゲット
	int nMaxTex = pTexture->GetMaxTexture();

	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{// Xファイルの読み込み
		D3DXLoadMeshFromX(&m_material[nCntModel].aFileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_material[nCntModel].pBuffer,
			NULL,
			&m_material[nCntModel].nNumMat,
			&m_material[nCntModel].pMesh);

		// マテリアルのテクスチャ情報のメモリ確保
		m_material[nCntModel].pNumTex = new int[m_material[nCntModel].nNumMat];
		assert(m_material[nCntModel].pNumTex != nullptr);

		// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_material[nCntModel].pBuffer->GetBufferPointer();

		// 各メッシュのマテリアル情報を取得する
		for (int nCntMat = 0; nCntMat < (int)m_material[nCntModel].nNumMat; nCntMat++)
		{
			// マテリアルのテクスチャ情報の初期化
			m_material[nCntModel].pNumTex[nCntMat] = -1;

			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				for (int nCntTexture = 0; nCntTexture < nMaxTex; nCntTexture++)
				{
					if (strcmp(pMat[nCntMat].pTextureFilename, pTextureData[nCntTexture].aFileName) == 0)
					{
						m_material[nCntModel].pNumTex[nCntMat] = nCntTexture;
						break;
					}
				}
			}
		}

		// 頂点座標の最小値・最大値の算出
		int		nNumVtx;	// 頂点数
		DWORD	sizeFVF;	// 頂点フォーマットのサイズ
		BYTE	*pVtxBuff;	// 頂点バッファへのポインタ

		// 頂点数の取得
		nNumVtx = m_material[nCntModel].pMesh->GetNumVertices();

		// 頂点フォーマットのサイズの取得
		sizeFVF = D3DXGetFVFVertexSize(m_material[nCntModel].pMesh->GetFVF());

		// 頂点バッファのロック
		m_material[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		// 最も大きな頂点
		D3DXVECTOR3 vtxMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		D3DXVECTOR3 vtxMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// 頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (vtx.x < vtxMin.x)
			{// 比較対象が現在の頂点座標(X)の最小値より小さい
				vtxMin.x = vtx.x;
			}
			if (vtx.y < vtxMin.y)
			{// 比較対象が現在の頂点座標(Y)の最小値より小さい
				vtxMin.y = vtx.y;
			}
			if (vtx.z < vtxMin.z)
			{// 比較対象が現在の頂点座標(Z)の最小値より小さい
				vtxMin.z = vtx.z;
			}

			if (vtx.x > vtxMax.x)
			{// 比較対象が現在の頂点座標(X)の最大値より大きい
				vtxMax.x = vtx.x;
			}
			if (vtx.y > vtxMax.y)
			{// 比較対象が現在の頂点座標(Y)の最大値より大きい
				vtxMax.y = vtx.y;
			}
			if (vtx.z > vtxMax.z)
			{// 比較対象が現在の頂点座標(Z)の最大値より大きい
				vtxMax.z = vtx.z;
			}

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}

		// 頂点バッファのアンロック
		m_material[nCntModel].pMesh->UnlockVertexBuffer();

		// 大きさの設定
		m_material[nCntModel].size = vtxMax - vtxMin;
	}
}

//=============================================================================
// モデル情報の終了
// Author : 唐﨑結斗
// 概要 : モデル情報の終了
//=============================================================================
void CModel3D::UninitModel()
{
	for (int nCnt = 0; nCnt < m_nMaxModel; nCnt++)
	{
		// メッシュの破棄
		if (m_material[nCnt].pMesh != nullptr)
		{
			m_material[nCnt].pMesh->Release();
			m_material[nCnt].pMesh = nullptr;
		}

		// マテリアルの破棄
		if (m_material[nCnt].pBuffer != nullptr)
		{
			m_material[nCnt].pBuffer->Release();
			m_material[nCnt].pBuffer = nullptr;
		}

		// メモリの解放
		delete[] m_material[nCnt].pNumTex;
		m_material[nCnt].pNumTex = nullptr;
	}

	// メモリの解放
	delete[] m_material;
	m_material = nullptr;
}

//=============================================================================
// Xファイルの読み込み
// Author : 唐﨑結斗
// 概要 : Xファイルの読み込みを行う
//=============================================================================
void CModel3D::LoadModel(const char *pFileName)
{
	// 変数宣言
	char aStr[128];
	int nCntModel = 0;

	// ファイルの読み込み
	FILE *pFile = fopen(pFileName, "r");

	if (pFile != nullptr)
	{
		while (fscanf(pFile, "%s", &aStr[0]) != EOF)
		{// "EOF"を読み込むまで 
			if (strncmp(&aStr[0], "#", 1) == 0)
			{// 一列読み込む
				fgets(&aStr[0], sizeof(aStr), pFile);
			}

			if (strstr(&aStr[0], "MAX_TYPE") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%d", &m_nMaxModel);
				m_material = new CModel3D::MODEL_MATERIAL[m_nMaxModel];
				assert(m_material != nullptr);
			}

			if (strstr(&aStr[0], "MODEL_FILENAME") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%s", &m_material[nCntModel].aFileName[0]);
				nCntModel++;
			}
		}
	}
	else
	{
		assert(false);
	}
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CModel3D::CModel3D()
{
	m_pParent = nullptr;										// 親モデルの情報
	m_mtxWorld = {};											// ワールドマトリックス
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 大きさ
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);				// カラー
	m_nModelID = -1;											// モデルID
	m_bColor = false;											// カラーを使用する
	m_bShadow = true;											// 影の使用状況
	m_bLighting = true;											// ライトの使用状況
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CModel3D::~CModel3D()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CModel3D::Init()
{
	// メンバ変数の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 向き
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);					// 大きさ
	m_nModelID = -1;										// モデルID

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CModel3D::Uninit()
{

}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CModel3D::Update()
{

}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 親モデルが指定されてる場合、親のワールドマトリックス元に描画する
//=============================================================================
void CModel3D::Draw()
{
	if (m_nModelID >= 0
		&& m_nModelID < m_nMaxModel)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

		// 親のワールドマトリックス
		D3DXMATRIX mtxParent = {};

		// 計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans, mtxScaling;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);											// 行列初期化関数

		// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);			// 行列回転関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);						// 行列掛け算関数 

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);				// 行列移動関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);					// 行列掛け算関数

		if (m_pParent != nullptr)
		{
			mtxParent = m_pParent->GetMtxWorld();

			// 行列掛け算関数
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
		}

		if (m_bShadow)
		{// 影の生成
			Shadow();
		}
		
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// マテリアル描画
		DrawMaterial();
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CModel3D::Draw(D3DXMATRIX mtxParent)
{
	if (m_nModelID >= 0
		&& m_nModelID < m_nMaxModel)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

		// 計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans, mtxScaling;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);											// 行列初期化関数

		// サイズの反映
		D3DXMatrixScaling(&mtxScaling, m_size.x, m_size.y, m_size.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScaling);					// 行列掛け算関数

		// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);			// 行列回転関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);						// 行列掛け算関数 

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);				// 行列移動関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);					// 行列掛け算関数

		// 行列掛け算関数
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

		if (m_bShadow)
		{// 影の生成
			Shadow();
		}

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// マテリアル描画
		DrawMaterial();
	}
}

//=============================================================================
// マテリアル描画
// Author : 唐﨑結斗
// 概要 : マテリアル描画
//=============================================================================
void CModel3D::DrawMaterial()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// テクスチャポインタの取得
	CTexture *pTexture = CApplication::GetTexture();

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	if (!m_bLighting && !m_bColor)
	{// ライトを無効
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	// 現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	if (m_material[m_nModelID].pBuffer != nullptr)
	{// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_material[m_nModelID].pBuffer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_material[m_nModelID].nNumMat; nCntMat++)
		{
			if (m_bColor)
			{// マテリアル情報の設定
				D3DMATERIAL9  matD3D = pMat[nCntMat].MatD3D;

				// 引数を色に設定
				matD3D.Diffuse = m_color;

				// マテリアルの設定
				pDevice->SetMaterial(&matD3D);
			}
			else
			{// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			}
			
			// テクスチャの設定
			pDevice->SetTexture(0, pTexture->GetTexture(m_material[m_nModelID].pNumTex[nCntMat]));

			// モデルパーツの描画
			m_material[m_nModelID].pMesh->DrawSubset(nCntMat);

			// テクスチャの設定
			pDevice->SetTexture(0, nullptr);
		}
	}

	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// ライトを有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 位置のセッター
// Author : 唐﨑結斗
// 概要 : 位置のメンバ変数に引数を代入
//=============================================================================
void CModel3D::SetPos(const D3DXVECTOR3 &pos)
{
	// 位置の設定
	m_pos = pos;
}

//=============================================================================
// 向きのセッター
// Author : 唐﨑結斗
// 概要 : 向きのメンバ変数に引数を代入
//=============================================================================
void CModel3D::SetRot(const D3DXVECTOR3 &rot)
{
	// 位置の設定
	m_rot = rot;
}

//=============================================================================
// 大きさのセッター
// Author : 唐﨑結斗
// 概要 : 大きさのメンバ変数に引数を代入
//=============================================================================
void CModel3D::SetSize(const D3DXVECTOR3 & size)
{
	// 大きさの設定
	m_size = size;
}

//=============================================================================
// カラーの設定
// Author : 唐﨑結斗
// 概要 : カラーを設定し、カラー使用を
//=============================================================================
void CModel3D::SetColor(const D3DXCOLOR color)
{
	// 色の設定
	m_color = color;
	m_bColor = true;
}

//=============================================================================
// 影の生成
// Author : 唐﨑結斗
// 概要 : モデルの影を生成する
//=============================================================================
void CModel3D::Shadow()
{
	// 変数宣言
	D3DXMATRIX		mtxShadow = {};													// シャドウマトリックス
	D3DXPLANE		planeField = {};												// 平面化用変数
	D3DXVECTOR4		vecLight = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);					// ライト方向の逆ベクトル
	D3DXVECTOR3		posShadow = D3DXVECTOR3(0.0f, 1.0f, 0.0f);;						// 影の位置
	D3DXVECTOR3		norShadow = D3DXVECTOR3(0.0f, 1.0f, 0.0f);;						// 影の法線
	D3DXVECTOR3		lightVec = CApplication::GetLight()->GetLightVec()  * -1;		// ライト方向

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	// シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);	// 行列初期化関数

	// ライト方向の逆ベクトルの設定
	vecLight = D3DXVECTOR4(lightVec.x, lightVec.y, lightVec.z, 0.0f);

	// 平面化用変数の設定
	D3DXPlaneFromPointNormal(&planeField, &posShadow, &norShadow);

	// シャドウマトリックスの設定
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	// 行列掛け算関数
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// シャドウマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// 現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	if (m_material[m_nModelID].pBuffer != nullptr)
	{// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_material[m_nModelID].pBuffer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_material[m_nModelID].nNumMat; nCntMat++)
		{// マテリアル情報の設定
			D3DMATERIAL9  matD3D = pMat[nCntMat].MatD3D;

			// 引数を色に設定
			matD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
			matD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);

			// マテリアルの設定
			pDevice->SetMaterial(&matD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, nullptr);

			// モデルパーツの描画
			m_material[m_nModelID].pMesh->DrawSubset(nCntMat);
		}
	}

	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
