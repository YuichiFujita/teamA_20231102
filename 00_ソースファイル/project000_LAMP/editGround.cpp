//============================================================
//
//	�G�f�B�b�g�n�Տ��� [editGround.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editGround.h"
#include "manager.h"
#include "input.h"
#include "collision.h"
#include "editStageManager.h"
#include "stage.h"
#include "effect3D.h"
#include "debugproc.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_TRIGGER		(DIK_LSHIFT)	// �g���K�[���L�[
#define NAME_TRIGGER	("LSHIFT")		// �g���K�[���\��
#define KEY_REVERSE		(DIK_LCONTROL)	// ����t�]���L�[
#define NAME_REVERSE	("LCTRL")		// ����t�]���\��

#define KEY_CREATE		(DIK_0)	// �����L�[
#define NAME_CREATE		("0")	// �����\��
#define KEY_RELEASE		(DIK_9)	// �j���L�[
#define NAME_RELEASE	("9")	// �j���\��
#define KEY_TYPE		(DIK_2)	// ��ޕύX�L�[
#define NAME_TYPE		("2")	// ��ޕύX�\��

#define KEY_UP_SCALE_X		(DIK_T)	// X���g��L�[
#define NAME_UP_SCALE_X		("T")	// X���g��\��
#define KEY_DOWN_SCALE_X	(DIK_G)	// X���k���L�[
#define NAME_DOWN_SCALE_X	("G")	// X���k���\��
#define KEY_UP_SCALE_Y		(DIK_Y)	// Y���g��L�[
#define NAME_UP_SCALE_Y		("Y")	// Y���g��\��
#define KEY_DOWN_SCALE_Y	(DIK_H)	// Y���k���L�[
#define NAME_DOWN_SCALE_Y	("H")	// Y���k���\��
#define KEY_UP_SCALE_Z		(DIK_U)	// Z���g��L�[
#define NAME_UP_SCALE_Z		("U")	// Z���g��\��
#define KEY_DOWN_SCALE_Z	(DIK_J)	// Z���k���L�[
#define NAME_DOWN_SCALE_Z	("J")	// Z���k���\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 INIT_SIZE = D3DXVECTOR3(50.0f, 50.0f, 50.0f);	// �傫��
	const float	MAX_SIZE	= 10000.0f;	// �ő�̑傫��
	const float	INIT_ALPHA	= 0.5f;		// �z�u�O�̃��l
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CEditGround::SInfo CEditGround::m_save = {};	// �ۑ����

