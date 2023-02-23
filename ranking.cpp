//=============================================================================
//
// ランキングクラス(score.cpp)
// Author : 唐﨑結斗
// 概要 : ランキング生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <stdio.h>
#include <thread>

#include "application.h"
#include "tcp_client.h"
#include "ranking.h"
#include "score.h"
#include "renderer.h"
#include "object2D.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : ランキングを生成する
//=============================================================================
CRanking *CRanking::Create(int nMaxRanking, int nNewScore)
{
	// オブジェクトインスタンス
	CRanking *pRanking = nullptr;

	pRanking = new CRanking;

	// メモリの確保ができなかった
	assert(pRanking != nullptr);

	// 数値の初期化
	pRanking->Init(nMaxRanking, nNewScore);

	// インスタンスを返す
	return pRanking;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CRanking::CRanking(int nPriority /*= CObject::PRIORITY_LEVEL3*/) : CObject(nPriority)
{
	m_pScore = nullptr;											// スコア
	pFailObj = nullptr;											// 失敗時のアナウンス用のオブジェクト
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 過去の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 大きさ
	m_wholeSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 全体の大きさ
	m_nMaxRanking = 0;											// ランキング数
	m_nDigit = 0;												// 桁数
	m_aRankingData = nullptr;									// ランキングデータ
	m_nPortNum = 0;												// ポート番号
	m_nNewScore = 0;											// 新しいスコア
	m_UpdateRank = -1;											// 更新されたランク
	m_fAddAlpha = 0.0f;											// アルファの加算値
	memset(&m_nIpAddress[0],0,sizeof(m_nIpAddress));			// IPアドレス
	m_bUpdatingRanking = true;									// 通信してるか否か
	m_bRankingUpdatedSuccess = true;							// 成功してるか否か
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CRanking::Init()
{
	return S_OK;
}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CRanking::Init(int nMaxRanking, int nNewScore)
{
	// ファイルの読み込み
	LoadFile();

	// ランキング数の設定
	m_nMaxRanking = nMaxRanking;

	// スコアの更新
	m_nNewScore = nNewScore;

	// ランキングデータの確保
	m_aRankingData = new int[m_nMaxRanking];
	assert(m_aRankingData != nullptr);

	// ランキングの読み込み
	LoadRanking();

	// 桁数の設定
	m_nDigit = 10;

	// 位置の設定
	m_pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);

	// 向きの設定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 大きさの設定
	m_size = D3DXVECTOR3(30.0f, 50.0f, 0.0f);

	// 全体の大きさの設定
	m_wholeSize = D3DXVECTOR3(300.0f, 300.0f, 0.0f);

	if (m_bRankingUpdatedSuccess)
	{// スコアの生成
		CreateScore();

		// スコアの設定
		SetScore();

		// ランキングスコアの設定
		SetScoreNomber();
	}
	else if (!m_bRankingUpdatedSuccess)
	{
		pFailObj = CObject2D::Create();
		pFailObj->SetPos(m_pos);
		pFailObj->SetRot(m_rot);
		pFailObj->SetSize(D3DXVECTOR3(500.0f, 70.0f, 0.0f));
		pFailObj->LoadTex(17);
		pFailObj->SetCol(D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f));
	}

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CRanking::Uninit()
{
	if (m_pScore != nullptr)
	{// 終了処理
		for (int nCntRanking = 0; nCntRanking < m_nMaxRanking; nCntRanking++)
		{
			m_pScore[nCntRanking]->Uninit();
			m_pScore[nCntRanking] = nullptr;
		}

		// メモリの解放
		delete[] m_pScore;
		m_pScore = nullptr;
	}

	// ランキングの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CRanking::Update()
{
	if (m_bRankingUpdatedSuccess)
	{
		FlashScore();
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CRanking::Draw()
{

}

//=============================================================================
// 位置のセッター
// Author : 唐﨑結斗
// 概要 : 位置のメンバ変数に引数を代入
//=============================================================================
void CRanking::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;

	SetScore();
}

//=============================================================================
// 向きのセッター
// Author : 唐﨑結斗
// 概要 : 向きのメンバ変数に引数を代入
//=============================================================================
void CRanking::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;

	SetScore();
}

//=============================================================================
// 大きさのセッター
// Author : 唐﨑結斗
// 概要 : 大きさのメンバ変数に引数を代入
//=============================================================================
void CRanking::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;

	SetScore();
}

//=============================================================================
// 全体の大きさのセッター
// Author : 唐﨑結斗
// 概要 : 全体の大きさのメンバ変数に引数を代入
//=============================================================================
void CRanking::SetWholeSize(const D3DXVECTOR3 & wholeSize)
{
	m_wholeSize = wholeSize;

	SetScore();
}

//=============================================================================
// ランキングの取得
// Author : 唐﨑結斗
// 概要 : ランキングの取得
//=============================================================================
void CRanking::GetRanking()
{
	// 変数宣言
	char aReceiveData[128] = {};
	char aResponseData[128] = {};
	WSADATA wsaData;

	// クライアントのインスタンスの生成
	CTCPClient *pTCPClient = new CTCPClient;
	assert(pTCPClient != nullptr);

	// WinSockの初期化
	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0)
	{
		printf("WinSock初期化失敗\n");
	}

	// 接続する
	if (pTCPClient != nullptr
		&& pTCPClient->Init(&m_nIpAddress[0], m_nPortNum))
	{
		// コマンドの設定
		aResponseData[0] = COMMAND_TYPE_GET;

		// データを送信する
		if (pTCPClient->Send(&aResponseData[0], sizeof(aResponseData[0])) <= 0)
		{// 接続相手が切断された
			assert(false);
		}

		if (pTCPClient->Recv(&aReceiveData[0], sizeof(aReceiveData)) <= 0)
		{// 接続相手が切断された
			assert(false);
		}

		for (int nCntData = 0; nCntData < m_nMaxRanking; nCntData++)
		{
			memcpy(&m_aRankingData[nCntData], &aReceiveData[sizeof(int) * nCntData], sizeof(int));

			// エンディアン変換(network to host)
			m_aRankingData[nCntData] = ntohl(m_aRankingData[nCntData]);
		}
	}
	else
	{
		m_bRankingUpdatedSuccess = false;
	}

	if (pTCPClient != nullptr)
	{// 終了
		pTCPClient->Uninit();

		// メモリの解放
		delete pTCPClient;
		pTCPClient = nullptr;
	}

	// WinSockの終了処理
	WSACleanup();
}

