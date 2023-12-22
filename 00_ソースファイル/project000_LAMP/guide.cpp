//============================================================
//
//	�K�C�h���� [guide.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "guide.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY = 6;	// �K�C�h�̗D�揇��
}

//************************************************************
//	�q�N���X [CGuide] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGuide::CGuide() : CObject3D(CObject::LABEL_UI, PRIORITY)
{
}

//============================================================
//	�f�X�g���N�^
//============================================================
CGuide::~CGuide()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGuide::Init(void)
{
	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	BindTexture("data\\TEXTURE\\Guide.png");
	SetEnableDraw(false);
	SetEnableDepthShadow(false);
	SetEnableZTex(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGuide::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGuide::Update(void)
{
	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CGuide::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// Z�e�X�g��ݒ肷��
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw();

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�
}

//============================================================
//	��������
//============================================================
CGuide *CGuide::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize	// �傫��
)
{
	// �|�C���^��錾
	CGuide *pGuide = NULL;	// �K�C�h�����p

	if (pGuide == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pGuide = new CGuide;	// �K�C�h
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pGuide != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �K�C�h�̏�����
		if (FAILED(pGuide->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pGuide;
			pGuide = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pGuide->SetVec3Position(rPos);

		// �傫����ݒ�
		pGuide->SetVec3Sizing(rSize);

		// �m�ۂ����A�h���X��Ԃ�
		return pGuide;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
