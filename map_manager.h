//=============================================================================
//
// �}�b�v�}�l�[�W���[�N���X(map_manager.h)
// Author : �������l
// �T�v : �}�b�v�}�l�[�W���[�̔h�����s��
//
//=============================================================================
#ifndef _MAP_MANAGER_H_		// ���̃}�N����`������ĂȂ�������
#define _MAP_MANAGER_H_		// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "manager.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObject;
class CMesh3D;

//=============================================================================
// �}�l�[�W���[�N���X
// Author : �������l
// �T�v : �}�l�[�W���[�������s���N���X
//=============================================================================
class CMapManager : public CManager
{
public:
	//=============================================================================
	// ���[�h�񋓌^
	// Author : �������l
	// �T�v : ���[�h�̎��
	//=============================================================================
	enum EDIT_MODE
	{
		MODE_NONE = 0,				// ���[�h����
		MODE_MODEL,					// ���f���̐ݒ�
		MODE_MESH,					// ���b�V���̐ݒ�
		MAX_MODE,					// ���[�h�̍ő吔
	};

	//--------------------------------------------------------------------
	// �ÓI�����o�֐�
	//--------------------------------------------------------------------
	static CMapManager *Create();							// �C���X�^���X�̐���

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CMapManager();
	~CMapManager() override;

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init() override;			// ������
	void Uninit() override;				// �I��
	void Update() override;				// �X�V
	void Draw() override;				// �`��
	void SetMap(char *pFileName);		// �}�b�v�̓ǂݍ���

private:
	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	void LoadFieldMesh(char *pFileName);
	void LoadSkayBox(char *pFileName);
};

#endif