//=============================================================================
//
// マップマネージャークラス(map_manager.cpp)
// Author : 唐﨑結斗
// 概要 : マップマネージャーの派生を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <stdio.h>
#include "map_manager.h"
#include "model_obj.h"
#include "mesh.h"
#include "sphere.h"
#include "keyboard.h"
#include "player.h"
#include "game.h"
#include "application.h"
#include "collision_rectangle3D.h"
#include "debug_proc.h"
#include "model3D.h"
#include "mesh.h"
#include "calculation.h"
#include "texture.h"
#include "super.h"
#include "score_item.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : モーションキャラクター3Dを生成する
//=============================================================================
CMapManager *CMapManager::Create()
{// オブジェクトインスタンス
	CMapManager *pMapManager = nullptr;

	// メモリの解放
	pMapManager = new CMapManager;

	// メモリの確保ができなかった
	assert(pMapManager != nullptr);

	// 数値の初期化
	pMapManager->Init();

	// インスタンスを返す
	return pMapManager;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CMapManager::CMapManager()
{

}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CMapManager::~CMapManager()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CMapManager::Init()
{
	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CMapManager::Uninit()
{
	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CMapManager::Update()
{
	
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CMapManager::Draw()
{

}

//=============================================================================
// マップの読み込み
// Author : 唐﨑結斗
// 概要 : マップの読み込み
//=============================================================================
void CMapManager::SetMap(char * pFileName)
{
	// メッシュフィールドの読み込み
	LoadFieldMesh(pFileName);

	// スカイボックスの読み込み
	LoadSkayBox(pFileName);

	// モデルの読み込み
	CModelObj::LoadFile(pFileName);

	// アイテムの読み込み
	LoadScoreItem(pFileName);
}

//=============================================================================
// フィールドの読み込み
// Author : 唐﨑結斗
// 概要 : フィールドの読み込み
//=============================================================================
void CMapManager::LoadFieldMesh(char * pFileName)
{
	// 変数宣言
	char aStr[128];

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

			if (strstr(&aStr[0], "FIELDSET") != NULL)
			{
				// メッシュの設置
				CMesh3D *pMesh = CMesh3D::Create();
				assert(pMesh != nullptr);
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3 size = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
				CMesh3D::DOUBLE_INT block = CMesh3D::DOUBLE_INT(1, 1);
				D3DXVECTOR2 move = D3DXVECTOR2(0.0f, 0.0f);
				int nType = -1;
				bool bSplitTex = true;
				bool bScrollTex = false;
				bool bCollison = false;

				while (strstr(&aStr[0], "END_FIELDSET") == NULL)
				{
					fscanf(pFile, "%s", &aStr[0]);

					if (strncmp(&aStr[0], "#", 1) == 0)
					{// 一列読み込む
						fgets(&aStr[0], sizeof(aStr), pFile);
					}
					
					if (strcmp(&aStr[0], "POS") == 0)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}

					if (strcmp(&aStr[0], "ROT") == 0)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &rot.x);
						fscanf(pFile, "%f", &rot.y);
						fscanf(pFile, "%f", &rot.z);
					}

					if (strcmp(&aStr[0], "SIZE") == 0)
					{// キー数の読み込み
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &size.x);
						fscanf(pFile, "%f", &size.y);
						fscanf(pFile, "%f", &size.z);
					}

					if (strstr(&aStr[0], "TEXTYPE") != NULL)
					{// 読み込むテクスチャ
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &nType);
					}

					if (strcmp(&aStr[0], "BLOCK") == 0)
					{// キー数の読み込み
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &block.x);
						fscanf(pFile, "%d", &block.y);
					}

					if (strcmp(&aStr[0], "SPLITTEX") == 0)
					{// キー数の読み込み
						int nSplitTex = 0;
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &nSplitTex);

						if (nSplitTex == 1)
						{
							bSplitTex = true;
						}
						else
						{
							bSplitTex = false;
						}
					}

					if (strcmp(&aStr[0], "COLLISON") == 0)
					{// キー数の読み込み
						int nCollison = 0;
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &nCollison);

						if (nCollison == 1)
						{
							bCollison = true;
						}
						else
						{
							bCollison = false;
						}
					}

					if (strcmp(&aStr[0], "MOVE") == 0)
					{// 移動情報
						bScrollTex = true;
						fscanf(pFile, "%f", &move.x);
						fscanf(pFile, "%f", &move.y);
					}
				}

				// 数値の設定
				pMesh->SetPos(pos);
				pMesh->SetRot(rot);
				pMesh->SetSize(size);
				pMesh->LoadTex(nType);
				pMesh->SetBlock(block);
				pMesh->SetSplitTex(bSplitTex);
				pMesh->SetScrollTex(move, bScrollTex);
				pMesh->SetUseCollison(bCollison);
			}
		}
	}
	else
	{
		assert(false);
	}
}

