//============================================================
//
//	�G�f�B�b�g�����ʒu���� [editSpawnPoint.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editSpawnPoint.h"
#include "manager.h"
#include "editStageManager.h"
#include "stage.h"
#include "effect3D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_CREATE		(DIK_0)	// �����L�[
#define NAME_CREATE		("0")	// �����\��
#define KEY_CHANGE_IDX	(DIK_2)	// �C���f�b�N�X�ύX�L�[
#define NAME_CHANGE_IDX	("2")	// �C���f�b�N�X�ύX�\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float	POS_EFFECT_RADIUS = 120.0f;	// �ʒu�\���̃G�t�F�N�g���a
	const float	ROT_EFFECT_RADIUS = 60.0f;	// �����\���̃G�t�F�N�g���a
	const float	EFFECT_ADDLENGTH = 100.0f;	// �����\���̃G�t�F�N�g�x�N�g�����Z��
	const int	EFFECT_LIFE = 10;			// �G�t�F�N�g����
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
int CEditSpawnPoint::m_nSave = 0;	// �ۑ����

//************************************************************
//	�e�N���X [CEditSpawnPoint] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditSpawnPoint::CEditSpawnPoint()
{
#if _DEBUG

	// �����o�ϐ����N���A
	memset(&m_apSpawnPoint[0], 0, sizeof(m_apSpawnPoint));	// �����ʒu���
	m_nIdxChange = 0;	// �ύX�C���f�b�N�X

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditSpawnPoint::~CEditSpawnPoint()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditSpawnPoint::Init(void)
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
	memset(&m_apSpawnPoint[0], 0, sizeof(m_apSpawnPoint));	// �����ʒu���
	m_nIdxChange = 0;	// �ύX�C���f�b�N�X

	// �����ʒu�̎g�p�m�F
	CheckUseSpawnPoint(MAX_PLAYER, &m_apSpawnPoint[0]);

	for (int nCntSpawn = 0; nCntSpawn < MAX_PLAYER; nCntSpawn++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		if (m_apSpawnPoint[nCntSpawn] == NULL)
		{ // �g�p����Ă��Ȃ������ꍇ

			// �����ʒu�̐���
			m_apSpawnPoint[nCntSpawn] = CSpawnPoint::Create(posEdit, rotEdit);
			if (m_apSpawnPoint[nCntSpawn] == NULL)
			{ // �����Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}
		}
	}

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
void CEditSpawnPoint::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditSpawnPoint::Update(void)
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

	// �����ʒu�̐���
	CreateSpawnPoint();

	// �C���f�b�N�X�ύX�̍X�V
	UpdateChangeIdx();

	// �����ʒu�̑S�\��
	LookAllSpawnPoint();

	// �G�f�B�b�g�ʒu�̕\��
	LookEffect(pEdit->GetVec3Position(), pEdit->GetVec3Rotation(), XCOL_RED);

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditSpawnPoint::DrawDebugControl(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�v���b�N�̏��

	pDebug->Print(CDebugProc::POINT_RIGHT, "�z�u�C���f�b�N�X�ύX�F[%s]\n", NAME_CHANGE_IDX);
	pDebug->Print(CDebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditSpawnPoint::DrawDebugInfo(void)
{
	// �|�C���^��錾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();	// �f�o�b�O�v���b�N�̏��

	pDebug->Print(CDebugProc::POINT_RIGHT, "%d�F[�z�u�C���f�b�N�X]\n", m_nIdxChange);
}

//============================================================
//	���ۑ�����
//============================================================
void CEditSpawnPoint::SaveInfo(void)
{
	// ���݂̏���ۑ�
	m_nSave = m_nIdxChange;
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditSpawnPoint::LoadInfo(void)
{
	// �ۑ�����ݒ�
	m_nIdxChange = m_nSave;
}

//============================================================
//	�ۑ�����
//============================================================
void CEditSpawnPoint::Save(FILE *pFile)
{
#if _DEBUG

	if (pFile != NULL)
	{ // �t�@�C�������݂���ꍇ

		// ���o���������o��
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "#	�����ʒu�̔z�u���\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		// ���J�n�n�_�������o��
		fprintf(pFile, "STAGE_SPAWNPOINTSET\n\n");

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
	
					if (pObjCheck->GetLabel() != CObject::LABEL_SPAWNPOINT)
					{ // �I�u�W�F�N�g���x���������ʒu�ł͂Ȃ��ꍇ
	
						// ���̃I�u�W�F�N�g�ւ̃|�C���^����
						pObjCheck = pObjectNext;
	
						// ���̌J��Ԃ��Ɉڍs
						continue;
					}

					// �����ʒu�̏����擾
					D3DXVECTOR3 posSpawnPoint = pObjCheck->GetVec3Position();	// �ʒu
					D3DXVECTOR3 rotSpawnPoint = pObjCheck->GetVec3Rotation();	// ����
	
					// ���������o��
					fprintf(pFile, "	SPAWNPOINTSET\n");
					fprintf(pFile, "		POS = %.2f %.2f %.2f\n", posSpawnPoint.x, posSpawnPoint.y, posSpawnPoint.z);
					fprintf(pFile, "		ROT = %.2f %.2f %.2f\n", rotSpawnPoint.x, rotSpawnPoint.y, rotSpawnPoint.z);
					fprintf(pFile, "	END_SPAWNPOINTSET\n\n");

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;
				}
			}
		}

		// ���I���n�_�������o��
		fprintf(pFile, "END_STAGE_SPAWNPOINTSET\n\n");
	}

#endif	// _DEBUG
}

//============================================================
//	�����ʒu�̃G�t�F�N�g�\������
//============================================================
void CEditSpawnPoint::LookEffect
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol		// �F
)
{
	// �ϐ���錾
	D3DXVECTOR3 vecRot = VEC3_ZERO;	// �����x�N�g��

	// �����x�N�g�����쐬
	vecRot.x = sinf(rRot.y + D3DX_PI) * EFFECT_ADDLENGTH;
	vecRot.z = cosf(rRot.y + D3DX_PI) * EFFECT_ADDLENGTH;

	// �����ʒu�ɃG�t�F�N�g��\��
	CEffect3D::Create(rPos, POS_EFFECT_RADIUS, CEffect3D::TYPE_NORMAL, EFFECT_LIFE, VEC3_ZERO, VEC3_ZERO, rCol);

	// ���������ɃG�t�F�N�g��\��
	CEffect3D::Create(rPos + vecRot, ROT_EFFECT_RADIUS, CEffect3D::TYPE_NORMAL, EFFECT_LIFE);
}

//============================================================
//	�����ʒu�̐�������
//============================================================
void CEditSpawnPoint::CreateSpawnPoint(void)
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

	// �����ʒu��z�u
	if (m_pKeyboard->IsTrigger(KEY_CREATE))
	{
		// �z�u����ύX
		m_apSpawnPoint[m_nIdxChange]->SetVec3Position(posEdit);
		m_apSpawnPoint[m_nIdxChange]->SetVec3Rotation(rotEdit);

		// ���ۑ���ݒ�
		pEdit->UnSave();
	}
}

