//=============================================================================
//
// �Q�[���N���X(game.cpp)
// Author : �������l
// �T�v : �Q�[���N���X�̊Ǘ����s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "game.h"
#include "calculation.h"
#include "keyboard.h"
#include "mouse.h"
#include "application.h"
#include "camera.h"
#include "renderer.h"
#include "object.h"
#include "object3D.h"
#include "player.h"
#include "motion_model3D.h"
#include "motion_enemy.h"
#include "model3D.h"
#include "mesh.h"
#include "sphere.h"
#include "bg.h"
#include "model_obj.h"
#include "weapon_obj.h"
#include "collision_rectangle3D.h"
#include "debug_proc.h"
#include "wire.h"
#include "map_manager.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CPlayer *CGame::m_pPlayer = nullptr;					// �v���C���[�N���X
CMesh3D *CGame::m_pMesh3D;								// ���b�V���N���X
CMotionEnemy *CGame::m_pMotionModel3D;					// ���[�V�������f���N���X
CWire *CGame::m_pWire;
D3DXCOLOR CGame::fogColor;								// �t�H�O�J���[
float CGame::fFogStartPos;								// �t�H�O�̊J�n�_
float CGame::fFogEndPos;								// �t�H�O�̏I���_
float CGame::fFogDensity;								// �t�H�O�̖��x
bool CGame::m_bGame = false;							// �Q�[���̏�

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CGame::Init()
{// �}�E�X�̎擾
	CMouse *pMouse = CApplication::GetMouse();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �d�͂̒l��ݒ�
	CCalculation::SetGravity(0.2f);

	// �}�b�v�̐ݒ�
	CMapManager *pMap = CMapManager::Create();
	pMap->SetMap("data/FILE/map000.txt");

	// �X�J�C�{�b�N�X�̐ݒ�
	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pSphere->SetRadius(50000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));

	// �v���C���[�̐ݒ�
	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetMotion("data/MOTION/motion.txt");
	m_pPlayer->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �G�̐ݒu
	m_pMotionModel3D = CMotionEnemy::Create();
	m_pMotionModel3D->SetMotion("data/MOTION/motion.txt");
	m_pMotionModel3D->SetPos(D3DXVECTOR3(100.0f, 0.0f, 0.0f));
	CMotionEnemy *pMotionModel3D = CMotionEnemy::Create();
	pMotionModel3D->SetMotion("data/MOTION/motion.txt");
	pMotionModel3D->SetPos(D3DXVECTOR3(-100.0f, 0.0f, 0.0f));

	// �J�����̒Ǐ]�ݒ�(�ڕW : �v���C���[)
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetFollowTarget(m_pPlayer, 1.0);
	pCamera->SetPosVOffset(D3DXVECTOR3(0.0f, 0.0f, -500.0f));
	pCamera->SetPosROffset(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
	pCamera->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �J�����̒Ǐ]�ݒ�(�ڕW : �v���C���[)
	pCamera = CApplication::GetMapCamera();
	pCamera->SetFollowTarget(m_pPlayer, 1.0);
	pCamera->SetPosVOffset(D3DXVECTOR3(0.0f, 1000.0f, 0.0f));
	pCamera->SetPosROffset(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pCamera->SetViewSize(0, 0, CRenderer::SCREEN_WIDTH / 4, CRenderer::SCREEN_HEIGHT / 4);

	// ����̂̐ݒu
	CWeaponObj *pWeapon = CWeaponObj::Create();
	pWeapon->SetPos(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
	pWeapon->SetType(11);
	pWeapon->SetWeaponType(CWeaponObj::WEAPONTYPE_KNIFE);
	pWeapon->SetAttack(5);
	CCollision_Rectangle3D *pCollision = pWeapon->GetCollision();
	pCollision->SetSize(D3DXVECTOR3(10.0f, 36.0f, 10.0f));
	pCollision->SetPos(D3DXVECTOR3(0.0f, 18.0f, 0.0f));

	// ���C���[
	m_pWire = CWire::Create();
	m_pWire->SetPos(D3DXVECTOR3(0.0f, 10.0f, 200.0f));

	// �}�E�X�J�[�\��������
	pMouse->SetShowCursor(false);

	// �t�H�O�̐��l�ݒ�
	fogColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �t�H�O�J���[
	fFogStartPos = 1.0f;								// �t�H�O�̊J�n�_
	fFogEndPos = 100000.0f;								// �t�H�O�̏I���_
	fFogDensity = 0.00001f;								// �t�H�O�̖��x

	// �t�H�O�̗L���ݒ�
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	// �t�H�O�J���[�̐ݒ�
	pDevice->SetRenderState(D3DRS_FOGCOLOR, fogColor);

	// �t�H�O�͈̔͐ݒ�
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fFogStartPos));
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fFogEndPos));

	// �t�H�O�̖��x�̐ݒ�
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&fFogDensity));

	m_bGame = true;

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CGame::Uninit()
{// �}�E�X�̎擾
	CMouse *pMouse = CApplication::GetMouse();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �t�H�O�̗L���ݒ�
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// �}�E�X�J�[�\�����o��
	pMouse->SetShowCursor(true);

	// �J�����̒Ǐ]�ݒ�
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetFollowTarget(false);
	pCamera->SetTargetPosR(false);

	// �ÓI�ϐ��̏�����
	if (m_pPlayer != nullptr)
	{
		m_pPlayer = nullptr;
	}

	if (m_pMesh3D != nullptr)
	{
		m_pMesh3D = nullptr;
	}

	if (m_pWire != nullptr)
	{
		m_pWire->Uninit();
	}

	// �X�R�A�̉��
	Release();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V���s��
//=============================================================================
void CGame::Update()
{
#ifdef _DEBUG
	CDebugProc::Print("�G�̗̑� : %d\n", m_pMotionModel3D->GetLife());

	// �L�[�{�[�h�̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// �J�����̒Ǐ]�ݒ�
	CCamera *pCamera = CApplication::GetCamera();

	if (pKeyboard->GetTrigger(DIK_F3))
	{
		CApplication::SetNextMode(CApplication::MODE_RESULT);
	}
	if (pKeyboard->GetTrigger(DIK_F4))
	{
		CCamera *pCamera = CApplication::GetCamera();

		if (!pCamera->GetTargetPosR())
		{
			pCamera->SetTargetPosR(m_pMotionModel3D);
			pCamera->SetPosVOffset(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
			pCamera->SetPosRDiff(D3DXVECTOR2(100.0f, 200.0f));
		}
		else
		{
			pCamera->SetTargetPosR(false);
		}
	}
	if (pKeyboard->GetTrigger(DIK_F5))
	{
		D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		switch (m_pWire->GetWireMode())
		{
		case CWire::MODE_STOP:
			m_pWire->SetWireMode(CWire::MODE_FIRING);
			vec = pCamera->GetPosV() - pCamera->GetPosR();
			break;

		case CWire::MODE_FIRING:
			m_pWire->SetWireMode(CWire::MODE_ATTRACT);
			pCamera->SetFollowTarget(m_pWire->GetStart(), 1.0);
			vec = m_pWire->GetStart()->GetPos() - m_pWire->GetGoal()->GetPos();
			break;

		case CWire::MODE_ATTRACT:
			m_pWire->SetWireMode(CWire::MODE_FIRING);
			vec = pCamera->GetPosV() - pCamera->GetPosR();
			break;

		default:
			break;
		}
		
		D3DXVec3Normalize(&vec, &vec);
		m_pWire->SetMoveVec(-vec);
	}

	if (pKeyboard->GetPress(DIK_LSHIFT))
	{
		pCamera->Zoom();
	}

#endif // _DEBUG

	if (!m_bGame)
	{
		CApplication::SetNextMode(CApplication::MODE_RESULT);
	}
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : �`����s��
//=============================================================================
void CGame::Draw()
{

}
