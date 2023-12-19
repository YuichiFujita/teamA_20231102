//============================================================
//
//	�p�[�e�B�N��3D�w�b�_�[ [orbitalParticle.h]
//	Author�F�O�엳�V��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _orbitalParticle_H_
#define _orbitalParticle_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �p�[�e�B�N��3D�N���X
class CorbitalParticle : CObject
{
public:
	// �R���X�g���N�^
	CorbitalParticle();

	// �f�X�g���N�^
	~CorbitalParticle();



	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	static CorbitalParticle * Create(D3DXVECTOR3 pos, D3DXVECTOR3 Offset, D3DXCOLOR col, D3DXVECTOR3 range, D3DXVECTOR3 rot, D3DXVECTOR3 forth, int nLife, int nSpeed,int nEffectLife, int nLength, int nNumSpawn, float fAngle = 1.0f, float fAttenuation = 0.9f);
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��


	// �����o�ϐ�
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_range;	// �͈�
	D3DXVECTOR3 m_rot;		// ����
	D3DXVECTOR3 m_Offset;	// �傫��
	D3DXVECTOR3 m_Forth;	// �ʒu
	D3DXCOLOR m_col;		// �F
	int  m_nLife;			// ����
	int m_nEffectLife;
	int m_nLength;
	int m_nSpeed;			// ���x
	int m_nNumSpawn;		// �o����
	float m_fAngle;			// �p�x
	float m_fAttenuation; // ������
	int m_nCount;
};

#endif	// _orbitalParticle_H_
