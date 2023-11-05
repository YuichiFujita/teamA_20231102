//============================================================
//
//	�e���� [shadow.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shadow.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"
#include "field.h"
#include "stage.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SHADOW_PRIO	(2)		// �e�̗D�揇��
#define PLUS_POSY	(0.01f)	// ������h�~�p�̏c���W���Z��

#define MAX_DIS_HEIGHT	(200.0f)	// �e�Ɛe�̏c�̋����̍ő�l
#define MAX_PLUS_SIZE	(120.0f)	// �e�̑傫�����Z�ʂ̍ő�l

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CShadow::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\shadow000.jpg",	// �ʏ�e�N�X�`��
};

//************************************************************
//	�q�N���X [CShadow] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CShadow::CShadow(const float fMinAlpha, const float fMaxAlpha) : CObject3D(CObject::LABEL_NONE, SHADOW_PRIO), m_fMinAlpha(fMinAlpha), m_fMaxAlpha(fMaxAlpha)
{
	// �����o�ϐ����N���A
	m_pParentObject = NULL;	// �e�I�u�W�F�N�g
}

//============================================================
//	�f�X�g���N�^
//============================================================
CShadow::~CShadow()
{

}

//============================================================
//	����������
//============================================================
HRESULT CShadow::Init(void)
{
	// �����o�ϐ���������
	m_pParentObject = NULL;	// �e�I�u�W�F�N�g

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// Z�e�X�g�̐ݒ�
	SetFunc(D3DCMP_LESSEQUAL);

	// Z�o�b�t�@�̎g�p�󋵂̐ݒ�
	SetZEnable(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CShadow::Uninit(void)
{
	// �e�I�u�W�F�N�g�̍폜
	DeleteObjectParent();

	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CShadow::Update(void)
{
	// �`�����ݒ�
	if (FAILED(SetDrawInfo()))
	{ // �`����̐ݒ�Ɏ��s�����ꍇ

		// �I�u�W�F�N�g�̏I��
		Uninit();

		// �֐��𔲂���
		return;
	}

	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CShadow::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================
//	��������
//============================================================
CShadow *CShadow::Create
(
	const ETexture texture,		// ���
	const D3DXVECTOR3& rSize,	// �傫��
	CObject *pObject,			// �e�I�u�W�F�N�g
	const float fMinAlpha,		// �����x�̍ŏ��l
	const float fMaxAlpha		// �����x�̍ő�l
)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CShadow *pShadow = NULL;	// �e�����p

	if (pShadow == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pShadow = new CShadow(fMinAlpha, fMaxAlpha);	// �e
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pShadow != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �e�̏�����
		if (FAILED(pShadow->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return NULL;
		}

		// �e�N�X�`����o�^
		nTextureID = pTexture->Regist(mc_apTextureFile[texture]);

		// �e�N�X�`��������
		pShadow->BindTexture(nTextureID);

		// ���̑傫����ݒ�
		pShadow->SetScalingOrigin(rSize);

		// �e�I�u�W�F�N�g��ݒ�
		pShadow->SetParentObject(pObject);

		// �`�����ݒ�
		if (FAILED(pShadow->SetDrawInfo()))
		{ // �`����̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pShadow;
			pShadow = NULL;

			// ���s��Ԃ�
			assert(false);
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pShadow;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	���̑傫���̐ݒ菈��
//============================================================
void CShadow::SetScalingOrigin(const D3DXVECTOR3& rSize)
{
	// �����̑傫������
	m_sizeOrigin = rSize;

	// �傫���̐ݒ�
	CObject3D::SetVec3Sizing(rSize);
}

//============================================================
//	�`����̐ݒ菈��
//============================================================
HRESULT CShadow::SetDrawInfo(void)
{
	if (m_pParentObject != NULL)
	{ // �e�I�u�W�F�N�g���g�p����Ă����ꍇ

		// �ϐ���錾
		D3DXVECTOR3 posParent = m_pParentObject->GetVec3Position();	// �e�I�u�W�F�N�g�ʒu
		D3DXVECTOR3 posShadow  = VEC3_ZERO;	// �e�ʒu
		D3DXVECTOR3 sizeShadow = VEC3_ZERO;	// �e�傫��
		float fDis = 0.0f;		// �e�Ɛe�̋���
		float fAlpha = 0.0f;	// �e�̓����x

		// �`��ʒu�̐ݒ�
		posShadow = SetDrawPosition();

		// �e�Ɛe�̏c���W�̋��������߂�
		fDis = fabsf(posParent.y - posShadow.y);		// �c�̋��������߂�
		useful::LimitNum(fDis, 0.0f, MAX_DIS_HEIGHT);	// �c�̋����𐧌�
		fDis *= 1.0f / MAX_DIS_HEIGHT;					// ������������

		// �e�̑傫�������߂�
		sizeShadow = D3DXVECTOR3(m_sizeOrigin.x + (MAX_PLUS_SIZE * fDis), 0.0f, m_sizeOrigin.z + (MAX_PLUS_SIZE * fDis));

		// ���l�����߂�
		fAlpha = fabsf(fDis - 1.0f);	// ���l��ݒ�
		useful::LimitNum(fAlpha, m_fMinAlpha, m_fMaxAlpha);	// ���l�𐧌�

		// �e�̕`�����ݒ�
		SetVec3Position(posShadow);	// �ʒu
		SetVec3Sizing(sizeShadow);	// �傫��
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));	// �F

		// ������Ԃ�
		return S_OK;
	}
	else
	{ // �e�I�u�W�F�N�g���g�p����Ă��Ȃ������ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}
}

//============================================================
//	�`��ʒu�̐ݒ菈��
//============================================================
D3DXVECTOR3 CShadow::SetDrawPosition(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posParent = m_pParentObject->GetVec3Position();	// �e�I�u�W�F�N�g�ʒu
	D3DXVECTOR3 posShadow = VEC3_ZERO;	// �e�ʒu
	float fPosHeight = 0.0f;	// �r���̏���W

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();	// �X�e�[�W�̏��
	if (pStage == NULL)
	{ // �X�e�[�W�����݂��Ȃ��ꍇ

		// �֐��𔲂���
		return VEC3_ZERO;
	}

	// �e�̈ʒu�����߂�
	posShadow = posParent;	// �e�I�u�W�F�N�g�̍��W���

	if (pStage->IsFieldPositionRange(posParent))
	{ // �n�ʂ͈͓̔��̏ꍇ

		// ������n�ʂɐݒ�
		posShadow.y = pStage->GetFieldPositionHeight(posParent) + PLUS_POSY;
	}
	else
	{ // �S�Ă͈̔͊O�̏ꍇ

		// �����𐧌��ʒu�ɐݒ�
		posShadow.y = pStage->GetStageLimit().fField + PLUS_POSY;
	}

	// �e�ʒu��Ԃ�
	return posShadow;
}

//============================================================
//	�e�I�u�W�F�N�g�̐ݒ菈��
//============================================================
void CShadow::SetParentObject(CObject *pObject)
{
	// �����̃I�u�W�F�N�g��ݒ�
	m_pParentObject = pObject;
}

//============================================================
//	�e�I�u�W�F�N�g�̍폜����
//============================================================
void CShadow::DeleteObjectParent(void)
{
	// �e�I�u�W�F�N�g��NULL�ɂ���
	m_pParentObject = NULL;
}
