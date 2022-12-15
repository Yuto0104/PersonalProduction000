//=============================================================================
//
// ���[�V�����G�l�~�[�N���X(motion_enemy.h)
// Author : �������l
// �T�v : ���[�V�����G�l�~�[�������s��
//
//=============================================================================
#ifndef _MOTION_ENEMY_H_			// ���̃}�N����`������ĂȂ�������
#define _MOTION_ENEMY_H_			// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// ���C�u�����[�����N
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")			// �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")		// [d3d9.lib]�̊g�����C�u����

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "d3dx9.h"							// �`�揈���ɕK�v
#include "motion_model3D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CMove;
class CCollision_Rectangle3D;

//=============================================================================
// ���[�V�����G�l�~�[�N���X
// Author : �������l
// �T�v : ���[�V�����G�l�~�[�������s���N���X
//=============================================================================
class CMotionEnemy : public CMotionModel3D
{
public:
	//--------------------------------------------------------------------
	// ���[�V�����G�l�~�[�̃A�N�V�����̗񋓌^
	//--------------------------------------------------------------------
	enum ACTION_TYPE
	{
		NEUTRAL_ACTION = 0,		// �j���[�g����
		MOVE_ACTION,			// �ړ�
		ATTACK_ACTION,			// �U��
		MAX_ACTION,				// �ő吔
	};

	//--------------------------------------------------------------------
	// �ÓI�����o�֐�
	//--------------------------------------------------------------------
	static CMotionEnemy *Create();			// ���[�V�����G�l�~�[�̐���

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CMotionEnemy();
	~CMotionEnemy();

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init() override;										// ������
	void Uninit() override;											// �I��
	void Update() override;											// �X�V
	void Draw() override;											// �`��
	void SetSpeed(const float fSpeed) { m_fSpeed = fSpeed; }		// �ړ����x�̐ݒ�
	
private:
	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	D3DXVECTOR3 Move();		// �ړ�
	void Rotate();			// ��]
		
	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	CMove						*m_pMove;				// �ړ����
	CCollision_Rectangle3D		*m_pCollRectangle3D;	// 3D��`�̓����蔻��
	ACTION_TYPE					m_EAction;				// �A�N�V�����^�C�v
	D3DXVECTOR3					m_move;					// �ړ��x�N�g��
	D3DXVECTOR3					m_rotDest;				// �ړI�̌���
	float						m_fSpeed;				// �ړ����x	
	int							m_nNumMotion;			// ���݂̃��[�V�����ԍ�
};

#endif

