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
#define KEY_COLL		(DIK_3)	// ����ύX�L�[
#define NAME_COLL		("3")	// ����ύX�\��

#define KEY_SCALE_UP	(DIK_RIGHT)	// �g�嗦�㏸�L�[
#define NAME_SCALE_UP	("��")		// �g�嗦�㏸�\��
#define KEY_SCALE_DOWN	(DIK_LEFT)	// �g�嗦���~�L�[
#define NAME_SCALE_DOWN	("��")		// �g�嗦���~�\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 INIT_SIZE = D3DXVECTOR3(50.0f, 50.0f, 50.0f);	// �傫��

	const float	INIT_ALPHA = 0.5f;	// �z�u�O�̃��l
}

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
	m_pEdit = NULL;	// �G�f�B�b�g�X�e�[�W�̏��
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

	// �����o�ϐ���������
	m_pEdit = NULL;	// �G�f�B�b�g�X�e�[�W�̏��

	m_ground.pGround = NULL;				// �n�Տ��
	m_ground.type = CGround::TYPE_GRASS;	// �n�Վ��

	// �n�Ղ̐���
	m_ground.pGround = CGround::Create(m_ground.type, VEC3_ZERO, VEC3_ZERO, INIT_SIZE);
	if (m_ground.pGround == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �F��ݒ�
	D3DXCOLOR col = m_ground.pGround->GetColor();	// ���̐F���擾
	m_ground.pGround->SetColor(D3DXCOLOR(col.r, col.g, col.b, INIT_ALPHA));

	// �\����OFF�ɂ���
	SetDisp(false);

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
#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditGround::Update(void)
{
#if _DEBUG

	if (m_pEdit == NULL)
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
	m_ground.pGround->SetVec3Position(m_pEdit->GetVec3Position());

	// �����𔽉f
	m_ground.pGround->SetVec3Rotation(m_pEdit->GetVec3Rotation());

	// ��ނ𔽉f
	m_ground.pGround->SetType(m_ground.type);

#endif	// _DEBUG
}

//============================================================
//	�\���̐ݒ菈��
//============================================================
void CEditGround::SetDisp(const bool bDisp)
{
	// �����X�V�E�����`���\���󋵂ɍ��킹��
	m_ground.pGround->SetEnableUpdate(bDisp);	// �X�V
	m_ground.pGround->SetEnableDraw(bDisp);		// �`��

	if (bDisp)
	{ // �\��ON�̏ꍇ

		// �ʒu�𔽉f
		m_ground.pGround->SetVec3Position(m_pEdit->GetVec3Position());
	}
	else
	{ // �\��OFF�̏ꍇ

		// �n�Ղ̐F�̑S������
		InitAllColorGround();

		// �ʒu���X�e�[�W�͈̔͊O�ɐݒ�
		D3DXVECTOR3 outLimit = D3DXVECTOR3(0.0f, 0.0f, CScene::GetStage()->GetStageLimit().fNear - m_ground.pGround->GetVec3Sizing().z);
		m_ground.pGround->SetVec3Position(outLimit);
	}
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
	
					if (pObjCheck == (CObject*)m_ground.pGround)
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
//	��������
//============================================================
CEditGround *CEditGround::Create(CEditStageManager *pEdit)
{
#if _DEBUG

	// �|�C���^��錾
	CEditGround *pEditGround = NULL;	// �G�f�B�b�g�n�Ր����p

	if (pEditGround == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pEditGround = new CEditGround;	// �G�f�B�b�g�n��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pEditGround != NULL)
	{ // �g�p����Ă���ꍇ
		
		// �G�f�B�b�g�n�Ղ̏�����
		if (FAILED(pEditGround->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pEditGround;
			pEditGround = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �G�f�B�b�g�X�e�[�W�̏���ݒ�
		pEditGround->m_pEdit = pEdit;

		// �m�ۂ����A�h���X��Ԃ�
		return pEditGround;
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
HRESULT CEditGround::Release(CEditGround *&prEditGround)
{
#if _DEBUG

	if (prEditGround != NULL)
	{ // �g�p���̏ꍇ

		// �G�f�B�b�g�n�Ղ̏I��
		prEditGround->Uninit();

		// �������J��
		delete prEditGround;
		prEditGround = NULL;

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
	// �ϐ���錾
	D3DXVECTOR3 posEdit = m_pEdit->GetVec3Position();	// �G�f�B�b�g�̈ʒu
	D3DXVECTOR3 rotEdit = m_pEdit->GetVec3Rotation();	// �G�f�B�b�g�̌���
	D3DXCOLOR colBuild = XCOL_WHITE;	// �F�ۑ��p

	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �n�Ղ�z�u
	if (m_pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	�n�Ղ̏���z�u�p�ɕύX
		//----------------------------------------------------
		// �����X�V�E�����`���ON�ɂ���
		m_ground.pGround->SetEnableUpdate(true);
		m_ground.pGround->SetEnableDraw(true);

		// �F��ݒ�
		colBuild = m_ground.pGround->GetColor();	// ���̐F���擾
		m_ground.pGround->SetColor(D3DXCOLOR(colBuild.r, colBuild.g, colBuild.b, 1.0f));

		// ���ۑ���ݒ�
		m_pEdit->UnSave();

		//----------------------------------------------------
		//	�V�����n�Ղ̐���
		//----------------------------------------------------
		// �n�Ղ̐���
		m_ground.pGround = CGround::Create(m_ground.type, posEdit, rotEdit, INIT_SIZE);
		assert(m_ground.pGround != NULL);

		// �F��ݒ�
		colBuild = m_ground.pGround->GetColor();	// ���̐F���擾
		m_ground.pGround->SetColor(D3DXCOLOR(colBuild.r, colBuild.g, colBuild.b, INIT_ALPHA));
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
	// �ϐ���錾
	D3DXVECTOR3 posEdit = m_pEdit->GetVec3Position();				// �G�f�B�b�g�̈ʒu
	D3DXVECTOR3 sizeEdit = m_ground.pGround->GetVec3Sizing();	// �G�f�B�b�g�n�Ղ̑傫��

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

				if (pObjCheck == (CObject*)m_ground.pGround)
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
						m_pEdit->UnSave();
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

				if (pObjCheck == (CObject*)m_ground.pGround)
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