//************************************************************
//	�e�N���X [CEditGround] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditGround::CEditGround()
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pGround = NULL;	// �n�Տ��
	memset(&m_ground, 0, sizeof(m_ground));	// �n�Քz�u���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditGround::~CEditGround()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditGround::Init(void)
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
	D3DXVECTOR3 posEdit = pEdit->GetVec3Position();	// �G�f�B�b�g�̈ʒu
	D3DXVECTOR3 rotEdit = pEdit->GetVec3Rotation();	// �G�f�B�b�g�̌���

	// �����o�ϐ���������
	m_pGround		= NULL;					// �n�Տ��
	m_ground.type	= CGround::TYPE_GRASS;	// �n�Վ��
	m_ground.size	= INIT_SIZE;			// �傫��
	m_ground.partX	= VEC2_ONE;				// �e�N�X�`��������X
	m_ground.partY	= VEC2_ONE;				// �e�N�X�`��������Y
	m_ground.partZ	= VEC2_ONE;				// �e�N�X�`��������Z

	// �n�Ղ̐���
	m_pGround = CGround::Create(m_ground.type, posEdit, rotEdit, m_ground.size);
	if (m_pGround == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �F��ݒ�
	D3DXCOLOR col = m_pGround->GetColor();	// ���̐F���擾
	m_pGround->SetColor(D3DXCOLOR(col.r, col.g, col.b, INIT_ALPHA));

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
void CEditGround::Uninit(void)
{
#if _DEBUG

	if (m_pGround != NULL)
	{ // �����Ɏ��s�����ꍇ

		// �n�Ղ̐F�̑S������
		InitAllColorGround();

		// �n�Ղ̏I��
		m_pGround->Uninit();
	}

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditGround::Update(void)
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

	// �傫���̍X�V
	UpdateSizing();

	// ��ޕύX�̍X�V
	UpdateChangeType();

	// �e�N�X�`�������̍X�V
	UpdateTexPart();

	// �n�Ղ̐���
	CreateGround();

	// �n�Ղ̔j��
	ReleaseGround();

	// �ʒu�𔽉f
	m_pGround->SetVec3Position(pEdit->GetVec3Position());

	// �����𔽉f
	m_pGround->SetVec3Rotation(pEdit->GetVec3Rotation());

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditGround::DrawDebugControl(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�v���b�N�̏��

	pDebug->Print(CDebugProc::POINT_RIGHT, "�傫���F[%s/%s/%s/%s/%s/%s+%s]\n", NAME_UP_SCALE_X, NAME_DOWN_SCALE_X, NAME_UP_SCALE_Y, NAME_DOWN_SCALE_Y, NAME_UP_SCALE_Z, NAME_DOWN_SCALE_Z, NAME_TRIGGER);
	pDebug->Print(CDebugProc::POINT_RIGHT, "��ޕύX�F[%s]\n", NAME_TYPE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�폜�F[%s]\n", NAME_RELEASE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditGround::DrawDebugInfo(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�v���b�N�̏��

	pDebug->Print(CDebugProc::POINT_RIGHT, "%d�F[���]\n", m_ground.type);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f %f�F[�傫��]\n", m_ground.size.x, m_ground.size.y, m_ground.size.z);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f�F[�e�N�X�`������X]\n", m_ground.partX.x, m_ground.partX.y);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f�F[�e�N�X�`������Y]\n", m_ground.partY.x, m_ground.partY.y);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f�F[�e�N�X�`������Z]\n", m_ground.partZ.x, m_ground.partZ.y);
}

//============================================================
//	���ۑ�����
//============================================================
void CEditGround::SaveInfo(void)
{
	// ���݂̏���ۑ�
	m_save = m_ground;
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditGround::LoadInfo(void)
{
	// �ۑ�����ݒ�
	m_ground = m_save;
}

//============================================================
//	�ۑ�����
//============================================================
void CEditGround::Save(FILE *pFile)
{
#if _DEBUG

	if (pFile != NULL)
	{ // �t�@�C�������݂���ꍇ

		// ���o���������o��
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#	�n�Ղ̔z�u���\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		// ���J�n�n�_�������o��
		fprintf(pFile, "STAGE_GROUNDSET\n\n");

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
	
					if (pObjCheck->GetLabel() != CObject::LABEL_GROUND)
					{ // �I�u�W�F�N�g���x�����n�Ղł͂Ȃ��ꍇ
	
						// ���̃I�u�W�F�N�g�ւ̃|�C���^����
						pObjCheck = pObjectNext;
	
						// ���̌J��Ԃ��Ɉڍs
						continue;
					}
	
					if (pObjCheck == (CObject*)m_pGround)
					{ // �����A�h���X�������ꍇ
	
						// ���̃I�u�W�F�N�g�ւ̃|�C���^����
						pObjCheck = pObjectNext;
	
						// ���̌J��Ԃ��Ɉڍs
						continue;
					}

					// �n�Ղ̏����擾
					D3DXVECTOR3 posGround = pObjCheck->GetVec3Position();	// �ʒu
					D3DXVECTOR3 rotGround = pObjCheck->GetVec3Rotation();	// ����
					D3DXVECTOR3 sizeGround = pObjCheck->GetVec3Sizing();	// �傫��
					int nType = pObjCheck->GetType();	// ���
	
					// ���������o��
					fprintf(pFile, "	GROUNDSET\n");
					fprintf(pFile, "		TYPE = %d\n", nType);
					fprintf(pFile, "		POS = %.2f %.2f %.2f\n", posGround.x, posGround.y, posGround.z);
					fprintf(pFile, "		ROT = %.2f %.2f %.2f\n", rotGround.x, rotGround.y, rotGround.z);
					fprintf(pFile, "		SIZE = %.2f %.2f %.2f\n", sizeGround.x, sizeGround.y, sizeGround.z);
					fprintf(pFile, "	END_GROUNDSET\n\n");

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;
				}
			}
		}

		// ���I���n�_�������o��
		fprintf(pFile, "END_STAGE_GROUNDSET\n\n");
	}

#endif	// _DEBUG
}

//============================================================
//	�傫���̍X�V����
//============================================================
void CEditGround::UpdateSizing(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �傫����ύX
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_UP_SCALE_X))
		{
			m_ground.size.x += INIT_SIZE.x;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN_SCALE_X))
		{
			m_ground.size.x -= INIT_SIZE.x;
		}
		if (m_pKeyboard->IsPress(KEY_UP_SCALE_Y))
		{
			m_ground.size.y += INIT_SIZE.y;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN_SCALE_Y))
		{
			m_ground.size.y -= INIT_SIZE.y;
		}
		if (m_pKeyboard->IsPress(KEY_UP_SCALE_Z))
		{
			m_ground.size.z += INIT_SIZE.z;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN_SCALE_Z))
		{
			m_ground.size.z -= INIT_SIZE.z;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_UP_SCALE_X))
		{
			m_ground.size.x += INIT_SIZE.x;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_SCALE_X))
		{
			m_ground.size.x -= INIT_SIZE.x;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP_SCALE_Y))
		{
			m_ground.size.y += INIT_SIZE.y;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_SCALE_Y))
		{
			m_ground.size.y -= INIT_SIZE.y;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP_SCALE_Z))
		{
			m_ground.size.z += INIT_SIZE.z;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_SCALE_Z))
		{
			m_ground.size.z -= INIT_SIZE.z;
		}
	}

	// �傫����␳
	useful::LimitNum(m_ground.size.x, INIT_SIZE.x, MAX_SIZE);
	useful::LimitNum(m_ground.size.y, INIT_SIZE.y, MAX_SIZE);
	useful::LimitNum(m_ground.size.z, INIT_SIZE.z, MAX_SIZE);

	// �傫���𔽉f
	m_pGround->SetVec3Sizing(m_ground.size);
}

