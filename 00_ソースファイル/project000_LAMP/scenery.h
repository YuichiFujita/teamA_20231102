//============================================================
//
//	�i�F�w�b�_�[ [scenery.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENERY_H_
#define _SCENERY_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectMeshCylinder.h"

//************************************************************
//	�N���X��`
//************************************************************
// �i�F�N���X
class CScenery : public CObjectMeshCylinder
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_BILL_WHITE = 0,	// �r���X�e�N�X�`�� (���F)
		TEXTURE_BILL_LIGHTBLUE,	// �r���X�e�N�X�`�� (���F)
		TEXTURE_BILL_BLUE,		// �r���X�e�N�X�`�� (�F)
		TEXTURE_MAX				// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CScenery();

	// �f�X�g���N�^
	~CScenery();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CScenery *Create	// ����
	( // ����
		const ETexture texture,		// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const float fRadius,		// ���a
		const float fHeight			// �c��
	);

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔
};

#endif	// _SCENERY_H_
