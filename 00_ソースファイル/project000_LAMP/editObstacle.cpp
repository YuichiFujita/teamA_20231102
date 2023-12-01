//============================================================
//
//	�G�f�B�b�g��Q������ [editObstacle.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editObstacle.h"
#include "manager.h"
#include "input.h"
#include "collision.h"
#include "editStageManager.h"
#include "objectMeshCube.h"
#include "stage.h"
#include "effect3D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_DOUBLE		(DIK_LCONTROL)	// ��d���L�[
#define NAME_DOUBLE		("LCTRL")		// ��d���\��
#define KEY_TRIGGER		(DIK_LSHIFT)	// �g���K�[���L�[
#define NAME_TRIGGER	("LSHIFT")		// �g���K�[���\��
#define KEY_REVERSE		(DIK_LCONTROL)	// ����t�]���L�[
#define NAME_REVERSE	("LCTRL")		// ����t�]���\��

#define KEY_SAVE	(DIK_F8)	// �ۑ��L�[
#define NAME_SAVE	("F8")		// �ۑ��\��

#define KEY_CREATE		(DIK_0)	// �����L�[
#define NAME_CREATE		("0")	// �����\��
#define KEY_RELEASE		(DIK_9)	// �j���L�[
#define NAME_RELEASE	("9")	// �j���\��
#define KEY_TYPE		(DIK_2)	// ��ޕύX�L�[
#define NAME_TYPE		("2")	// ��ޕύX�\��
#define KEY_BREAK		(DIK_3)	// �j��ύX�L�[
#define NAME_BREAK		("3")	// �j��ύX�\��
#define KEY_LIFE		(DIK_4)	// �̗͕ύX�L�[
#define NAME_LIFE		("4")	// �̗͕ύX�\��

#define KEY_MOVE_RIGHT	(DIK_RIGHT)	// �E�ړ��L�[
#define NAME_MOVE_RIGHT	("��")		// �E�ړ��\��
#define KEY_MOVE_LEFT	(DIK_LEFT)	// ���ړ��L�[
#define NAME_MOVE_LEFT	("��")		// ���ړ��\��
#define KEY_MOVE_FAR	(DIK_UP)	// ���ړ��L�[
#define NAME_MOVE_FAR	("��")		// ���ړ��\��
#define KEY_MOVE_NEAR	(DIK_DOWN)	// ��O�ړ��L�[
#define NAME_MOVE_NEAR	("��")		// ��O�ړ��\��

#define KEY_UP_SCALE_X		(DIK_T)	// X���g��L�[
#define NAME_UP_SCALE_X		("T")	// X���g��\��
#define KEY_DOWN_SCALE_X	(DIK_G)	// X���k���L�[
#define NAME_DOWN_SCALE_X	("G")	// X���k���\��
#define KEY_UP_SCALE_Z		(DIK_U)	// Z���g��L�[
#define NAME_UP_SCALE_Z		("U")	// Z���g��\��
#define KEY_DOWN_SCALE_Z	(DIK_J)	// Z���k���L�[
#define NAME_DOWN_SCALE_Z	("J")	// Z���k���\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float	INIT_ALPHA		= 0.5f;		// �z�u�O�̃��l
	const float	MOVE_POS		= 10.0f;	// �ʒu�ړ���
	const float	SIZE_COLLY		= 300.0f;	// ����̑傫��Y
	const float	MIN_SIZE		= 10.0f;	// �ŏ��̑傫��
	const float	MAX_SIZE		= 10000.0f;	// �ő�̑傫��
	const float	MOVE_SIZE		= 10.0f;	// �傫���ϓ���
	const int	MIN_LIFE		= 1;		// �ŏ��̗̑�
	const int	MAX_LIFE		= 6;		// �ő�̗̑�
	const D3DXCOLOR	COLL_COL	= D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.4f);	// �����蔻��F
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CEditObstacle::SInfo CEditObstacle::m_save = {};	// �ۑ����

