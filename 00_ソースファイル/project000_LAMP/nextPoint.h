//============================================================
//
//	�J�ڈʒu�w�b�_�[ [nextPoint.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _NEXTPOINT_H_
#define _NEXTPOINT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object3D.h"

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
class CObjectGauge3D;	// �I�u�W�F�N�g�Q�[�W3D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �J�ڈʒu�N���X
class CNextPoint : public CObject3D
{
public:
	// �R���X�g���N�^
	CNextPoint();

	// �f�X�g���N�^
	~CNextPoint();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CNextPoint *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize	// �傫��
	);

private:
	// �����o�֐�
	bool Collision(void);	// �v���C���[�S���Ƃ̓����蔻��

	// �����o�ϐ�
	CObjectGauge3D *m_pGauge;	// �ҋ@�Q�[�W���
};

#endif	// _NEXTPOINT_H_
