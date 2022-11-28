//=============================================================================
//
// テクスチャ設定処理(texture.cpp)
// Author : 唐﨑結斗
// 概要 : テクスチャ設定を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "texture.h"
#include "renderer.h" 
#include "application.h"

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CTexture::CTexture()
{
	m_pTexture = nullptr;		// テクスチャ情報
	m_nMaxTexture = 0;			// テクスチャの最大数
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 貼り付けるテクスチャ情報を格納する
//=============================================================================
void CTexture::Init(void)
{// レンダラーのゲット
	CRenderer *pRenderer = CApplication::GetRenderer();

	// ファイルの読み込み
	LoadFile();

	for (int nCnt = 0; nCnt < m_nMaxTexture; nCnt++)
	{//ポリゴンに貼り付けるテクスチャの読み込み
		D3DXCreateTextureFromFile(pRenderer->GetDevice(),
			&m_pTexture[nCnt].aFileName[0],
			&m_pTexture[nCnt].pTexture);
	}
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャの解放
//=============================================================================
void CTexture::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_nMaxTexture; nCnt++)
	{//テクスチャの破棄	  
		if (m_pTexture[nCnt].pTexture != nullptr)
		{
			m_pTexture[nCnt].pTexture->Release();

			m_pTexture[nCnt].pTexture = nullptr;
		}
	}

	if (m_pTexture != nullptr)
	{
		delete[] m_pTexture;
		m_pTexture = nullptr;
	}
}

//=============================================================================
// テクスチャポインタのゲッター	
// Author : 唐﨑結斗
// 概要 : テクスチャのゲッター
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(const int nNumTex)
{
	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	if (nNumTex != -1)
	{// タイプが設定されてる
		pTexture = m_pTexture[nNumTex].pTexture;
	}

	return pTexture;
}

//=============================================================================
// ファイルの読み込み
// Author : 唐﨑結斗
// 概要 : ファイルから読み込むテクスチャ数と名前を読み込む
//=============================================================================
void CTexture::LoadFile()
{
	// 変数宣言
	char aStr[128];
	int nCntTex = 0;

	// ファイルの読み込み
	FILE *pFile = fopen("data/FILE/texture.txt", "r");

	if (pFile != nullptr)
	{
		while (fscanf(pFile, "%s", &aStr[0]) != EOF)
		{// "EOF"を読み込むまで 
			if (strncmp(&aStr[0], "#", 1) == 0)
			{// 一列読み込む
				fgets(&aStr[0], sizeof(aStr), pFile);
			}

			if (strstr(&aStr[0], "MAX_TEXTURE") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%d", &m_nMaxTexture);
				m_pTexture = new TEXTURE[m_nMaxTexture];
				assert(m_pTexture != nullptr);
				memset(&m_pTexture[0], 0, sizeof(TEXTURE));
			}

			if (strstr(&aStr[0], "TEXTURE_FILENAME") != NULL)
			{
				fscanf(pFile, "%s", &aStr[0]);
				fscanf(pFile, "%s", &m_pTexture[nCntTex].aFileName[0]);
				nCntTex++;
			}
		}
	}
	else
	{
		assert(false);
	}
}
