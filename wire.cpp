//=============================================================================
//
// ���C���[�N���X(wire.cpp)
// Author : �������l
// �T�v : ���C���[�������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>
#include <stdio.h>

#include "wire.h"
#include "application.h"
#include "renderer.h"
#include "move.h"
#include "line.h"
#include "collision_rectangle3D.h"
#include "motion_enemy.h"
#include "calculation.h"
#include "camera.h"
#include "debug_proc.h"

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : �C���X�^���X�𐶐�����
//=============================================================================
CWire *CWire::Create()
{
	// �I�u�W�F�N�g�C���X�^���X
	CWire *pWire = nullptr;

	// �������̉��
	pWire = new CWire;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pWire != nullptr);

	// ���l�̏�����
	pWire->Init();

	// �C���X�^���X��Ԃ�
	return pWire;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CWire::CWire() : m_pStart(nullptr),			// �X�^�[�g�n�_
m_pGoal(nullptr),							// �S�[���n�_
m_pMove(nullptr),							// �ړ����
m_pRoll(nullptr),							// ��]���
m_pLine(nullptr),							// ���C���[
m_EMode(MODE_STOP),							// ���C���[���[�h
m_ENextMode(MODE_STOP),						// ���̃��C���[���[�h
m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),		// �ړ�
m_moveVec(D3DXVECTOR3(0.0f,0.0f,0.0f)),		// �ړ��x�N�g��
m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),		// ����
m_rotDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	// �ړI�̌���
m_rotVec(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),	// �����x�N�g��
m_fLength(0.0f)								// ����
{

}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CWire::~CWire()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CWire::Init()
{
	// ���C���[�̏�����
	CModelObj::Init();

	// �I�u�W�F�N�g�̃^�C�v�ݒ�
	SetObjType(CObject::OBJETYPE_WIRE);

	// �����蔻��
	CCollision_Rectangle3D *m_pCollision = nullptr;

	// �X�^�[�g�n�_
	m_pStart = CModelObj::Create();
	m_pStart->SetObjType(CObject::OBJETYPE_WIRE);
	m_pCollision = m_pStart->GetCollision();
	m_pCollision->SetSize(D3DXVECTOR3(10.0f, 10.0f, 10.0f));

	// �S�[���n�_
	m_pGoal = CModelObj::Create();
	m_pGoal->SetObjType(CObject::OBJETYPE_WIRE);
	m_pCollision = m_pGoal->GetCollision();
	m_pCollision->SetSize(D3DXVECTOR3(10.0f, 10.0f, 10.0f));

	// ���C��
	m_pLine = CLine::Create();

	// �ړ��N���X�̃������m��
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(40.0f, 1000.0f, 0.0f, 0.01f);

	// ��]�N���X�̃������m��
	m_pRoll = new CMove;
	assert(m_pRoll != nullptr);
	m_pRoll->SetMoving(0.05f, 1.0f, 0.0f, 0.01f);

	return E_NOTIMPL;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CWire::Uninit()
{
	if (m_pStart != nullptr)
	{// �I��
		m_pStart->Uninit();
		m_pStart = nullptr;
	}

	if (m_pGoal != nullptr)
	{// �I��
		m_pGoal->Uninit();
		m_pGoal = nullptr;
	}

	if (m_pLine != nullptr)
	{// �I��
		m_pLine->Uninit();
		m_pLine = nullptr;
	}

	if (m_pMove != nullptr)
	{// �������̉��
		delete m_pMove;
		m_pMove = nullptr;
	}

	if (m_pRoll != nullptr)
	{// �������̉��
		delete m_pRoll;
		m_pRoll = nullptr;
	}

	// ���C���[�̏I��
	CModelObj::Uninit();

	// �I�u�W�F�N�g�̉��
	Release();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V���s��
//=============================================================================
void CWire::Update()
{
	if (m_EMode == MODE_HANGING)
	{
		Hanging();
	}
	else
	{// �ړ�
		Move();
	}

	// �����蔻��
	CCollision_Rectangle3D *pCollision = nullptr;
	m_ETargetObjType = CObject::OBJTYPE_NONE;

	switch (m_EMode)
	{
	case CWire::MODE_STOP:
		break;

	case CWire::MODE_FIRING:
		pCollision = m_pGoal->GetCollision();

		if (pCollision->Collision(CObject::OBJETYPE_ENEMY, false))
		{
			m_EMode = MODE_PULL;
			m_pMove->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			D3DXVECTOR3 vec = GetStart()->GetPos() - GetGoal()->GetPos();
			D3DXVec3Normalize(&vec, &vec);
			SetMoveVec(vec);

			// �ړ�
			Move();

			CMotionEnemy *pEnemy = (CMotionEnemy*)pCollision->GetCollidedObj();
			CMove *pMoveEnemy = pEnemy->GetMove();
			pMoveEnemy->SetMove(m_pMove->GetMove());
		}
		if (pCollision->Collision(CObject::OBJTYPE_3DMODEL, false))
		{
			m_EMode = MODE_STOP;
			m_ETargetObjType = OBJTYPE_3DMODEL;
		}

		break;

	case CWire::MODE_ATTRACT:
	case CWire::MODE_PULL:
		pCollision = m_pGoal->GetCollision();

		if (pCollision->Collision(CObject::OBJETYPE_WIRE, false))
		{
			m_EMode = MODE_STOP;
			SetPos(m_pStart->GetPos());
		}

		break;

	case CWire::MODE_HANGING:
		break;

	default:
		assert(false);
		break;
	}

	// ���C���[�̍X�V
	CModelObj::Update();
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : �`����s��
//=============================================================================
void CWire::Draw()
{
	// ���C���[�̕`��
	CModelObj::Draw();

	if (m_pLine != nullptr)
	{// ���C���̐ݒ�
		m_pLine->SetLine(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pStart->GetPos(), m_pGoal->GetPos(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//=============================================================================
// �ʒu�̐ݒ�
// Author : �������l
// �T�v : �ʒu��ݒ肷��
//=============================================================================
void CWire::SetPos(const D3DXVECTOR3 & pos)
{
	m_pStart->SetPos(pos);
	m_pGoal->SetPos(pos);
	CModelObj::SetPos(pos);
}

//=============================================================================
// �ړ�
// Author : �������l
// �T�v : �ړ����������肵
//=============================================================================
void CWire::Move()
{
	// �ʒu
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	switch (m_EMode)
	{
	case CWire::MODE_STOP:
		break;

	case CWire::MODE_FIRING:
	case CWire::MODE_PULL:
		pos = m_pGoal->GetPos();
		break;

	case CWire::MODE_ATTRACT:
		pos = m_pStart->GetPos();
		break;

	default:
		assert(false);
		break;
	}

	// �ړ����
	m_pMove->Moving(m_moveVec);
	pos += m_pMove->GetMove();

	switch (m_EMode)
	{
	case CWire::MODE_STOP:
		m_pMove->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;

	case CWire::MODE_FIRING:
	case CWire::MODE_PULL:
		m_pGoal->SetPos(pos);
		break;

	case CWire::MODE_ATTRACT:
		m_pStart->SetPos(pos);
		break;

	default:
		break;
	}

	// �ړ��x�N�g���̃��Z�b�g
	m_moveVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �Ԃ牺���菈��
// Author : �������l
// �T�v : 
//=============================================================================
void CWire::Hanging()
{
	// �ʒu
	D3DXVECTOR3 pos = m_pStart->GetPos();
	D3DXVECTOR3 posGoal = m_pGoal->GetPos();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �J�������̎擾
	CCamera *pCamera = CApplication::GetCamera();

	// �ړ������̎Z�o
	m_rotDest.y = pCamera->GetRot().y - D3DX_PI;

	// �ړ������̐��K��
	m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);

	// �ړI�̌����̕␳
	if (m_rotDest.y - m_rot.y >= D3DX_PI)
	{// �ړ������̐��K��
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y <= -D3DX_PI)
	{// �ړ������̐��K��
		m_rotDest.y += D3DX_PI * 2;
	}

	// ���K��
	m_rotDest.x = CCalculation::RotNormalization(m_rotDest.x);
	m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);
	m_rotDest.z = CCalculation::RotNormalization(m_rotDest.z);

	// �����̍X�V
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.1f;

	// �����̐��K��
	m_rot.y = CCalculation::RotNormalization(m_rot.y);

	// ���C�W���̌v�Z
	m_pRoll->Moving(m_rotVec);
	D3DXVECTOR3 rollDir = m_pRoll->GetMove();

	// ���K��
	rollDir.x = CCalculation::RotNormalization(rollDir.x);
	rollDir.y = CCalculation::RotNormalization(rollDir.y);
	rollDir.z = CCalculation::RotNormalization(rollDir.z);

	// ��]
	m_rot += rollDir;

	// ���K��
	m_rot.x = CCalculation::RotNormalization(m_rot.x);
	m_rot.y = CCalculation::RotNormalization(m_rot.y);
	m_rot.z = CCalculation::RotNormalization(m_rot.z);

	// �ړ������̐ݒ�
	pos.z = posGoal.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fLength;
	pos.x = posGoal.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fLength;
	pos.y = posGoal.y + cosf(m_rot.x) * m_fLength;

	// �ړ�
	pos += move;
	m_move = pos - m_pStart->GetPos();
	m_pStart->SetPos(pos);
	m_rotVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fLimit = 0.45f;

	if ((m_rot.x <= D3DX_PI * fLimit  && m_rot.x >= D3DX_PI * 0.0f)
		|| (m_rot.x >= D3DX_PI * -fLimit  && m_rot.x <= D3DX_PI * 0.0f))
	{
		m_EMode = MODE_STOP;
	}

	CDebugProc::Print("���C���[�̌��� : %.3f\n", m_rot.x);
}

//=============================================================================
// ���[�h�̐ݒ�
// Author : �������l
// �T�v : 
//=============================================================================
void CWire::SetWireMode(WIRE_MODE EWireMode)
{
	m_EMode = EWireMode;
	m_pMove->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
// �����̐ݒ�
// Author : �������l
// �T�v : 
//=============================================================================
void CWire::SetHanging()
{
	// �����x�N�g��
	D3DXVECTOR3 vec = GetStart()->GetPos() - GetGoal()->GetPos();

	// �����̐ݒ�
	m_rot.z = sqrtf((vec.x * vec.x) + (vec.z * vec.z));
	m_rot.x = atan2f(m_rot.z, vec.y);
	m_rot.y = atan2f(vec.x, vec.z);
	m_rot.z = 0.0f;

	// �����̐ݒ�
	m_fLength = sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

D3DXVECTOR3 CWire::HangingSearch()
{
	return D3DXVECTOR3();
}
