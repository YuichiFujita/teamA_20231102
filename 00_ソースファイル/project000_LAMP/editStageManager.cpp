//============================================================
//
//	�G�f�B�b�g�X�e�[�W�}�l�[�W���[���� [editStageManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editStageManager.h"
#include "manager.h"
#include "input.h"
#include "camera.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_DOUBLE		(DIK_LCONTROL)	// ��d���L�[
#define NAME_DOUBLE		("LCTRL")		// ��d���\��
#define KEY_TRIGGER		(DIK_LSHIFT)	// �g���K�[���L�[
#define NAME_TRIGGER	("LSHIFT")		// �g���K�[���\��

#define KEY_SAVE	(DIK_F9)	// �ۑ��L�[
#define NAME_SAVE	("F9")		// �ۑ��\��

#define KEY_CHANGE_THING	(DIK_1)	// �z�u���ύX�L�[
#define NAME_CHANGE_THING	("1")	// �z�u���ύX�\��

#define KEY_MOVE_UP		(DIK_UP)	// �ړ��ʏ㏸�L�[
#define NAME_MOVE_UP	("��")		// �ړ��ʏ㏸�\��
#define KEY_MOVE_DOWN	(DIK_DOWN)	// �ړ��ʉ��~�L�[
#define NAME_MOVE_DOWN	("��")		// �ړ��ʉ��~�\��

#define KEY_FAR		(DIK_W)	// ���ړ��L�[
#define NAME_FAR	("W")	// ���ړ��\��
#define KEY_NEAR	(DIK_S)	// ��O�ړ��L�[
#define NAME_NEAR	("S")	// ��O�ړ��\��
#define KEY_RIGHT	(DIK_D)	// �E�ړ��L�[
#define NAME_RIGHT	("D")	// �E�ړ��\��
#define KEY_LEFT	(DIK_A)	// ���ړ��L�[
#define NAME_LEFT	("A")	// ���ړ��\��
#define KEY_UP		(DIK_E)	// ��ړ��L�[
#define NAME_UP		("E")	// ��ړ��\��
#define KEY_DOWN	(DIK_Q)	// ���ړ��L�[
#define NAME_DOWN	("Q")	// ���ړ��\��

#define KEY_ROTA_RIGHT	(DIK_Z)	// �E��]�L�[
#define NAME_ROTA_RIGHT	("Z")	// �E��]�\��
#define KEY_ROTA_LEFT	(DIK_C)	// ����]�L�[
#define NAME_ROTA_LEFT	("C")	// ����]�\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SAVE_TXT	= "data\\TXT\\save_stage.txt";	// �X�e�[�W�Z�[�u�e�L�X�g

	const float INIT_MOVE	= 10.0f;	// �z�u���̏����ړ���
	const float CHANGE_MOVE = 2.0f;		// �z�u���̈ړ��ʂ̕ϓ���
	const float MIN_MOVE	= 10.0f;	// �z�u���̍ŏ��ړ���
	const float MAX_MOVE	= 100.0f;	// �z�u���̍ő�ړ���
	const float ADDROT		= 0.05f;	// �z�u���̉�]��
}

