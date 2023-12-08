//============================================================
//
//	�n�Տ��� [ground.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "ground.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define GROUND_PRIO	(1)	// �n�Ղ̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CGround::mc_apTextureFile[][6] =	// �e�N�X�`���萔
{
	{ // �����e�N�X�`��
		"data\\TEXTURE\\soil2.png",	// ���e�N�X�`��
		"data\\TEXTURE\\soil2.png",	// �E�e�N�X�`��
		"data\\TEXTURE\\soil2.png",	// ���e�N�X�`��
		"data\\TEXTURE\\soil.png",	// ��e�N�X�`��
		"data\\TEXTURE\\soil2.png",	// �O�e�N�X�`��
		"data\\TEXTURE\\soil2.png",	// ��e�N�X�`��
	},

	{ // �΃e�N�X�`��
		"data\\TEXTURE\\block000.png",	// ���e�N�X�`��
		"data\\TEXTURE\\block000.png",	// �E�e�N�X�`��
		"data\\TEXTURE\\block000.png",	// ���e�N�X�`��
		"data\\TEXTURE\\block000.png",	// ��e�N�X�`��
		"data\\TEXTURE\\block000.png",	// �O�e�N�X�`��
		"data\\TEXTURE\\block000.png",	// ��e�N�X�`��
	},
};

//************************************************************
//	�q�N���X [CGround] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGround::CGround() : CObjectMeshCube(CObject::LABEL_GROUND, GROUND_PRIO)
{
	// �����o�ϐ����N���A
	m_type = TYPE_GRASS;	// ���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CGround::~CGround()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGround::Init(void)
{
	// �����o�ϐ���������
	m_type = TYPE_GRASS;	// ���

	// �I�u�W�F�N�g���b�V���L���[�u�̏�����
	if (FAILED(CObjectMeshCube::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���_��ݒ�
	SetOrigin(CObjectMeshCube::ORIGIN_UP);

	// �L���[�u�F��ݒ�
	SetColor(XCOL_WHITE);

	// �����F��ݒ�
	SetBorderColor(XCOL_WHITE);

	// �����̏�Ԃ�ݒ�
	if (FAILED(SetBorderState(CObjectMeshCube::BORDER_OFF)))
	{ // �������̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����̑�����ݒ�
	SetBorderThick(0.0f);

	// �e�N�X�`���̏�Ԃ�ݒ�
	SetTextureState(CObjectMeshCube::TEXSTATE_SELECT);

	// �J�����O��ݒ�
	SetCulling(D3DCULL_CCW);

	// ���C�e�B���O��ݒ�
	SetLighting(true);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGround::Uninit(void)
{
	// �I�u�W�F�N�g���b�V���L���[�u�̏I��
	CObjectMeshCube::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGround::Update(void)
{
	// �I�u�W�F�N�g���b�V���L���[�u�̍X�V
	CObjectMeshCube::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CGround::Draw(void)
{
	// �I�u�W�F�N�g���b�V���L���[�u�̕`��
	CObjectMeshCube::Draw();
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CGround::SetType(const int nType)
{
	// �ϐ���錾
	SFaceTex faceTex;	// �e�N�X�`���C���f�b�N�X�ݒ�p

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^
	if (pTexture == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return;
	}

	if (nType > NONE_IDX && nType < TYPE_MAX)
	{ // ��ނ��͈͓��̏ꍇ

		// �����̎�ނ�ݒ�
		m_type = (EType)nType;

		// �����̎�ނ̃e�N�X�`����o�^
		faceTex = SFaceTex
		( // ����
			pTexture->Regist(mc_apTextureFile[nType][0]),	// ��
			pTexture->Regist(mc_apTextureFile[nType][1]),	// �E
			pTexture->Regist(mc_apTextureFile[nType][2]),	// ��
			pTexture->Regist(mc_apTextureFile[nType][3]),	// ��
			pTexture->Regist(mc_apTextureFile[nType][4]),	// �O
			pTexture->Regist(mc_apTextureFile[nType][5])	// ��
		);
		BindTexture(faceTex);	// �e�N�X�`��������
	}
	else { assert(false); }	// ��ރI�[�o�[
}

//============================================================
//	��ގ擾����
//============================================================
int CGround::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//============================================================
//	��������
//============================================================
CGround *CGround::Create
(
	const EType type,				// ���
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rRot,		// ����
	const D3DXVECTOR3& rSize,		// �傫��
	const D3DXVECTOR2& rTexPartX,	// �e�N�X�`��������X
	const D3DXVECTOR2& rTexPartY,	// �e�N�X�`��������Y
	const D3DXVECTOR2& rTexPartZ	// �e�N�X�`��������Z
)
{
	// �|�C���^��錾
	CGround *pGround = NULL;	// �n�Ր����p

	if (pGround == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pGround = new CGround;	// �n��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pGround != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �n�Ղ̏�����
		if (FAILED(pGround->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pGround;
			pGround = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// ��ނ�ݒ�
		pGround->SetType(type);

		// �ʒu��ݒ�
		pGround->SetVec3Position(rPos);

		// ������ݒ�
		pGround->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pGround->SetVec3Sizing(rSize);
		int nSprit;
		// �e�N�X�`���̕�����X��ݒ�
		if (type == TYPE_GRASS)
		{
			nSprit = 5;
		}
		else
		{
			nSprit = 2;
		}
		pGround->SetTexturePatternX(rTexPartX / nSprit);

		// �e�N�X�`���̕�����Y��ݒ�
		pGround->SetTexturePatternY(rTexPartY / nSprit);

		// �e�N�X�`���̕�����Z��ݒ�
		pGround->SetTexturePatternZ(rTexPartZ / nSprit);

		// �m�ۂ����A�h���X��Ԃ�
		return pGround;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
