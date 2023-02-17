//=============================================================================
//
// カメラマネージャークラス(camera_action.cpp)
// Author : 唐﨑結斗
// 概要 : カメラマネージャーの派生を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <stdio.h>
#include "camera_action.h"
#include "calculation.h"
#include "object.h"
#include "model_obj.h"
#include "camera.h"

//--------------------------------------------------------------------
// 定数定義
//--------------------------------------------------------------------
const unsigned int CCameraAction::nACTION_BLEND_FRAM = 5;

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : モーションキャラクター3Dを生成する
//=============================================================================
CCameraAction *CCameraAction::Create()
{// オブジェクトインスタンス
	CCameraAction *pCameraAction = nullptr;

	// メモリの解放
	pCameraAction = new CCameraAction;

	// メモリの確保ができなかった
	assert(pCameraAction != nullptr);

	// 数値の初期化
	pCameraAction->Init();

	// インスタンスを返す
	return pCameraAction;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CCameraAction::CCameraAction() : m_pPivot(nullptr),
m_pCamera(nullptr),
m_posDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_posVDest(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_posRDest(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_rotDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_nMaxAction(0),
m_nNumAction(0),
m_bAction(false),
m_bActionBlend(false)
{
	// アクションの初期化
	m_action.clear();
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CCameraAction::~CCameraAction()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CCameraAction::Init()
{
	m_pPivot = CModelObj::Create();

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CCameraAction::Uninit()
{
	m_action.clear();

	if (m_pPivot != nullptr)
	{// 終了処理
		m_pPivot->Uninit();
		m_pPivot = nullptr;
	}

	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CCameraAction::Update()
{
	if (m_bActionBlend)
	{
		ActionBlend();
	}
	else if (m_bAction
		&& !m_bActionBlend)
	{
		PlayAction();
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CCameraAction::Draw()
{

}

//=============================================================================
// アクションの読み込み
// Author : 唐﨑結斗
// 概要 : アクションの読み込み
//=============================================================================
void CCameraAction::LoadAction(char * pFileName)
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

			if (strstr(&aStr[0], "ACTIONSET") != NULL)
			{
				m_nMaxAction++;

				// 変数宣言
				int nCntKeySet = 0;
				int nNumKey = 0;			// キー数
				bool bLoop = false;			// ループ使用状況
				bool bParent = false;		// 親がいるか否か

				while (strstr(&aStr[0], "END_FIELDSET") == NULL)
				{
					fscanf(pFile, "%s", &aStr[0]);

					if (strncmp(&aStr[0], "#", 1) == 0)
					{// 一列読み込む
						fgets(&aStr[0], sizeof(aStr), pFile);
					}

					if (strcmp(&aStr[0], "NUMKEY") == 0)
					{// モデルのファイル名の設定
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &nNumKey);

						// メモリの確保
						m_action.at(m_nMaxAction - 1).nNumKey = nNumKey;
						m_action.at(m_nMaxAction - 1).pKeySet.resize(m_action.at(m_nMaxAction - 1).nNumKey);
					}

					if (strcmp(&aStr[0], "LOOP") == 0)
					{// モデルのファイル名の設定
						int nLoop = 0;
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &nLoop);

						if (nLoop == 0)
						{
							bLoop = false;
						}
						else
						{
							bLoop = true;
						}
					}

					if (strcmp(&aStr[0], "PARENT") == 0)
					{// モデルのファイル名の設定
						int nParent = 0;
						fscanf(pFile, "%s", &aStr[0]);
						fscanf(pFile, "%d", &nParent);

						if (nParent == 0)
						{
							bParent = false;
						}
						else
						{
							bParent = true;
						}
					}

					if (strcmp(&aStr[0], "KEYSET") == 0)
					{
						// 情報格納用変数
						D3DXVECTOR3	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 現在位置
						D3DXVECTOR3	posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 視点
						D3DXVECTOR3	posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 注視点
						D3DXVECTOR3	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 現在の向き
						int nFrame = 0;										// フレーム数

						while (strstr(&aStr[0], "END_KEYSET") == NULL)
						{
							fscanf(pFile, "%s", &aStr[0]);

							if (strncmp(&aStr[0], "#", 1) == 0)
							{// 一列読み込む
								fgets(&aStr[0], sizeof(aStr), pFile);
							}

							if (strcmp(&aStr[0], "POS") == 0)
							{// 位置の読み込み
								fscanf(pFile, "%s", &aStr[0]);
								fscanf(pFile, "%f", &pos.x);
								fscanf(pFile, "%f", &pos.y);
								fscanf(pFile, "%f", &pos.z);
							}
							if (strcmp(&aStr[0], "ROT") == 0)
							{// 向きの読み込み
								fscanf(pFile, "%s", &aStr[0]);
								fscanf(pFile, "%f", &rot.x);
								fscanf(pFile, "%f", &rot.y);
								fscanf(pFile, "%f", &rot.z);
							}
							if (strcmp(&aStr[0], "POSV") == 0)
							{// 向きの読み込み
								fscanf(pFile, "%s", &aStr[0]);
								fscanf(pFile, "%f", &rot.x);
								fscanf(pFile, "%f", &rot.y);
								fscanf(pFile, "%f", &rot.z);
							}
							if (strcmp(&aStr[0], "POSR") == 0)
							{// 向きの読み込み
								fscanf(pFile, "%s", &aStr[0]);
								fscanf(pFile, "%f", &rot.x);
								fscanf(pFile, "%f", &rot.y);
								fscanf(pFile, "%f", &rot.z);
							}
							if (strcmp(&aStr[0], "FRAME") == 0)
							{// 向きの読み込み
								fscanf(pFile, "%s", &aStr[0]);
								fscanf(pFile, "%d", &nFrame);
							}
						}

						// 数値の設定
						m_action.at(m_nMaxAction - 1).pKeySet.at(nCntKeySet).nFrame = nFrame;
						m_action.at(m_nMaxAction - 1).pKeySet.at(nCntKeySet).pos = pos;
						m_action.at(m_nMaxAction - 1).pKeySet.at(nCntKeySet).rot = rot;
						m_action.at(m_nMaxAction - 1).pKeySet.at(nCntKeySet).posV = posV;
						m_action.at(m_nMaxAction - 1).pKeySet.at(nCntKeySet).posR = posR;
					}
				}

				// 数値の設定
				m_action.at(m_nMaxAction - 1).bLoop = bLoop;
				m_action.at(m_nMaxAction - 1).bParent = bParent;
			}
		}
	}
	else
	{
		assert(false);
	}
}

//=============================================================================
// アクションの設定
// Author : 唐﨑結斗
// 概要 : アクション設定をする
//=============================================================================
void CCameraAction::SetAction(const int nCntMotionSet)
{
	if (m_pCamera == nullptr)
	{
		return;
	}

	CCameraAction::Action& action = m_action.at(m_nNumAction);
	CObject *pParent = m_pCamera->GetTarget();

	// 変数宣言
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	D3DXVECTOR3 posV = m_pCamera->GetPosV();					// 視点
	D3DXVECTOR3 posR = m_pCamera->GetPosR();					// 注視点
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き

	if (pParent != nullptr)
	{
		pos = pParent->GetPos();
	}
	else
	{
		pos = m_pPivot->GetPos();
	}

	// 位置の設定
	pos = action.pKeySet[action.nCntKeySet].pos;

	// 視点の設定
	posV = action.pKeySet[action.nCntKeySet].posV;

	// 注視点の設定
	posR = action.pKeySet[action.nCntKeySet].posR;

	//	向きの設定
	rot = action.pKeySet[action.nCntKeySet].rot;

	// 角度の正規化
	rot.x = CCalculation::RotNormalization(rot.x);
	rot.y = CCalculation::RotNormalization(rot.y);
	rot.z = CCalculation::RotNormalization(rot.z);

	if (pParent != nullptr)
	{
		pParent->SetPos(pos);
	}

	m_pCamera->SetPosVOffset(posV);
	m_pCamera->SetPosROffset(posR);
	m_pCamera->SetRot(rot);
}

//=============================================================================
// アクションの設定
// Author : 唐﨑結斗
// 概要 : アクション設定をする
//=============================================================================
void CCameraAction::SetAction(const int nCntMotionSet, const int nNumKeySet)
{
	if (m_pCamera == nullptr)
	{
		return;
	}

	CCameraAction::Action& action = m_action.at(m_nNumAction);
	CObject *pParent = m_pCamera->GetTarget();

	// 変数宣言
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	D3DXVECTOR3 posV = m_pCamera->GetPosV();					// 視点
	D3DXVECTOR3 posR = m_pCamera->GetPosR();					// 注視点
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き

	if (pParent != nullptr)
	{
		pos = pParent->GetPos();
	}
	else
	{
		pos = m_pPivot->GetPos();
	}

	// 位置の設定
	pos = action.pKeySet[nNumKeySet].pos;

	// 視点の設定
	posV = action.pKeySet[nNumKeySet].posV;

	// 注視点の設定
	posR = action.pKeySet[nNumKeySet].posR;

	//	向きの設定
	rot = action.pKeySet[nNumKeySet].rot;

	// 角度の正規化
	rot.x = CCalculation::RotNormalization(rot.x);
	rot.y = CCalculation::RotNormalization(rot.y);
	rot.z = CCalculation::RotNormalization(rot.z);

	if (pParent != nullptr)
	{
		pParent->SetPos(pos);
	}

	m_pCamera->SetPosVOffset(posV);
	m_pCamera->SetPosROffset(posR);
	m_pCamera->SetRot(rot);
}

//=============================================================================
// アクションの再生
// Author : 唐﨑結斗
// 概要 : アクションを再生する
//=============================================================================
void CCameraAction::PlayAction()
{
	if (m_pCamera == nullptr)
	{
		return;
	}

	CCameraAction::Action& action = m_action.at(m_nNumAction);
	CObject *pParent = m_pCamera->GetTarget();

	// 変数宣言
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	D3DXVECTOR3 posV = m_pCamera->GetPosV();					// 視点
	D3DXVECTOR3 posR = m_pCamera->GetPosR();					// 注視点
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き

	if (pParent != nullptr)
	{
		pos = pParent->GetPos();
	}
	else
	{
		pos = m_pPivot->GetPos();
	}

	if (action.nCntFrame == 0)
	{// フレームカウントが0の時
		// 目的の位置と向きの算出
		m_posDest = action.pKeySet[action.nCntKeySet].pos - pos;
		m_posVDest = action.pKeySet[action.nCntKeySet].posV - posV;
		m_posRDest = action.pKeySet[action.nCntKeySet].posR - posR;
		m_rotDest = action.pKeySet[action.nCntKeySet].rot - rot;

		// 角度の正規化
		m_rotDest.x = CCalculation::RotNormalization(m_rotDest.x);
		m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);
		m_rotDest.z = CCalculation::RotNormalization(m_rotDest.z);
	}

	// 変数宣言
	D3DXVECTOR3 addPos = D3DXVECTOR3(m_posDest / (float)(action.pKeySet[action.nCntKeySet].nFrame));
	D3DXVECTOR3 addPosV = D3DXVECTOR3(m_posVDest / (float)(action.pKeySet[action.nCntKeySet].nFrame));
	D3DXVECTOR3 addPosR = D3DXVECTOR3(m_posRDest / (float)(action.pKeySet[action.nCntKeySet].nFrame));
	D3DXVECTOR3 addRot = D3DXVECTOR3(m_rotDest / (float)(action.pKeySet[action.nCntKeySet].nFrame));

	if (pParent != nullptr)
	{
		// 位置の加算
		pos += addPos;
	}

	// 視点の加算
	posV += addPosV;

	// 注視点の加算
	posR += addPosR;

	//	向きの加算
	rot += addRot;

	// 角度の正規化
	rot.x = CCalculation::RotNormalization(rot.x);
	rot.y = CCalculation::RotNormalization(rot.y);
	rot.z = CCalculation::RotNormalization(rot.z);

	if (pParent != nullptr)
	{
		pParent->SetPos(pos);
	}

	m_pCamera->SetPosVOffset(posV);
	m_pCamera->SetPosROffset(posR);
	m_pCamera->SetRot(rot);

	// フレームカウントの加算
	action.nCntFrame++;

	if (action.nCntFrame >= action.pKeySet[action.nCntKeySet].nFrame)
	{// フレームカウントが指定のフレーム数を超えた場合
		// フレーム数の初期化
		action.nCntFrame = 0;

		// 再生中のキー番号数の加算
		action.nCntKeySet++;

		if (action.nCntKeySet >= action.nNumKey && action.bLoop)
		{// 再生中のキー数カウントがキー数の最大値を超えたとき、そのモーションがループを使用している
			// 再生中のキー数カウントを初期化
			action.nCntKeySet = 0;

		}
		else if (action.nCntKeySet >= action.nNumKey)
		{
			action.nCntKeySet = 0;
			m_bAction = false;
		}
	}
}

//=============================================================================
// アクションブレンド
// Author : 唐﨑結斗
// 概要 : アクションブレンドを再生する
//=============================================================================
void CCameraAction::ActionBlend()
{
	if (m_pCamera == nullptr)
	{
		return;
	}

	CCameraAction::Action& action = m_action.at(m_nNumAction);
	CObject *pParent = m_pCamera->GetTarget();

	// 変数宣言
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	D3DXVECTOR3 posV = m_pCamera->GetPosV();					// 視点
	D3DXVECTOR3 posR = m_pCamera->GetPosR();					// 注視点
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き

	if (action.nCntFrame == 0)
	{// フレームカウントが0の時
		// 目的の位置と向きの算出
		m_posDest = action.pKeySet[action.nCntKeySet].pos - pos;
		m_posVDest = action.pKeySet[action.nCntKeySet].posV - posV;
		m_posRDest = action.pKeySet[action.nCntKeySet].posR - posR;
		m_rotDest = action.pKeySet[action.nCntKeySet].rot - rot;

		// 角度の正規化
		m_rotDest.x = CCalculation::RotNormalization(m_rotDest.x);
		m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);
		m_rotDest.z = CCalculation::RotNormalization(m_rotDest.z);
	}

	// 変数宣言
	D3DXVECTOR3 addPos = D3DXVECTOR3(m_posDest / (float)(nACTION_BLEND_FRAM));
	D3DXVECTOR3 addPosV = D3DXVECTOR3(m_posVDest / (float)(nACTION_BLEND_FRAM));
	D3DXVECTOR3 addPosR = D3DXVECTOR3(m_posRDest / (float)(nACTION_BLEND_FRAM));
	D3DXVECTOR3 addRot = D3DXVECTOR3(m_rotDest / (float)(nACTION_BLEND_FRAM));

	if (pParent != nullptr)
	{
		// 位置の加算
		pos += addPos;
	}

	// 視点の加算
	posV += addPosV;

	// 注視点の加算
	posR += addPosR;

	//	向きの加算
	rot += addRot;

	// 角度の正規化
	rot.x = CCalculation::RotNormalization(rot.x);
	rot.y = CCalculation::RotNormalization(rot.y);
	rot.z = CCalculation::RotNormalization(rot.z);

	if (pParent != nullptr)
	{
		pParent->SetPos(pos);
	}

	m_pCamera->SetPosVOffset(posV);
	m_pCamera->SetPosROffset(posR);
	m_pCamera->SetRot(rot);

	// フレームカウントの加算
	action.nCntFrame++;

	if (action.nCntFrame >= nACTION_BLEND_FRAM)
	{// フレームカウントが指定のフレーム数を超えた場合
		action.nCntFrame = 0;	// フレーム数の初期化
		action.nCntKeySet++;	// 再生中のキー番号数の加算

		m_bActionBlend = false;
	}
}
