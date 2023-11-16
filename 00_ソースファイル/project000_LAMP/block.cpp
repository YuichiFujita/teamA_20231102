//============================================================
//
//	�u���b�N���� [block.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "block.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define BLOCK_PRIO	(1)	// �u���b�N�̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CBlock::mc_apTextureFile[][6] =	// �e�N�X�`���萔
{
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
//	�q�N���X [CBlock] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBlock::CBlock() : CObjectMeshCube(CObject::LABEL_BLOCK, BLOCK_PRIO)
{
	// �����o�ϐ����N���A
	m_type = TYPE_STONE;	// ���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CBlock::~CBlock()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBlock::Init(void)
{
	// �����o�ϐ���������
	m_type = TYPE_STONE;	// ���

	// �I�u�W�F�N�g���b�V���L���[�u�̏�����
	if (FAILED(CObjectMeshCube::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���_��ݒ�
	SetOrigin(CObjectMeshCube::ORIGIN_DOWN);

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

	// �e�N�X�`���̕�����X��ݒ�
	SetTexturePatternX(VEC2_ONE);

	// �e�N�X�`���̕�����Y��ݒ�
	SetTexturePatternY(VEC2_ONE);

	// �e�N�X�`���̕�����Z��ݒ�
	SetTexturePatternZ(VEC2_ONE);

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
void CBlock::Uninit(void)
{
	// �I�u�W�F�N�g���b�V���L���[�u�̏I��
	CObjectMeshCube::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CBlock::Update(void)
{
	// �I�u�W�F�N�g���b�V���L���[�u�̍X�V
	CObjectMeshCube::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CBlock::Draw(void)
{
	// �I�u�W�F�N�g���b�V���L���[�u�̕`��
	CObjectMeshCube::Draw();
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CBlock::SetType(const int nType)
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
int CBlock::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//============================================================
//	��������
//============================================================
CBlock *CBlock::Create
(
	const EType type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR3& rSize	// �傫��
)
{
	// �|�C���^��錾
	CBlock *pBlock = NULL;	// �u���b�N�����p

	if (pBlock == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pBlock = new CBlock;	// �u���b�N
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pBlock != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �u���b�N�̏�����
		if (FAILED(pBlock->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pBlock;
			pBlock = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// ��ނ�ݒ�
		pBlock->SetType(type);

		// �ʒu��ݒ�
		pBlock->SetVec3Position(rPos);

		// ������ݒ�
		pBlock->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pBlock->SetVec3Sizing(rSize);

		// �m�ۂ����A�h���X��Ԃ�
		return pBlock;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
