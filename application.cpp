//=============================================================================
//
// アプリケーションクラス(application.h)
// Author : 唐﨑結斗
// 概要 : 各クラスの呼び出しを行うクラス
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "application.h"

#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "super.h"
#include "renderer.h"
#include "debug_proc.h"
#include "keyboard.h"
#include "mouse.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "sound.h"
#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include "model3D.h"
#include "scene_mode.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "collision.h"
#include "pause.h"
#include "tutorial.h"
#include "joypad.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
HWND CApplication::m_hWnd = nullptr;								// ウィンドウ
CDebugProc *CApplication::m_pDebugProc = nullptr;					// デバック表示
CRenderer *CApplication::m_pRenderer = nullptr;						// レンダラーインスタンス
CKeyboard *CApplication::m_pKeyboard = {};							// キーボードインスタンス
CMouse *CApplication::m_pMouse = {};								// マウスインスタンス
CTexture *CApplication::m_pTexture = nullptr;						// テクスチャインスタンス
CCamera *CApplication::m_pCamera = nullptr;							// カメラインスタンス
CCamera *CApplication::m_pMapCamera = nullptr;						// マップカメラ
CApplication::SCENE_MODE CApplication::m_mode = MODE_NONE;			// 現在のモードの格納
CApplication::SCENE_MODE CApplication::m_nextMode = MODE_TITLE;		// 次のモードの格納
CSceneMode *CApplication::pSceneMode = nullptr;						// シーンモードを格納
CFade *CApplication::m_pFade = nullptr;								// フェードクラス
CLight *CApplication::m_pLight = nullptr;							// ライトクラス
CSound *CApplication::m_pSound = nullptr;							// サウンドクラス
CPause *CApplication::m_pPause = nullptr;							// ポーズクラス
CJoypad *CApplication::m_pJoy = nullptr;							// ジョイパッドクラス
int CApplication::m_nPriority = 0;									// プライオリティ番号
int CApplication::m_nScore = 0;										// 現在のスコア
bool CApplication::m_bWireFrame = false;							// ワイヤーフレームを使うか

//=============================================================================
// スクリーン座標をワールド座標にキャストする
// Author : 唐﨑結斗
// 概要 : 
//=============================================================================
D3DXVECTOR3 CApplication::ScreenCastWorld(const D3DXVECTOR3 &pos)
{
	// 計算用マトリックス
	D3DXMATRIX mtx, mtxTrans, mtxView, mtxPrj, mtxViewPort;

	// 行列移動関数 (第一引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);

	// カメラのビューマトリックスの取得
	mtxView = m_pCamera->GetMtxView();

	// カメラのプロジェクションマトリックスの取得
	mtxPrj = m_pCamera->GetMtxProj();

	// マトリックスの乗算
	mtx = mtxTrans * mtxView * mtxPrj;

	// ビューポート行列（スクリーン行列）の作成
	float w = (float)CRenderer::SCREEN_WIDTH / 2.0f;
	float h = (float)CRenderer::SCREEN_HEIGHT / 2.0f;

	mtxViewPort = {
		w , 0 , 0 , 0 ,
		0 ,-h , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		w , h , 0 , 1
	};

	// マトリックスのXYZ
	D3DXVECTOR3 vec = D3DXVECTOR3(mtx._41, mtx._42, mtx._43);

	D3DXVec3TransformCoord(&vec, &vec, &mtxViewPort);

	return vec;
}

//=============================================================================
// ワールド座標をスクリーン座標にキャストする
// Author : 唐﨑結斗
// 概要 : 
//=============================================================================
D3DXVECTOR3 CApplication::WorldCastScreen(const D3DXVECTOR3 &pos)
{
	// 計算用ベクトル
	D3DXVECTOR3 vec = pos;

	// 計算用マトリックス
	D3DXMATRIX mtx, mtxTrans, mtxView, mtxPrj, mtxViewPort;

	// 行列移動関数 (第一引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);

	// カメラのビューマトリックスの取得
	mtxView = m_pCamera->GetMtxView();

	// カメラのプロジェクションマトリックスの取得
	mtxPrj = m_pCamera->GetMtxProj();
	
	// ビューポート行列（スクリーン行列）の作成
	D3DXMatrixIdentity(&mtxViewPort);
	float w = (float)CRenderer::SCREEN_WIDTH / 2.0f;
	float h = (float)CRenderer::SCREEN_HEIGHT / 2.0f;
	mtxViewPort = {
		w , 0 , 0 , 0 ,
		0 ,-h , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		w , h , 0 , 1
	};

	// 逆行列の算出
	D3DXMatrixInverse(&mtxView, NULL, &mtxView);
	D3DXMatrixInverse(&mtxPrj, NULL, &mtxPrj);
	D3DXMatrixInverse(&mtxViewPort, NULL, &mtxViewPort);

	// 逆変換
	mtx = mtxViewPort * mtxPrj * mtxView;
	D3DXVec3TransformCoord(&vec, &D3DXVECTOR3(vec.x, vec.y, vec.z), &mtx);

	return vec;
}