//************************************************************
//	�e�N���X [CEditStageManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditStageManager::CEditStageManager()
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pStage	= NULL;						// �G�f�B�b�g�X�e�[�W�̏��
	m_thing		= CEditStage::THING_GROUND;	// �z�u��
	m_pos		= VEC3_ZERO;				// �ʒu
	m_rot		= VEC3_ZERO;				// ����
	m_angle		= ANGLE_0;					// �p�x
	m_state		= STATE_PRESS;				// �����ύX���
	m_fMove		= 0.0f;						// �ʒu�ړ���
	m_bSave		= false;					// �ۑ���
	m_bEdit		= false;					// �G�f�B�b�g��

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditStageManager::~CEditStageManager()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditStageManager::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pStage	= NULL;						// �G�f�B�b�g�X�e�[�W�̏��
	m_thing		= CEditStage::THING_GROUND;	// �z�u��
	m_pos		= VEC3_ZERO;				// �ʒu
	m_rot		= VEC3_ZERO;				// ����
	m_angle		= ANGLE_0;					// �p�x
	m_state		= STATE_PRESS;				// �����ύX���
	m_fMove		= INIT_MOVE;				// �ʒu�ړ���
	m_bSave		= false;					// �ۑ���
	m_bEdit		= false;					// �G�f�B�b�g��

	// ������Ԃ�
	return S_OK;

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	�I������
//============================================================
void CEditStageManager::Uninit(void)
{
#if _DEBUG

	if (m_pStage != NULL)
	{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

		// �G�f�B�b�g�X�e�[�W�̔j��
		CEditStage::Release(m_pStage);
	}

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditStageManager::Update(void)
{
#if _DEBUG

	if (!m_bEdit)
	{ // �G�f�B�b�g���[�h�ł͂Ȃ��ꍇ

		// �����𔲂���
		return;
	}

	// �z�u���ύX�̍X�V
	UpdateChangeThing();

	// �ړ��ʕύX�̍X�V
	UpdateChangeMove();

	// �ʒu�̍X�V
	UpdatePosition();

	// �����̍X�V
	UpdateRotation();

	if (m_pStage != NULL)
	{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

		// �G�f�B�b�g�X�e�[�W�̍X�V
		m_pStage->Update();
	}
	else { assert(false); }	// ��g�p��

	// �X�e�[�W�ۑ�
	SaveStage();

	// ����\���̕`��
	DrawDebugControl();

	// ���\���̕`��
	DrawDebugInfo();

#endif	// _DEBUG
}

//============================================================
//	���ۑ��̐ݒ菈��
//============================================================
void CEditStageManager::UnSave(void)
{
	// �ۑ����Ă��Ȃ��󋵂ɂ���
	m_bSave = false;
}

//============================================================
//	�G�f�B�b�g�󋵂̐ؑ֏���
//============================================================
void CEditStageManager::SwitchEnableEdit(void)
{
	// �G�f�B�b�g�󋵂𔽓]
	m_bEdit = !m_bEdit;

	if (m_bEdit)
	{ // �G�f�B�b�gON�̏ꍇ

		// �G�f�B�b�g�X�e�[�W�̐���
		if (m_pStage == NULL)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă��Ȃ��ꍇ

			m_pStage = CEditStage::Create(this, m_thing);
			assert(m_pStage != NULL);	// �������s
		}

		// ���Ǎ�
		m_pStage->LoadInfo();
	}
	else
	{ // �G�f�B�b�gOFF�̏ꍇ

		// ���ۑ�
		m_pStage->SaveInfo();

		// �G�f�B�b�g�X�e�[�W�̔j��
		if (m_pStage != NULL)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

			HRESULT hr = CEditStage::Release(m_pStage);
			assert(hr != E_FAIL);	// �j�����s
		}
	}
}

//============================================================
//	�G�f�B�b�g�󋵂̐ݒ菈��
//============================================================
void CEditStageManager::SetEnableEdit(const bool bEdit)
{
	// �����̃G�f�B�b�g�󋵂ɂ���
	m_bEdit = bEdit;

	if (bEdit)
	{ // �G�f�B�b�g���[�h�̏ꍇ

		// �G�f�B�b�g�X�e�[�W�̐���
		if (m_pStage == NULL)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă��Ȃ��ꍇ

			m_pStage = CEditStage::Create(this, m_thing);
			assert(m_pStage != NULL);	// �������s
		}
	}
	else
	{ // �G�f�B�b�g���[�h�ł͂Ȃ��ꍇ

		// �G�f�B�b�g�X�e�[�W�̔j��
		if (m_pStage != NULL)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

			HRESULT hr = CEditStage::Release(m_pStage);
			assert(hr != E_FAIL);	// �j�����s
		}
	}
}