//************************************************************
//	�e�N���X [CEditObstacle] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditObstacle::CEditObstacle()
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pObstacle = NULL;	// ��Q�����
	memset(&m_obstacle, 0, sizeof(m_obstacle));	// ��Q���z�u���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditObstacle::~CEditObstacle()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditObstacle::Init(void)
{
#if _DEBUG

	// �|�C���^��錾
	CEditStageManager *pEdit = GetPtrEditStage();	// �G�f�B�b�g�X�e�[�W���
	if (pEdit == NULL)
	{ // �G�f�B�b�g�X�e�[�W�����݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �ϐ���錾
	D3DXVECTOR3 posEdit		= pEdit->GetVec3Position();	// �G�f�B�b�g�̈ʒu
	D3DXVECTOR3 rotEdit		= pEdit->GetVec3Rotation();	// �G�f�B�b�g�̌���
	D3DXVECTOR3 posColl		= VEC3_ZERO;				// ����ʒu
	D3DXVECTOR3 sizeColl	= CObstacle::GetStatusInfo(m_obstacle.type).sizeColl;		// ����傫��
	float fLengthColl		= CObstacle::GetStatusInfo(m_obstacle.type).fLengthCenter;	// ���蒆�S�ʒu�̋���
	float fAngleColl		= CObstacle::GetStatusInfo(m_obstacle.type).fAngleCenter;	// ���蒆�S�ʒu�̊p�x

	// ����ʒu�����߂�
	posColl = posEdit;
	posColl.x += sinf(rotEdit.y + fAngleColl) * fLengthColl;
	posColl.y = 0.0f;
	posColl.z += cosf(rotEdit.y + fAngleColl) * fLengthColl;

	// �����o�ϐ���������
	m_pObstacle		= NULL;						// ��Q�����
	m_pVisual		= NULL;						// �����蔻�莋�F�L���[�u
	m_obstacle.type	= CObstacle::TYPE_CONIFER;	// ��Q�����
	m_obstacle.posCollCenter = posColl;			// ����̒��S�ʒu

	// ��Q���̐���
	m_pObstacle = CObstacle::Create(m_obstacle.type, posEdit, rotEdit);
	if (m_pObstacle == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����x��ݒ�
	m_pObstacle->SetAlpha(INIT_ALPHA);

	// ���莋�F�L���[�u�̐���
	m_pVisual = CObjectMeshCube::Create
	( // ����
		posColl,	// �ʒu
		rotEdit,	// ����
		sizeColl,	// �傫��
		COLL_COL	// �L���[�u�F
	);

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
void CEditObstacle::Uninit(void)
{
#if _DEBUG

	if (m_pObstacle != NULL)
	{ // �g�p���̏ꍇ

		// ��Q���̐F�̑S������
		InitAllColorObstacle();

		// ��Q���̏I��
		m_pObstacle->Uninit();
	}

	if (m_pVisual != NULL)
	{ // �g�p���̏ꍇ

		// ���莋�F�L���[�u�̏I��
		m_pVisual->Uninit();
	}

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditObstacle::Update(void)
{
#if _DEBUG

	// �|�C���^��錾
	CEditStageManager *pEdit = GetPtrEditStage();	// �G�f�B�b�g�X�e�[�W���
	if (pEdit == NULL)
	{ // �G�f�B�b�g�X�e�[�W�����݂��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return;
	}

	// �ϐ���錾
	D3DXVECTOR3 posEdit = pEdit->GetVec3Position();	// �G�f�B�b�g�̈ʒu
	D3DXVECTOR3 rotEdit = pEdit->GetVec3Rotation();	// �G�f�B�b�g�̌���

	// ��ޕύX�̍X�V
	UpdateChangeType();

	// ����ʒu�̍X�V
	UpdateCollPosition();

	// ����傫���̍X�V
	UpdateCollSizing();

	// �j��ύX�̍X�V
	UpdateChangeBreak();

	// �̗͕ύX�̍X�V
	UpdateChangeLife();

	// ��Q���̐���
	CreateObstacle();

	// ��Q���̔j��
	ReleaseObstacle();

	// �X�e�[�^�X���̕ۑ�
	SaveStatusInfo();

	// �ʒu�𔽉f
	m_pObstacle->SetVec3Position(posEdit);
	m_pVisual->SetVec3Position(m_obstacle.posCollCenter);

	// �����𔽉f
	m_pObstacle->SetVec3Rotation(rotEdit);
	m_pVisual->SetVec3Rotation(rotEdit);

	// ���莋�F�L���[�u�̍X�V
	m_pVisual->Update();

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditObstacle::DrawDebugControl(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�v���b�N�̏��

	pDebug->Print(CDebugProc::POINT_RIGHT, "��Q���X�e�[�^�X�ۑ��F[%s+%s]\n", NAME_DOUBLE, NAME_SAVE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "����ʒu�F[%s/%s/%s/%s]\n", NAME_MOVE_RIGHT, NAME_MOVE_LEFT, NAME_MOVE_FAR, NAME_MOVE_NEAR);
	pDebug->Print(CDebugProc::POINT_RIGHT, "����傫���F[%s/%s/%s/%s+%s]\n", NAME_UP_SCALE_X, NAME_DOWN_SCALE_X, NAME_UP_SCALE_Z, NAME_DOWN_SCALE_Z, NAME_TRIGGER);
	pDebug->Print(CDebugProc::POINT_RIGHT, "��ޕύX�F[%s+%s]\n", NAME_TYPE, NAME_REVERSE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�j��ύX�F[%s]\n", NAME_BREAK);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�̗͕ύX�F[%s]\n", NAME_LIFE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�폜�F[%s]\n", NAME_RELEASE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditObstacle::DrawDebugInfo(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�v���b�N�̏��
	static char* apBreak[] = { "�s�\", "�\" };	// �j���

	// �j��󋵐��̕s��v
	assert((sizeof(apBreak) / sizeof(apBreak[0])) == CObstacle::BREAK_MAX);

	pDebug->Print(CDebugProc::POINT_RIGHT, "%d�F[���]\n", m_obstacle.type);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f %f�F[����ʒu]\n", m_obstacle.posCollCenter.x, m_obstacle.posCollCenter.y, m_obstacle.posCollCenter.z);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f %f�F[����傫��]\n", m_pVisual->GetVec3Sizing().x, m_pVisual->GetVec3Sizing().y, m_pVisual->GetVec3Sizing().z);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%s�F[�j��]\n", apBreak[CObstacle::GetStatusInfo(m_obstacle.type).state]);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%d�F[�̗�]\n", CObstacle::GetStatusInfo(m_obstacle.type).nLife);
}

//============================================================
//	���ۑ�����
//============================================================
void CEditObstacle::SaveInfo(void)
{
	// ���݂̏���ۑ�
	m_save = m_obstacle;
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditObstacle::LoadInfo(void)
{
	// �ۑ�����ݒ�
	m_obstacle = m_save;
}

//============================================================
//	�ۑ�����
//============================================================
void CEditObstacle::Save(FILE *pFile)
{
#if _DEBUG

	if (pFile != NULL)
	{ // �t�@�C�������݂���ꍇ

		// ���o���������o��
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#	��Q���̔z�u���\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		// ���J�n�n�_�������o��
		fprintf(pFile, "STAGE_OBSTACLESET\n\n");

		for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
		{ // �D�揇�ʂ̑������J��Ԃ�
	
			// �|�C���^��錾
			CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g
	
			if (pObjectTop != NULL)
			{ // �擪�����݂���ꍇ
	
				// �|�C���^��錾
				CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p
	
				while (pObjCheck != NULL)
				{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�
		
					// �|�C���^��錾
					CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g
	
					if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
					{ // �I�u�W�F�N�g���x������Q���ł͂Ȃ��ꍇ
	
						// ���̃I�u�W�F�N�g�ւ̃|�C���^����
						pObjCheck = pObjectNext;
	
						// ���̌J��Ԃ��Ɉڍs
						continue;
					}
	
					if (pObjCheck == (CObject*)m_pObstacle)
					{ // �����A�h���X�������ꍇ
	
						// ���̃I�u�W�F�N�g�ւ̃|�C���^����
						pObjCheck = pObjectNext;
	
						// ���̌J��Ԃ��Ɉڍs
						continue;
					}

					// ��Q���̏����擾
					D3DXVECTOR3 posObs = pObjCheck->GetVec3Position();	// �ʒu
					D3DXVECTOR3 rotObs = pObjCheck->GetVec3Rotation();	// ����
					int nType = pObjCheck->GetType();	// ���
	
					// ���������o��
					fprintf(pFile, "	OBSTACLESET\n");
					fprintf(pFile, "		TYPE = %d\n", nType);
					fprintf(pFile, "		POS = %.2f %.2f %.2f\n", posObs.x, posObs.y, posObs.z);
					fprintf(pFile, "		ROT = %.2f %.2f %.2f\n", rotObs.x, rotObs.y, rotObs.z);
					fprintf(pFile, "	END_OBSTACLESET\n\n");

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;
				}
			}
		}

		// ���I���n�_�������o��
		fprintf(pFile, "END_STAGE_OBSTACLESET\n\n");
	}

#endif	// _DEBUG
}

//============================================================
//	��ޕύX�̍X�V����
//============================================================
void CEditObstacle::UpdateChangeType(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// ��ނ�ύX
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsTrigger(KEY_TYPE))
		{
			// ��ނ�ύX
			m_obstacle.type = (CObstacle::EType)((m_obstacle.type + 1) % CObstacle::TYPE_MAX);

			// ��ނ𔽉f
			m_pObstacle->SetType(m_obstacle.type);

			// �����x��ݒ�
			m_pObstacle->SetAlpha(INIT_ALPHA);
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_TYPE))
		{
			// ��ނ�ύX
			m_obstacle.type = (CObstacle::EType)((m_obstacle.type + (CObstacle::TYPE_MAX - 1)) % CObstacle::TYPE_MAX);

			// ��ނ𔽉f
			m_pObstacle->SetType(m_obstacle.type);

			// �����x��ݒ�
			m_pObstacle->SetAlpha(INIT_ALPHA);
		}
	}
}

//============================================================
//	����ʒu�̍X�V����
//============================================================
void CEditObstacle::UpdateCollPosition(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���
	CEditStageManager *pEdit = GetPtrEditStage();	// �G�f�B�b�g�X�e�[�W���
	if (pEdit == NULL)
	{ // �G�f�B�b�g�X�e�[�W�����݂��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return;
	}

	// �ϐ���錾
	CObstacle::SStatusInfo status = CObstacle::GetStatusInfo(m_obstacle.type);	// �X�e�[�^�X���
	D3DXVECTOR3 posEdit	= pEdit->GetVec3Position();	// �G�f�B�b�g�̈ʒu
	D3DXVECTOR3 rotEdit	= pEdit->GetVec3Rotation();	// �G�f�B�b�g�̌���
	float fLengthColl	= status.fLengthCenter;		// ���蒆�S�ʒu�̋���
	float fAngleColl	= status.fAngleCenter;		// ���蒆�S�ʒu�̊p�x

	// ����ʒu�����߂�
	m_obstacle.posCollCenter = posEdit;
	m_obstacle.posCollCenter.x += sinf(rotEdit.y + fAngleColl) * fLengthColl;
	m_obstacle.posCollCenter.y = 0.0f;
	m_obstacle.posCollCenter.z += cosf(rotEdit.y + fAngleColl) * fLengthColl;

	// �ʒu��ύX
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_MOVE_RIGHT))
		{
			m_obstacle.posCollCenter.x += MOVE_POS;
		}
		if (m_pKeyboard->IsPress(KEY_MOVE_LEFT))
		{
			m_obstacle.posCollCenter.x -= MOVE_POS;
		}
		if (m_pKeyboard->IsPress(KEY_MOVE_FAR))
		{
			m_obstacle.posCollCenter.z += MOVE_POS;
		}
		if (m_pKeyboard->IsPress(KEY_MOVE_NEAR))
		{
			m_obstacle.posCollCenter.z -= MOVE_POS;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_MOVE_RIGHT))
		{
			m_obstacle.posCollCenter.x += MOVE_POS;
		}
		if (m_pKeyboard->IsTrigger(KEY_MOVE_LEFT))
		{
			m_obstacle.posCollCenter.x -= MOVE_POS;
		}
		if (m_pKeyboard->IsTrigger(KEY_MOVE_FAR))
		{
			m_obstacle.posCollCenter.z += MOVE_POS;
		}
		if (m_pKeyboard->IsTrigger(KEY_MOVE_NEAR))
		{
			m_obstacle.posCollCenter.z -= MOVE_POS;
		}
	}

	// ���蒆�S�ʒu�̋�����ݒ�
	status.fLengthCenter = sqrtf((m_obstacle.posCollCenter.x - posEdit.x) * (m_obstacle.posCollCenter.x - posEdit.x)
						 + (m_obstacle.posCollCenter.z - posEdit.z) * (m_obstacle.posCollCenter.z - posEdit.z));

	// ���蒆�S�ʒu�̊p�x��ݒ�
	status.fAngleCenter = atan2f((m_obstacle.posCollCenter.x - posEdit.x), (m_obstacle.posCollCenter.z - posEdit.z)) - rotEdit.y;
	
	// �X�e�[�^�X���𔽉f
	CObstacle::SetStatusInfo(m_obstacle.type, status);
}

