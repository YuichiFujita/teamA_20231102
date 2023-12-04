//============================================================
//
//	�}�X�N�w�b�_�[ [mask.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MASK_H_
#define _MASK_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �}�X�N�N���X
class CMask : public CObject2D
{
public:
	// �R���X�g���N�^
	CMask();

	// �f�X�g���N�^
	~CMask();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CMask *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize,	// �傫��
		const D3DXCOLOR& rCol		// �F
	);

private:
};

#endif	// _MASK_H_