//============================================================
//	�G�f�B�b�g�󋵎擾����
//============================================================
bool CEditStageManager::IsEdit(void) const
{
	// �G�f�B�b�g�󋵂�Ԃ�
	return m_bEdit;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CEditStageManager::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CEditStageManager::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	��������
//============================================================
CEditStageManager *CEditStageManager::Create(void)
{
#if _DEBUG

	// �|�C���^��錾
	CEditStageManager *pEditStageManager = NULL;	// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�����p

	if (pEditStageManager == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pEditStageManager = new CEditStageManager;	// �G�f�B�b�g�X�e�[�W�}�l�[�W���[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pEditStageManager != NULL)
	{ // �g�p����Ă���ꍇ
		
		// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�̏�����
		if (FAILED(pEditStageManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pEditStageManager;
			pEditStageManager = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEditStageManager;
	}
	else { assert(false); return NULL; }	// �m�ێ��s

#else	// NDEBUG

	// NULL��Ԃ�
	return NULL;

#endif	// _DEBUG
}

//============================================================
//	�j������
//============================================================
HRESULT CEditStageManager::Release(CEditStageManager *&prEditStageManager)
{
#if _DEBUG

	if (prEditStageManager != NULL)
	{ // �g�p���̏ꍇ

		// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�̏I��
		prEditStageManager->Uninit();

		// �������J��
		delete prEditStageManager;
		prEditStageManager = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	�z�u���ύX�̍X�V����
//============================================================
void CEditStageManager::UpdateChangeThing(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �z�u����ύX
	if (!m_pKeyboard->IsPress(KEY_DOUBLE))
	{
		if (m_pKeyboard->IsTrigger(KEY_CHANGE_THING))
		{
			// ���ۑ�
			m_pStage->SaveInfo();

			// �G�f�B�b�g�X�e�[�W�̔j��
			if (m_pStage != NULL)
			{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

				HRESULT hr = CEditStage::Release(m_pStage);
				assert(hr != E_FAIL);	// �j�����s
			}

			// �z�u���̕ύX
			m_thing = (CEditStage::EThing)((m_thing + 1) % CEditStage::THING_MAX);

			// �G�f�B�b�g�X�e�[�W�̐���
			if (m_pStage == NULL)
			{ // �G�f�B�b�g�X�e�[�W���g�p����Ă��Ȃ��ꍇ

				m_pStage = CEditStage::Create(this, m_thing);
				assert(m_pStage != NULL);	// �������s
			}

			// ���Ǎ�
			m_pStage->LoadInfo();
		}
	}
}

//============================================================
//	�ړ��ʕύX�̍X�V����
//============================================================
void CEditStageManager::UpdateChangeMove(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �ړ��ʂ�ύX
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_MOVE_UP))
		{
			m_fMove += CHANGE_MOVE;
		}
		if (m_pKeyboard->IsPress(KEY_MOVE_DOWN))
		{
			m_fMove -= CHANGE_MOVE;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_MOVE_UP))
		{
			m_fMove += CHANGE_MOVE;
		}
		if (m_pKeyboard->IsTrigger(KEY_MOVE_DOWN))
		{
			m_fMove -= CHANGE_MOVE;
		}
	}

	// �ړ��ʂ�␳
	useful::LimitNum(m_fMove, MIN_MOVE, MAX_MOVE);
}

//============================================================
//	�ʒu�̍X�V����
//============================================================
void CEditStageManager::UpdatePosition(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �ʒu��ύX
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_FAR))
		{
			m_pos.z += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_NEAR))
		{
			m_pos.z -= m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_RIGHT))
		{
			m_pos.x += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_LEFT))
		{
			m_pos.x -= m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_UP))
		{
			m_pos.y += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN))
		{
			m_pos.y -= m_fMove;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_FAR))
		{
			m_pos.z += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_NEAR))
		{
			m_pos.z -= m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_RIGHT))
		{
			m_pos.x += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_LEFT))
		{
			m_pos.x -= m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP))
		{
			m_pos.y += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN))
		{
			m_pos.y -= m_fMove;
		}
	}
}