//============================================================
//	����傫���̍X�V����
//============================================================
void CEditObstacle::UpdateCollSizing(void)
{
	// �ϐ���錾
	CObstacle::SStatusInfo status = CObstacle::GetStatusInfo(m_obstacle.type);	// �X�e�[�^�X���

	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �傫����ύX
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_UP_SCALE_X))
		{
			status.sizeColl.x += MOVE_SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN_SCALE_X))
		{
			status.sizeColl.x -= MOVE_SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_UP_SCALE_Z))
		{
			status.sizeColl.z += MOVE_SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN_SCALE_Z))
		{
			status.sizeColl.z -= MOVE_SIZE;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_UP_SCALE_X))
		{
			status.sizeColl.x += MOVE_SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_SCALE_X))
		{
			status.sizeColl.x -= MOVE_SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP_SCALE_Z))
		{
			status.sizeColl.z += MOVE_SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_SCALE_Z))
		{
			status.sizeColl.z -= MOVE_SIZE;
		}
	}

	// �傫����␳
	status.sizeColl.y = SIZE_COLLY;	// �c�̑傫���͌Œ�
	useful::LimitNum(status.sizeColl.x, MIN_SIZE, MAX_SIZE);
	useful::LimitNum(status.sizeColl.z, MIN_SIZE, MAX_SIZE);

	// �X�e�[�^�X���𔽉f
	CObstacle::SetStatusInfo(m_obstacle.type, status);

	// �傫���𔽉f
	m_pVisual->SetVec3Sizing(status.sizeColl);
}

