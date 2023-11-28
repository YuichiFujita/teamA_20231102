//============================================================
//
//	��w�b�_�[ [sky.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SKY_H_
#define _SKY_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectMeshDome.h"

//************************************************************
//	�N���X��`
//************************************************************
// ��N���X
class CSky : public CObjectMeshDome
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_NORMAL = 0,	// �ʏ�e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CSky();

	// �f�X�g���N�^
	~CSky();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CSky *Create	// ����
	( // ����
		const ETexture texture,		// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const float fRadius			// ���a
	);

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔
};

#endif	// _SKY_H_