//=============================================================================
// ランキングの設定
// Author : 唐﨑結斗
// 概要 : ランキングの設定
//=============================================================================
int CRanking::SetRanking(const int nTime)
{
	// 変数宣言
	char aReceiveData[128] = {};
	char aResponseData[128] = {};
	WSADATA wsaData;
	int nCopyTime = nTime;
	int nRank = 0;

	// クライアントのインスタンスの生成
	CTCPClient *pTCPClient = new CTCPClient;
	assert(pTCPClient != nullptr);

	// WinSockの初期化
	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0)
	{
		printf("WinSock初期化失敗\n");
	}

	// 接続する
	if (pTCPClient != nullptr
		&& pTCPClient->Init(&m_nIpAddress[0], m_nPortNum))
	{
		// コマンドの設定
		aResponseData[0] = COMMAND_TYPE_SET;

		// 数値の代入
		nCopyTime = htonl(nCopyTime);
		memcpy(&aResponseData[1], &nCopyTime, sizeof(nCopyTime));

		// データを送信する
		if (pTCPClient->Send(&aResponseData[0], 1 + sizeof(int)) <= 0)
		{// 接続相手が切断された
			assert(false);
		}

		if (pTCPClient->Recv(&aReceiveData[0], sizeof(aReceiveData)) <= 0)
		{// 接続相手が切断された
			assert(false);
		}

		memcpy(&nRank, &aReceiveData[0], sizeof(int));

		// エンディアン変換(network to host)
		nRank = ntohl(nRank);
	}
	else
	{
		m_bRankingUpdatedSuccess = false;
	}

	if (pTCPClient != nullptr)
	{// 終了
		pTCPClient->Uninit();

		// メモリの解放
		delete pTCPClient;
		pTCPClient = nullptr;
	}

	// WinSockの終了処理
	WSACleanup();

	return nRank;
}

