//============================================================
//
//	�i�F���� [scenery.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scenery.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SCENERY_PRIO	(0)	// �i�F�̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CScenery::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\Mountain0.png",	// �r���X�e�N�X�`�� (���F)
	"data\\TEXTURE\\Mountain1.png",	// �r���X�e�N�X�`�� (���F)
	"data\\TEXTURE\\Mountain2.png",	// �r���X�e�N�X�`�� (�F)
};

//************************************************************
//	�q�N���X [CScenery] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScenery::CScenery() : CObjectMeshCylinder(CObject::LABEL_SCENERY, SCENERY_PRIO)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CScenery::~CScenery()
{

}

//============================================================
//	����������
//============================================================
HRESULT CScenery::Init(void)
{
	// �I�u�W�F�N�g���b�V���V�����_�[�̏�����
	if (FAILED(CObjectMeshCylinder::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CScenery::Uninit(void)
{
	// �I�u�W�F�N�g���b�V���V�����_�[�̏I��
	CObjectMeshCylinder::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CScenery::Update(void)
{
	// �I�u�W�F�N�g���b�V���V�����_�[�̍X�V
	CObjectMeshCylinder::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CScenery::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 180);				// ���e�X�g�̎Q�ƒl�ݒ�

	// �I�u�W�F�N�g���b�V���V�����_�[�̕`��
	CObjectMeshCylinder::Draw();

	// ���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// ���e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// ���e�X�g�̎Q�ƒl�ݒ�
}

//============================================================
//	��������
//============================================================
CScenery *CScenery::Create
(
	const ETexture texture,		// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const float fRadius,		// ���a
	const float fHeight			// �c��
)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CScenery *pScenery = NULL;	// �i�F�����p

	if (pScenery == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pScenery = new CScenery;	// �i�F
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pScenery != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �i�F�̏�����
		if (FAILED(pScenery->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pScenery;
			pScenery = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`����o�^
		nTextureID = pTexture->Regist(mc_apTextureFile[texture]);

		// �e�N�X�`��������
		pScenery->BindTexture(nTextureID);

		// �ʒu��ݒ�
		pScenery->SetVec3Position(rPos);

		// ������ݒ�
		pScenery->SetVec3Rotation(rRot);

		// �F��ݒ�
		pScenery->SetColor(rCol);

		// ���a��ݒ�
		pScenery->SetRadius(fRadius);

		// �c����ݒ�
		pScenery->SetHeight(fHeight);

		// �J�����O��ݒ�
		pScenery->SetCulling(D3DCULL_CW);

		// ���C�e�B���O��ݒ�
		pScenery->SetLighting(false);

		// ��������ݒ�
		if (FAILED(pScenery->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pScenery;
			pScenery = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pScenery;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