//=============================================================================
// 角度の正規化処理
// Author : 唐﨑結斗
// 概要 : 角度が円周率の2倍を超えたときに範囲内に戻す
//=============================================================================
float CApplication::RotNormalization(float fRot)
{
	if (fRot >= D3DX_PI)
	{// 移動方向の正規化
		fRot -= D3DX_PI * 2;
	}
	else if (fRot <= -D3DX_PI)
	{// 移動方向の正規化
		fRot += D3DX_PI * 2;
	}

	return fRot;
}

//=============================================================================
// 角度の正規化処理
// Author : 唐﨑結斗
// 概要 : 角度が円周率の2倍を超えたときに範囲内に戻す
//=============================================================================
float CApplication::RotNormalization(float fRot, float fMin, float fMax)
{
	if (fRot >= fMax)
	{// 移動方向の正規化
		fRot -= D3DX_PI * 2;
	}
	else if (fRot <= fMin)
	{// 移動方向の正規化
		fRot += D3DX_PI * 2;
	}

	return fRot;
}

//=============================================================================
// シーンの設定
// Author : 唐﨑結斗
// 概要 : 現在のシーンを終了し、新しいシーンを設定する
//=============================================================================
void CApplication::SetMode(SCENE_MODE mode)
{
	if (pSceneMode != nullptr)
	{
		pSceneMode->Uninit();
		pSceneMode = nullptr;
	}

	// オブジェクトの解放
	CSuper::ReleaseAll(false);

	// 当たり判定の終了
	CCollision::ReleaseAll();

	m_mode = mode;

	switch (m_mode)
	{
	case CApplication::MODE_TITLE:
		pSceneMode = new CTitle;
		break;

	case CApplication::MODE_GAME:
		pSceneMode = new CGame;
		break;

	case CApplication::MODE_RESULT:
		pSceneMode = new CResult;
		break;

	case CApplication::MODE_TUTORIAL:
		pSceneMode = new CTutorial;
		break;

	default:
		break;
	}

	if (pSceneMode != nullptr)
	{
		pSceneMode->Init();
	}
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CApplication::CApplication()
{
	// 疑似乱数の初期化
	srand((unsigned int)time(NULL));
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CApplication::~CApplication()
{
	assert(m_pRenderer == nullptr);
	assert(m_pKeyboard == nullptr);
	assert(m_pMouse == nullptr);
	assert(m_pJoy == nullptr);
	assert(m_pTexture == nullptr);
	assert(m_pCamera == nullptr); 
	assert(m_pSound == nullptr);
}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : メンバ変数を解放し、他クラスのクリエイト関数を呼び出す
//=============================================================================
HRESULT CApplication::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ウィンドウ
	m_hWnd = hWnd;

	// メモリの確保	
	m_pRenderer = new CRenderer;
	m_pDebugProc = new CDebugProc;
	m_pTexture = new CTexture;
	m_pCamera = new CCamera;
	m_pMapCamera = new CCamera;
	m_pSound = new CSound;
	m_pJoy = new CJoypad;

	// 入力デバイスのメモリ確保
	m_pKeyboard = new CKeyboard;
	m_pMouse = new CMouse;

	// 初期化処理
	assert(m_pRenderer != nullptr);

	//if (FAILED(m_pRenderer->Init(m_hWnd, TRUE)) != 0)
	//{//初期化処理が失敗した場合
	//	return-1;
	//}

#ifdef _DEBUG
	if (FAILED(m_pRenderer->Init(m_hWnd, TRUE)) != 0)
	{//初期化処理が失敗した場合
		return-1;
	}
#else
	if (FAILED(m_pRenderer->Init(m_hWnd, FALSE)) != 0)
	{//初期化処理が失敗した場合
		return-1;
	}
#endif

	// 初期化処理
	assert(m_pTexture != nullptr);
	m_pTexture->Init();

	// 初期化処理
	assert(m_pCamera != nullptr);
	m_pCamera->Init();
	m_pCamera->SetViewType(CCamera::TYPE_CLAIRVOYANCE);

	// 初期化処理
	assert(m_pMapCamera != nullptr);
	m_pMapCamera->Init();
	m_pMapCamera->SetViewType(CCamera::TYPE_PARALLEL);

	// 初期化
	assert(m_pDebugProc != nullptr);
	m_pDebugProc->Init();

	// 初期化処理
	assert(m_pSound != nullptr);
	m_pSound->Init(m_hWnd);

	// 初期化処理
	assert(m_pKeyboard != nullptr);
	if (FAILED(m_pKeyboard->Init(hInstance, m_hWnd)))
	{
		return E_FAIL;
	}

	// 初期化処理
	assert(m_pMouse != nullptr);
	if (FAILED(m_pMouse->Init(hInstance, m_hWnd)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pJoy->Init(1)))
	{
		return E_FAIL;
	}

	// モデル情報の初期化
	CModel3D::InitModel();

	// ライトの作成
	m_pLight = CLight::Create(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CLight::Create(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// フェードの設定
	m_pFade = CFade::Create();

	// ポーズの生成
	m_pPause = CPause::Create();
	m_pPause->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : メモリの解放とリリース関数を呼び出す
//=============================================================================
void CApplication::Uninit()
{
	// オブジェクトの解放
	CSuper::ReleaseAll(true);

	// 当たり判定の終了
	CCollision::ReleaseAll();

	// モデル情報の終了
	CModel3D::UninitModel();

	if (m_pRenderer != nullptr)
	{// 終了処理
		m_pRenderer->Uninit();

		// メモリの解放
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pDebugProc != nullptr)
	{// 終了処理
		m_pDebugProc->Uninit();

		// メモリの解放
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}

	if (m_pKeyboard != nullptr)
	{// 終了処理
		m_pKeyboard->Uninit();

		// メモリの解放
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	if (m_pMouse != nullptr)
	{// 終了処理
		m_pMouse->Uninit();

		// メモリの解放
		delete m_pMouse;
		m_pMouse = nullptr;
	}

	if (m_pTexture != nullptr)
	{// 終了処理
		m_pTexture->Uninit();

		// メモリの解放
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pCamera != nullptr)
	{// 終了処理
		m_pCamera->Uninit();

		// メモリの解放
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pMapCamera != nullptr)
	{// 終了処理
		m_pMapCamera->Uninit();

		// メモリの解放
		delete m_pMapCamera;
		m_pMapCamera = nullptr;
	}

	if (m_pSound != nullptr)
	{// 終了処理
		m_pSound->Uninit();

		// メモリの解放
		delete m_pSound;
		m_pSound = nullptr;
	}

	if (m_pJoy != nullptr)
	{// 終了処理
		m_pJoy->Uninit();

		delete m_pJoy;
		m_pJoy = nullptr;
	}

	// ライトの解放
	CLight::ReleaseAll();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新
//=============================================================================
void CApplication::Update()
{
	if (m_nextMode != MODE_NONE)
	{
		m_pFade->SetFade(m_nextMode);
		m_nextMode = MODE_NONE;
	}

	m_pKeyboard->Update();
	m_pMouse->Update();
	m_pJoy->Update();

	if (!CSuper::GetPause())
	{
		m_pCamera->Update();
		m_pMapCamera->Update();
	}

	m_pRenderer->Update();

#ifdef _DEBUG
	CDebugProc::Print("FPS : %d\n", GetFps());
	CDebugProc::Print("現在のシーン : %d\n", (int)m_mode);
	CDebugProc::Print("コントローラーの使用数 : %d\n", m_pJoy->GetUseJoyPad());

	if (m_pKeyboard->GetTrigger(DIK_F1))
	{
		bool bDrawFlag = CDebugProc::GetDrawFlag();
		bDrawFlag ^= 1;
		CDebugProc::SetDrawFlag(bDrawFlag);
	}

	if (m_pKeyboard->GetTrigger(DIK_F2))
	{
		m_bWireFrame ^= 1;
	}

	if (m_bWireFrame)
	{
		m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{
		m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

#endif // DEBUG
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画
//=============================================================================
void CApplication::Draw()
{
	m_pRenderer->Draw();
}
