//============================================================
//
//	�u���b�N�w�b�_�[ [block.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BLOCK_H_
#define _BLOCK_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectMeshCube.h"

//************************************************************
//	�N���X��`
//************************************************************
// �u���b�N�N���X
class CBlock : public CObjectMeshCube
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_STONE = 0,	// �΃e�N�X�`��
		TYPE_BOX,		// ���e�N�X�`��
		TYPE_BRICK,		// �����K
		TYPE_CONCRETE,	// �R���N���[�g
		TYPE_ICE,		// �X
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �j���
	enum EBreak
	{
		BREAK_FALSE = 0,	// �j��OFF
		BREAK_TRUE,			// �j��ON
		BREAK_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CBlock();

	// �f�X�g���N�^
	~CBlock();

	// �X�e�[�^�X�\����
	struct SStatusInfo
	{
		EBreak state;	// �j���
		int nLife;		// �̗�
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void Hit(void) override;		// �q�b�g
	void SetType(const int nType) override;		// ��ސݒ�
	int GetType(void) const override;			// ��ގ擾
	void SetState(const int nState) override;	// �j��󋵐ݒ�
	int GetState(void) const override;			// �j��󋵎擾

	// �ÓI�����o�֐�
	static CBlock *Create	// ����
	( // ����
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rSize,	// �傫��
		const D3DXVECTOR2& rTexPartX = VEC2_ONE,	// �e�N�X�`��������X
		const D3DXVECTOR2& rTexPartY = VEC2_ONE,	// �e�N�X�`��������Y
		const D3DXVECTOR2& rTexPartZ = VEC2_ONE		// �e�N�X�`��������Z
	);
	static void LoadSetup(void);	// �Z�b�g�A�b�v

private:
	// �ÓI�����o�ϐ�
	static SStatusInfo m_aStatusInfo[TYPE_MAX];	// �X�e�[�^�X���
	static const char *mc_apTextureFile[][6];	// �e�N�X�`���萔

	// �����o�ϐ�
	SStatusInfo m_status;	// �X�e�[�^�X
	EType m_type;	// ���
	int m_nLife;	// �̗�
};

#endif	// _BLOCK_H_
