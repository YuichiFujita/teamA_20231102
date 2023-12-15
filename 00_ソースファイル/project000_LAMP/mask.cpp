//============================================================
//
//	�}�X�N���� [mask.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "mask.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY = 5;	// �}�X�N�̗D�揇��
}

//************************************************************
//	�q�N���X [CMask] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMask::CMask() : CObject2D(CObject::LABEL_UI)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CMask::~CMask()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMask::Init(void)
{
	// �I�u�W�F�N�g2D�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�̕\����OFF�ɂ���
	SetEnableDepthShadow(false);
	SetEnableZTex(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CMask::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CMask::Update(void)
{
	// �I�u�W�F�N�g2D�̍X�V
	CObject2D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CMask::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �X�e���V���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// ��r�Q�ƒl��ݒ肷��
	pDevice->SetRenderState(D3DRS_STENCILREF, 2);

	// �X�e���V���}�X�N���w�肷�� 
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	// �X�e���V����r�֐����w�肷��
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);

	// �X�e���V�����ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS,	D3DSTENCILOP_KEEP);	// Z�e�X�g�E�X�e���V���e�X�g����
	pDevice->SetRenderState(D3DRS_STENCILFAIL,	D3DSTENCILOP_KEEP);	// Z�e�X�g�E�X�e���V���e�X�g���s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL,	D3DSTENCILOP_KEEP);	// Z�e�X�g���s�E�X�e���V���e�X�g����

	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw();

	// �X�e���V���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//============================================================
//	��������
//============================================================
CMask *CMask::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize,	// �傫��
	const D3DXCOLOR& rCol		// �F
)
{
	// �|�C���^��錾
	CMask *pMask = NULL;	// �}�X�N�����p

	if (pMask == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pMask = new CMask;	// �}�X�N
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pMask != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �}�X�N�̏�����
		if (FAILED(pMask->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pMask;
			pMask = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pMask->SetVec3Position(rPos);

		// �傫����ݒ�
		pMask->SetVec3Sizing(rSize);

		// �F��ݒ�
		pMask->SetColor(rCol);

		// �m�ۂ����A�h���X��Ԃ�
		return pMask;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
