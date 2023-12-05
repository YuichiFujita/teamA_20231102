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
COrbitalEffect::COrbitalEffect() : CObject()
{
	// �����o�ϐ����N���A
	m_move = VEC3_ZERO;		// �ړ���
	m_nLife = 0;		// ����
	m_bGravity = false;		// �傫���̌��Z��
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
	
}

//============================================================
//	�X�V����
//============================================================
void COrbitalEffect::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu
	D3DXVECTOR3 rot = GetVec3Rotation();	// ����
	D3DXVECTOR3 size = GetVec3Sizing();		// �傫��
	D3DXCOLOR   col = GetColor();			// �F
	float fRadius = size.x;				// ���a

	if (m_nLife <= 0		// �������}����
		|| fRadius <= 0.0f)	// ���a��0.0f�ȉ�
	{ // ��L�̂ǂꂩ�ɂȂ����ꍇ

	  // �I�u�W�F�N�g��j��
		Uninit();

		// �֐��𔲂���
		return;
	}

	// �ړ��ʂ����Z
	pos += m_move;

	// ���������Z
	m_nLife--;


}

//============================================================
//	�`�揈��
//============================================================
void COrbitalEffect::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	if (m_bAdd)
	{ // ���Z������ON�̏ꍇ

	  // ���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	m_pOrbit->Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//
////============================================================
////	��������
////============================================================
//COrbitalEffect *COrbitalEffect::Create
//(
//	 D3DXVECTOR3& rPos,	// �ʒu
//	D3DXCOLOR col,
//	 float fOffset,		// �傫��
//	 int nLife ,					// ����
//	 D3DXVECTOR3& rMove ,	// �ړ���
//	 float fAttenuation ,
//	 bool bAdd ,
//	 bool bGravity ,
//	 CObject::ELabel label 	// �I�u�W�F�N�g���x��
//)
//{
//	// �ϐ���錾
//	int nTextureID;	// �e�N�X�`���C���f�b�N�X
//
//	// �|�C���^��錾
//	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^
//	COrbitalEffect *pOrbitalEffect = NULL;	// �G�t�F�N�g3D�����p
//
//	if (pOrbitalEffect == NULL)
//	{ // �g�p����Ă��Ȃ��ꍇ
//
//	  // �������m��
//		pOrbitalEffect = new COrbitalEffect();	// �G�t�F�N�g3D
//	}
//	else { assert(false); return NULL; }	// �g�p��
//
//	if (pOrbitalEffect != NULL)
//	{ // �m�ۂɐ������Ă���ꍇ
//
//	  // �G�t�F�N�g3D�̏�����
//		if (FAILED(pOrbitalEffect->Init()))
//		{ // �������Ɏ��s�����ꍇ
//
//		  // �������J��
//			delete pOrbitalEffect;
//			pOrbitalEffect = NULL;
//
//			// ���s��Ԃ�
//			return NULL;
//		}
//
//	
//		// �ʒu��ݒ�
//		pOrbitalEffect->SetVec3Position(rPos);
//		// �����̏���ݒ�
//		pOrbitalEffect->m_move = rMove;		// �ړ���
//		pOrbitalEffect->m_nLife = nLife;		// ����
//		pOrbitalEffect->m_bAdd = bAdd;			// ���Z������
//		pOrbitalEffect->m_pOrbit = CObjectOrbit::Create(&pOrbitalEffect->m_Mtx, CObjectOrbit::SOffset(D3DXVECTOR3(rPos.x, rPos.y, rPos.z), D3DXVECTOR3(rPos.x, rPos.y, rPos.z), col, col));
//		pOrbitalEffect->m_pOrbit->SetEnableDraw(false);
//		// �m�ۂ����A�h���X��Ԃ�
//		return pOrbitalEffect;
//	}
//	else { assert(false); return NULL; }	// �m�ێ��s
//}

//============================================================
//	�j������
//============================================================
void COrbitalEffect::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}