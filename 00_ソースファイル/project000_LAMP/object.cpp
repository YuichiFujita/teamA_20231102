//============================================================
//
//	�I�u�W�F�N�g���� [object.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "manager.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CObject *CObject::m_apTop[MAX_PRIO] = {};	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
CObject *CObject::m_apCur[MAX_PRIO] = {};	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
DWORD CObject::m_dwNextID = 0;				// ���̃��j�[�NID
int CObject::m_nNumAll = 0;					// �I�u�W�F�N�g�̑���

//************************************************************
//	�e�N���X [CObject] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObject::CObject()
{
	if (m_apCur[DEFAULT_PRIO] != NULL)
	{ // �Ō�������݂���ꍇ

		// ���݂̍Ō���I�u�W�F�N�g�̎��I�u�W�F�N�g�����g�ɐݒ�
		m_apCur[DEFAULT_PRIO]->m_pNext = this;

		// �O�I�u�W�F�N�g��ݒ�
		m_pPrev = m_apCur[DEFAULT_PRIO];	// ���݂̍Ō���I�u�W�F�N�g

		// ���I�u�W�F�N�g���N���A
		m_pNext = NULL;

		// ���g�̏��A�h���X���Ō���ɐݒ�
		m_apCur[DEFAULT_PRIO] = this;
	}
	else
	{ // �Ō�������݂��Ȃ��ꍇ

		// ���g�̏��A�h���X��擪�ɐݒ�
		m_apTop[DEFAULT_PRIO] = this;

		// ���g�̏��A�h���X���Ō���ɐݒ�
		m_apCur[DEFAULT_PRIO] = this;

		// �O�I�u�W�F�N�g�̃N���A
		m_pPrev = NULL;

		// ���I�u�W�F�N�g�̃N���A
		m_pNext = NULL;
	}

	// ���g�̏����N���A
	m_label		= LABEL_NONE;	// �I�u�W�F�N�g���x��
	m_nPriority	= DEFAULT_PRIO;	// �D�揇��
	m_dwID		= m_dwNextID;	// ���g�̃��j�[�NID
	m_bUpdate	= true;			// ���g�̍X�V��
	m_bDraw		= true;			// ���g�̕`���
	m_bDeath	= false;		// ���g�̎��S�t���O

	// ���j�[�NID�����Z
	m_dwNextID++;

	// �I�u�W�F�N�g�̑��������Z
	m_nNumAll++;
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObject::CObject(const ELabel label, const int nPriority)
{
	if (m_apCur[nPriority] != NULL)
	{ // �Ō�������݂���ꍇ

		// ���݂̍Ō���I�u�W�F�N�g�̎��I�u�W�F�N�g�����g�ɐݒ�
		m_apCur[nPriority]->m_pNext = this;

		// �O�I�u�W�F�N�g��ݒ�
		m_pPrev = m_apCur[nPriority];	// ���݂̍Ō���I�u�W�F�N�g

		// ���I�u�W�F�N�g���N���A
		m_pNext = NULL;

		// ���g�̏��A�h���X���Ō���ɐݒ�
		m_apCur[nPriority] = this;
	}
	else
	{ // �Ō�������݂��Ȃ��ꍇ

		// ���g�̏��A�h���X��擪�ɐݒ�
		m_apTop[nPriority] = this;

		// ���g�̏��A�h���X���Ō���ɐݒ�
		m_apCur[nPriority] = this;

		// �O�I�u�W�F�N�g�̃N���A
		m_pPrev = NULL;

		// ���I�u�W�F�N�g�̃N���A
		m_pNext = NULL;
	}

	// ���g�̏���ݒ�
	m_label		= label;		// �I�u�W�F�N�g���x��
	m_nPriority	= nPriority;	// �D�揇��
	m_dwID		= m_dwNextID;	// ���g�̃��j�[�NID
	m_bUpdate	= true;			// ���g�̍X�V��
	m_bDraw		= true;			// ���g�̕`���
	m_bDeath	= false;		// ���g�̎��S�t���O

	// ���j�[�NID�����Z
	m_dwNextID++;

	// �I�u�W�F�N�g�̑��������Z
	m_nNumAll++;
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObject::~CObject()
{
	// �I�u�W�F�N�g�̑��������Z
	m_nNumAll--;
}

//============================================================
//	�j������
//============================================================
void CObject::Release(void)
{
	if (this != NULL)
	{ // �g�p����Ă���ꍇ

		if (!m_bDeath)
		{ // ���S�t���O�������Ă��Ȃ��ꍇ

			// ���S�t���O�𗧂Ă�
			m_bDeath = true;
		}
		else { assert(false); }	// ���S�ς�
	}
}

//============================================================
//	�q�b�g����
//============================================================
void CObject::Hit(void)
{
	// ��O����
	assert(false);
}

//============================================================
//	�_���[�W�q�b�g����
//============================================================
void CObject::Hit(const int /*nDmg*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�m�b�N�o�b�N�q�b�g����
//============================================================
void CObject::HitKnockBack(const int /*nDmg*/, const D3DXVECTOR3& /*vec*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CObject::BindTexture(const int /*nTextureID*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�e�N�X�`���������� (�p�X)
//============================================================
void CObject::BindTexture(const char * /*pTexturePass*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	���f���������� (�C���f�b�N�X)
//============================================================
void CObject::BindModel(const int /*nModelID*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	���f���������� (�p�X)
//============================================================
void CObject::BindModel(const char * /*pModelPass*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�C���f�b�N�X�̐ݒ菈��
//============================================================
void CObject::SetIndex(const int /*nIndex*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�C���f�b�N�X�擾����
//============================================================
int CObject::GetIndex(void) const
{
	// ��O����
	assert(false);

	// NONE_IDX��Ԃ�
	return NONE_IDX;
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CObject::SetState(const int /*nState*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	��Ԏ擾����
//============================================================
int CObject::GetState(void) const
{
	// ��O����
	assert(false);

	// NONE_IDX��Ԃ�
	return NONE_IDX;
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CObject::SetType(const int /*nType*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	��ގ擾����
//============================================================
int CObject::GetType(void) const
{
	// ��O����
	assert(false);

	// NONE_IDX��Ԃ�
	return NONE_IDX;
}

//============================================================
//	���@�̐ݒ菈��
//============================================================
void CObject::SetDodge(const int /*nDodge*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	���@�擾����
//============================================================
int CObject::GetDodge(void) const
{
	// ��O����
	assert(false);

	// NONE_IDX��Ԃ�
	return NONE_IDX;
}

//============================================================
//	�p�x�ݒ�̐ݒ菈��
//============================================================
void CObject::SetAngle(const float /*fAngle*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�p�x�擾����
//============================================================
float CObject::GetAngle(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return 0.0f;
}

//============================================================
//	�����ݒ�̐ݒ菈��
//============================================================
void CObject::SetLength(const float /*fLength*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�����擾����
//============================================================
float CObject::GetLength(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return 0.0f;
}

//============================================================
//	���a�̐ݒ菈��
//============================================================
void CObject::SetRadius(const float /*fRadius*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	���a�擾����
//============================================================
float CObject::GetRadius(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return 0.0f;
}

//============================================================
//	�c���̐ݒ菈��
//============================================================
void CObject::SetHeight(const float /*fHeight*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�c���擾����
//============================================================
float CObject::GetHeight(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return 0.0f;
}

//============================================================
//	�g�嗦�̐ݒ菈��
//============================================================
void CObject::SetScale(const float /*fScale*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�g�嗦�擾����
//============================================================
float CObject::GetScale(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return 0.0f;
}

//============================================================
//	�񎲂̈ʒu�̐ݒ菈��
//============================================================
void CObject::SetVec2Position(const D3DXVECTOR2& /*rPos*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�񎲂̈ʒu�擾����
//============================================================
D3DXVECTOR2 CObject::GetVec2Position(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return VEC2_ZERO;
}

//============================================================
//	�O���̈ʒu�̐ݒ菈��
//============================================================
void CObject::SetVec3Position(const D3DXVECTOR3& /*rPos*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�O���̈ʒu�擾����
//============================================================
D3DXVECTOR3 CObject::GetVec3Position(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return VEC3_ZERO;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObject::SetVec3Rotation(const D3DXVECTOR3& /*rRot*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObject::GetVec3Rotation(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return VEC3_ZERO;
}

//============================================================
//	�񎲂̑傫���̐ݒ菈��
//============================================================
void CObject::SetVec2Sizing(const D3DXVECTOR2& /*rSize*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�񎲂̑傫���擾����
//============================================================
D3DXVECTOR2 CObject::GetVec2Sizing(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return VEC2_ZERO;
}

//============================================================
//	�O���̑傫���̐ݒ菈��
//============================================================
void CObject::SetVec3Sizing(const D3DXVECTOR3& /*rSize*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�O���̑傫���擾����
//============================================================
D3DXVECTOR3 CObject::GetVec3Sizing(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return VEC3_ZERO;
}

//============================================================
//	�g�嗦�̐ݒ菈��
//============================================================
void CObject::SetVec3Scaling(const D3DXVECTOR3& /*rScale*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�g�嗦�擾����
//============================================================
D3DXVECTOR3 CObject::GetVec3Scaling(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return VEC3_ZERO;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObject::SetColor(const D3DXCOLOR& /*rCol*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObject::GetColor(void) const
{
	// ��O����
	assert(false);

	// �����l��Ԃ�
	return XCOL_WHITE;
}

//============================================================
//	�}�e���A���̑S�ݒ菈��
//============================================================
void CObject::SetAllMaterial(const D3DXMATERIAL& /*rMat*/)
{
	// ��O����
	assert(false);
}

//============================================================
//	�}�e���A���̍Đݒ菈��
//============================================================
void CObject::ResetMaterial(void)
{
	// ��O����
	assert(false);
}

//============================================================
//	�D�揇�ʂ̐ݒ菈��
//============================================================
void CObject::SetPriority(const int nPriority)
{
	//--------------------------------------------------------
	//	���X�g�̍Đڑ�
	//--------------------------------------------------------
	// �O�̃I�u�W�F�N�g���Ȃ��Ȃ���
	if (m_pNext != NULL)
	{ // ���̃I�u�W�F�N�g�����݂���ꍇ

		// �O�̃I�u�W�F�N�g��ύX
		m_pNext->m_pPrev = m_pPrev;
	}

	// ���̃I�u�W�F�N�g���Ȃ��Ȃ���
	if (m_pPrev != NULL)
	{ // �O�̃I�u�W�F�N�g�����݂���ꍇ

		// ���̃I�u�W�F�N�g��ύX
		m_pPrev->m_pNext = m_pNext;
	}

	// �擪�I�u�W�F�N�g�̕ύX
	if (m_apTop[m_nPriority] == this)
	{ // �擪�I�u�W�F�N�g���j������I�u�W�F�N�g�������ꍇ

		// ���̃I�u�W�F�N�g��擪�Ɏw��
		m_apTop[m_nPriority] = m_pNext;
	}

	// �Ō���I�u�W�F�N�g�̕ύX
	if (m_apCur[m_nPriority] == this)
	{ // �Ō���I�u�W�F�N�g���j������I�u�W�F�N�g�������ꍇ

		// �O�̃I�u�W�F�N�g���Ō���Ɏw��
		m_apCur[m_nPriority] = m_pPrev;
	}

	//--------------------------------------------------------
	//	�D�揇�ʂ̐ݒ�E���X�g�ւ̒ǉ�
	//--------------------------------------------------------
	// �����̗D�揇�ʂ�ݒ�
	m_nPriority = nPriority;

	// ���g�̃I�u�W�F�N�g�������̗D�揇�ʃ��X�g�ɕύX
	if (m_apCur[nPriority] != NULL)
	{ // �Ō�������݂���ꍇ

		// ���݂̍Ō���I�u�W�F�N�g�̎��I�u�W�F�N�g�����g�ɐݒ�
		m_apCur[nPriority]->m_pNext = this;

		// �O�I�u�W�F�N�g��ݒ�
		m_pPrev = m_apCur[nPriority];	// ���݂̍Ō���I�u�W�F�N�g

		// ���I�u�W�F�N�g���N���A
		m_pNext = NULL;

		// ���g�̏��A�h���X���Ō���ɐݒ�
		m_apCur[nPriority] = this;
	}
	else
	{ // �Ō�������݂��Ȃ��ꍇ

		// ���g�̏��A�h���X��擪�ɐݒ�
		m_apTop[nPriority] = this;

		// ���g�̏��A�h���X���Ō���ɐݒ�
		m_apCur[nPriority] = this;

		// �O�I�u�W�F�N�g�̃N���A
		m_pPrev = NULL;

		// ���I�u�W�F�N�g�̃N���A
		m_pNext = NULL;
	}
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CObject::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	m_bUpdate = bUpdate;
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CObject::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	m_bDraw = bDraw;
}

//============================================================
//	�}�g���b�N�X�|�C���^�擾����
//============================================================
D3DXMATRIX *CObject::GetPtrMtxWorld(void)
{
	// ��O����
	assert(false);

	// NULL��Ԃ�
	return NULL;
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CObject::GetMtxWorld(void) const
{
	// �ϐ���錾
	D3DXMATRIX mtxNone;	// �Ԃ�l�p

	// �}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxNone);

	// ��O����
	assert(false);

	// �P�ʃ}�g���b�N�X��Ԃ�
	return mtxNone;
}

//============================================================
//	�S�j������
//============================================================
void CObject::ReleaseAll(void)
{
	// �|�C���^��錾
	CObject *pObject;	// �I�u�W�F�N�g����p

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		if (m_apTop[nCntPri] != NULL)
		{ // �擪�����݂���ꍇ

			// �I�u�W�F�N�g�̐擪����
			pObject = m_apTop[nCntPri];

			while (pObject != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^

				if (pObject->m_label != LABEL_NONE)
				{ // �I�u�W�F�N�g���x�����ݒ肳��Ă���ꍇ

					if (!pObject->m_bDeath)
					{ // ���S���Ă��Ȃ��ꍇ

						// �I�u�W�F�N�g�̏I��
						pObject->Uninit();
					}
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObject = pObjectNext;
			}
		}
	}

	// �S���S����
	DeathAll();
}

//============================================================
//	�S�X�V����
//============================================================
void CObject::UpdateAll(void)
{
	// �|�C���^��錾
	CObject *pObject;	// �I�u�W�F�N�g����p

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�I�u�W�F�N�g��]�F%d\n", m_nNumAll);

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		if (m_apTop[nCntPri] != NULL)
		{ // �擪�����݂���ꍇ

			// �I�u�W�F�N�g�̐擪����
			pObject = m_apTop[nCntPri];

			while (pObject != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^

				if (pObject->m_label != LABEL_NONE)
				{ // �I�u�W�F�N�g���x�����ݒ肳��Ă���ꍇ

					if (pObject->m_bUpdate)
					{ // �X�V����ꍇ

						if (!pObject->m_bDeath)
						{ // ���S���Ă��Ȃ��ꍇ

							// �I�u�W�F�N�g�̍X�V
							pObject->Update();
						}
					}
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObject = pObjectNext;
			}
		}
	}

	// �S���S����
	DeathAll();
}

//============================================================
//	�S�`�揈��
//============================================================
void CObject::DrawAll(void)
{
	// �|�C���^��錾
	CObject *pObject;	// �I�u�W�F�N�g����p

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		if (m_apTop[nCntPri] != NULL)
		{ // �擪�����݂���ꍇ

			// �I�u�W�F�N�g�̐擪����
			pObject = m_apTop[nCntPri];

			while (pObject != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^

				if (pObject->m_bDraw)
				{ // �`�悷��ꍇ

					if (!pObject->m_bDeath)
					{ // ���S���Ă��Ȃ��ꍇ

						// �I�u�W�F�N�g�̕`��
						pObject->Draw();
					}
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObject = pObjectNext;
			}
		}
	}
}

//============================================================
//	�S���S����
//============================================================
void CObject::DeathAll(void)
{
	// �|�C���^��錾
	CObject *pObject;	// �I�u�W�F�N�g����p

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		if (m_apTop[nCntPri] != NULL)
		{ // �擪�����݂���ꍇ

			// �I�u�W�F�N�g�̐擪����
			pObject = m_apTop[nCntPri];

			while (pObject != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^

				if (pObject->m_bDeath)
				{ // ���S�t���O�������Ă���ꍇ

					if (pObject != NULL)
					{ // �g�p����Ă���ꍇ

						// �O�̃I�u�W�F�N�g���Ȃ��Ȃ���
						if (pObject->m_pNext != NULL)
						{ // ���̃I�u�W�F�N�g�����݂���ꍇ

							// �O�̃I�u�W�F�N�g��ύX
							pObject->m_pNext->m_pPrev = pObject->m_pPrev;
						}

						// ���̃I�u�W�F�N�g���Ȃ��Ȃ���
						if (pObject->m_pPrev != NULL)
						{ // �O�̃I�u�W�F�N�g�����݂���ꍇ

							// ���̃I�u�W�F�N�g��ύX
							pObject->m_pPrev->m_pNext = pObject->m_pNext;
						}

						// �擪�I�u�W�F�N�g�̕ύX
						if (m_apTop[pObject->m_nPriority] == pObject)
						{ // �擪�I�u�W�F�N�g���j������I�u�W�F�N�g�������ꍇ

							// ���̃I�u�W�F�N�g��擪�Ɏw��
							m_apTop[pObject->m_nPriority] = pObject->m_pNext;
						}

						// �Ō���I�u�W�F�N�g�̕ύX
						if (m_apCur[pObject->m_nPriority] == pObject)
						{ // �Ō���I�u�W�F�N�g���j������I�u�W�F�N�g�������ꍇ

							// �O�̃I�u�W�F�N�g���Ō���Ɏw��
							m_apCur[pObject->m_nPriority] = pObject->m_pPrev;
						}

						// �������J��
						delete pObject;
					}
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObject = pObjectNext;
			}
		}
	}
}

//============================================================
//	�����擾����
//============================================================
int CObject::GetNumAll(void)
{
	// ���݂̃I�u�W�F�N�g�̑�����Ԃ�
	return m_nNumAll;
}

//============================================================
//	�擪�I�u�W�F�N�g�擾����
//============================================================
CObject *CObject::GetTop(const int nPriority)
{
	// �����̗D�揇�ʂ̐擪�I�u�W�F�N�g��Ԃ�
	return m_apTop[nPriority];
}

//============================================================
//	�Ō���I�u�W�F�N�g�擾����
//============================================================
CObject *CObject::GetCur(const int nPriority)
{
	// �����̗D�揇�ʂ̍Ō���I�u�W�F�N�g��Ԃ�
	return m_apCur[nPriority];
}

//============================================================
//	���x���̐ݒ菈��
//============================================================
void CObject::SetLabel(const ELabel label)
{
	if (label < LABEL_MAX)
	{ // �����̃��x�����g�p�\�ȏꍇ

		// �����̃��x����ݒ�
		m_label = label;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	���x���̎擾����
//============================================================
CObject::ELabel CObject::GetLabel(void) const
{
	// ���x����Ԃ�
	return m_label;
}

//============================================================
//	�D�揇�ʂ̎擾����
//============================================================
int CObject::GetPriority(void) const
{
	// �D�揇�ʂ�Ԃ�
	return m_nPriority;
}

//============================================================
//	���j�[�NID�̎擾����
//============================================================
DWORD CObject::GetUniqueID(void) const
{
	// ���j�[�NID��Ԃ�
	return m_dwID;
}

//============================================================
//	�X�V�󋵂̎擾����
//============================================================
bool CObject::IsUpdate(void) const
{
	// �X�V�󋵂�Ԃ�
	return m_bUpdate;
}

//============================================================
//	�`��󋵂̎擾����
//============================================================
bool CObject::IsDraw(void) const
{
	// �`��󋵂�Ԃ�
	return m_bDraw;
}

//============================================================
//	���S�t���O�擾����
//============================================================
bool CObject::IsDeath(void) const
{
	// ���S�t���O��Ԃ�
	return m_bDeath;
}

//============================================================
//	�I�u�W�F�N�g�擾����
//============================================================
CObject *CObject::GetObject(void)
{
	// ���g�̃|�C���^��Ԃ�
	return this;
}

//============================================================
//	�O�I�u�W�F�N�g�擾����
//============================================================
CObject *CObject::GetPrev(void) const
{
	// �O�I�u�W�F�N�g��Ԃ�
	return m_pPrev;
}

//============================================================
//	���I�u�W�F�N�g�擾����
//============================================================
CObject *CObject::GetNext(void) const
{
	// ���I�u�W�F�N�g��Ԃ�
	return m_pNext;
}
