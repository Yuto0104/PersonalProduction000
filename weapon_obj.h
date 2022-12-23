//=============================================================================
//
// 武器クラス(weapon_obj.h)
// Author : 唐﨑結斗
// 概要 : 武器生成を行う
//
//=============================================================================
#ifndef _WEAPON_OBJ_H_			// このマクロ定義がされてなかったら
#define _WEAPON_OBJ_H_			// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "model_obj.h"

//=============================================================================
// モデルオブジェクトクラス
// Author : 唐﨑結斗
// 概要 : モデルオブジェクト生成を行うクラス
//=============================================================================
class CWeaponObj : public CModelObj
{
public:
	//--------------------------------------------------------------------
	// 武器の種別の列挙型
	// Author : 唐﨑結斗
	// 概要 : 武器の種別情報を格納
	//--------------------------------------------------------------------
	enum EWeaponType
	{
		WEAPONTYPE_NONE = 0,		// タイプ無し
		WEAPONTYPE_KNIFE,			// ナイフ
		MAX_WEAPONTYPE,				// 種別の最大数
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CWeaponObj *Create();							// モデルオブジェクトの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CWeaponObj();
	~CWeaponObj();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;														// 初期化
	void Uninit() override;															// 終了
	void Update() override;															// 更新
	void Draw() override;															// 描画
	void SetParent(CModel3D *pParent) { m_pParent = pParent; }						// 親の設定
	void SetParent() { m_pParent = nullptr; }										// 親の設定解除
	void SetWeaponType(EWeaponType weaponType) { m_weaponType = weaponType; }		// 武器のタイプの設定
	void SetPosOffset(D3DXVECTOR3 posOffset) { m_posOffset = posOffset; }			// 位置の差分の設定
	void SetRotOffset(D3DXVECTOR3 rotOffset) { m_rotOffset = rotOffset; }			// 向きの差分の設定
	void SetAttack(const int nAttack) { m_nAttack = nAttack; }						// 攻撃力の設定
	EWeaponType GetWeaponType() { return m_weaponType; }							// 武器のタイプの取得
	int GetAttack() { return m_nAttack; }											// 攻撃力の取得

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CModel3D *m_pParent;			// 親モデルの情報
	EWeaponType m_weaponType;		// 武器の種別
	D3DXVECTOR3 m_posOffset;		// 位置の差分
	D3DXVECTOR3 m_rotOffset;		// 向きの差分
	D3DXVECTOR3 m_colliSize;		// 当たり判定の大きさ
	int m_nAttack;					// 攻撃力
};

#endif

