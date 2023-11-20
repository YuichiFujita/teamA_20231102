//============================================================
//
//	�G�f�B�b�g�u���b�N���� [editBlock.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editBlock.h"
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
#define KEY_REVERSE		(DIK_LCONTROL)	// ����t�]���L�[
#define NAME_REVERSE	("LCTRL")		// ����t�]���\��

#define KEY_CREATE		(DIK_0)	// �����L�[
#define NAME_CREATE		("0")	// �����\��
#define KEY_RELEASE		(DIK_9)	// �j���L�[
#define NAME_RELEASE	("9")	// �j���\��
#define KEY_TYPE		(DIK_2)	// ��ޕύX�L�[
#define NAME_TYPE		("2")	// ��ޕύX�\��

#define KEY_UP_TEXPART_X	(DIK_U)	// �e�N�X�`������X�g��L�[
#define NAME_UP_TEXPART_X	("U")	// �e�N�X�`������X�g��\��
#define KEY_DOWN_TEXPART_X	(DIK_J)	// �e�N�X�`������X�k���L�[
#define NAME_DOWN_TEXPART_X	("J")	// �e�N�X�`������X�k���\��
#define KEY_UP_TEXPART_Y	(DIK_I)	// �e�N�X�`������Y�g��L�[
#define NAME_UP_TEXPART_Y	("I")	// �e�N�X�`������Y�g��\��
#define KEY_DOWN_TEXPART_Y	(DIK_K)	// �e�N�X�`������Y�k���L�[
#define NAME_DOWN_TEXPART_Y	("K")	// �e�N�X�`������Y�k���\��
#define KEY_UP_TEXPART_Z	(DIK_O)	// �e�N�X�`������Z�g��L�[
#define NAME_UP_TEXPART_Z	("O")	// �e�N�X�`������Z�g��\��
#define KEY_DOWN_TEXPART_Z	(DIK_L)	// �e�N�X�`������Z�k���L�[
#define NAME_DOWN_TEXPART_Z	("L")	// �e�N�X�`������Z�k���\��

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
CEditBlock::SInfo CEditBlock::m_save = {};	// �ۑ����

