//=============================================================================
//
// �v���C���[�N���X(player.cpp)
// Author : �������l
// �T�v : �v���C���[�������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "motion_enemy.h"
#include "game.h"
#include "mesh.h"
#include "motion.h"
#include "renderer.h"
#include "application.h"
#include "calculation.h"
#include "move.h"
#include "debug_proc.h"
#include "collision_rectangle3D.h"
#include "camera.h"
#include "parts.h"

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : ���[�V�����L�����N�^�[3D�𐶐�����
//=============================================================================
CMotionEnemy * CMotionEnemy::Create()
{
	// �I�u�W�F�N�g�C���X�^���X
	CMotionEnemy *pPlayer = nullptr;

	// �������̉��
	pPlayer = new CMotionEnemy;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pPlayer != nullptr);

	// ���l�̏�����
	pPlayer->Init();

	// �C���X�^���X��Ԃ�
	return pPlayer;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CMotionEnemy::CMotionEnemy() : m_pMove(nullptr),
m_pCollRectangle3D(nullptr),
m_EAction(NEUTRAL_ACTION),
m_rotDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_fSpeed(0.0f),
m_nNumMotion(0),
m_nLife(0),
m_nCntState(0),			
m_bDamage(false)
{

}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CMotionEnemy::~CMotionEnemy()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CMotionEnemy::Init()
{
	// ������
	CMotionModel3D::Init();

	// �ړ��N���X�̃������m��
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(1.0f, 5.0f, 0.5f, 0.1f);

	// 3D��`�̓����蔻��̐ݒ�
	m_pCollRectangle3D = CCollision_Rectangle3D::Create();
	m_pCollRectangle3D->SetParent(this);
	m_pCollRectangle3D->SetPos(D3DXVECTOR3(0.0f, 25.0f, 0.0f));
	m_pCollRectangle3D->SetSize(D3DXVECTOR3(30.0f, 50.0f, 20.0f));

	// �̗͂̏����l
	m_nLife = 10;

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	SetObjType(OBJETYPE_ENEMY);

	return E_NOTIMPL;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CMotionEnemy::Uninit()
{
	// �J�����̎擾
	CCamera *pCamera = CApplication::GetCamera();

	if (pCamera->GetTargetPosR() == this)
	{// �J�����̃^�[�Q�b�g���I��
		pCamera->SetTargetPosR(false);
		pCamera->SetPosVOffset(D3DXVECTOR3(0.0f, 0.0f, -500.0f));
	}

	if (m_pMove != nullptr)
	{// �������̉��
		delete m_pMove;
		m_pMove = nullptr;
	}

	if (m_pCollRectangle3D != nullptr)
	{// �I������
		m_pCollRectangle3D->Uninit();
		m_pCollRectangle3D = nullptr;
	}

	// �I��
	CMotionModel3D::Uninit();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : 2D�X�V���s��
//=============================================================================
void CMotionEnemy::Update()
{
	// ���[�V�������̎擾
	CMotion *pMotion = CMotionModel3D::GetMotion();

	if (m_bDamage)
	{// ��Ԃ̃J�E���g������
		m_nCntState--;

		if (m_nCntState <= 0)
		{// �J�E���g���Z�b�g
			m_nCntState = 0;
			m_bDamage = false;

			// �p�[�c���̎擾
			int nMaxParts = pMotion->GetMaxParts();

			for (int nCntParts = 0; nCntParts < nMaxParts; nCntParts++)
			{// �p�[�c�̎擾
				CParts *pParts = pMotion->GetParts(nCntParts);
				pParts->SetColor(false);
			}
		}
	}

	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// �ړ�
	pos += Move();

	pos.y -= CCalculation::Gravity();

	// ��]
	Rotate();

	// �j���[�g�������[�V�����̐ݒ�
	if (pMotion != nullptr
		&& !pMotion->GetMotion())
	{
		pMotion->SetNumMotion(NEUTRAL_ACTION);
		m_EAction = NEUTRAL_ACTION;
	}

	// �ʒu�̐ݒ�
	SetPos(pos);

	// ���b�V���̓����蔻��
	CMesh3D *pMesh = CGame::GetMesh();
	pMesh->Collison(this);

	// �X�V
	CMotionModel3D::Update();
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : 2D�`����s��
//=============================================================================
void CMotionEnemy::Draw()
{
	// �`��
	CMotionModel3D::Draw();
}

//=============================================================================
// �U�����󂯂�
// Author : �������l
// �T�v : �U�����󂯁A�_���[�W��Ԃɂ���
//=============================================================================
void CMotionEnemy::Hit(const int nAttack)
{
	if (!m_bDamage
		&& m_nLife > 0)
	{// ��ԃJ�E���^�[�̐ݒ�
		m_nCntState = 30;

		// �_���[�W�t���O
		m_bDamage = true;

		// �̗͂̌���
		m_nLife -= nAttack;

		if (m_nLife <= 0)
		{// �̗͂�0�ȉ�
			Uninit();
			return;
		}

		// ���[�V�������̎擾
		CMotion *pMotion = GetMotion();
		int nMaxParts = pMotion->GetMaxParts();

		for (int nCntParts = 0; nCntParts < nMaxParts; nCntParts++)
		{// �p�[�c�̎擾
			CParts *pParts = pMotion->GetParts(nCntParts);
			pParts->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}				
}

//=============================================================================
// �ړ�
// Author : �������l
// �T�v : �ړ��x�N�g�����Z�o����
//=============================================================================
D3DXVECTOR3 CMotionEnemy::Move()
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// ��]
// Author : �������l
// �T�v : �ړI�̌����܂ŉ�]����
//=============================================================================
void CMotionEnemy::Rotate()
{
	// �����̎擾
	D3DXVECTOR3 rot = GetRot();

	// �����̍X�V
	rot.y += (m_rotDest.y - rot.y) * 0.5f;

	// �����̐��K��
	rot.y = CCalculation::RotNormalization(rot.y);

	// �����̐ݒ�
	SetRot(rot);
}


