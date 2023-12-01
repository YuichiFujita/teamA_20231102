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
#include "multiValue.h"
#include "retentionManager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 14;	// �X�e�[�^�X�̗D�揇��

	namespace life
	{
		const int MAX_LIFE	= 100;	// ����
		const int MAX_DIG	= 3;	// ����
		const D3DXVECTOR3 POS	= D3DXVECTOR3(200.0f, 200.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(200.0f, 200.0f, 0.0f);	// �傫��
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(200.0f, 0.0f, 0.0f);		// �s��
	}

	namespace knockrate
	{
		const int MAX_LIFE	= 100;	// ����
		const int MAX_DIG	= 3;	// ����
		const D3DXVECTOR3 POS	= D3DXVECTOR3(200.0f, 400.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(200.0f, 200.0f, 0.0f);	// �傫��
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(200.0f, 0.0f, 0.0f);		// �s��
	}
}

//************************************************************
//	�e�N���X [CStatusManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CStatusManager::CStatusManager(const int nPadID) : m_nPadID(nPadID)
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
	m_pLife = CMultiValue::Create
	( // ����
		CValue::TEXTURE_UI,	// �e�N�X�`��
		life::MAX_LIFE,		// ����
		life::MAX_DIG,		// ����
		life::POS,			// �ʒu
		life::SIZE,			// �傫��
		life::SPACE			// �s��
	);
	if (m_pLife == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pLife->SetPriority(PRIORITY);

	// ������ї��̏��̐ݒ�
	m_pKnockRate = CMultiValue::Create
	( // ����
		CValue::TEXTURE_UI,	// �e�N�X�`��
		life::MAX_LIFE,		// ����
		life::MAX_DIG,		// ����
		life::POS,			// �ʒu
		life::SIZE,			// �傫��
		life::SPACE			// �s��
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
//	�ő�̗͎擾����
//============================================================
int CStatusManager::GetNumMaxLife(void) const
{
	// �ő�̗͂�Ԃ�
	return m_pLife->GetMax();
}

//============================================================
//	�̗͂̕`��󋵂̐ݒ菈��
//============================================================
void CStatusManager::SetEnableDrawLife(const bool bDraw)
{
	if (CManager::GetInstance()->GetRetentionManager()->GetKillState() == CRetentionManager::KILL_LIFE)
	{ // ���S�������̗͂̏ꍇ

		// �����̕`��󋵂�ݒ�
		m_pLife->SetEnableDraw(bDraw);
	}
}

//============================================================
//	������ї��̉��Z����
//============================================================
void CStatusManager::AddNumRate(const int nAdd)
{
	// �����̐�����ї������Z
	m_pKnockRate->AddNum(nAdd);
}

//============================================================
//	������ї��̐ݒ菈��
//============================================================
void CStatusManager::SetNumRate(const int nSet)
{
	// �����̐�����ї���ݒ�
	m_pKnockRate->SetNum(nSet);
}

//============================================================
//	������ї��擾����
//============================================================
int CStatusManager::GetNumRate(void) const
{
	// ������ї���Ԃ�
	return m_pKnockRate->GetNum();
}

//============================================================
//	������ї��擾����
//============================================================
int CStatusManager::GetNumMaxRate(void) const
{
	// �ő吁����ї���Ԃ�
	return m_pKnockRate->GetMax();
}

//============================================================
//	������ї��̕`��󋵂̐ݒ菈��
//============================================================
void CStatusManager::SetEnableDrawRate(const bool bDraw)
{
	if (CManager::GetInstance()->GetRetentionManager()->GetKillState() == CRetentionManager::KILL_KNOCK)
	{ // ���S������������ї��̏ꍇ

		// �����̕`��󋵂�ݒ�
		m_pKnockRate->SetEnableDraw(bDraw);
	}
}

//============================================================
//	��������
//============================================================
CStatusManager *CStatusManager::Create(const int nPadID)
{
	// �|�C���^��錾
	CStatusManager *pStatusManager = NULL;		// �X�e�[�^�X�}�l�[�W���[�����p

	if (pStatusManager == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pStatusManager = new CStatusManager(nPadID);	// �X�e�[�^�X�}�l�[�W���[
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
