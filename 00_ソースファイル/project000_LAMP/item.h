//============================================================
//
//	�A�C�e���X�|�[���w�b�_�[ [itemspawn.h]
//	Author�F�X��x��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ITEMSPAWN_H_
#define _ITEMSPAWN_H_
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectmodel.h"

//************************************************************
//	�N���X��`
//************************************************************
// �A�C�e���N���X
class CItem : public CObjectModel
{
public:
	// �R���X�g���N�^
	CItem();

	// �f�X�g���N�^
	~CItem();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	//�����o�֐�
	void Collision();

	static CItem *Create(D3DXVECTOR3 pos);

private:
	// �ÓI�����o�ϐ�
	static const char *ModelFileName;

	// �����o�ϐ�
};

#endif
