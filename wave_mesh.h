//=============================================================================
//
// �g���b�V���N���X(wave_mesh.h)
// Author : �������l
// �T�v : �g���b�V���������s��
//
//=============================================================================
#ifndef _WAVE_MESH_H_		// ���̃}�N����`������ĂȂ�������
#define _WAVE_MESH_H_		// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "mesh.h"
#include "texture.h"

//=============================================================================
// �g���b�V���N���X
// Author : �������l
// �T�v : �g���b�V���������s���N���X
//=============================================================================
class CWaveMesh : public CMesh3D
{
public:
	//--------------------------------------------------------------------
	// �ÓI�����o�֐�
	//--------------------------------------------------------------------
	static CWaveMesh *Create(void);				// �X�t�B�A�̐���

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CWaveMesh();
	~CWaveMesh();

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	void Update() override;											// �X�V
	void SetWave(const float fAddWave, const float fAmplitude);		// �g�̐ݒ�

private:
	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	void SetVtx() override;			// ���_���W�Ȃǂ̐ݒ�
	void Wave();					// �g

	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	float	*m_fVtxHeight;		// ���_�̍���
	float	m_fAddWave;			// �g�̒ǉ����̑傫��
	float	m_fAmplitude;		// �U��
};

#endif