//============================================================
//	�����̍X�V����
//============================================================
void CEditStageManager::UpdateRotation(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// ������ύX
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_ROTA_RIGHT))
		{
			m_rot.y += ADDROT;

			// �����ύX��Ԃ�Press��Ԃɂ���
			m_state = STATE_PRESS;
		}
		if (m_pKeyboard->IsPress(KEY_ROTA_LEFT))
		{
			m_rot.y -= ADDROT;

			// �����ύX��Ԃ�Press��Ԃɂ���
			m_state = STATE_PRESS;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_ROTA_RIGHT))
		{
			// �p�x������]
			m_angle = (EAngle)((m_angle + (ANGLE_MAX - 1)) % ANGLE_MAX);

			// ���݂̊p�x����������v�Z
			int nTemp = ((m_angle - 1) + (ANGLE_MAX - 1)) % ANGLE_MAX;
			m_rot.y = ((float)nTemp * HALF_PI) - D3DX_PI;

			// �����ύX��Ԃ�Trigger��Ԃɂ���
			m_state = STATE_TRIGGER;
		}
		if (m_pKeyboard->IsTrigger(KEY_ROTA_LEFT))
		{
			// �p�x���E��]
			m_angle = (EAngle)((m_angle + 1) % ANGLE_MAX);

			// ���݂̊p�x����������v�Z
			int nTemp = ((m_angle - 1) + (ANGLE_MAX - 1)) % ANGLE_MAX;
			m_rot.y = ((float)nTemp * HALF_PI) - D3DX_PI;

			// �����ύX��Ԃ�Trigger��Ԃɂ���
			m_state = STATE_TRIGGER;
		}
	}

	// �����𐳋K��
	useful::Vec3NormalizeRot(m_rot);
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditStageManager::DrawDebugControl(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�v���b�N�̏��

	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "[�G�f�B�b�g����]�@\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "�X�e�[�W�ۑ��F[%s+%s]\n", NAME_DOUBLE, NAME_SAVE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�ړ��F[%s/%s/%s/%s/%s/%s+%s]\n", NAME_FAR, NAME_LEFT, NAME_NEAR, NAME_RIGHT, NAME_UP, NAME_DOWN, NAME_TRIGGER);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�ړ��ʕύX�F[%s/%s]\n", NAME_MOVE_UP, NAME_MOVE_DOWN);
	pDebug->Print(CDebugProc::POINT_RIGHT, "��]�F[%s/%s]\n", NAME_ROTA_RIGHT, NAME_ROTA_LEFT);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�z�u���ύX�F[%s]\n", NAME_CHANGE_THING);
	pDebug->Print(CDebugProc::POINT_RIGHT, "--------------------------------------\n");

	if (m_pStage != NULL)
	{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

		// �G�f�B�b�g�X�e�[�W�̑���\��
		m_pStage->DrawDebugControl();
	}
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditStageManager::DrawDebugInfo(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();			// �f�o�b�O�v���b�N�̏��
	static char* apThing[] = { "�n��", "�u���b�N", "��Q��", "�����ʒu" };	// �z�u��

	// �z�u�����̕s��v
	assert((sizeof(apThing) / sizeof(apThing[0])) == CEditStage::THING_MAX);

	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "[�G�f�B�b�g���]�@\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "======================================\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, (m_bSave) ? "�ۑ��ρF[�ۑ���]\n" : "���ۑ��F[�ۑ���]\n");
	pDebug->Print(CDebugProc::POINT_RIGHT, "%s�F[�z�u��]\n", apThing[m_thing]);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f�F[�ړ���]\n", m_fMove);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f %f�F[�ʒu]\n", m_pos.x, m_pos.y, m_pos.z);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f %f�F[����]\n", m_rot.x, m_rot.y, m_rot.z);
	pDebug->Print(CDebugProc::POINT_RIGHT, "--------------------------------------\n");

	if (m_pStage != NULL)
	{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

		// �G�f�B�b�g�X�e�[�W�̏��\��
		m_pStage->DrawDebugInfo();
	}
}

//============================================================
//	�X�e�[�W�ۑ�����
//============================================================
void CEditStageManager::SaveStage(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �X�e�[�W��ۑ�
	if (m_pKeyboard->IsPress(KEY_DOUBLE))
	{
		if (m_pKeyboard->IsTrigger(KEY_SAVE))
		{
			// �ۑ�����
			Save();

			// �ۑ�������Ԃɂ���
			m_bSave = true;
		}
	}
}

//============================================================
//	�ۑ�����
//============================================================
void CEditStageManager::Save(void)
{
	// �|�C���^��錾
	FILE *pFile = NULL;	// �t�@�C���|�C���^

	// �t�@�C���������o���`���ŊJ��
	pFile = fopen(SAVE_TXT, "w");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		// ���o���������o��
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#	�X�e�[�W�Z�[�u�e�L�X�g [save_stage.txt]\n");
		fprintf(pFile, "#	Author : you\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#==============================================================================\n");
		fprintf(pFile, "---------->--<---------- �������牺�� �R�s�[���\��t�� ---------->--<----------\n\n");

		// ���ۑ�
		m_pStage->SaveInfo();

		for (int nCntThing = 0; nCntThing < CEditStage::THING_MAX; nCntThing++)
		{ // �z�u���̑������J��Ԃ�

			// �G�f�B�b�g�X�e�[�W�̔j��
			if (m_pStage != NULL)
			{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

				HRESULT hr = CEditStage::Release(m_pStage);
				assert(hr != E_FAIL);	// �j�����s
			}

			// �G�f�B�b�g�X�e�[�W�̐���
			if (m_pStage == NULL)
			{ // �G�f�B�b�g�X�e�[�W���g�p����Ă��Ȃ��ꍇ

				m_pStage = CEditStage::Create(this, (CEditStage::EThing)nCntThing);
				assert(m_pStage != NULL);	// �������s
			}

			// �z�u���̕ۑ�
			m_pStage->Save(pFile);
		}

		// �G�f�B�b�g�X�e�[�W�̔j��
		if (m_pStage != NULL)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă���ꍇ

			HRESULT hr = CEditStage::Release(m_pStage);
			assert(hr != E_FAIL);	// �j�����s
		}

		// �G�f�B�b�g�X�e�[�W�̐���
		if (m_pStage == NULL)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă��Ȃ��ꍇ

			m_pStage = CEditStage::Create(this, m_thing);
			assert(m_pStage != NULL);	// �������s
		}

		// ���Ǎ�
		m_pStage->LoadInfo();

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�X�e�|�W�Z�[�u�t�@�C���̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
