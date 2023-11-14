//============================================================
//
//	���f��UI�w�b�_�[ [flail.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FLAIL_H_
#define _FLAIL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	�N���X��`
//************************************************************
// ���f��UI�N���X
class CFlail : public CObjectModel
{
public:
	// �R���X�g���N�^
	CFlail();

	// �f�X�g���N�^
	~CFlail();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	void SetVec3Move(const D3DXVECTOR3& rMove);		// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Move(void);			// �ʒu�擾

	// �ÓI�����o�֐�
	static CFlail *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �傫��
	);

private:

	// �����o�ϐ�
	D3DXVECTOR3 m_move;			// �ړ���

};

#endif	// _FLAIL_H_