//************************************************************
//	�e�N���X [CEditBlock] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditBlock::CEditBlock()
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pBlock = NULL;	// �u���b�N���
	memset(&m_block, 0, sizeof(m_block));	// �u���b�N�z�u���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditBlock::~CEditBlock()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditBlock::Init(void)
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
	m_pBlock		= NULL;					// �u���b�N���
	m_block.type	= CBlock::TYPE_STONE;	// �u���b�N���
	m_block.partX	= VEC2_ONE;				// �e�N�X�`��������X
	m_block.partY	= VEC2_ONE;				// �e�N�X�`��������Y
	m_block.partZ	= VEC2_ONE;				// �e�N�X�`��������Z

	// �u���b�N�̐���
	m_pBlock = CBlock::Create(m_block.type, posEdit, rotEdit, sizeEdit);
	if (m_pBlock == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �F��ݒ�
	D3DXCOLOR col = m_pBlock->GetColor();	// ���̐F���擾
	m_pBlock->SetColor(D3DXCOLOR(col.r, col.g, col.b, INIT_ALPHA));

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
void CEditBlock::Uninit(void)
{
#if _DEBUG

	if (m_pBlock != NULL)
	{ // �����Ɏ��s�����ꍇ

		// �u���b�N�̐F�̑S������
		InitAllColorBlock();

		// �u���b�N�̏I��
		m_pBlock->Uninit();
	}

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditBlock::Update(void)
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

	// �e�N�X�`�������̍X�V
	UpdateTexPart();

	// �u���b�N�̐���
	CreateBlock();

	// �u���b�N�̔j��
	ReleaseBlock();

	// �ʒu�𔽉f
	m_pBlock->SetVec3Position(pEdit->GetVec3Position());

	// �����𔽉f
	m_pBlock->SetVec3Rotation(pEdit->GetVec3Rotation());

	// �傫���𔽉f
	m_pBlock->SetVec3Sizing(pEdit->GetVec3Sizing());

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditBlock::DrawDebugControl(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�v���b�N�̏��

	pDebug->Print(CDebugProc::POINT_RIGHT, "��ޕύX�F[%s]\n", NAME_TYPE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�e�N�X�`�������F[%s/%s/%s/%s/%s/%s+%s]\n", NAME_UP_TEXPART_X, NAME_DOWN_TEXPART_X, NAME_UP_TEXPART_Y, NAME_DOWN_TEXPART_Y, NAME_UP_TEXPART_Z, NAME_DOWN_TEXPART_Z, NAME_REVERSE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�폜�F[%s]\n", NAME_RELEASE);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditBlock::DrawDebugInfo(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�v���b�N�̏��

	pDebug->Print(CDebugProc::POINT_RIGHT, "%d�F[���]\n", m_block.type);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f�F[�e�N�X�`������X]\n", m_block.partX.x, m_block.partX.y);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f�F[�e�N�X�`������Y]\n", m_block.partY.x, m_block.partY.y);
	pDebug->Print(CDebugProc::POINT_RIGHT, "%f %f�F[�e�N�X�`������Z]\n", m_block.partZ.x, m_block.partZ.y);
}

//============================================================
//	���ۑ�����
//============================================================
void CEditBlock::SaveInfo(void)
{
	// ���݂̏���ۑ�
	m_save = m_block;
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditBlock::LoadInfo(void)
{
	// �ۑ�����ݒ�
	m_block = m_save;
}

//============================================================
//	�ۑ�����
//============================================================
void CEditBlock::Save(FILE *pFile)
{
#if _DEBUG

	if (pFile != NULL)
	{ // �t�@�C�������݂���ꍇ

		// ���o���������o��
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#	�u���b�N�̔z�u���\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		// ���J�n�n�_�������o��
		fprintf(pFile, "STAGE_BLOCKSET\n\n");

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
	
					if (pObjCheck->GetLabel() != CObject::LABEL_BLOCK)
					{ // �I�u�W�F�N�g���x�����u���b�N�ł͂Ȃ��ꍇ
	
						// ���̃I�u�W�F�N�g�ւ̃|�C���^����
						pObjCheck = pObjectNext;
	
						// ���̌J��Ԃ��Ɉڍs
						continue;
					}
	
					if (pObjCheck == (CObject*)m_pBlock)
					{ // �����A�h���X�������ꍇ
	
						// ���̃I�u�W�F�N�g�ւ̃|�C���^����
						pObjCheck = pObjectNext;
	
						// ���̌J��Ԃ��Ɉڍs
						continue;
					}

					// �u���b�N�̏����擾
					D3DXVECTOR3 posBuild = pObjCheck->GetVec3Position();	// �ʒu
					D3DXVECTOR3 rotBuild = pObjCheck->GetVec3Rotation();	// ����
					D3DXVECTOR3 sizeBuild = pObjCheck->GetVec3Sizing();		// �傫��
					int nType = pObjCheck->GetType();	// ���
	
					// ���������o��
					fprintf(pFile, "	BLOCKSET\n");
					fprintf(pFile, "		TYPE = %d\n", nType);
					fprintf(pFile, "		POS = %.2f %.2f %.2f\n", posBuild.x, posBuild.y, posBuild.z);
					fprintf(pFile, "		ROT = %.2f %.2f %.2f\n", rotBuild.x, rotBuild.y, rotBuild.z);
					fprintf(pFile, "		SIZE = %.2f %.2f %.2f\n", sizeBuild.x, sizeBuild.y, sizeBuild.z);
					fprintf(pFile, "	END_BLOCKSET\n\n");

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;
				}
			}
		}

		// ���I���n�_�������o��
		fprintf(pFile, "END_STAGE_BLOCKSET\n\n");
	}

#endif	// _DEBUG
}

//============================================================
//	��ޕύX�̍X�V����
//============================================================
void CEditBlock::UpdateChangeType(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// ��ނ�ύX
	if (m_pKeyboard->IsTrigger(KEY_TYPE))
	{
		m_block.type = (CBlock::EType)((m_block.type + 1) % CBlock::TYPE_MAX);
	}

	// ��ނ𔽉f
	m_pBlock->SetType(m_block.type);
}