//============================================================
//	�C���f�b�N�X�ύX�̍X�V����
//============================================================
void CEditSpawnPoint::UpdateChangeIdx(void)
{
	// �|�C���^��錾
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���

	if (m_pKeyboard->IsTrigger(KEY_CHANGE_IDX))
	{
		// �C���f�b�N�X��ύX
		m_nIdxChange = (m_nIdxChange + 1) % MAX_PLAYER;
	}
}

//============================================================
//	�����ʒu�̑S�\��
//============================================================
void CEditSpawnPoint::LookAllSpawnPoint(void)
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

				if (pObjCheck->GetLabel() != CObject::LABEL_SPAWNPOINT)
				{ // �I�u�W�F�N�g���x���������ʒu�ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �ϐ���錾
				D3DXVECTOR3 posSpawn = pObjCheck->GetVec3Position();	// �X�|�[���|�C���g�ʒu
				D3DXVECTOR3 rotSpawn = pObjCheck->GetVec3Rotation();	// �X�|�[���|�C���g����
				D3DXCOLOR colEffect = XCOL_CYAN;		// �G�t�F�N�g�F
				int nIdxSpawn = pObjCheck->GetIndex();	// �X�|�[���|�C���g�C���f�b�N�X

				// �͈͊O��O
				assert(nIdxSpawn > NONE_IDX && nIdxSpawn < MAX_PLAYER);

				if (nIdxSpawn == m_nIdxChange)
				{ // �X�|�[���|�C���g�C���f�b�N�X�����ݑI�𒆂̃C���f�b�N�X�̏ꍇ

					// �F��΂ɕύX
					colEffect = XCOL_GREEN;
				}

				// �����ʒu�̃G�t�F�N�g�\��
				LookEffect
				( // ����
					posSpawn,	// �ʒu
					rotSpawn,	// ����
					colEffect	// �F
				);

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	�����ʒu�̎g�p�m�F
//============================================================
void CEditSpawnPoint::CheckUseSpawnPoint(const int nMaxPoint, CObject **ppAllSpawnPoint)
{
	for (int nCntSpawn = 0; nCntSpawn < nMaxPoint; nCntSpawn++)
	{ // �����ʒu�̐����J��Ԃ�

		// ��g�p���ɂ���
		ppAllSpawnPoint[nCntSpawn] = NULL;
	}

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

				if (pObjCheck->GetLabel() != CObject::LABEL_SPAWNPOINT)
				{ // �I�u�W�F�N�g���x���������ʒu�ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �͈͊O��O
				assert(pObjCheck->GetIndex() > NONE_IDX && pObjCheck->GetIndex() < MAX_PLAYER);

				for (int nCntSpawn = 0; nCntSpawn < nMaxPoint; nCntSpawn++)
				{ // �����ʒu�̐����J��Ԃ�

					if (pObjCheck->GetIndex() == nCntSpawn)
					{ // ���݂̒l���C���f�b�N�X�̏ꍇ

						// �g�p���ɂ���
						ppAllSpawnPoint[nCntSpawn] = pObjCheck;
					}
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}
