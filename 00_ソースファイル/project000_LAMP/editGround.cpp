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

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_TRIGGER		(DIK_LSHIFT)	// �g���K�[���L�[
#define NAME_TRIGGER	("LSHIFT")		// �g���K�[���\��

#define KEY_CREATE		(DIK_0)	// �����L�[
#define NAME_CREATE		("0")	// �����\��
#define KEY_RELEASE		(DIK_9)	// �j���L�[
#define NAME_RELEASE	("9")	// �j���\��
#define KEY_TYPE		(DIK_2)	// ��ޕύX�L�[
#define NAME_TYPE		("2")	// ��ޕύX�\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float	INIT_ALPHA = 0.5f;	// �z�u�O�̃��l
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
	D3DXVECTOR3 sizeEdit = pEdit->GetVec3Sizing();	// �G�f�B�b�g�̑傫��

	// �����o�ϐ���������
	m_pGround = NULL;	// �n�Տ��
	m_ground.type = CGround::TYPE_GRASS;	// �n�Վ��

	// �n�Ղ̐���
	m_pGround = CGround::Create(m_ground.type, posEdit, rotEdit, sizeEdit);
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

	// ��ޕύX�̍X�V
	UpdateChangeType();

	// �n�Ղ̐���
	CreateGround();

	// �n�Ղ̔j��
	ReleaseGround();

	// �ʒu�𔽉f
	m_pGround->SetVec3Position(pEdit->GetVec3Position());

	// �����𔽉f
	m_pGround->SetVec3Rotation(pEdit->GetVec3Rotation());

	// �傫���𔽉f
	m_pGround->SetVec3Sizing(pEdit->GetVec3Sizing());

	// ��ނ𔽉f
	m_pGround->SetType(m_ground.type);

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditGround::DrawDebugControl(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�v���b�N�̏��

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
					D3DXVECTOR3 posBuild = pObjCheck->GetVec3Position();	// �ʒu
					D3DXVECTOR3 rotBuild = pObjCheck->GetVec3Rotation();	// ����
					D3DXVECTOR3 sizeBuild = pObjCheck->GetVec3Sizing();		// �傫��
					int nType = pObjCheck->GetType();	// ���
	
					// ���������o��
					fprintf(pFile, "	GROUNDSET\n");
					fprintf(pFile, "		TYPE = %d\n", nType);
					fprintf(pFile, "		POS = %.2f %.2f %.2f\n", posBuild.x, posBuild.y, posBuild.z);
					fprintf(pFile, "		ROT = %.2f %.2f %.2f\n", rotBuild.x, rotBuild.y, rotBuild.z);
					fprintf(pFile, "		SIZE = %.2f %.2f %.2f\n", sizeBuild.x, sizeBuild.y, sizeBuild.z);
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
	D3DXVECTOR3 sizeEdit = pEdit->GetVec3Sizing();	// �G�f�B�b�g�̑傫��
	D3DXCOLOR colBuild = XCOL_WHITE;	// �F�ۑ��p

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
		colBuild = m_pGround->GetColor();	// ���̐F���擾
		m_pGround->SetColor(D3DXCOLOR(colBuild.r, colBuild.g, colBuild.b, 1.0f));

		// ���ۑ���ݒ�
		pEdit->UnSave();

		//----------------------------------------------------
		//	�V�����n�Ղ̐���
		//----------------------------------------------------
		// �n�Ղ̐���
		m_pGround = CGround::Create(m_ground.type, posEdit, rotEdit, sizeEdit);
		assert(m_pGround != NULL);

		// �F��ݒ�
		colBuild = m_pGround->GetColor();	// ���̐F���擾
		m_pGround->SetColor(D3DXCOLOR(colBuild.r, colBuild.g, colBuild.b, INIT_ALPHA));
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
	D3DXVECTOR3 sizeEdit = pEdit->GetVec3Sizing();	// �G�f�B�b�g�̑傫��

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
				D3DXVECTOR3 posBuild = VEC3_ZERO;	// �n�Ոʒu
				D3DXVECTOR3 sizeBuild = VEC3_ZERO;	// �n�Ց傫��

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
				posBuild = pObjCheck->GetVec3Position();

				// �n�Ղ̑傫�����擾
				sizeBuild = pObjCheck->GetVec3Sizing();

				// ���̂̓����蔻��
				if (collision::Circle3D
				( // ����
					posEdit,							// ����ʒu
					posBuild,							// ����ڕW�ʒu
					(sizeBuild.x + sizeBuild.z) * 0.5f,	// ���蔼�a
					(sizeEdit.x + sizeEdit.z) * 0.5f	// ����ڕW���a
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