//=============================================================================
// スカイボックスの読み込み
// Author : 唐﨑結斗
// 概要 : スカイボックスの読み込み
//=============================================================================
void CMapManager::LoadSkayBox(char * pFileName)
{
	// 変数宣言
	char aStr[128];

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

			if (strstr(&aStr[0], "SKYSET") != NULL)
			{
				// スカイボックスの設定
				CSphere *pSphere = CSphere::Create();
				assert(pSphere != nullptr);
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3 rot = D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f);
				D3DXVECTOR3 size = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
				CMesh3D::DOUBLE_INT block = CMesh3D::DOUBLE_INT(1, 1);
				D3DXVECTOR2 move = D3DXVECTOR2(0.0f, 0.0f);
				D3DXVECTOR2 sphereRange = D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f);
				int nType = -1;
				float fRadius = 100.0f;
				bool bSplitTex = false;
				bool bScrollTex = false;

				while (strstr(&aStr[0], "END_SKYSET") == NULL)
				{
					fscanf(pFile, "%s", &aStr[0]);

					if (strncmp(&aStr[0], "#", 1) == 0)
					{// 一列読み込む
						fgets(&aStr[0], sizeof(aStr), pFile);
					}

					if (strstr(&aStr[0], "POS") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}

					if (strstr(&aStr[0], "ROT") != NULL)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &rot.x);
						fscanf(pFile, "%f", &rot.y);
						fscanf(pFile, "%f", &rot.z);
					}

					if (strcmp(&aStr[0], "SIZE") == 0)
					{// キー数の読み込み
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &size.x);
						fscanf(pFile, "%f", &size.z);
						fscanf(pFile, "%f", &size.y);
					}

					if (strstr(&aStr[0], "TEXTYPE") != NULL)
					{// 読み込むテクスチャ
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &nType);
					}

					if (strcmp(&aStr[0], "BLOCK") == 0)
					{// キー数の読み込み
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &block.x);
						fscanf(pFile, "%d", &block.y);
					}

					if (strcmp(&aStr[0], "SPLITTEX") == 0)
					{// キー数の読み込み
						int nSplitTex = 0;
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &nSplitTex);

						if (nSplitTex == 0)
						{
							bSplitTex = true;
						}
						else
						{
							bSplitTex = false;
						}
					}

					if (strcmp(&aStr[0], "MOVE") == 0)
					{// 移動情報
						bScrollTex = true;
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &move.x);
						fscanf(pFile, "%f", &move.y);
					}

					if (strcmp(&aStr[0], "RADIUS") == 0)
					{// 移動情報
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &fRadius);
					}

					if (strcmp(&aStr[0], "SPHERERANGE") == 0)
					{// 移動情報
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &sphereRange.x);
						fscanf(pFile, "%f", &sphereRange.y);
					}
				}

				// スカイボックスの設定
				pSphere->SetPos(pos);
				pSphere->SetRot(rot);
				pSphere->SetSize(size);
				pSphere->LoadTex(nType);
				pSphere->SetBlock(block);
				pSphere->SetSplitTex(bSplitTex);
				pSphere->SetScrollTex(move, bScrollTex);
				pSphere->SetRadius(fRadius);
				pSphere->SetSphereRange(sphereRange);
			}
		}
	}
	else
	{
		assert(false);
	}
}

//=============================================================================
// スコアアイテムの読み込み
// Author : 唐﨑結斗
// 概要 : スコアアイテムの読み込み
//=============================================================================
void CMapManager::LoadScoreItem(char * pFileName)
{
	// 変数宣言
	char aStr[128];
	int nCntSetModel = 0;

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

			if (strstr(&aStr[0], "SCOREITEMSET") != NULL)
			{
				// モデルの設置
				CScoreItem *pScoreItem = CScoreItem::Create();
				assert(pScoreItem != nullptr);
				CCollision_Rectangle3D *pCollision = pScoreItem->GetCollision();
				CModel3D *pModel = pScoreItem->GetModel();

				while (strstr(&aStr[0], "END_SCOREITEMSET") == NULL)
				{
					fscanf(pFile, "%s", &aStr[0]);

					if (strncmp(&aStr[0], "#", 1) == 0)
					{// 一列読み込む
						fgets(&aStr[0], sizeof(aStr), pFile);
					}
					else if (strcmp(&aStr[0], "POS") == 0)
					{// モデルのファイル名の設定
						D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
						pScoreItem->SetPos(pos);
					}

					if (strstr(&aStr[0], "ROT") != NULL)
					{// モデルのファイル名の設定
						D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &rot.x);
						fscanf(pFile, "%f", &rot.y);
						fscanf(pFile, "%f", &rot.z);
						pScoreItem->SetRot(rot);
					}

					if (strcmp(&aStr[0], "TYPE") == 0)
					{// キー数の読み込み
						int nID = 0;
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &nID);
						pScoreItem->SetType(nID);
					}

					if (strcmp(&aStr[0], "SHADOW") == 0)
					{// キー数の読み込み
						int nUse = 0;
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &nUse);
						pModel->SetShadow(nUse);
					}

					if (strcmp(&aStr[0], "SCORE") == 0)
					{// キー数の読み込み
						int nScore = 0;
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &nScore);
						pScoreItem->SetScore(nScore);
					}

					if (strcmp(&aStr[0], "COLLISION_POS") == 0)
					{// キー数の読み込み
						D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
						pCollision->SetPos(pos);
					}

					if (strcmp(&aStr[0], "COLLISION_SIZE") == 0)
					{// キー数の読み込み
						D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%f", &size.x);
						fscanf(pFile, "%f", &size.y);
						fscanf(pFile, "%f", &size.z);
						pCollision->SetSize(size);
					}
				}

				nCntSetModel++;
			}
		}
	}
	else
	{
		assert(false);
	}
}
