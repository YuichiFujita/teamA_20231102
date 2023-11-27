//============================================================
//
//	�X�e�[�^�X�}�l�[�W���[���� [statusManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "statusManager.h"
#include "manager.h"
#include "objectGauge2D.h"
#include "valueUI.h"
#include "multiValue.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 14;	// �X�e�[�^�X�̗D�揇��

	namespace life
	{
		const int MAX_LIFE		= 100;	// �ő�̗�
		const int CHANGE_FRAME	= 10;	// �Q�[�W�ϓ��t���[��

		const D3DXVECTOR3	POS = D3DXVECTOR3(400.0f, 200.0f, 0.0f);		// �Q�[�W�ʒu
		const D3DXVECTOR3	SIZE_GAUGE = D3DXVECTOR3(300.0f, 20.0f, 0.0f);	// �Q�[�W�傫��
		const D3DXCOLOR		COL_GAUGE_FRONT	= XCOL_YELLOW;	// �Q�[�W�O�i�F
		const D3DXCOLOR		COL_GAUGE_BACK	= XCOL_RED;		// �Q�[�W�w�i�F
	}

	namespace knockrate
	{
		const char* TITLE_TEX_PASS = "data\\TEXTURE\\continue001.png";	// �^�C�g���e�N�X�`��

		const int MAX_DIG = 3;	// ��������

		const D3DXVECTOR3 POS			= D3DXVECTOR3(200.0f, 400.0f, 0.0f);	// UI�ʒu
		const D3DXVECTOR3 SPACE_TITLE	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �^�C�g����
		const D3DXVECTOR3 SPACE_VALUE	= D3DXVECTOR3(100.0f, 0.0f, 0.0f);		// ������
		const D3DXVECTOR3 SIZE_TITLE	= D3DXVECTOR3(400.0f, 100.0f, 0.0f);	// �^�C�g���傫��
		const D3DXVECTOR3 SIZE_VALUE	= D3DXVECTOR3(100.0f, 100.0f, 0.0f);	// �����傫��
	}
}

//************************************************************
//	�e�N���X [CStatusManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CStatusManager::CStatusManager()
{
	// �����o�ϐ����N���A
	m_pLife = NULL;			// �̗͂̏��
	m_pKnockRate = NULL;	// ������ї��̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CStatusManager::~CStatusManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CStatusManager::Init(void)
{
	// �����o�ϐ���������
	m_pLife = NULL;			// �̗͂̏��
	m_pKnockRate = NULL;	// ������ї��̏��

	// �̗͂̏��̐���
	m_pLife = CObjectGauge2D::Create
	( // ����
		CObject::LABEL_UI,		// �I�u�W�F�N�g���x��
		life::MAX_LIFE,			// �ő�\���l
		life::CHANGE_FRAME,		// �\���l�ϓ��t���[��
		life::POS,				// �ʒu
		life::SIZE_GAUGE,		// �Q�[�W�傫��
		life::COL_GAUGE_FRONT,	// �\�Q�[�W�F
		life::COL_GAUGE_BACK	// ���Q�[�W�F
	);
	if (m_pLife == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pLife->SetPriority(PRIORITY);

	// ������ї��̏��̐ݒ�
	m_pKnockRate = CValueUI::Create
	( // ����
		knockrate::TITLE_TEX_PASS,	// �^�C�g���e�N�X�`���p�X
		CValue::TEXTURE_NORMAL,		// �����e�N�X�`��
		knockrate::MAX_DIG,			// ����
		knockrate::POS,				// �ʒu
		knockrate::SPACE_TITLE,		// �s��
		knockrate::SPACE_VALUE,		// �����s��
		knockrate::SIZE_TITLE,		// �^�C�g���傫��
		knockrate::SIZE_VALUE		// �����傫��
	);
	if (m_pKnockRate == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pKnockRate->SetPriority(PRIORITY);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CStatusManager::Uninit(void)
{
	// �̗͂̏I��
	m_pLife->Uninit();

	// ������ї��̏I��
	m_pKnockRate->Uninit();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�X�V����
//============================================================
void CStatusManager::Update(void)
{
	// �̗͂̍X�V
	m_pLife->Update();

	// ������ї��̍X�V
	m_pKnockRate->Update();
}

//============================================================
//	�`�揈��
//============================================================
void CStatusManager::Draw(void)
{

}

//============================================================
//	�̗͂̉��Z����
//============================================================
void CStatusManager::AddNumLife(const int nAdd)
{
	// �����̗̑͂����Z
	m_pLife->AddNum(nAdd);
}

//============================================================
//	�̗͂̐ݒ菈��
//============================================================
void CStatusManager::SetNumLife(const int nSet)
{
	// �����̗̑͂�ݒ�
	m_pLife->SetNum(nSet);
}

//============================================================
//	�̗͎擾����
//============================================================
int CStatusManager::GetNumLife(void) const
{
	// �̗͂�Ԃ�
	return m_pLife->GetNum();
}

//============================================================
//	�̗͂̕`��󋵂̐ݒ菈��
//============================================================
void CStatusManager::SetEnableDrawLife(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	m_pLife->SetEnableDraw(bDraw);
}

//============================================================
//	������ї��̉��Z����
//============================================================
void CStatusManager::AddNumRate(const int nAdd)
{
	// �����̐�����ї������Z
	m_pKnockRate->GetMultiValue()->AddNum(nAdd);
}

//============================================================
//	������ї��̐ݒ菈��
//============================================================
void CStatusManager::SetNumRate(const int nSet)
{
	// �����̐�����ї���ݒ�
	m_pKnockRate->GetMultiValue()->SetNum(nSet);
}

//============================================================
//	������ї��擾����
//============================================================
int CStatusManager::GetNumRate(void) const
{
	// ������ї���Ԃ�
	return m_pKnockRate->GetMultiValue()->GetNum();
}

//============================================================
//	������ї��̕`��󋵂̐ݒ菈��
//============================================================
void CStatusManager::SetEnableDrawRate(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	m_pKnockRate->SetEnableDraw(bDraw);
}

//============================================================
//	��������
//============================================================
CStatusManager *CStatusManager::Create(void)
{
	// �|�C���^��錾
	CStatusManager *pStatusManager = NULL;		// �X�e�[�^�X�}�l�[�W���[�����p

	if (pStatusManager == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pStatusManager = new CStatusManager;	// �X�e�[�^�X�}�l�[�W���[
	}
	else { assert(false); return NULL; }		// �g�p��

	if (pStatusManager != NULL)
	{ // �g�p����Ă���ꍇ
		
		// �X�e�[�^�X�}�l�[�W���[�̏�����
		if (FAILED(pStatusManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pStatusManager;
			pStatusManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pStatusManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CStatusManager::Release(CStatusManager *&prStatusManager)
{
	if (prStatusManager != NULL)
	{ // �g�p���̏ꍇ

		// �X�e�[�^�X�}�l�[�W���[�̏I��
		if (FAILED(prStatusManager->Uninit()))
		{ // �j���Ɏ��s�����ꍇ

			// �������J��
			delete prStatusManager;
			prStatusManager = NULL;

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �������J��
		delete prStatusManager;
		prStatusManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}
