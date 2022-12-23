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

#include "player.h"
#include "game.h"
#include "mesh.h"
#include "motion.h"
#include "renderer.h"
#include "application.h"
#include "camera.h"
#include "keyboard.h"
#include "calculation.h"
#include "move.h"
#include "debug_proc.h"
#include "orbit.h"
#include "collision_rectangle3D.h"
#include "collision_sphere.h"
#include "parts.h"
#include "model_obj.h"
#include "weapon_obj.h"
#include "motion_enemy.h"

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : ���[�V�����L�����N�^�[3D�𐶐�����
//=============================================================================
CPlayer * CPlayer::Create()
{
	// �I�u�W�F�N�g�C���X�^���X
	CPlayer *pPlayer = nullptr;

	// �������̉��
	pPlayer = new CPlayer;

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
CPlayer::CPlayer() : m_pMove(nullptr),
m_pOrbit(nullptr),
m_pCollisionRectangle3D(nullptr),
m_pAttack(nullptr),
m_pColliAttack(nullptr),
m_pMyWeapon(nullptr),
m_EAction(NEUTRAL_ACTION),
m_rotDest(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_fSpeed(0.0f),
m_nNumMotion(0),
m_nNumHandParts(0)
{

}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CPlayer::Init()
{
	// ������
	CMotionModel3D::Init();

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	SetObjType(OBJETYPE_PLAYER);

	// �ړ��N���X�̃������m��
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(1.0f, 5.0f, 0.5f, 0.1f);

	// �O�Ղ̐ݒ�
	m_pOrbit = COrbit::Create();
	m_pOrbit->SetMtxParent(GetMtxWorld());
	m_pOrbit->SetOffset(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	m_pOrbit->SetDivision(100);
	m_pOrbit->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
	m_pOrbit->SetBlendMode(COrbit::MODE_ADD);

	// 3D��`�̓����蔻��̐ݒ�
	m_pCollisionRectangle3D = CCollision_Rectangle3D::Create();
	m_pCollisionRectangle3D->SetParent(this);
	m_pCollisionRectangle3D->SetPos(D3DXVECTOR3(0.0f, 25.0f, 0.0f));
	m_pCollisionRectangle3D->SetSize(D3DXVECTOR3(30.0f, 50.0f, 20.0f));

	// ��̃I�u�W�F�N�g�̐ݒ�
	m_pAttack = CModelObj::Create();
	m_pAttack->SetObjType(OBJETYPE_PLAYER);
	m_nNumHandParts = 3;
	m_pColliAttack = CCollision_Rectangle3D::Create();
	m_pColliAttack->SetParent(m_pAttack);
	m_pColliAttack->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pColliAttack->SetSize(D3DXVECTOR3(10.0f, 10.0f, 10.0f));

	return E_NOTIMPL;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CPlayer::Uninit()
{
	// �J�����̒Ǐ]�ݒ�(�ڕW : �v���C���[)
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetFollowTarget(false);
	pCamera->SetTargetPosR(false);

	if (m_pMove != nullptr)
	{// �������̉��
		delete m_pMove;
		m_pMove = nullptr;
	}

	if (m_pOrbit != nullptr)
	{// �I������
		m_pOrbit->Uninit();
		m_pOrbit = nullptr;
	}

	if (m_pCollisionRectangle3D != nullptr)
	{// �I������
		m_pCollisionRectangle3D->Uninit();
		m_pCollisionRectangle3D = nullptr;
	}

	if (m_pAttack != nullptr)
	{// �I������
		m_pAttack->Uninit();
		m_pAttack = nullptr;
	}

	if (m_pColliAttack != nullptr)
	{// �I������
		m_pColliAttack->Uninit();
		m_pColliAttack = nullptr;
	}

	// �I��
	CMotionModel3D::Uninit();

	// �Q�[���I��
	CGame::SetGame(false);
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : 2D�X�V���s��
//=============================================================================
void CPlayer::Update()
{// �L�[�{�[�h�̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// ���[�V�������̎擾
	CMotion *pMotion = CMotionModel3D::GetMotion();

	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// �ߋ��ʒu�̍X�V
	SetPosOld(pos);
	m_pAttack->SetPosOld(m_pAttack->GetPos());

	if (pMotion != nullptr)
	{// ��̃I�u�W�F�N�g�̈ʒu
		CParts *pHand = pMotion->GetParts(m_nNumHandParts);
		D3DXMATRIX mtxParts = pHand->GetMtxWorld();
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &mtxParts);
	}
	
	if (pKeyboard->GetTrigger(DIK_RETURN)
		&& pMotion != nullptr)
	{// �U��
		if (m_pMyWeapon == nullptr)
		{
			m_EAction = ATTACK_ACTION;
		}
		else
		{
			switch (m_pMyWeapon->GetWeaponType())
			{
			case CWeaponObj::WEAPONTYPE_KNIFE:
				m_EAction = KNIFE_ATTACK_ACTION;
				break;

			default:
				assert(false);
				break;
			}
		}

		pMotion->SetNumMotion(m_EAction);
	}

	// �ړ�
	pos += Move();

	pos.y -= CCalculation::Gravity();

	// ��]
	Rotate();

	if (pMotion != nullptr
		&& !pMotion->GetMotion())
	{// �j���[�g�������[�V�����̐ݒ�
		if (m_pMyWeapon == nullptr)
		{
			m_EAction = NEUTRAL_ACTION;
		}
		else
		{
			switch (m_pMyWeapon->GetWeaponType())
			{
			case CWeaponObj::WEAPONTYPE_KNIFE:
				m_EAction = KNIFE_NEUTRAL_ACTION;
				break;

			default:
				assert(false);
				break;
			}
		}

		pMotion->SetNumMotion(m_EAction);
	}

	// �X�V
	CMotionModel3D::Update();

	// �ʒu�̐ݒ�
	SetPos(pos);

	// ���f���Ƃ̓����蔻��
	m_pCollisionRectangle3D->Collision(CObject::OBJTYPE_NONE, true);
	m_pCollisionRectangle3D->Collision(CObject::OBJETYPE_ENEMY, true);

	// ����Ƃ̓����蔻��
	bool bCollisionWeapon = m_pCollisionRectangle3D->Collision(CObject::OBJETYPE_WEAPON, false);

	if (bCollisionWeapon
		&& pKeyboard->GetTrigger(DIK_E))
	{// ����̎擾
		GetWeapon();
	}

	if (pMotion != nullptr
		&& m_pMyWeapon == nullptr)
	{// ��̃I�u�W�F�N�g�̈ʒu
		CParts *pHand = pMotion->GetParts(m_nNumHandParts);
		D3DXMATRIX mtxParts = pHand->GetMtxWorld();
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &mtxParts);
		m_pAttack->SetPos(pos);
	}
	else if (pMotion != nullptr
		&& m_pMyWeapon != nullptr)
	{
		D3DXMATRIX mtxParts = m_pMyWeapon->GetMtxWorld();
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 25.0f, 0.0f);

		D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(0.0f, 25.0f, 0.0f), &mtxParts);
		m_pAttack->SetPos(pos);
	}

	if (m_EAction == ATTACK_ACTION
		|| m_EAction == KNIFE_ATTACK_ACTION)
	{
		bool bCollision = m_pColliAttack->Collision(CObject::OBJETYPE_ENEMY, false);

		if (bCollision)
		{// �^�[�Q�b�g�̎擾
			CMotionEnemy *pTarget = (CMotionEnemy*)m_pColliAttack->GetCollidedObj();

			// �U���͂̐ݒ�
			int nAttack = FIST_ATTACK;

			if (m_pMyWeapon != nullptr)
			{// ����𑕔����Ă���
				nAttack = m_pMyWeapon->GetAttack();
			}

			// �U������
			pTarget->Hit(nAttack);
		}
	}
	

	// ���b�V���̓����蔻��
	CMesh3D *pMesh = CGame::GetMesh();
	pMesh->Collison(this);
	
	// �ʒu�̎擾
	pos = GetPos();
	D3DXVECTOR3 attackPos = m_pAttack->GetPos();

	// �f�o�b�N�\��
	CDebugProc::Print("�v���C���[�̈ʒu | X : %.3f | Y : %.3f | Z : %.3f |\n", pos.x, pos.y, pos.z);
	CDebugProc::Print("�v���C���[�̎�̈ʒu | X : %.3f | Y : %.3f | Z : %.3f |\n", attackPos.x, attackPos.y, attackPos.z);
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : 2D�`����s��
//=============================================================================
void CPlayer::Draw()
{
	// �`��
	CMotionModel3D::Draw();
}

//=============================================================================
// �ړ�
// Author : �������l
// �T�v : �L�[���͂ŕ��������߂āA�ړ��x�N�g�����Z�o����
//=============================================================================
D3DXVECTOR3 CPlayer::Move()
{
	// �ϐ��錾
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �L�[�{�[�h�̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// ���[�V�������̎擾
	CMotion *pMotion = CMotionModel3D::GetMotion();

	if (pKeyboard->GetPress(DIK_W)
		|| pKeyboard->GetPress(DIK_A)
		|| pKeyboard->GetPress(DIK_D)
		|| pKeyboard->GetPress(DIK_S))
	{// �ړ��L�[�������ꂽ
		if (pKeyboard->GetPress(DIK_W))
		{// [W]�L�[�������ꂽ��
			if (pKeyboard->GetPress(DIK_A))
			{// [A]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * -0.25f;
			}
			else if (pKeyboard->GetPress(DIK_D))
			{// [D]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * 0.25f;
			}
			else
			{// �ړ������̍X�V
				m_rotDest.y = D3DX_PI * 0.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_S))
		{// [S]�L�[�������ꂽ��
			if (pKeyboard->GetPress(DIK_A))
			{// [A]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * -0.75f;
			}
			else if (pKeyboard->GetPress(DIK_D))
			{// [D]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * 0.75f;
			}
			else
			{// �ړ������̍X�Vq
				m_rotDest.y = D3DX_PI;
			}
		}
		else if (pKeyboard->GetPress(DIK_A))
		{// [A]�L�[�������ꂽ��
		 // �ړ������̍X�V
			m_rotDest.y = D3DX_PI * -0.5f;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{// [D]�L�[�������ꂽ��
		 // �ړ������̍X�V
			m_rotDest.y = D3DX_PI * 0.5f;
		}

		// �J�������̎擾
		CCamera *pCamera = CApplication::GetCamera();

		// �ړ������̎Z�o
		m_rotDest.y += pCamera->GetRot().y;

		// �ړ������̐��K��
		m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);

		// �ړ��ʂ̌v�Z
		move = D3DXVECTOR3(sinf(m_rotDest.y), 0.0f, cosf(m_rotDest.y));

		// �p�x�̐��K��
		m_rotDest.y -= D3DX_PI;

		if (m_EAction == NEUTRAL_ACTION
			|| m_EAction == KNIFE_NEUTRAL_ACTION)
		{// �ړ�
			if (m_pMyWeapon == nullptr)
			{
				m_EAction = MOVE_ACTION;
			}
			else
			{
				switch (m_pMyWeapon->GetWeaponType())
				{
				case CWeaponObj::WEAPONTYPE_KNIFE:
					m_EAction = KNIFE_MOVE_ACTION;
					break;

				default:
					assert(false);
					break;
				}
			}

			if (pMotion != nullptr)
			{
				pMotion->SetNumMotion(m_EAction);
			}
		}
	}

	// �ړ����̌v�Z
	m_pMove->Moving(move);

	// �ړ����̎擾
	D3DXVECTOR3 moveing = m_pMove->GetMove();

	if (sqrtf((moveing.x * moveing.x) + (moveing.z * moveing.z)) <= 0.0f
		&& pMotion != nullptr
		&& (m_EAction == MOVE_ACTION
		|| m_EAction == KNIFE_MOVE_ACTION))
	{
		if (m_pMyWeapon == nullptr)
		{
			m_EAction = NEUTRAL_ACTION;
		}
		else
		{
			switch (m_pMyWeapon->GetWeaponType())
			{
			case CWeaponObj::WEAPONTYPE_KNIFE:
				m_EAction = KNIFE_NEUTRAL_ACTION;
				break;

			default:
				assert(false);
				break;
			}
		}

		pMotion->SetNumMotion(m_EAction);
	}

	// �f�o�b�N�\��
	CDebugProc::Print("�ړ��x�N�g�� : %.3f\n", m_pMove->GetMoveLength());

	// �����̎擾
	D3DXVECTOR3 rot = GetRot();

	// �ړI�̌����̕␳
	if (m_rotDest.y - rot.y >= D3DX_PI)
	{// �ړ������̐��K��
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - rot.y <= -D3DX_PI)
	{// �ړ������̐��K��
		m_rotDest.y += D3DX_PI * 2;
	}

	return moveing;
}

//=============================================================================
// ��]
// Author : �������l
// �T�v : �ړI�̌����܂ŉ�]����
//=============================================================================
void CPlayer::Rotate()
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

//=============================================================================
// ����̓���
// Author : �������l
// �T�v : ���݂̕�����̂ĂāA�V����������擾����
//=============================================================================
void CPlayer::GetWeapon()
{
	// ���[�V�������̎擾
	CMotion *pMotion = CMotionModel3D::GetMotion();

	// ��̃p�[�c�̎擾
	CParts *pHand = pMotion->GetParts(m_nNumHandParts);
	CCollision_Rectangle3D *pWeaponCollision = nullptr;

	if (m_pMyWeapon != nullptr)
	{// �����Ă�������̑�������
		m_pMyWeapon->SetParent();
		pWeaponCollision = m_pMyWeapon->GetCollision();
		pWeaponCollision->SetUseFlag(true);
	}

	// ����̑���
	CWeaponObj *pWeapon = (CWeaponObj*)m_pCollisionRectangle3D->GetCollidedObj();
	pWeapon->SetParent(pHand);
	m_pMyWeapon = pWeapon;
	m_pMyWeapon->SetPosOffset(D3DXVECTOR3(3.0f, 0.0f, 0.0f));
	m_pMyWeapon->SetRotOffset(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	pWeaponCollision = m_pMyWeapon->GetCollision();
	pWeaponCollision->SetUseFlag(false);

	if (m_EAction == NEUTRAL_ACTION
		|| m_EAction == KNIFE_NEUTRAL_ACTION)
	{
		switch (m_pMyWeapon->GetWeaponType())
		{
		case CWeaponObj::WEAPONTYPE_KNIFE:
			m_EAction = KNIFE_NEUTRAL_ACTION;
			break;

		default:
			assert(false);
			break;
		}

		pMotion->SetNumMotion(m_EAction);
	}
}


