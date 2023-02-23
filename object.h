//=============================================================================
//
// オブジェクトクラス(object.h)
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================
#ifndef _OBJECT_H_		// このマクロ定義がされてなかったら
#define _OBJECT_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "super.h"

//=============================================================================
// オブジェクトクラス
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行うクラス
//=============================================================================
class CObject : public CSuper
{
public:
	//--------------------------------------------------------------------
	// オブジェクトの種別の列挙型
	// Author : 唐﨑結斗
	// 概要 : オブジェクトの種別情報を格納
	//--------------------------------------------------------------------
	enum EObjectType
	{
		OBJTYPE_NONE = 0,			// タイプ無し
		OBJTYPE_FADE,				// フェード
		OBJTYPE_PAUSE,				// ポーズ
		OBJETYPE_PLAYER,			// プレイヤー
		OBJETYPE_ENEMY,				// エネミー
		OBJTYPE_3DMODEL,			// 3Dモデル
		OBJETYPE_WEAPON,			// 武器
		OBJETYPE_WIRE,				// ワイヤー
		OBJETYPE_MESH,				// メッシュ
		OBJETYPE_SKYBOX,			// スカイボックス
		OBJETYPE_SCOREITEM,			// スコアアイテム
		MAX_OBJTYPE,				// 種別の最大数
	};

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	explicit CObject(int nPriority = PRIORITY_LEVEL0);
	virtual ~CObject();

	//--------------------------------------------------------------------
	// 純粋仮想関数
	//--------------------------------------------------------------------
	virtual void SetPos(const D3DXVECTOR3 &pos) = 0;							// 位置のセッター
	virtual void SetPosOld(const D3DXVECTOR3 &posOld) = 0;						// 過去位置のセッタ
	virtual void SetRot(const D3DXVECTOR3 &rot) = 0;							// 向きのセッター
	virtual void SetSize(const D3DXVECTOR3 &size) = 0;							// 大きさのセッター
	virtual D3DXVECTOR3 GetPos() = 0;											// 位置のゲッター
	virtual D3DXVECTOR3 GetPosOld() = 0;										// 過去位置のゲッタ
	virtual D3DXVECTOR3 GetRot() = 0;											// 向きのゲッター
	virtual D3DXVECTOR3 GetSize() = 0;											// 大きさのゲッター
	void SetObjType(EObjectType objectType) { m_objectType = objectType; }		// オブジェクトの種別設定
	EObjectType GetObjType() { return m_objectType; }							// オブジェクトの種別設定

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	EObjectType m_objectType;		// オブジェクトの種別
};

#endif

