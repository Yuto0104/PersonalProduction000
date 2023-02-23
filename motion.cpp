//**************************************************************************************************
//
// モーション処理(motion.cpp)
// Auther：唐﨑結斗
//
//**************************************************************************************************

//***************************************************************************
// インクルード
//***************************************************************************
#include <stdio.h>
#include <assert.h>

#include "application.h"
#include "motion.h"	
#include "model3D.h"
#include "parts.h"
#include "calculation.h"
#include "texture.h"

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CMotion::CMotion(const char * pFileName)
{
	// パーツ名の初期化
	memset(&m_partsFile, 0, sizeof(m_partsFile));

	// モーションの初期化
	m_motion.clear();

	// パーツの初期化
	m_pParts.clear();

	// パーツ数の初期化
	m_nMaxParts = 0;

	// モーションの読み込み
	LoodSetMotion(pFileName);

	// 扱うモーション
	m_nNumMotion = 0;

	// モーションを行うか
	m_bMotion = false;

	// モーションブレンド
	m_bMotionBlend = false;

	// モーションの停止
	m_bStop = false;								
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CMotion::~CMotion()
{
}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : パーツ情報のXファイル読み込み
//=============================================================================
void CMotion::Init(void)
{
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{// カウントのリセット
		CntReset(nCntMotion);
	}
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : モーションの更新
//=============================================================================
void CMotion::Update()
{
	if (!m_bStop)
	{
		if (m_bMotionBlend)
		{
			MotionBlend();
		}
		else if (m_bMotion
			&& !m_bMotionBlend)
		{
			PlayMotion();
		}
	}
}

//=============================================================================
// モーションの設定
// Author : 唐﨑結斗
// 概要 : モーションの初期位置に設定
//=============================================================================
void CMotion::SetMotion(const int nCntMotionSet)
{
	CMotion::MyMotion& motion = m_motion.at(nCntMotionSet);

	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{
		// 変数宣言
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 位置
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 向き
		D3DXVECTOR3 posOrigin = m_pParts.at(nCntParts)->GetPosOrigin();		// 元の位置
		D3DXVECTOR3 rotOrigin = m_pParts.at(nCntParts)->GetRotOrigin();		// 元の向き

		// 位置の設定
		pos = (posOrigin + motion.pKeySet[motion.nCntKeySet].pKey[nCntParts].pos);

		//	向きの設定
		rot = (rotOrigin + motion.pKeySet[motion.nCntKeySet].pKey[nCntParts].rot);

		// 角度の正規化
		rot.x = CCalculation::RotNormalization(rot.x);
		rot.y = CCalculation::RotNormalization(rot.y);
		rot.z = CCalculation::RotNormalization(rot.z);

		// 情報の更新
		m_pParts.at(nCntParts)->SetPos(pos);
		m_pParts.at(nCntParts)->SetRot(rot);
	}
}

//=============================================================================
// モーションの設定
// Author : 唐﨑結斗
// 概要 : モーションの初期位置に設定
//=============================================================================
void CMotion::SetMotion(const int nCntMotionSet, const int nNumKeySet)
{
	CMotion::MyMotion& motion = m_motion.at(nCntMotionSet);

	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{
		// 変数宣言
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 位置
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 向き
		D3DXVECTOR3 posOrigin = m_pParts.at(nCntParts)->GetPosOrigin();		// 元の位置
		D3DXVECTOR3 rotOrigin = m_pParts.at(nCntParts)->GetRotOrigin();		// 元の向き

		// 位置の設定
		pos = (posOrigin + motion.pKeySet[nNumKeySet].pKey[nCntParts].pos);

		//	向きの設定
		rot = (rotOrigin + motion.pKeySet[nNumKeySet].pKey[nCntParts].rot);

		// 角度の正規化
		rot.x = CCalculation::RotNormalization(rot.x);
		rot.y = CCalculation::RotNormalization(rot.y);
		rot.z = CCalculation::RotNormalization(rot.z);

		// 情報の更新
		m_pParts.at(nCntParts)->SetPos(pos);
		m_pParts.at(nCntParts)->SetRot(rot);
	}
}

//=============================================================================
// パーツの設定
// Author : 唐﨑結斗
// 概要 : 行列を利用して、パーツの親子関係と描画設定を行う
//=============================================================================
void CMotion::SetParts(D3DXMATRIX mtxWorld)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{// モデルの描画
		if (m_pParts.at(nCntParts)->GetParent() != nullptr)
		{
			m_pParts.at(nCntParts)->Draw();
		}
		else
		{
			m_pParts.at(nCntParts)->Draw(mtxWorld);
		}
	}

	// 新規深度値とZバッファの深度値が同じ値ならテスト成功にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// モーションの再生
// Author : 唐﨑結斗
// 概要 : 目的の位置まで特定のフレーム数で到着する処理をパーツごとに行う
//=============================================================================
void CMotion::PlayMotion()
{
	CMotion::MyMotion& motion = m_motion.at(m_nNumMotion);

	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{
		// 変数宣言
		D3DXVECTOR3 pos = m_pParts.at(nCntParts)->GetPos();			// 位置
		D3DXVECTOR3 rot = m_pParts.at(nCntParts)->GetRot();			// 向き
		D3DXVECTOR3 posDest = m_pParts.at(nCntParts)->GetPosDest();	// 目的の位置
		D3DXVECTOR3 rotDest = m_pParts.at(nCntParts)->GetRotDest();	// 目的の向き

		if (motion.nCntFrame == 0)
		{// フレームカウントが0の時
			// 変数宣言
			D3DXVECTOR3 posOrigin = m_pParts.at(nCntParts)->GetPosOrigin();		// 元の位置
			D3DXVECTOR3 rotOrigin = m_pParts.at(nCntParts)->GetRotOrigin();		// 元の向き

			// 目的の位置と向きの算出
			posDest = (posOrigin + motion.pKeySet[motion.nCntKeySet].pKey[nCntParts].pos) - pos;
			rotDest = (rotOrigin + motion.pKeySet[motion.nCntKeySet].pKey[nCntParts].rot) - rot;

			// 角度の正規化
			rotDest.x = CCalculation::RotNormalization(rotDest.x);
			rotDest.y = CCalculation::RotNormalization(rotDest.y);
			rotDest.z = CCalculation::RotNormalization(rotDest.z);

			// 情報の更新
			m_pParts.at(nCntParts)->SetPosDest(posDest);
			m_pParts.at(nCntParts)->SetRotDest(rotDest);
		}

		// 変数宣言
		D3DXVECTOR3 addPos = D3DXVECTOR3(posDest / (float)(motion.pKeySet[motion.nCntKeySet].nFrame));
		D3DXVECTOR3 addRot = D3DXVECTOR3(rotDest / (float)(motion.pKeySet[motion.nCntKeySet].nFrame));

		// 位置の加算
		pos += addPos;

		//	向きの加算
		rot += addRot;

		// 角度の正規化
		rot.x = CCalculation::RotNormalization(rot.x);
		rot.y = CCalculation::RotNormalization(rot.y);
		rot.z = CCalculation::RotNormalization(rot.z);

		// 情報の更新
		m_pParts.at(nCntParts)->SetPos(pos);
		m_pParts.at(nCntParts)->SetRot(rot);
	}

	// フレームカウントの加算
	motion.nCntFrame++;

	if (motion.nCntFrame >= motion.pKeySet[motion.nCntKeySet].nFrame)
	{// フレームカウントが指定のフレーム数を超えた場合
		// フレーム数の初期化
		motion.nCntFrame = 0;

		// 再生中のキー番号数の加算
		motion.nCntKeySet++;

		if (motion.nCntKeySet >= motion.nNumKey && motion.bLoop)
		{// 再生中のキー数カウントがキー数の最大値を超えたとき、そのモーションがループを使用している
			// 再生中のキー数カウントを初期化
			motion.nCntKeySet = 0;

		}
		else if (motion.nCntKeySet >= motion.nNumKey)
		{
			motion.nCntKeySet = 0;
			m_bMotion = false;
		}
	}
}

//=============================================================================
// モーションブレンド
// Author : 唐﨑結斗
// 概要 : モーションとモーションのつなぎ目を調整する
// 返り値 : モーションブレンド使用の可否
//=============================================================================
void CMotion::MotionBlend()
{
	CMotion::MyMotion& motion = m_motion.at(m_nNumMotion);

	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{
		// 変数宣言
		D3DXVECTOR3 pos = m_pParts.at(nCntParts)->GetPos();			// 位置
		D3DXVECTOR3 rot = m_pParts.at(nCntParts)->GetRot();			// 向き
		D3DXVECTOR3 posDest = m_pParts.at(nCntParts)->GetPosDest();	// 目的の位置
		D3DXVECTOR3 rotDest = m_pParts.at(nCntParts)->GetRotDest();	// 目的の向き

		if (motion.nCntFrame == 0)
		{// フレームカウントが0の時
			// 変数宣言
			D3DXVECTOR3 posOrigin = m_pParts.at(nCntParts)->GetPosOrigin();		// 元の位置
			D3DXVECTOR3 rotOrigin = m_pParts.at(nCntParts)->GetRotOrigin();		// 元の向き

			// 目的の位置と向きの算出
			CMotion::MyKey myKey = motion.pKeySet[motion.nCntKeySet].pKey[nCntParts];
			posDest = posOrigin + myKey.pos - pos;
			rotDest = rotOrigin + myKey.rot - rot;

			// 角度の正規化
			rotDest.x = CCalculation::RotNormalization(rotDest.x);
			rotDest.y = CCalculation::RotNormalization(rotDest.y);
			rotDest.z = CCalculation::RotNormalization(rotDest.z);

			// 情報の更新
			m_pParts.at(nCntParts)->SetPosDest(posDest);
			m_pParts.at(nCntParts)->SetRotDest(rotDest);
		}

		// 変数宣言
		D3DXVECTOR3 addPos = D3DXVECTOR3(posDest / (float)(MOTION_BLEND_FRAM));
		D3DXVECTOR3 addRot = D3DXVECTOR3(rotDest / (float)(MOTION_BLEND_FRAM));

		// 位置の加算
		pos += addPos;

		//	向きの加算
		rot += addRot;

		// 角度の正規化
		rot.x = CCalculation::RotNormalization(rot.x);
		rot.y = CCalculation::RotNormalization(rot.y);
		rot.z = CCalculation::RotNormalization(rot.z);

		// 情報の更新
		m_pParts.at(nCntParts)->SetPos(pos);
		m_pParts.at(nCntParts)->SetRot(rot);
	}

	// フレームカウントの加算
	motion.nCntFrame++;

	if (motion.nCntFrame >= MOTION_BLEND_FRAM)
	{// フレームカウントが指定のフレーム数を超えた場合

		motion.nCntFrame = 0;	// フレーム数の初期化
		motion.nCntKeySet++;	// 再生中のキー番号数の加算

		if (motion.nCntKeySet >= motion.nNumKey)
		{
			motion.nCntKeySet = 0;
		}

		m_bMotionBlend = false;
	}
}

//=============================================================================
// パーツとモーションの読み込み
// Author : 唐﨑結斗
// 概要 : パーツとモーションの読み込み、初期化を呼び出す
//=============================================================================
void CMotion::LoodSetMotion(const char *pFileName)
{
	// 変数宣言
	char aString[0xff] = {};		// 文字列比較用の変数
	char g_aEqual[0xff] = {};		// ＝読み込み用変数
	int	nCntKeySet = 0;				// KeySetカウント
	int	nCntKey = 0;				// Keyカウント
	int nCntFileName = 0;			// ファイル名
	int nCntParts = 0;				// パーツ数のカウント
	int nCntMotion = 0;				// モーション数のカウント

	// ファイルポインタの宣言
	FILE * pFile;

	//ファイルを開く
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", &aString);

		while (strncmp(&aString[0], "SCRIPT", 6) != 0)
		{// 文字列が一致した場合
			aString[0] = {};
			// 文字列の読み込み
			fscanf(pFile, "%s", &aString[0]);
		}
		while (strncmp(&aString[0], "END_SCRIPT", 10) != 0)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strncmp(&aString[0], "#", 1) == 0)
			{// 一列読み込む
				fgets(&aString[0], sizeof(aString), pFile);
			}

			if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
			{// ファイル名の読み込み
				fscanf(pFile, "%s", &g_aEqual[0]);
				fscanf(pFile, "%s", &m_partsFile[nCntFileName][0]);
				nCntFileName++;
			}

			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{// キャラクターの読み込み
				while (strcmp(&aString[0], "END_CHARACTERSET") != 0)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strncmp(&aString[0], "#", 1) == 0)
					{// 一列読み込む
						fgets(&aString[0], sizeof(aString), pFile);
					}

					if (strcmp(&aString[0], "NUM_PARTS") == 0)
					{// 読み込むパーツ数
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", &m_nMaxParts);

						// メモリの解放
						m_pParts.resize(m_nMaxParts);
						m_motion.resize(MAX_MOTION);

						for (int i = 0; i < m_nMaxParts; i++)
						{
							m_pParts[i] = nullptr;
						}

						for (int i = 0; i < m_nMaxParts; i++)
						{// パーツの生成
							m_pParts[i] = CParts::Create();
						}
					}

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{// パーツの読み込み
						while (strcmp(&aString[0], "END_PARTSSET") != 0)
						{
							fscanf(pFile, "%s", &aString[0]);

							if (strncmp(&aString[0], "#", 1) == 0)
							{// 一列読み込む
								fgets(&aString[0], sizeof(aString), pFile);
							}

							if (strcmp(&aString[0], "INDEX") == 0)
							{// タイプの読み込み
								int nType = -1;
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &nType);

								// マテリアル情報の取得
								CModel3D::MODEL_MATERIAL *pMaterial = CModel3D::GetMaterial();

								for (int nCntModel = 0; nCntModel < CModel3D::GetMaxModel(); nCntModel++)
								{
									if (strcmp(&m_partsFile[nType][0], &pMaterial[nCntModel].aFileName[0]) == 0)
									{// モデルのIDの設定
										m_pParts.at(nCntParts)->SetModelID(nCntModel);
										break;
									}
								}
							}
							if (strcmp(&aString[0], "PARENT") == 0)
							{// 親の読み込み
								int nIdxParent = -1;
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &nIdxParent);

								if (nIdxParent != -1)
								{// 親のモデルの設定
									m_pParts.at(nCntParts)->SetParent(m_pParts[nIdxParent]);
								}
							}
							if (strcmp(&aString[0], "POS") == 0)
							{// 位置の読み込み
								D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &pos.x);
								fscanf(pFile, "%f", &pos.y);
								fscanf(pFile, "%f", &pos.z);
								m_pParts.at(nCntParts)->SetPos(pos);
								m_pParts.at(nCntParts)->SetPosOrigin(pos);
							}
							if (strcmp(&aString[0], "ROT") == 0)
							{// 向きの読み込み
								D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%f", &rot.x);
								fscanf(pFile, "%f", &rot.y);
								fscanf(pFile, "%f", &rot.z);
								m_pParts.at(nCntParts)->SetRot(rot);
								m_pParts.at(nCntParts)->SetRotOrigin(rot);
							}
						}

						// パーツカウントのインクリメント
						nCntParts++;
					}
				}
			}
			if (strcmp(&aString[0], "MOTIONSET") == 0)
			{// モーションの読み込み
				while (strcmp(&aString[0], "END_MOTIONSET") != 0)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strncmp(&aString[0], "#", 1) == 0)
					{// 一列読み込む
						fgets(&aString[0], sizeof(aString), pFile);
					}

					if (strcmp(&aString[0], "LOOP") == 0)
					{// ループ有無の読み込み
						int nLoop = 0;
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", &nLoop);

						if (nLoop == 0)
						{
							m_motion.at(nCntMotion).bLoop = false;
						}
						else
						{
							m_motion.at(nCntMotion).bLoop = true;
						}
					}
					if (strcmp(&aString[0], "NUM_KEY") == 0)
					{// キー数の読み込み
						fscanf(pFile, "%s", &g_aEqual[0]);
						fscanf(pFile, "%d", &m_motion.at(nCntMotion).nNumKey);

						// メモリの確保
						m_motion.at(nCntMotion).pKeySet.resize(m_motion.at(nCntMotion).nNumKey);

						for (int nCntNumKeySet = 0; nCntNumKeySet < m_motion.at(nCntMotion).nNumKey; nCntNumKeySet++)
						{
							m_motion.at(nCntMotion).pKeySet[nCntNumKeySet].pKey.resize(m_nMaxParts);
						}
					}
					if (strcmp(&aString[0], "KEYSET") == 0)
					{// キーセットの読み込み
						while (strcmp(&aString[0], "END_KEYSET") != 0)
						{
							fscanf(pFile, "%s", &aString[0]);

							if (strncmp(&aString[0], "#", 1) == 0)
							{// 一列読み込む
								fgets(&aString[0], sizeof(aString), pFile);
							}

							if (strcmp(&aString[0], "FRAME") == 0)
							{// フレーム数の読み込み
								fscanf(pFile, "%s", &g_aEqual[0]);
								fscanf(pFile, "%d", &m_motion.at(nCntMotion).pKeySet.at(nCntKeySet).nFrame);
							}
							if (strcmp(&aString[0], "KEY") == 0)
							{// キーの読み込み
								while (strcmp(&aString[0], "END_KEY") != 0)
								{
									fscanf(pFile, "%s", &aString[0]);

									if (strncmp(&aString[0], "#", 1) == 0)
									{// 一列読み込む
										fgets(&aString[0], sizeof(aString), pFile);
									}

									if (strcmp(&aString[0], "POS") == 0)
									{// 位置の読み込み
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%f", &m_motion.at(nCntMotion).pKeySet.at(nCntKeySet).pKey.at(nCntKey).pos.x);
										fscanf(pFile, "%f", &m_motion.at(nCntMotion).pKeySet.at(nCntKeySet).pKey.at(nCntKey).pos.y);
										fscanf(pFile, "%f", &m_motion.at(nCntMotion).pKeySet.at(nCntKeySet).pKey.at(nCntKey).pos.z);
									}
									if (strcmp(&aString[0], "ROT") == 0)
									{// 向きの読み込み
										fscanf(pFile, "%s", &g_aEqual[0]);
										fscanf(pFile, "%f", &m_motion.at(nCntMotion).pKeySet.at(nCntKeySet).pKey.at(nCntKey).rot.x);
										fscanf(pFile, "%f", &m_motion.at(nCntMotion).pKeySet.at(nCntKeySet).pKey.at(nCntKey).rot.y);
										fscanf(pFile, "%f", &m_motion.at(nCntMotion).pKeySet.at(nCntKeySet).pKey.at(nCntKey).rot.z);
									}
								}
								// キーカウントのインクリメント
								nCntKey++;
							}
						}

						// キーカウントの初期化
						nCntKey = 0;

						// キーセットカウントのインクリメント
						nCntKeySet++;
					}
				}
				// キーセットカウントの初期化
				nCntKeySet = 0;

				// パーツ情報のインクリメント
				nCntMotion++;
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けない場合
		assert(false);
	}

	// 初期化
	Init();
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : メモリの解放
//=============================================================================
void CMotion::Uninit(void)
{
	m_motion.clear();

	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{
		if (m_pParts.at(nCntParts) != nullptr)
		{
			delete m_pParts.at(nCntParts);
			m_pParts.at(nCntParts) = nullptr;
		}
	}

	for(int i = 0; i < (int)m_pParts.size();i++)
	{
		if (m_pParts.at(i) != nullptr)
		{
			delete m_pParts.at(i);
			m_pParts.at(i) = nullptr;
		}
	}

	m_pParts.clear();
}
//=============================================================================
// パーツの位置をもとの位置に戻す
// Author : 唐﨑結斗
// 概要 : パーツの位置をもとの位置に戻す
//=============================================================================
void CMotion::SetPartsOrigin()
{
	for (int nCntParts = 0; nCntParts < m_nMaxParts; nCntParts++)
	{
		// 位置の設定
		m_pParts.at(nCntParts)->SetPos(m_pParts.at(nCntParts)->GetPosOrigin());

		//	向きの設定
		m_pParts.at(nCntParts)->SetRot(m_pParts.at(nCntParts)->GetRotOrigin());
	}
}

//=============================================================================
// カウントのリセット
// Author : 唐﨑結斗
// 概要 : カウントのリセット
//=============================================================================
void CMotion::CntReset(const int nNumMotionOld)
{
	m_motion[nNumMotionOld].nCntFrame = 0;
	m_motion[nNumMotionOld].nCntKeySet = 0;
}

//=============================================================================
// モーション番号の設定
// Author : 唐﨑結斗
// 概要 : モーションの番号の設定
//=============================================================================
void CMotion::SetNumMotion(const int nNumMotion)
{
	// モーションカウントのリセット
	CntReset(m_nNumMotion);

	// モーション番号の設定
	m_nNumMotion = nNumMotion;

	// モーションブレンドを行う
	m_bMotionBlend = true;
	m_bMotion = true;
}