//============================================================
//	�e�N�X�`�������̍X�V����
//============================================================
void CEditBlock::UpdateTexPart(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �e�N�X�`��������ύX
	if (!m_pKeyboard->IsPress(KEY_REVERSE))
	{
		if (m_pKeyboard->IsTrigger(KEY_UP_TEXPART_X))
		{
			m_block.partX.x += 1.0f;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_TEXPART_X))
		{
			m_block.partX.y += 1.0f;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP_TEXPART_Y))
		{
			m_block.partY.x += 1.0f;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_TEXPART_Y))
		{
			m_block.partY.y += 1.0f;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP_TEXPART_Z))
		{
			m_block.partZ.x += 1.0f;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_TEXPART_Z))
		{
			m_block.partZ.y += 1.0f;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_UP_TEXPART_X))
		{
			m_block.partX.x -= 1.0f;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_TEXPART_X))
		{
			m_block.partX.y -= 1.0f;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP_TEXPART_Y))
		{
			m_block.partY.x -= 1.0f;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_TEXPART_Y))
		{
			m_block.partY.y -= 1.0f;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP_TEXPART_Z))
		{
			m_block.partZ.x -= 1.0f;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN_TEXPART_Z))
		{
			m_block.partZ.y -= 1.0f;
		}
	}

	// �e�N�X�`��������������
	m_pBlock->SetTexturePatternX(m_block.partX);
	m_pBlock->SetTexturePatternY(m_block.partY);
	m_pBlock->SetTexturePatternZ(m_block.partZ);
}

//============================================================
//	�u���b�N�̐�������
//============================================================
void CEditBlock::CreateBlock(void)
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

	// �u���b�N��z�u
	if (m_pKeyboard->IsTrigger(KEY_CREATE))
	{
		//----------------------------------------------------
		//	�u���b�N�̏���z�u�p�ɕύX
		//----------------------------------------------------
		// �����X�V�E�����`���ON�ɂ���
		m_pBlock->SetEnableUpdate(true);
		m_pBlock->SetEnableDraw(true);

		// �F��ݒ�
		colBuild = m_pBlock->GetColor();	// ���̐F���擾
		m_pBlock->SetColor(D3DXCOLOR(colBuild.r, colBuild.g, colBuild.b, 1.0f));

		// ���ۑ���ݒ�
		pEdit->UnSave();

		//----------------------------------------------------
		//	�V�����u���b�N�̐���
		//----------------------------------------------------
		// �u���b�N�̐���
		m_pBlock = CBlock::Create(m_block.type, posEdit, rotEdit, sizeEdit);
		assert(m_pBlock != NULL);

		// �F��ݒ�
		colBuild = m_pBlock->GetColor();	// ���̐F���擾
		m_pBlock->SetColor(D3DXCOLOR(colBuild.r, colBuild.g, colBuild.b, INIT_ALPHA));
	}
}

//============================================================
//	�u���b�N�̔j������
//============================================================
void CEditBlock::ReleaseBlock(void)
{
	// �ϐ���錾
	bool bRelease = false;	// �j����

	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	// �u���b�N���폜
	if (m_pKeyboard->IsTrigger(KEY_RELEASE))
	{
		// �j�������Ԃ�ݒ�
		bRelease = true;
	}

	// �u���b�N�̍폜����
	DeleteCollisionBlock(bRelease);
}

//============================================================
//	�u���b�N�̍폜����
//============================================================
void CEditBlock::DeleteCollisionBlock(const bool bRelase)
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
				D3DXVECTOR3 posBuild = VEC3_ZERO;	// �u���b�N�ʒu
				D3DXVECTOR3 sizeBuild = VEC3_ZERO;	// �u���b�N�傫��

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_BLOCK)
				{ // �I�u�W�F�N�g���x�����u���b�N�ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (pObjCheck == (CObject*)m_pBlock)
				{ // �����A�h���X�������ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �u���b�N�̈ʒu���擾
				posBuild = pObjCheck->GetVec3Position();

				// �u���b�N�̑傫�����擾
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
//	�u���b�N�̐F�̑S����������
//============================================================
void CEditBlock::InitAllColorBlock(void)
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

				if (pObjCheck->GetLabel() != CObject::LABEL_BLOCK)
				{ // �I�u�W�F�N�g���x�����u���b�N�ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (pObjCheck == (CObject*)m_pBlock)
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
