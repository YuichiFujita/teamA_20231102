//============================================================
//
//	�G�t�F�N�g3D�w�b�_�[ [OrbitalEffect.h]
//	Author�FTANNO RYUNOSUKE
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OrbitalEffect_H_
#define _OrbitalEffect_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectOrbit.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�t�F�N�g3D�N���X
class COrbitalEffect : public CObject
{
public:
	

	// �R���X�g���N�^
	COrbitalEffect();

	// �f�X�g���N�^
	~COrbitalEffect();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

									// �ÓI�����o�֐�
	static COrbitalEffect *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& Offset,	// �傫��
		const D3DXCOLOR col,

		const int nLife = 1,					// ����
		const D3DXVECTOR3& rMove = VEC3_ZERO,	// �ړ���
		const float fAttenuation = 0.9f,
		const bool bAdd = true,
		const bool bGravity = false,
		const CObject::ELabel label = LABEL_EFFECT	// �I�u�W�F�N�g���x��
	);

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔
	CObjectOrbit * m_pOrbit;	//�I�[�r�b�g
	D3DXMATRIX m_Mtx;
							// �����o�ϐ�
	D3DXVECTOR3 m_pos;		//���W
	D3DXVECTOR3 m_move;		// �ړ���
	int		m_nLife;		// ����
	float	m_fOffset;		//�T�C�Y
	float	m_fAttenuation; //�ړ��ʌ�����
	bool	m_bAdd;			// ���Z������
	bool	m_bGravity;		//�d��
};

#endif	// _OrbitalEffect_H_