//============================================================
//	�j��ύX�̍X�V����
//============================================================
void CEditObstacle::UpdateChangeBreak(void)
{
	// �ϐ���錾
	CObstacle::SStatusInfo status = CObstacle::GetStatusInfo(m_obstacle.type);	// �X�e�[�^�X���

	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �j��󋵂�ύX
	if (m_pKeyboard->IsTrigger(KEY_BREAK))
	{
		// �j��󋵂�ύX
		status.state = (CObstacle::EBreak)((status.state + 1) % CObstacle::BREAK_MAX);
	}

	// �X�e�[�^�X���𔽉f
	CObstacle::SetStatusInfo(m_obstacle.type, status);
}

//============================================================
//	�̗͕ύX�̍X�V����
//============================================================
void CEditObstacle::UpdateChangeLife(void)
{
	// �ϐ���錾
	CObstacle::SStatusInfo status = CObstacle::GetStatusInfo(m_obstacle.type);	// �X�e�[�^�X���

	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �̗͂�ύX
	if (!m_pKeyboard->IsPress(KEY_REVERSE))
	{
		if (m_pKeyboard->IsTrigger(KEY_LIFE))
		{
			// �̗͂����Z
			status.nLife++;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_LIFE))
		{
			// �̗͂����Z
			status.nLife--;
		}
	}

	// �̗͂�␳
	useful::LimitNum(status.nLife, MIN_LIFE, MAX_LIFE);

	// �X�e�[�^�X���𔽉f
	CObstacle::SetStatusInfo(m_obstacle.type, status);
}

