//============================================================
//
//	�p�[�e�B�N��3D�w�b�_�[ [particle3D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MODELPARTICLE_H_
#define _MODELPARTICLE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "Object.h"

//<************************************************************
//�N���X�錾
//<************************************************************
class CModParticle :public CObject
{
public:

	CModParticle();
	~CModParticle() {}

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;					// ������
	void Uninit(void) override { Release(); };		// �I��
	void Update(void) override;						// �X�V
	void Draw(void) override {};					// �`��

	static CModParticle *Create(const D3DXVECTOR3 rPos);

private:

	// �I�[�o�[���C�h�֐�
	void Release(void) override { CObject::Release(); }	// �j��
	void Set(void);


	// �����o�ϐ�
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_move;	// �ړ���
	D3DXVECTOR3 m_rot;	// ����
	D3DXCOLOR m_col;	// �F
	int  m_nLife;		// ����

};

#endif