//=============================================================================
// スコアの生成
// Author : 唐﨑結斗
// 概要 : スコアを生成する
//=============================================================================
void CRanking::CreateScore()
{
	// スコアの確保
	m_pScore = new CScore*[m_nMaxRanking];
	assert(m_pScore != nullptr);

	for (int nCntRanking = 0; nCntRanking < m_nMaxRanking; nCntRanking++)
	{// ナンバーの設定
		m_pScore[nCntRanking] = CScore::Create(m_nDigit, false);
	}
}

//=============================================================================
// スコアの設定
// Author : 唐﨑結斗
// 概要 : スコアの位置や、大きさの設定
//=============================================================================
void CRanking::SetScore()
{
	if (m_pScore != nullptr)
	{
		for (int nCntRanking = 0; nCntRanking < m_nMaxRanking; nCntRanking++)
		{// ナンバーの設定
			m_pScore[nCntRanking]->SetWholeSize(D3DXVECTOR3(m_wholeSize.x, m_wholeSize.y / m_nMaxRanking, 0.0f));

			D3DXVECTOR3 scoreWholeSize = m_pScore[nCntRanking]->GetWholeSize();
			m_pScore[nCntRanking]->SetSize(D3DXVECTOR3(scoreWholeSize.x / m_nDigit, scoreWholeSize.y, 0.0f));

			D3DXVECTOR3 size = m_pScore[nCntRanking]->GetSize();
			m_pScore[nCntRanking]->SetPos(D3DXVECTOR3(m_pos.x + m_wholeSize.x / 2.0f, m_pos.y - m_wholeSize.y / 2.0f + (size.y / 2.0f) + (size.y * nCntRanking), 0.0f));
		}
	}
	else
	{
		SetFailObj();
	}
}

//=============================================================================
// ランキングの読み込み
// Author : 唐﨑結斗
// 概要 : スコアの位置や、大きさの設定
//=============================================================================
void CRanking::LoadRanking()
{
	// ランキングの更新
	m_UpdateRank = SetRanking(m_nNewScore);

	// ランキングの取得
	GetRanking();

	m_bUpdatingRanking = false;
}

//=============================================================================
// ファイルの読み込み
// Author : 唐﨑結斗
// 概要 : ファイルからIPアドレスからポート番号を取得する
//=============================================================================
void CRanking::LoadFile()
{
	// 変数宣言
	char aStr[128];

	// ファイルの読み込み
	FILE *pFile = fopen("data/FILE/server.txt", "r");

	if (pFile != nullptr)
	{
		while (fscanf(pFile, "%s", &aStr[0]) != EOF)
		{// "EOF"を読み込むまで 

			if (strstr(&aStr[0], "IPADDRESS") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%s", &m_nIpAddress[0]);
			}

			if (strstr(&aStr[0], "PORT_NUM") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%d", &m_nPortNum);
			}
		}
	}
	else
	{
		printf("ファイル読み込み失敗\n");
	}
}

//=============================================================================
// ランキングスコアの設定
// Author : 唐﨑結斗
// 概要 : ランキングスコアの設定を行う
//=============================================================================
void CRanking::SetScoreNomber()
{
	for (int nCntRank = 0; nCntRank < m_nMaxRanking; nCntRank++)
	{
		m_pScore[nCntRank]->SetDestScore(m_aRankingData[nCntRank]);
		m_pScore[nCntRank]->SetScore(m_aRankingData[nCntRank]);
	}
}

//=============================================================================
// ランキングスコアの点滅処理
// Author : 唐﨑結斗
// 概要 : ランキングスコアを点滅させる
//=============================================================================
void CRanking::FlashScore()
{
	if (m_UpdateRank >= 0)
	{
		m_fAddAlpha += 0.5f;
		m_pScore[m_UpdateRank]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.2f, sinf(m_fAddAlpha) * 3.0f));
	}
}

//=============================================================================
// 通信エラー表示処理
// Author : 唐﨑結斗
// 概要 : 通信エラー表示をする
//=============================================================================
void CRanking::SetFailObj()
{
	if (pFailObj != nullptr)
	{
		pFailObj->SetPos(m_pos);
		pFailObj->SetRot(m_rot);
	}
}
