//============================================================
//
//	�G�t�F�N�g3D���� [OrbitalEffect.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "OrbitalEffect.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define EFF_PRIO	(9)	// �G�t�F�N�g3D�̗D�揇��
#define PART_PRIO	(9)	// �p�[�e�B�N��3D�̗D�揇��



//************************************************************
//	�q�N���X [COrbitalEffect] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
COrbitalEffect::COrbitalEffect() : CObject(LABEL_PARTICLE, PART_PRIO)
{
	// �����o�ϐ����N���A
	m_move = VEC3_ZERO;		// �ړ���
	m_nLife = 0;		// ����
	m_fOffset = 0.0f;		// �����x�̌��Z��
	m_bAdd = false;	// ���Z������

}



//============================================================
//	�f�X�g���N�^
//============================================================
COrbitalEffect::~COrbitalEffect()
{

}

//============================================================
//	����������
//============================================================
HRESULT COrbitalEffect::Init(void)
{
	// �����o�ϐ���������
	m_move = VEC3_ZERO;	// �ړ���
	m_nLife = 0;	// ����

	m_bAdd = true;	// ���Z������




	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void COrbitalEffect::Uninit(void)
{
	// �I�u�W�F�N�g�r���{�[�h�̏I��
	if (m_pOrbit != NULL)
	{
		(*m_pOrbit)->SetState(CObjectOrbit::STATE_VANISH);
		delete m_pOrbit;
		m_pOrbit = NULL;
		
	}
	Release();
}

//============================================================
//	�X�V����
//============================================================
void COrbitalEffect::Update(void)
{
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���N�X
	D3DXMATRIX mtxParent;

	D3DXVECTOR3 angles;

	// Yaw�i���[�j���v�Z
	angles.y = atan2(m_move.x, m_move.z);

	// Pitch�i�s�b�`�j���v�Z
	angles.x = atan2(m_move.y, sqrt(m_move.x * m_move.x + m_move.z * m_move.z));

	// Roll�i���[���j��0�x�ɐݒ�
	angles.z = 0.0f;



	//���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&m_Mtx);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, angles.y, angles.x, angles.z);
	D3DXMatrixMultiply(&m_Mtx, &m_Mtx, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_Mtx, &m_Mtx, &mtxTrans);


	// �ړ��ʂ����Z
	m_pos += m_move;
	m_move *= m_fAttenuation;
	
	m_move += m_Force;

	// ���������Z
	m_nLife--;

	if (m_nLife <= 0 || m_pos.y < -100)
	{
		Uninit();
	}
}

//============================================================
//	�`�揈��
//============================================================
void COrbitalEffect::Draw(void)
{

}

//============================================================
//	��������
//============================================================
COrbitalEffect *COrbitalEffect::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& Offset,	// �傫��
	const D3DXCOLOR col,
	const int nLife,					// ����
	const int nLength,
	const D3DXVECTOR3& rMove,	// �ړ���
	const D3DXVECTOR3& rFoce,
	const float fAttenuation,
	const bool bAdd ,
	const CObject::ELabel label	// �I�u�W�F�N�g���x��
)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^
	COrbitalEffect *pOrbitalEffect = NULL;	// �G�t�F�N�g3D�����p

	if (pOrbitalEffect == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

	  // �������m��
		pOrbitalEffect = new COrbitalEffect();	// �G�t�F�N�g3D
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pOrbitalEffect != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

	  // �G�t�F�N�g3D�̏�����
		if (FAILED(pOrbitalEffect->Init()))
		{ // �������Ɏ��s�����ꍇ

		  // �������J��
			delete pOrbitalEffect;
			pOrbitalEffect = NULL;

			// ���s��Ԃ�
			return NULL;
		}

	
		// �ʒu��ݒ�
		pOrbitalEffect->m_pos = rPos;
		pOrbitalEffect->m_fAttenuation = fAttenuation;
		// �����̏���ݒ�
		pOrbitalEffect->m_move = rMove;		// �ړ���
		pOrbitalEffect->m_nLife = nLife;		// ����
		pOrbitalEffect->m_bAdd = bAdd;			// ���Z������
		pOrbitalEffect->m_Force = rFoce;
												//���[���h�}�g���N�X�̏�����
		D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���N�X
		D3DXMATRIX mtxParent;

		D3DXVECTOR3 angles;


		//���[���h�}�g���N�X�̏�����
		D3DXMatrixIdentity(&pOrbitalEffect->m_Mtx);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pOrbitalEffect->m_pos.x, pOrbitalEffect->m_pos.y, pOrbitalEffect->m_pos.z);
		D3DXMatrixMultiply(&pOrbitalEffect->m_Mtx, &pOrbitalEffect->m_Mtx, &mtxTrans);
		pOrbitalEffect->m_pOrbit = new CObjectOrbit*;
		(*pOrbitalEffect->m_pOrbit) = CObjectOrbit::Create(&pOrbitalEffect->m_Mtx, CObjectOrbit::SOffset(Offset, -Offset, col),nLength);
		(*pOrbitalEffect->m_pOrbit)->SetPriority(0);
		(*pOrbitalEffect->m_pOrbit)->SetEnableDraw(true);
		// �m�ۂ����A�h���X��Ԃ�
		return pOrbitalEffect;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
void COrbitalEffect::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}