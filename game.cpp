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
#include "model3D.h"
#include "mesh.h"
#include "sphere.h"
#include "bg.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CPlayer *CGame::m_pPlayer = nullptr;					// �v���C���[�N���X
CMesh3D *CGame::m_pMesh3D;								// ���b�V���N���X
CMotionModel3D *CGame::m_pMotionModel3D;				// ���[�V�������f���N���X
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

	// �d�͂̒l��ݒ�
	CCalculation::SetGravity(10.0f);

	// �n�ʂ̐ݒ�
	m_pMesh3D = CMesh3D::Create();
	m_pMesh3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMesh3D->SetSize(D3DXVECTOR3(5000.0f, 0, 5000.0f));
	m_pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(10, 10));
	m_pMesh3D->SetSplitTex(true);
	m_pMesh3D->LoadTex(13);

	// �X�J�C�{�b�N�X�̐ݒ�
	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pSphere->SetRadius(50000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
	pSphere->LoadTex(12);

	// �v���C���[�̐ݒ�
	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetMotion("data/MOTION/motion.txt");
	m_pPlayer->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ���f���̕\��
	m_pMotionModel3D = CMotionModel3D::Create();
	m_pMotionModel3D->SetMotion("data/MOTION/motion.txt");
	m_pMotionModel3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 100.0f));

	// �J�����̒Ǐ]�ݒ�(�ڕW : �v���C���[)
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetFollowTarget(m_pPlayer, 1.0);
	pCamera->SetPosVOffset(D3DXVECTOR3(0.0f, 0.0f, -500.0f));
	pCamera->SetPosROffset(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
	pCamera->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �}�E�X�J�[�\��������
	pMouse->SetShowCursor(false);

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

	// �}�E�X�J�[�\�����o��
	pMouse->SetShowCursor(true);

	// �J�����̒Ǐ]�ݒ�(�ڕW : �v���C���[)
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
	// �L�[�{�[�h�̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

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
			pCamera->SetPosRDiff(D3DXVECTOR2(100.0f, 200.0f));
		}
		else
		{
			pCamera->SetTargetPosR(false);
		}
	}
#endif // _DEBUG
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : �`����s��
//=============================================================================
void CGame::Draw()
{

}