//============================================================
//	��Q���̐�������
//============================================================
void CEditObstacle::CreateObstacle(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���
	CEditStageManager *pEdit = GetPtrEditStage();	// �G�f�B�b�g�X�e�[�W���
	if (pEdit == NULL)
	{ // �G�f�B�b�g�X�e�[�W�����݂��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return;
	}

	// �ϐ���錾
	D3DXVECTOR3 posEdit = pEdit->GetVec3Position();	// �G�f�B�b�g�̈ʒu
	D3DXVECTOR3 rotEdit = pEdit->GetVec3Rotation();	// �G�f�B�b�g�̌���

	// ��Q����z�u
	if (m_pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	��Q���̏���z�u�p�ɕύX
		//----------------------------------------------------
		// �����X�V�E�����`���ON�ɂ���
		m_pObstacle->SetEnableUpdate(true);
		m_pObstacle->SetEnableDraw(true);

		// �����x��ݒ�
		m_pObstacle->SetAlpha(1.0f);

		// ���ۑ���ݒ�
		pEdit->UnSave();

		//----------------------------------------------------
		//	�V������Q���̐���
		//----------------------------------------------------
		// ��Q���̐���
		m_pObstacle = CObstacle::Create(m_obstacle.type, posEdit, rotEdit);
		assert(m_pObstacle != NULL);

		// �����x��ݒ�
		m_pObstacle->SetAlpha(INIT_ALPHA);
	}
}

