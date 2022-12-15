//=============================================================================
//
// 3D���f���N���X(model3D.h)
// Author : �������l
// �T�v : 3D���f���������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <stdio.h>
#include <assert.h>

#include "parts.h"

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : 3D���f���𐶐�����
//=============================================================================
CParts * CParts::Create()
{
	// �I�u�W�F�N�g�C���X�^���X
	CParts *pParts = nullptr;

	// �������̉��
	pParts = new CParts;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pParts != nullptr);

	// ���l�̏�����
	pParts->Init();

	// �C���X�^���X��Ԃ�
	return pParts;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CParts::CParts()
{
	m_posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���̈ʒu
	m_rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���̌���
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړI�̈ʒu
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړI�̌���
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CParts::~CParts()
{

}