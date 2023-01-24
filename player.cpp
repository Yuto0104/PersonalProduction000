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
#include "mouse.h"
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
#include "wire.h"

//--------------------------------------------------------------------
// �ÓI�����o�ϐ��̒�`
//--------------------------------------------------------------------
const float CPlayer::fSPEED = 1.0f;
const float CPlayer::fJAMP = 30.0f;

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
m_pWire(nullptr),
m_EAction(NEUTRAL_ACTION),
m_rotDest(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_fSpeed(0.0f),
m_fGravity(0.0f),
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
	m_pMove->SetMoving(fSPEED, 100.0f, 0.5f, 0.1f);

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

	// ���C���[
	m_pWire = CWire::Create();

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

	if (m_pWire != nullptr)
	{// �I������
		m_pWire->Uninit();
		m_pWire = nullptr;
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

	// �}�E�X�̎擾
	CMouse *pMouse = CApplication::GetMouse();

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
	
	if (pMouse->GetTrigger(CMouse::MOUSE_KEY_LEFT))
	{// �U��
		Attack();
	}

	if (pKeyboard->GetTrigger(DIK_SPACE))
	{// �W�����v
		Jump();
	}

	if (m_pWire->GetTargetObjType() == CObject::OBJTYPE_3DMODEL)
	{
		m_pWire->SetWireMode(CWire::MODE_HANGING);
		m_pWire->SetHanging();
		m_pWire->SetRotVec(D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	}

	if (pKeyboard->GetTrigger(DIK_F)
		&& m_pWire->GetWireMode() == CWire::MODE_STOP)
	{// ���C���[�̎ˏo
		m_pWire->SetWireMode(CWire::MODE_FIRING);
		D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 rot = GetRot();
		rot.x = CCalculation::RotNormalization(rot.x + D3DX_PI * -0.5f);
		vec.z = sinf(rot.x) * cosf(rot.y);
		vec.x = sinf(rot.x) * sinf(rot.y);
		vec.y = cosf(rot.x);
		D3DXVec3Normalize(&vec, &vec);
		m_pWire->SetMoveVec(vec);
	}
	else if (pKeyboard->GetTrigger(DIK_F)
		&& m_pWire->GetWireMode() == CWire::MODE_FIRING)
	{
		m_pWire->SetWireMode(CWire::MODE_ATTRACT);
		D3DXVECTOR3 vec = m_pWire->GetStart()->GetPos() - m_pWire->GetGoal()->GetPos();
		m_pWire->SetMoveVec(-vec);
		m_pMove->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// �d�͂̐ݒ�
	m_fGravity -= CCalculation::Gravity();

	if (m_pWire->GetWireMode() != CWire::MODE_ATTRACT
		&& m_pWire->GetWireMode() != CWire::MODE_HANGING)
	{// �ړ�
		pos.y += m_fGravity;
		pos += Move();
	}
	else if (m_pWire->GetWireMode() == CWire::MODE_HANGING)
	{
		// �J�������̎擾
		CCamera *pCamera = CApplication::GetCamera();

		// �ړ������̎Z�o
		m_rotDest.y = pCamera->GetRot().y - D3DX_PI;

		// �ړ������̐��K��
		m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);

		// �ړI�̌����̕␳
		if (m_rotDest.y - rot.y >= D3DX_PI)
		{// �ړ������̐��K��
			m_rotDest.y -= D3DX_PI * 2;
		}
		else if (m_rotDest.y - rot.y <= -D3DX_PI)
		{// �ړ������̐��K��
			m_rotDest.y += D3DX_PI * 2;
		}

		m_fGravity = 0.0f;

		D3DXVECTOR3 move = m_pMove->GetMove();
		move.y = 0.0f;
		m_pMove->SetMove(move);

		pos += m_pWire->GetMoveing();
	}
	else
	{// �ړ�
		pos += m_pWire->GetMove()->GetMove();
	}

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

	if (m_pCollisionRectangle3D->Collision(CObject::OBJTYPE_3DMODEL, true))
	{
		CCollision_Rectangle3D::EState state = m_pCollisionRectangle3D->GetState();

		if (state == CCollision_Rectangle3D::STATE_Y)
		{
			m_fGravity = 0.0f;

			D3DXVECTOR3 move = m_pMove->GetMove();
			move.y = 0.0f;
			m_pMove->SetMove(move);
		}

		if ((state == CCollision_Rectangle3D::STATE_X
			|| state == CCollision_Rectangle3D::STATE_Y
			|| state == CCollision_Rectangle3D::STATE_Z)
			&& m_pWire->GetWireMode() == CWire::MODE_HANGING)
		{
			m_pWire->SetWireMode(CWire::MODE_STOP);
		}
	}

	m_pCollisionRectangle3D->Collision(CObject::OBJTYPE_3DMODEL);

	// ����Ƃ̓����蔻��
	bool bCollisionWeapon = m_pCollisionRectangle3D->Collision(CObject::OBJETYPE_WEAPON, false);

	if (bCollisionWeapon
		&& pKeyboard->GetTrigger(DIK_E))
	{// ����̎擾
		GetWeapon();
	}

	if (pMotion != nullptr)
	{// ��̃I�u�W�F�N�g�̈ʒu
		CParts *pHand = pMotion->GetParts(m_nNumHandParts);
		D3DXMATRIX mtxParts = pHand->GetMtxWorld();
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVec3TransformCoord(&pos, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &mtxParts);

		if (m_pMyWeapon == nullptr)
		{
			m_pAttack->SetPos(pos);
		}

		switch (m_pWire->GetWireMode())
		{
		case CWire::MODE_STOP:
			// ���C���[�̈ʒu�̐ݒ�
			m_pWire->SetPos(pos);
			break;

		case CWire::MODE_FIRING:
		case CWire::MODE_ATTRACT:
			m_pWire->GetStart()->SetPos(pos);
			break;

		case CWire::MODE_PULL:
			break;

		default:
			break;
		}
	}
	
	if (pMotion != nullptr
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
			int nAttack = nFIST_ATTACK;

			if (m_pMyWeapon != nullptr)
			{// ����𑕔����Ă���
				nAttack = m_pMyWeapon->GetAttack();
			}

			// �U������
			pTarget->Hit(nAttack);
		}
	}

	// ���b�V���̓����蔻��
	if (CMesh3D::CollisonMesh(this))
	{
		m_fGravity = 0.0f;

		D3DXVECTOR3 move = m_pMove->GetMove();
		move.y = 0.0f;
		m_pMove->SetMove(move);
	}
	
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

		// �ړ������̐��K��
		m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);

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
// �W�����v
// Author : �������l
// �T�v : �W�����v����
//=============================================================================
void CPlayer::Jump()
{
	// �ړ����̌v�Z
	D3DXVECTOR3 move = m_pMove->GetMove();
	move.y = 0.0f;
	m_fGravity = 0.0f;
	m_pMove->SetMove(move);
	m_pMove->SetSpeed(fJAMP);
	m_pMove->Moving(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pMove->SetSpeed(fSPEED);
}

//=============================================================================
// �U��
// Author : �������l
// �T�v : �U������
//=============================================================================
void CPlayer::Attack()
{
	// ���[�V�������̎擾
	CMotion *pMotion = CMotionModel3D::GetMotion();

	if (pMotion != nullptr)
	{
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


