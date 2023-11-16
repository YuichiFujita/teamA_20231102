//============================================================
//
//	�n�Ճw�b�_�[ [ground.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GROUND_H_
#define _GROUND_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectMeshCube.h"

//************************************************************
//	�N���X��`
//************************************************************
// �n�ՃN���X
class CGround : public CObjectMeshCube
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_GRASS = 0,	// �����e�N�X�`��
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CGround();

	// �f�X�g���N�^
	~CGround();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void SetType(const int nType) override;	// ��ސݒ�
	int GetType(void) const override;		// ��ގ擾

	// �ÓI�����o�֐�
	static CGround *Create	// ����
	( // ����
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rSize	// �傫��
	);

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[][6];	// �e�N�X�`���萔

	// �����o�ϐ�
	EType m_type;	// ���
};

#endif	// _GROUND_H_
