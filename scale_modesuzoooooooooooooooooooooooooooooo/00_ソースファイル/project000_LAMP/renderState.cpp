//============================================================
//
//	�����_�[�X�e�[�g���� [renderState.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "renderState.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CRenderState::SInfo CRenderState::m_save = {};	// �ۑ������_�[�X�e�[�g���

//************************************************************
//	�e�N���X [CRenderState] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CRenderState::CRenderState()
{
	// �����o�ϐ����N���A
	memset(&m_info, 0, sizeof(m_info));	// �����_�[�X�e�[�g���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CRenderState::~CRenderState()
{

}

//============================================================
//	����������
//============================================================
HRESULT CRenderState::Init(void)
{
	// �����o�ϐ���������
	m_info = m_save;	// �����_�[�X�e�[�g���

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CRenderState::Uninit(void)
{

}

//============================================================
//	�ݒ菈��
//============================================================
void CRenderState::Set(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �ݒ蒆�̃����_�[�X�e�[�g��ݒ�
	SetRenderState(pDevice, m_info);
}

//============================================================
//	�Đݒ菈��
//============================================================
void CRenderState::Reset(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �ۑ��ς݂̃����_�[�X�e�[�g�ɖ߂�
	SetRenderState(pDevice, m_save);
}

//============================================================
//	��������
//============================================================
CRenderState *CRenderState::Create(void)
{
	// �|�C���^��錾
	CRenderState *pRenderState = NULL;	// �����_�[�X�e�[�g�����p

	if (pRenderState == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pRenderState = new CRenderState;	// �����_�[�X�e�[�g
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pRenderState != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �����_�[�X�e�[�g�̏�����
		if (FAILED(pRenderState->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pRenderState;
			pRenderState = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pRenderState;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�����_�[�X�e�[�g���̏���������
//============================================================
void CRenderState::InitRenderState(const LPDIRECT3DDEVICE9 &rDevice)
{
	// ���u�����h����������
	m_save.blendAlpha.op		= D3DBLENDOP_ADD;
	m_save.blendAlpha.scr		= D3DBLEND_SRCALPHA;
	m_save.blendAlpha.dest		= D3DBLEND_INVSRCALPHA;
	m_save.blendAlpha.bBlend	= true;

	// ���e�X�g����������
	m_save.testAlpha.func	= D3DCMP_ALWAYS;
	m_save.testAlpha.nRef	= 0;
	m_save.testAlpha.bTest	= false;

	// Z�e�X�g����������
	m_save.testZ.func		= D3DCMP_LESSEQUAL;
	m_save.testZ.bUpdate	= true;

	// �J�����O����������
	m_save.cull = D3DCULL_CCW;

	// ���C�e�B���O�󋵂�������
	m_save.bLight = true;

	// �����_�[�X�e�[�g�̏�����
	SetRenderState(rDevice, m_save);
}

//============================================================
//	�����_�[�X�e�[�g�̐ݒ菈��
//============================================================
void CRenderState::SetRenderState(const LPDIRECT3DDEVICE9 &rDevice, const SInfo info)
{
	// ���u�����f�B���O��ݒ�
	rDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, info.blendAlpha.bBlend);
	rDevice->SetRenderState(D3DRS_BLENDOP, info.blendAlpha.op);
	rDevice->SetRenderState(D3DRS_SRCBLEND, info.blendAlpha.scr);
	rDevice->SetRenderState(D3DRS_DESTBLEND, info.blendAlpha.dest);

	// ���e�X�g��ݒ�
	rDevice->SetRenderState(D3DRS_ALPHAFUNC, info.testAlpha.func);			// ���e�X�g�̐ݒ�
	rDevice->SetRenderState(D3DRS_ALPHAREF, info.testAlpha.nRef);			// ���e�X�g�̎Q�ƒl�ݒ�
	rDevice->SetRenderState(D3DRS_ALPHATESTENABLE, info.testAlpha.bTest);	// ���e�X�g�̗L�� / �����̐ݒ�

	// Z�e�X�g��ݒ�
	rDevice->SetRenderState(D3DRS_ZFUNC, info.testZ.func);				// Z�e�X�g�̐ݒ�
	rDevice->SetRenderState(D3DRS_ZWRITEENABLE, info.testZ.bUpdate);	// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// �|���S���̕\����Ԃ�ݒ�
	rDevice->SetRenderState(D3DRS_CULLMODE, info.cull);

	// ���C�e�B���O��ݒ�
	rDevice->SetRenderState(D3DRS_LIGHTING, info.bLight);
}