//============================================================
//	��Q���̔j������
//============================================================
void CEditObstacle::ReleaseObstacle(void)
{
	// �ϐ���錾
	bool bRelease = false;	// �j����

	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// ��Q�����폜
	if (m_pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// �j�������Ԃ�ݒ�
		bRelease = true;
	}

	// ��Q���̍폜����
	DeleteCollisionObstacle(bRelease);
}


//============================================================
//	�X�e�[�^�X���ۑ�����
//============================================================
void CEditObstacle::SaveStatusInfo(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// ��Q����ۑ�
	if (m_pKeyboard->IsPress(KEY_DOUBLE))
	{
		if (m_pKeyboard->IsTrigger(KEY_SAVE))
		{
			// ��Q���̃X�e�[�^�X�ۑ�����
			CObstacle::SaveStatus();
		}
	}
}

//============================================================
//	��Q���̍폜����
//============================================================
void CEditObstacle::DeleteCollisionObstacle(const bool bRelase)
{
	// �|�C���^��錾
	CEditStageManager *pEdit = GetPtrEditStage();	// �G�f�B�b�g�X�e�[�W���
	if (pEdit == NULL)
	{ // �G�f�B�b�g�X�e�[�W�����݂��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return;
	}

	// �ϐ���錾
	D3DXVECTOR3 posEdit = pEdit->GetVec3Position();	// �G�f�B�b�g�̈ʒu

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (pObjectTop != NULL)
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (pObjCheck != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �ϐ���錾
				D3DXVECTOR3 posObs = VEC3_ZERO;	// ��Q���ʒu
				float fRadiusObs = 0.0f;		// ��Q�����a

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
				{ // �I�u�W�F�N�g���x������Q���ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (pObjCheck == (CObject*)m_pObstacle)
				{ // �����A�h���X�������ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// ��Q���̈ʒu���擾
				posObs = pObjCheck->GetVec3Position();

				// ��Q���̔��a���擾
				fRadiusObs = pObjCheck->GetRadius();

				// ���̂̓����蔻��
				if (collision::Circle3D
				( // ����
					posEdit,	// ����ʒu
					posObs,		// ����ڕW�ʒu
					fRadiusObs,	// ���蔼�a
					200.0f		// ����ڕW���a
				))
				{ // ������������ꍇ

					if (bRelase)
					{ // �j������ꍇ

						// �I������
						pObjCheck->Uninit();

						// ���ۑ���ݒ�
						pEdit->UnSave();
					}
					else
					{ // �j�����Ȃ��ꍇ

						// �ԃ}�e���A����ݒ�
						pObjCheck->SetAllMaterial(material::Red());
					}
				}
				else
				{ // ����O�������ꍇ

					// �}�e���A�����Đݒ�
					pObjCheck->ResetMaterial();
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	��Q���̐F�̑S����������
//============================================================
void CEditObstacle::InitAllColorObstacle(void)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (pObjectTop != NULL)
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (pObjCheck != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
				{ // �I�u�W�F�N�g���x������Q���ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (pObjCheck == (CObject*)m_pObstacle)
				{ // �����A�h���X�������ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (!pObjCheck->IsDeath())
				{ // ���S���Ă��Ȃ��ꍇ

					// �}�e���A�����Đݒ�
					pObjCheck->ResetMaterial();
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}