//============================================================
//	��ޕύX�̍X�V����
//============================================================
void CEditGround::UpdateChangeType(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// ��ނ�ύX
	if (m_pKeyboard->IsTrigger(KEY_TYPE))
	{
		m_ground.type = (CGround::EType)((m_ground.type + 1) % CGround::TYPE_MAX);
	}

	// ��ނ𔽉f
	m_pGround->SetType(m_ground.type);
}

//============================================================
//	�e�N�X�`�������̍X�V����
//============================================================
void CEditGround::UpdateTexPart(void)
{
	// �ϐ���錾
	D3DXVECTOR3 partTex = VEC3_ZERO;	// �e�N�X�`��������

	// ��������ݒ�
	partTex.x = m_ground.size.x / INIT_SIZE.x;
	partTex.y = m_ground.size.y / INIT_SIZE.y;
	partTex.z = m_ground.size.z / INIT_SIZE.z;

	// �e�N�X�`����������ݒ�
	m_ground.partX.x = partTex.z;
	m_ground.partX.y = partTex.y;
	m_ground.partY.x = partTex.x;
	m_ground.partY.y = partTex.z;
	m_ground.partZ.x = partTex.x;
	m_ground.partZ.y = partTex.y;

	// �e�N�X�`��������������
	m_pGround->SetTexturePatternX(m_ground.partX);
	m_pGround->SetTexturePatternY(m_ground.partY);
	m_pGround->SetTexturePatternZ(m_ground.partZ);
}

//============================================================
//	�n�Ղ̐�������
//============================================================
void CEditGround::CreateGround(void)
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
	D3DXCOLOR colGround = XCOL_WHITE;	// �F�ۑ��p

	// �n�Ղ�z�u
	if (m_pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	�n�Ղ̏���z�u�p�ɕύX
		//----------------------------------------------------
		// �����X�V�E�����`���ON�ɂ���
		m_pGround->SetEnableUpdate(true);
		m_pGround->SetEnableDraw(true);

		// �F��ݒ�
		colGround = m_pGround->GetColor();	// ���̐F���擾
		m_pGround->SetColor(D3DXCOLOR(colGround.r, colGround.g, colGround.b, 1.0f));

		// ���ۑ���ݒ�
		pEdit->UnSave();

		//----------------------------------------------------
		//	�V�����n�Ղ̐���
		//----------------------------------------------------
		// �n�Ղ̐���
		m_pGround = CGround::Create(m_ground.type, posEdit, rotEdit, m_ground.size);
		assert(m_pGround != NULL);

		// �F��ݒ�
		colGround = m_pGround->GetColor();	// ���̐F���擾
		m_pGround->SetColor(D3DXCOLOR(colGround.r, colGround.g, colGround.b, INIT_ALPHA));
	}
}

//============================================================
//	�n�Ղ̔j������
//============================================================
void CEditGround::ReleaseGround(void)
{
	// �ϐ���錾
	bool bRelease = false;	// �j����

	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �n�Ղ��폜
	if (m_pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// �j�������Ԃ�ݒ�
		bRelease = true;
	}

	// �n�Ղ̍폜����
	DeleteCollisionGround(bRelease);
}

//============================================================
//	�n�Ղ̍폜����
//============================================================
void CEditGround::DeleteCollisionGround(const bool bRelase)
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
				D3DXVECTOR3 posGround = VEC3_ZERO;	// �n�Ոʒu
				D3DXVECTOR3 sizeGround = VEC3_ZERO;	// �n�Ց傫��

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_GROUND)
				{ // �I�u�W�F�N�g���x�����n�Ղł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (pObjCheck == (CObject*)m_pGround)
				{ // �����A�h���X�������ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �n�Ղ̈ʒu���擾
				posGround = pObjCheck->GetVec3Position();

				// �n�Ղ̑傫�����擾
				sizeGround = pObjCheck->GetVec3Sizing();

				// ���̂̓����蔻��
				if (collision::Circle3D
				( // ����
					posEdit,	// ����ʒu
					posGround,	// ����ڕW�ʒu
					(sizeGround.x + sizeGround.z) * 0.5f,		// ���蔼�a
					(m_ground.size.x + m_ground.size.z) * 0.5f	// ����ڕW���a
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

						// �Ԃ�ݒ�
						pObjCheck->SetColor(XCOL_RED);
					}
				}
				else
				{ // ����O�������ꍇ

					// �ʏ�F��ݒ�
					pObjCheck->SetColor(XCOL_WHITE);
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	�n�Ղ̐F�̑S����������
//============================================================
void CEditGround::InitAllColorGround(void)
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

				if (pObjCheck->GetLabel() != CObject::LABEL_GROUND)
				{ // �I�u�W�F�N�g���x�����n�Ղł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (pObjCheck == (CObject*)m_pGround)
				{ // �����A�h���X�������ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �ʏ�F��ݒ�
				pObjCheck->SetColor(XCOL_WHITE);

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}
